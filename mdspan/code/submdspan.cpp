#include <experimental/mdspan>
#include <iostream>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

struct initialized_vector {
  std::vector<float> data;
  initialized_vector() : data(80) {
    auto v_span = std::mdspan(data.data(), std::extents{8, 10});
    for (std::size_t i : std::ranges::iota_view(0, 7)) {
      for (std::size_t j : std::ranges::iota_view(0, 9)) {
        v_span[i, j] = i * 10 + j;
      }
    }
  }
  auto to_mdspan() {
    return std::mdspan(data.data(),
                       std::extents<int, std::dynamic_extent, 10>{8});
  }
};

int main() {
  auto data = initialized_vector{};
  auto m = data.to_mdspan();

  // m = std::mdspan(..., std::extents<int, std::dynamic_extent, 10>{8}) with
  // m[i, j] == i * 10 + j

  auto s1 = std::submdspan(m, std::tuple(1, 3), std::full_extent);
  assert(s1.extent(0) == 2);
  assert((s1[0, 0] == 10));
  assert((s1[1, 1] == 21));

  auto s2 = std::submdspan(m, std::tuple(1, 3), 5);
  assert(s2[0] == 15);
  assert(s2[1] == 25);

  auto s3 = std::submdspan(m, 4, 2);
  static_assert(decltype(s3)::rank() == 0);
  assert(s3[] == 42);

  auto s4 = std::submdspan(m,
                           std::tuple(std::integral_constant<int, 1>{},
                                      std::integral_constant<int, 3>{}),
                           std::tuple(3, 5));
  static_assert(decltype(s4)::rank() == 2);
  static_assert(decltype(s4)::static_extent(0) == 2);
  assert((s4[0, 0] == 13));
  assert((s4[1, 1] == 24));

  auto s5 = std::submdspan(
      m, 0, std::strided_slice{.offset = 2, .extent = 4, .stride = 2});
  static_assert(decltype(s5)::rank() == 1);
  static_assert(decltype(s5)::static_extent(0) == std::dynamic_extent);
  assert(s5.extent(0) == 2);

  auto s6 = std::submdspan(
      m, 0,
      std::strided_slice{.offset = 2,
                         .extent = std::integral_constant<int, 4>{},
                         .stride = std::integral_constant<int, 1>{}});
  static_assert(decltype(s6)::rank() == 1);
  static_assert(decltype(s6)::static_extent(0) == 4);

  std::cout << "passed!" << std::endl;
}
