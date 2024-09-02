#define MDSPAN_USE_PAREN_OPERATOR 1
#include <mdspan/mdspan.hpp>
#include "experimental/__p2630_bits/submdspan.hpp"
#include <experimental/linalg>
#include <iostream>

#if (! defined(__GNUC__)) || (__GNUC__ > 9)
#  define MDSPAN_EXAMPLES_USE_EXECUTION_POLICIES 1
#endif

#ifdef MDSPAN_EXAMPLES_USE_EXECUTION_POLICIES
#  include <execution>
#endif

#include <iostream>
#include <vector>

using Kokkos::mdspan;
using Kokkos::submdspan;
using Kokkos::extents;
namespace linalg = Kokkos::Experimental::linalg;
using linalg::lower_triangle_t;
using linalg::upper_triangle_t;
using std::pair;
using std::tuple;

#if defined(__cpp_lib_span)
#include <span>
  using std::dynamic_extent;
#else
  using Kokkos::dynamic_extent;
#endif

  // Flip upper to lower, and lower to upper
lower_triangle_t opposite_triangle(upper_triangle_t) {
  return {};
}
upper_triangle_t opposite_triangle(lower_triangle_t) {
  return {};
}

// Returns nullopt if no bad pivots,
// else the index of the first bad pivot.
// A "bad" pivot is zero or NaN.
template<class InOutMat,
         class Triangle>
std::optional<typename InOutMat::size_type>
cholesky_factor(InOutMat A, Triangle t)
{
  using value_type = typename InOutMat::value_type;
  using size_type = typename InOutMat::size_type;

  constexpr value_type ZERO {};
  constexpr value_type ONE (1.0);
  const size_type n = A.extent(0);

  if (n == 0) {
    return std::nullopt;
  }
  else if (n == 1) {
    if (A[0,0] <= ZERO || std::isnan(A[0,0])) {
      return {size_type(1)};
    }
    A[0,0] = std::sqrt(A[0,0]);
  }
  else {
    // Partition A into [A11, A12,
    //                   A21, A22],
    // where A21 is the transpose of A12.
    const size_type n1 = n / 2;
    // n2 = n - n1;
    auto A11 = submdspan(A, pair{0, n1}, pair{0, n1});
    auto A22 = submdspan(A, pair{n1, n}, pair{n1, n});

    // Factor A11
    const auto info1 = cholesky_factor(A11, t);
    if (info1.has_value()) {
      return info1;
    }

    using linalg::explicit_diagonal;
    using linalg::symmetric_matrix_rank_k_update;
    using linalg::transposed;
    if constexpr (std::is_same_v<Triangle, upper_triangle_t>) {
      // Update and scale A12
      auto A12 = submdspan(A, tuple{0, n1}, tuple{n1, n});
      using linalg::triangular_matrix_matrix_left_solve;
      // BLAS would use original triangle; we need to flip it
      triangular_matrix_matrix_left_solve(transposed(A11), opposite_triangle(t), explicit_diagonal, A12, A12);

      // A22 = A22 - A12^T * A12
      symmetric_matrix_rank_k_update(-ONE, transposed(A12), A22, t);
    }
    else {
      // Update and scale A21
      auto A21 = submdspan(A, tuple{n1, n}, tuple{0, n1});
      using linalg::triangular_matrix_matrix_right_solve;
      // BLAS would use original triangle; we need to flip it
      triangular_matrix_matrix_right_solve(transposed(A11), opposite_triangle(t), explicit_diagonal, A21, A21);

      // A22 = A22 - A21 * A21^T
      symmetric_matrix_rank_k_update(-ONE, A21, A22, t);
    }

    // Factor A22
    const auto info2 = cholesky_factor(A22, t);
    if (info2.has_value()) {
      return {info2.value() + n1};
    }
  }

  return std::nullopt;
}

int main() {

  using IT = int;
  IT N = 4;
  IT ld = 5;

  std::vector<double> vec(ld*N);

  for (IT ii = 0; ii < ld*N; ++ii) {
    auto j = ii / ld;
    auto i = ii % ld;
    if (i >= N)
      vec[ii] = -99;
    else if (i == j)
      vec[ii] = N;
    else if (i > j)
      vec[ii] = .5;
    else
      vec[ii] = -.5;
  }

  mdspan<double, extents<IT, dynamic_extent,dynamic_extent>, Kokkos::layout_left> A_full(vec.data(), ld, N);
  auto A = submdspan(A_full, pair{0, N}, pair{0, N});

  for (IT i = 0; i < A.extent(0); ++i) {
    for (IT j = 0; j < A.extent(1); ++j) {
      std::cout << "(" << i << ", " << j << "): " << A[i, j] << " CM:" << vec[i + j * ld] << std::endl;
    }
  }

  //cholesky_factor(A, linalg::lower_triangle);
  cholesky_factor(A, linalg::upper_triangle);

  for (IT i = 0; i < A.extent(0); ++i) {
    for (IT j = 0; j < A.extent(1); ++j) {
      std::cout << "(" << i << ", " << j << "): " << A[i, j] << " CM:" << vec[i + j * ld] << std::endl;
    }
  }

}
