#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

#include <mpi.h>

using namespace std;

typedef vector<double> MatrixData;

// don't care what the tag is!
static const int TAG = 99;

template <typename T>
void broadcast(const MPI_Comm comm, const int root, /* const */ vector<T>& data)
{
    const int sz = data.size() * sizeof(typename vector<T>::value_type);
    MPI_Bcast(&data[0], sz, MPI_CHAR, root, comm);
}

template <typename T>
void send(const MPI_Comm comm, const int to, /* const */ vector<T>& data)
{
    const int sz = data.size() * sizeof(typename vector<T>::value_type);
    MPI_Send(&data[0], sz, MPI_CHAR, to, TAG, comm);
}

template <typename T>
void receive(const MPI_Comm comm,
             const int from,
             /* const */ vector<T>& data,
             const int offset,
             const int size)
{
    const int sz = size * sizeof(typename vector<T>::value_type);
    MPI_Recv(&data[offset], sz, MPI_CHAR, from, TAG, comm, MPI_STATUS_IGNORE);
}

template <typename T>
void calculate(const int start, const int end, const int dim, 
	const vector<T>& mat_a, const vector<T>& mat_b, typename vector<T>::iterator it)
{
	int row = start / dim;
	int col = start % dim;

	for(int c = start; c < end; ++c)
	{
		// make iterators at start for row and col
		typename vector<T>::const_iterator rowIt = mat_a.begin() + (row * dim);
		typename vector<T>::const_iterator colIt = mat_b.begin() + col;

		// calculate dot product
		T val = 0;
		for( int i = 0; i < dim; ++i)
		{
			val += (*rowIt) * (*colIt);

			// increment iterators
			++rowIt;
			colIt += dim;
		}
		// assign value
		*it = val;

		// move to next element
		++col;
		if(col == dim)
		{
			++row;
			col = 0;
		}
		++it;
	}
}

template <typename T>
void print_matrix(const string& title, const vector<T>& data, const int dim)
{
	cout << title << '\n';
	
	typename vector<T>::const_iterator it = data.begin();

	for(int i = 0; i < dim; ++i)
	{
		for(int j = 0; j < dim; ++j)
		{
			cout << *it << ' ';
			++it;
		}
		cout << '\n';
	}
}

template <typename T>
T myrand()
{
	return static_cast<T>(rand() % 10);
}

void usage()
{
    std::cout << "\n--------------------------------\n";
    std::cout << "Error - Usage: matrix_mult <matrix size>\n";
    std::cout << "--------------------------------\n\n";
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		usage();
		return 1;
	}

	MPI_Init(&argc, &argv);

	const int dim = atoi(argv[1]);
	const int data_size = dim * dim;
	int my_rank;
	int num_procs;

	MatrixData mat_a(data_size);
	MatrixData mat_b(data_size);

        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// generate the numbers on root node
	if(my_rank == 0)
	{
		generate(mat_a.begin(), mat_a.end(), myrand<MatrixData::value_type>);
		generate(mat_b.begin(), mat_b.end(), myrand<MatrixData::value_type>);
	}

	// distribute the data
	broadcast(MPI_COMM_WORLD, 0, mat_a);
	broadcast(MPI_COMM_WORLD, 0, mat_b);
	
	// integer division
	// don't care about the remainder - root will do rest
	const int partition_size = data_size / num_procs;
	
	if(my_rank == 0)
	{
		MatrixData mat_c(data_size);
		const int start = partition_size * (num_procs - 1);
		const int end = data_size;
		
		// calculate matrix section
		calculate(start, end, dim, mat_a, mat_b, mat_c.begin() + start);
		
		// only receive if we have data
		if(partition_size != 0)
		{
			// receive from other processes
			for(int p = 1; p < num_procs; ++p)
			{
				const int offset = partition_size * (p - 1);
				receive(MPI_COMM_WORLD, p, mat_c, offset, partition_size);
			}
		}
		
		print_matrix("Matrix A", mat_a, dim);
		print_matrix("Matrix B", mat_b, dim);
		print_matrix("Matrix C", mat_c, dim);
	}
	else
	{
		// check we have values to calculate
		if(partition_size != 0)
		{
			MatrixData mat_c_section(partition_size);
			const int start = partition_size * (my_rank - 1);
			const int end = start + partition_size;

			// calulate matrix section
			calculate(start, end, dim, mat_a, mat_b, mat_c_section.begin());

			// send to root
			send(MPI_COMM_WORLD, 0, mat_c_section);
		}
	}

	MPI_Finalize();
}
