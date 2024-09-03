#include <experimental/mdarray>

#include <iostream>
#include <memory>
template <class T> struct FancyAllocator : public std::allocator<T> {
  [[nodiscard]] T *allocate(std::size_t n) {
    std::cout << "Allocating " << n << " ints" << std::endl;
    T *ptr = std::allocator<T>::allocate(n);
    return ptr;
  }
};

int main() {
  std::experimental::mdarray<double, std::dextents<int, 2>> a{3, 4};
  std::cout << a[1, 1] << std::endl;

  std::vector<double> v(42, 3.14);
  std::experimental::mdarray<double, std::dextents<int, 2>> b{
      std::dextents<int, 2>{2, 21}, v};
  std::cout << b[1, 1] << std::endl;

  std::experimental::mdarray<double, std::dextents<int, 2>> c{
      std::dextents<int, 2>{2, 21}, std::move(v)};
  std::cout << c[1, 1] << std::endl;

  std::experimental::mdarray<int, std::dextents<int, 2>, std::layout_right,
                             std::vector<int, FancyAllocator<int>>>
      d{std::dextents<int, 2>{2, 21}, FancyAllocator<int>{}};
  std::cout << d[0, 0] << std::endl;
  std::cout << d[1, 1] << std::endl;
}
