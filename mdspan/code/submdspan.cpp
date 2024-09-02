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
                       std::extents<int, std::dynamic_extent, 10>{8, 10});
  }
};

int main() {
  auto data = initialized_vector{};
  auto m = data.to_mdspan();

  auto s0 = std::submdspan(m, 4, 2);
  static_assert(decltype(s0)::rank() == 0);
  assert(s0[] == 42);

  auto s1 = std::submdspan(m, std::tuple(1, 3), std::tuple(3, 5));
  std::cout << s1[0, 0] << std::endl;
  std::cout << s1[1, 1] << std::endl;
  assert((s1[0, 0] == 13));
  assert((s1[1, 1] == 24));

  auto s2 = std::submdspan(m,
                           std::tuple(std::integral_constant<int, 1>{},
                                      std::integral_constant<int, 3>{}),
                           std::tuple(3, 5));
  static_assert(decltype(s2)::rank() == 2);
  static_assert(decltype(s2)::static_extent(0) == 2);
  std::cout << s2[0, 0] << std::endl;
  std::cout << s2[1, 1] << std::endl;
  assert((s2[0, 0] == 13));
  assert((s2[1, 1] == 24));

  //   auto s2 = std::submdspan(
  //       m, 0, std::strided_slice{.offset = 2, .extent = 2, .stride = 2});
  //   //???
  //   static_assert(decltype(s2)::static_extent(0) == std::dynamic_extent);
  //   static_assert(decltype(s2)::rank() == 1);
  //   std::cout << s2.extent(0) << std::endl;
  //   std::cout << s2[0] << std::endl;
  //   std::cout << s2[1] << std::endl;
  //   assert(s2.extent(0) == 2);

  std::cout << "passed!" << std::endl;
}
