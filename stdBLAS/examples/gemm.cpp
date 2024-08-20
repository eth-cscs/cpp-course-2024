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
#include <chrono>

template <class clock = std::chrono::high_resolution_clock>
class Timer {
  using time_point = std::chrono::time_point<clock>;

  time_point start_;

  inline time_point now() const {
    return clock::now();
  }

public:
  Timer() : start_(now()) {}

  double elapsed() const {
    using namespace std::chrono;
    return duration_cast<duration<double>>(now() - start_).count();
  }
};

using Kokkos::mdspan;
using Kokkos::extents;
using Kokkos::layout_left;
using Kokkos::layout_right;
using Kokkos::Experimental::layout_left_padded;
using Kokkos::Experimental::layout_right_padded;
using Kokkos::default_accessor;
namespace linalg = Kokkos::Experimental::linalg;
using linalg::lower_triangle_t;
using linalg::upper_triangle_t;
using linalg::conjugated;
using linalg::conjugate_transposed;
using linalg::transposed;
using linalg::scaled;
#if defined(__cpp_lib_span)
#include <span>
  using std::dynamic_extent;
#else
  using Kokkos::dynamic_extent;
#endif

template<class MatA, class MatB, class MatC>
void gemm_std(MatA A, MatB B, MatC C) {
  linalg::matrix_product(A, B, C, C);
}

template<class MatA, class MatB, class MatC>
void gemm_1(MatA A, MatB B, MatC C) {
  static_assert(std::is_same_v<typename MatA::value_type, typename MatB::value_type>);
  static_assert(std::is_same_v<typename MatA::value_type, typename MatC::value_type>);
  using value_type = typename MatA::value_type;
  static_assert(std::is_same_v<typename MatA::index_type, typename MatB::index_type>);
  static_assert(std::is_same_v<typename MatA::index_type, typename MatC::index_type>);
  using INT = typename MatA::index_type;

  const INT m = A.extent(0);
  const INT k = A.extent(1);
  const INT n = B.extent(1);

  if (m != C.extent(0) || k != B.extent(0) || n != C.extent(1))
    std::terminate();

  for (INT j = 0; j < n; ++j) {
    for (INT l = 0; l < k; ++l) {
      auto blj = B[l, j];
      for (INT i = 0; i < m; ++i) {
        C[i, j] += A[i, l] * blj;
      }
    }
  }
}

template<class MatA, class MatB, class MatC>
void gemm_2(MatA A, MatB B, MatC C) {
  static_assert(std::is_same_v<typename MatA::value_type, typename MatB::value_type>);
  static_assert(std::is_same_v<typename MatA::value_type, typename MatC::value_type>);
  using value_type = typename MatA::value_type;
  static_assert(std::is_same_v<typename MatA::index_type, typename MatB::index_type>);
  static_assert(std::is_same_v<typename MatA::index_type, typename MatC::index_type>);
  using INT = typename MatA::index_type;

  const INT m = A.extent(0);
  const INT k = A.extent(1);
  const INT n = B.extent(1);

  if (m != C.extent(0) || k != B.extent(0) || n != C.extent(1))
    std::terminate();

  if (k >= 64) {
    auto k1 = k / 4 * 4;
    for (INT i = 0; i < m; ++i) {
      for (INT j = 0; j < n; ++j) {
        std::array<value_type,4> cij{{value_type{}, value_type{}, value_type{}, value_type{}}};
        for (INT l = 0; l < k1; l += 4) {
          cij[0] += A[i, l  ] * B[l  , j];
          cij[1] += A[i, l+1] * B[l+1, j];
          cij[2] += A[i, l+2] * B[l+2, j];
          cij[3] += A[i, l+3] * B[l+3, j];
        }
        for (INT l = k1; l < k; ++l) {
          cij[0] += A[i, l] * B[l, j];
        }
        C[i, j] += cij[0] + cij[1] + cij[2] + cij[3];
      }
    }

  }
  else {
    for (INT i = 0; i < m; ++i) {
      for (INT j = 0; j < n; ++j) {
        value_type cij{};
        for (INT l = 0; l < k; ++l) {
          cij += A[i, l] * B[l, j];
        }
        C[i, j] += cij;
      }
    }
  }
}

