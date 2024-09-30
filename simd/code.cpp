#include <cstddef>
#include <experimental/simd>
#include <format>
#include <iostream>
#include <numeric>
#include <cmath>
namespace stdx = std::experimental;
 
void print(auto const& a)
{
    for (std::size_t i{}; i != std::size(a); ++i)
        std::cout << a[i] << ' ';
    std::cout << '\n';
}

template<class A>
stdx::simd<int, A> simd_select_l(stdx::simd_mask<int, A> where_mask, stdx::simd<int, A> a, stdx::simd<int, A> b)
{
    where(where_mask, a) = b;
    return a;
}

stdx::native_simd<int> f(stdx::native_simd<int> x, stdx::native_simd<int> y) {
  x = simd_select_l(x < y, y, x);
  if (all_of(x <= y)) {
    std::cout << "simd_select_l works!\n";
  }
  std::cout << "min_value: " << stdx::hmin(x) << "\n";
  std::cout << "sum: " << stdx::reduce(x) << "\n";
  return x;
}

int main()
{
    const auto native_simd_length{stdx::native_simd<int>::size()};
    alignas(stdx::memory_alignment_v<stdx::native_simd<int>>)
        std::array<int, native_simd_length * 2> a{};
    std::iota(a.begin(), a.end(), 0);
    print(a);

    std::array<int, native_simd_length * 2> c;
 
    for (std::size_t i{}; i < a.size(); i += native_simd_length) {
        std::cout << std::format("[i: {}] y: ", i);
        stdx::native_simd<int> x;
        stdx::native_simd<int> y{[](int k){ return k; }};
        print(y);
        x.copy_from(&a[i], stdx::vector_aligned);
        const auto z = x + y;
        z.copy_to(&c[i], stdx::element_aligned);
    }
    print(c);

    stdx::native_simd<int> A{3};
    stdx::native_simd<int> B{[](int i) { return 2 * i; } };

    print(A);
    print(B);
    const auto K = f(A, B);
    print(K);
    const stdx::native_simd<double> x([](int i) { return i; });
    print(x);
}

void print_cos_sin(stdx::native_simd<double> x) {
    std::cout << "cos²(x) + sin²(x) = ";
    print(stdx::pow(stdx::cos(x), 2) + stdx::pow(stdx::sin(x), 2));
}