template<class MatA, class MatB, class MatC>
void gemm_3(MatA A, MatB B, MatC C) {
  static_assert(std::is_same_v<typename MatA::value_type, typename MatB::value_type>);
  static_assert(std::is_same_v<typename MatA::value_type, typename MatC::value_type>);
  using value_type = typename MatA::value_type;
  static_assert(std::is_same_v<typename MatA::index_type, typename MatB::index_type>);
  static_assert(std::is_same_v<typename MatA::index_type, typename MatC::index_type>);
  using INT = typename MatA::index_type;

  const INT m = A.extent(0);
  const INT k = A.extent(1);
  const INT n = B.extent(1);

  if (m != C.extent(0) || k != B.extent(0) || n != C.extent(1))
    std::terminate();

  for (INT i = 0; i < m; ++i) {
    for (INT l = 0; l < k; ++l) {
      auto ail = A[i, l];
      for (INT j = 0; j < n; ++j) {
        C[i, j] += ail * B[l, j];
      }
    }
  }
}

template<class Layout>
static constexpr bool is_left_v = false;

template<>
constexpr bool is_left_v<layout_left> = true;

template <size_t PaddingValue>
static constexpr bool is_left_v<layout_left_padded<PaddingValue>> = true;

template<class Layout>
static constexpr bool is_right_v = false;

template<>
constexpr bool is_right_v<layout_right> = true;

template <size_t PaddingValue>
static constexpr bool is_right_v<layout_right_padded<PaddingValue>> = true;

template<class MatA, class MatB, class MatC>
void gemm(MatA A, MatB B, MatC C) {
  if constexpr (is_left_v<typename MatA::layout_type> && is_left_v<typename MatC::layout_type>)
    gemm_1(A, B, C);
  else if constexpr (is_right_v<typename MatB::layout_type> && is_right_v<typename MatC::layout_type>)
    gemm_3(A, B, C);
  else
    gemm_2(A, B, C);
}

#ifdef GEMMBLAS

extern "C" void sgemm_(...);
extern "C" void dgemm_(...);
extern "C" void cgemm_(...);
extern "C" void zgemm_(...);

void gemm_(const char* ta, const char* tb, const int* m, const int* n, const int* k, const float* alpha, const float* a, const int* lda, const float* b, const int* ldb, const float* beta, float* c, const int* ldc) {
  sgemm_(ta, tb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}
void gemm_(const char* ta, const char* tb, const int* m, const int* n, const int* k, const double* alpha, const double* a, const int* lda, const double* b, const int* ldb, const double* beta, double* c, const int* ldc) {
  dgemm_(ta, tb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}
void gemm_(const char* ta, const char* tb, const int* m, const int* n, const int* k, const std::complex<float>* alpha, const std::complex<float>* a, const int* lda, const std::complex<float>* b, const int* ldb, const std::complex<float>* beta, std::complex<float>* c, const int* ldc) {
  cgemm_(ta, tb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}
void gemm_(const char* ta, const char* tb, const int* m, const int* n, const int* k, const std::complex<double>* alpha, const std::complex<double>* a, const int* lda, const std::complex<double>* b, const int* ldb, const std::complex<double>* beta, std::complex<double>* c, const int* ldc) {
  zgemm_(ta, tb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}

#ifdef GEMM
#error "Do not define GEMM when GEMMBLAS is defined"
#endif
#define GEMM gemm_blas

template<class Accessor>
static constexpr bool is_accessor_scaled_v = false;

template<class ScalingFactor, class NestedAccessor>
static constexpr bool is_accessor_scaled_v<linalg::accessor_scaled<ScalingFactor, NestedAccessor>> = true;

template<class Accessor>
static constexpr bool is_conjugated_accessor_v = false;

template<class NestedAccessor>
static constexpr bool is_conjugated_accessor_v<linalg::conjugated_accessor<NestedAccessor>> = true;

template<class Accessor>
static constexpr bool is_default_accessor_v = false;

template<class T>
static constexpr bool is_default_accessor_v<default_accessor<T>> = true;

template <class T, class A>
auto get_scaling(const A& a) {
  if constexpr (is_accessor_scaled_v<A>)
    return a.scaling_factor() * get_scaling<T>(a.nested_accessor());

  else if constexpr (is_conjugated_accessor_v<A>)
    return std::conj(get_scaling<T>(a.nested_accessor()));

  else {
    static_assert(is_default_accessor_v<A>);
    static_assert(std::is_same_v<std::remove_cv_t<typename A::element_type>, std::remove_cv_t<T>>);
    return T{1.};
  }
}

template <class A>
constexpr bool get_conjugated() {

  if constexpr (is_accessor_scaled_v<A>) {
    using nested_accessor_t = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<A>().nested_accessor())>>;
    return get_conjugated<nested_accessor_t>();
  }

  else if constexpr (is_conjugated_accessor_v<A>) {
    using nested_accessor_t = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<A>().nested_accessor())>>;
    return !get_conjugated<nested_accessor_t>();
  }

  else {
    static_assert(is_default_accessor_v<A>);
    return false;
  }
}

// w.r.t column major layout
enum class Op {Left, ConjLeft, Right, ConjRight};

template <class Mat>
constexpr Op get_op() {
  constexpr bool is_left = is_left_v<typename Mat::layout_type>;
  constexpr bool is_right = is_right_v<typename Mat::layout_type>;
  constexpr bool is_conjugated = get_conjugated<typename Mat::accessor_type>();

  static_assert(is_left || is_right);

  if constexpr (is_conjugated) {
    if constexpr (is_left)
      return Op::ConjLeft;
    else
      return Op::ConjRight;
  }
  if constexpr (is_left)
    return Op::Left;
  return Op::Right;
}

template <Op op>
constexpr char get_op_blas_col_major() {
  if constexpr (op == Op::Left)
    return 'N';
  else if constexpr (op == Op::Right)
    return 'T';
  else {
    static_assert(op == Op::ConjRight);
    return 'C';
  }
}

template <Op op>
constexpr char get_op_blas_row_major() {
  if constexpr (op == Op::Right)
    return 'N';
  else if constexpr (op == Op::Left)
    return 'T';
  else {
    static_assert(op == Op::ConjLeft);
    return 'C';
  }
}

template <class Mat>
auto get_ld(Mat& A) {
  constexpr bool is_left = is_left_v<typename Mat::layout_type>;
  constexpr bool is_right = is_right_v<typename Mat::layout_type>;

  if constexpr (is_right)
    return std::max(1, A.stride(0));

  else {
    static_assert(is_left);
    return std::max(1, A.stride(1));
  }

}

template<class MatA, class MatB, class MatC>
void gemm_blas(MatA A, MatB B, MatC C) {
  static_assert(std::is_same_v<typename MatA::value_type, typename MatB::value_type>);
  static_assert(std::is_same_v<typename MatA::value_type, typename MatC::value_type>);
  using value_type = typename MatA::value_type;
  static_assert(std::is_same_v<typename MatA::index_type, typename MatB::index_type>);
  static_assert(std::is_same_v<typename MatA::index_type, typename MatC::index_type>);
  using INT = typename MatA::index_type;

  const INT m = A.extent(0);
  const INT k = A.extent(1);
  const INT n = B.extent(1);

  value_type alpha = get_scaling<value_type>(A.accessor()) * get_scaling<value_type>(B.accessor());
  value_type beta{1.f};

  if (m != C.extent(0) || k != B.extent(0) || n != C.extent(1))
    std::terminate();

  const value_type* a = A.data_handle();
  const value_type* b = B.data_handle();
  value_type* c = C.data_handle();

  constexpr Op opA = get_op<MatA>();
  constexpr Op opB = get_op<MatB>();
  constexpr Op opC = get_op<MatC>();

  const int m_ = m;
  const int k_ = k;
  const int n_ = n;

  const int lda = get_ld(A);
  const int ldb = get_ld(B);
  const int ldc = get_ld(C);

  if constexpr (opC == Op::Left) {
    constexpr char opA_ = get_op_blas_col_major<opA>();
    constexpr char opB_ = get_op_blas_col_major<opB>();
    gemm_(&opA_, &opB_, &m_, &n_, &k_, &alpha, a, &lda, b, &ldb, &beta, c, &ldc);
  }
  else {
    static_assert(opC == Op::Right); 
    constexpr char opA_ = get_op_blas_row_major<opA>();
    constexpr char opB_ = get_op_blas_row_major<opB>();
    gemm_(&opB_, &opA_, &n_, &m_, &k_, &alpha, b, &ldb, a, &lda, &beta, c, &ldc);
  }
}

#endif


#ifndef GEMM
#define GEMM gemm_std
#endif

template<class T>
struct is_complex : std::false_type {};

template<class T>
struct is_complex<std::complex<T>> : std::true_type {};

template<class T>
constexpr bool is_complex_v = is_complex<T>::value;

template<class T>
struct basetype {
  using type = T;
};

template<class T>
struct basetype<std::complex<T>> {
  using type = T;
};

template<class T>
using basetype_t = basetype<T>::type;


template<class MatA, class MatB, class MatC>
void invoke_gemm2(MatA A, MatB B, MatC C) {
#ifdef SCALED_A
  using T = basetype_t<typename MatA::value_type>;
  auto sc = static_cast<T>(1);
  sc /= static_cast<T> (A.extent(1));
  GEMM(scaled(sc, A), B, C);
#elif defined(SCALED_B)
  using T = basetype_t<typename MatB::value_type>;
  auto sc = static_cast<T>(1);
  sc /= static_cast<T> (A.extent(1));
  GEMM(A, scaled(sc, B), C);
#else
  GEMM(A, B, C);
#endif
}

template<class MatA, class MatB, class MatC>
void invoke_gemm(std::string ts, MatA A, MatB B, MatC C) {
  if (ts[0] == 'N') {
    switch (ts[1]) {
      case 'N':
        invoke_gemm2(A, B, C);
        return;
      case 'T':
        invoke_gemm2(A, transposed(B), C);
        return;
      case 'C':
        invoke_gemm2(A, conjugated(transposed(B)), C);
        return;
    }
  }
  else if (ts[0] == 'T') {
    auto AT = transposed(A);
    switch (ts[1]) {
      case 'N':
        invoke_gemm2(AT, B, C);
        return;
      case 'T':
        invoke_gemm2(AT, transposed(B), C);
        return;
      case 'C':
        invoke_gemm2(AT, conjugated(transposed(B)), C);
        return;
    }
  }
  else if (ts[0] == 'C') {
    auto AC = conjugate_transposed(A);
    switch (ts[1]) {
      case 'N':
        invoke_gemm2(AC, B, C);
        return;
      case 'T':
        invoke_gemm2(AC, transposed(B), C);
        return;
      case 'C':
        invoke_gemm2(AC, conjugated(transposed(B)), C);
        return;
    }
  }
  std::terminate();

}

#define xstr(s) str(s)
#define str(s) #s

// Warning: polar_if_complex, basetype_t and conj_if_complex don't work with custom defined complex types.

template<class T>
struct polar_if_complex {
  static T get(T r, T) noexcept {
    return r;
  }
};

template<class T>
struct polar_if_complex<std::complex<T>> {
  static std::complex<T> get(T r, T phi) noexcept {
    return std::polar(r, phi);
  }
};

template<class T>
T conj_if_complex(T x) {
  return x;
}

template<class T>
std::complex<T> conj_if_complex(std::complex<T> x) {
  return std::conj(x);
}

template <class MatA, class F>
void set_val(char t, MatA A, const F&& val) {
  using INT = typename MatA::index_type;

  if (t == 'N') {
    for (INT i = 0; i < A.extent(0); ++i) {
      for (INT j = 0; j < A.extent(1); ++j)
        A[i, j] = val(i, j);
    }
  }
  else if (t == 'T') {
    for (INT i = 0; i < A.extent(0); ++i) {
      for (INT j = 0; j < A.extent(1); ++j)
        A[i, j] = val(j, i);
    }
  }
  else if (t == 'C') {
    for (INT i = 0; i < A.extent(0); ++i) {
      for (INT j = 0; j < A.extent(1); ++j)
        A[i, j] = conj_if_complex(val(j, i));
    }
  }
  else {
    std::terminate();
  }
}

template<class MatA, class MatB, class MatC>
void test_gemm(std::string ts, MatA A, MatB B, MatC C)
{
  static_assert(std::is_same_v<typename MatA::value_type, typename MatB::value_type>);
  static_assert(std::is_same_v<typename MatA::value_type, typename MatC::value_type>);
  using value_type = typename MatA::value_type;
  static_assert(std::is_same_v<typename MatA::index_type, typename MatB::index_type>);
  static_assert(std::is_same_v<typename MatA::index_type, typename MatC::index_type>);
  using INT = typename MatA::index_type;

  auto BT = [](auto i) {return static_cast<basetype_t<value_type>>(i);};

  const INT m = C.extent(0);
  const INT k = ts[0] == 'N' ? A.extent(1) : A.extent(0);
  const INT n = C.extent(1);

  if (m != (ts[0] == 'N' ? A.extent(0) : A.extent(1)) || k != (ts[1] == 'N' ? B.extent(0) : B.extent(1)) || n != (ts[1] == 'N' ? B.extent(1) : B.extent(0)))
    std::terminate();

  set_val(ts[0], A, [m, &BT](INT i, INT l) { return polar_if_complex<value_type>::get(BT(i - m/2) / BT(l + 1), BT(i - l)); });

  set_val(ts[1], B,
#if defined(SCALED_A) || defined(SCALED_B)
          [&BT](INT l, INT j) { return polar_if_complex<value_type>::get(BT(l + 1) / BT(j + 1), BT(l - j)); }
#else
          [k, &BT](INT l, INT j) { return polar_if_complex<value_type>::get(BT(l + 1) / BT(j + 1) / BT(k), BT(l - j)); }
#endif
         );

  auto valC = [m, &BT](int i, int j){return polar_if_complex<value_type>::get(-BT(i - m/2) / BT(j + 1), BT(i - j)); };

  for (INT i = 0; i < m; ++i) {
    for (INT j = 0; j < n; ++j) {
      C[i, j] = valC(i, j);
    }
  }

  // Warmup
  invoke_gemm(ts, A, B, C);


  // reset C
  for (INT i = 0; i < m; ++i) {
    for (INT j = 0; j < n; ++j) {
      C[i, j] = valC(i, j);
    }
  }

  Timer t;
  invoke_gemm(ts, A, B, C);
  double elapsed = t.elapsed();

  std::cout << xstr(GEMM) << ", " << ts << ", "
            << "m = " << m << ", n = " << n << ", k = " << k << ", "
            << elapsed << " s, " << (is_complex_v<value_type> ? 8 : 2) * static_cast<double>(m) * static_cast<double>(n) * static_cast<double>(k) / 1e9 / elapsed << " GFlop/s" << std::endl;


  auto eps = std::numeric_limits<basetype_t<value_type>>::epsilon();

  for (INT i = 0; i < m; ++i) {
    for (INT j = 0; j < n; ++j) {
      if (std::abs(C[i, j] / valC(i, j)) > k * eps) {
        std::cout << i << ", " << j << " is wrong " << C[i, j] << std::endl;
      }
    }
  }
}

template <class T>
void test_gemm(std::string ts, int m, int n, int k) {

  std::vector<T> avec(m * k);
  std::vector<T> bvec(k * n);
  std::vector<T> cvec(m * n);

  int am = ts[0] == 'N' ? m : k;
  int an = ts[0] == 'N' ? k : m;
  int bm = ts[1] == 'N' ? k : n;
  int bn = ts[1] == 'N' ? n : k;

  if (ts[2] == 'C') { // Col-major
    mdspan<T, extents<int, dynamic_extent,dynamic_extent>, layout_left> A(avec.data(), am, an);
    mdspan<T, extents<int, dynamic_extent,dynamic_extent>, layout_left> B(bvec.data(), bm, bn);
    mdspan<T, extents<int, dynamic_extent,dynamic_extent>, layout_left> C(cvec.data(), m, n);

    test_gemm(ts, A, B, C);
  }
  else if (ts[2] == 'R') {// Row-major
    mdspan<T, extents<int, dynamic_extent,dynamic_extent>, layout_right> A(avec.data(), am, an);
    mdspan<T, extents<int, dynamic_extent,dynamic_extent>, layout_right> B(bvec.data(), bm, bn);
    mdspan<T, extents<int, dynamic_extent,dynamic_extent>, layout_right> C(cvec.data(), m, n);

    test_gemm(ts, A, B, C);
  }
  else {
    std::terminate();
  }
}

void print_usage_and_terminate() noexcept {
  std::cout << "Usage: gemm {sdcz} [{NTC}{NTC}{CR} [<m> [<n> [<k>]]]]" << std::endl;
  std::terminate();
}

int main(int argc, const char* const* argv) {

  int m = 1024;
  int n = 1000;
  int k = 1280;

  std::string ts = "NNC";

  if (argc < 2)
    print_usage_and_terminate();

  if (argc >= 3)
  {
    if (!(argv[2][0] == 'N' || argv[2][0] == 'T' || argv[2][0] == 'C'))
      print_usage_and_terminate();
    if (!(argv[2][1] == 'N' || argv[2][1] == 'T' || argv[2][1] == 'C'))
      print_usage_and_terminate();
    if (!(argv[2][2] == 'C' || argv[2][2] == 'R'))
      print_usage_and_terminate();

    ts[0] = argv[2][0];
    ts[1] = argv[2][1];
    ts[2] = argv[2][2];
  }
  if (argc >= 4)
    m = std::atoi(argv[3]);
  if (argc >= 5)
    n = std::atoi(argv[4]);
  if (argc >= 6)
    k = std::atoi(argv[5]);

  if (argv[1][0] == 's')
    test_gemm<float>(ts, m, n, k);
  else if (argv[1][0] == 'd')
    test_gemm<double>(ts, m, n, k);
  else if (argv[1][0] == 'c')
    test_gemm<std::complex<float>>(ts, m, n, k);
  else if (argv[1][0] == 'z')
    test_gemm<std::complex<double>>(ts, m, n, k);
  else
    print_usage_and_terminate();
}
