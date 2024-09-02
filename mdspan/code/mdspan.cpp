#include <experimental/mdspan>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

#if defined(__cpp_multidimensional_subscript) and                              \
    (__cpp_multidimensional_subscript >= 202110L)

void extents_snippets() {
  auto ext1 =
      std::extents<int, std::dynamic_extent, 3, std::dynamic_extent, 4>{42, 43};
  static_assert(decltype(ext1)::rank() == 4);
  static_assert(decltype(ext1)::rank_dynamic() == 2);
  static_assert(decltype(ext1)::static_extent(0) == std::dynamic_extent);
  assert(ext1.extent(0) == 42);
  assert(ext1.extent(2) == 43);
  static_assert(decltype(ext1)::static_extent(1) == 3);

  auto ext2 = std::extents<std::uint8_t, 3, 4>{};
  static_assert(decltype(ext2)::static_extent(0) == 3);
  static_assert(decltype(ext2)::static_extent(1) == 4);

  auto ext3 = std::extents{42, 44};
  static_assert(decltype(ext3)::static_extent(42) == std::dynamic_extent);
  assert(ext3.extent(0) == 42);
  static_assert(std::is_same_v<std::size_t, decltype(ext3)::index_type>);

  auto ext4 = std::dextents<int, 3>{42, 43, 44};
}

void layout_strides() {
  std::vector<float> v(100);

  // using strided_md_span = std::mdspan<float, std::dextents<std::size_t, 3>,
  // std::layout_stride>; auto s = strided_md_span(v.data(), {
  // std::dextents<std::size_t, 3>(2, 5, 10), std::array<std::size_t, 3>{ 5, 1,
  // 10 } });

  auto s =
      std::mdspan(v.data(), std::layout_stride::mapping(std::extents(2, 5, 10),
                                                        std::array{5, 1, 10}));

  s[1, 2, 3] = 42;
  assert((v[2 + 5 + 30] == s[1, 2, 3]));
}

enum class DeviceType { CPU, CUDA };

template <class ElementType, DeviceType Device> struct host_device_protector {
  using offset_policy = host_device_protector;
  using element_type = ElementType;
  using reference = ElementType &;
  using data_handle_type = ElementType *;

  constexpr data_handle_type offset(data_handle_type p,
                                    size_t i) const noexcept {
    return p + i;
  }

  constexpr reference access(data_handle_type p, size_t i) const noexcept {
#ifdef __CUDA_ARCH__
    static_assert(Device == DeviceType::CUDA);
#else
    static_assert(Device == DeviceType::CPU);
#endif
    return p[i];
  }
};

void test_host_device_protector() {
  float dev_ptr[] = {1, 2, 3, 4};
  auto s = std::mdspan<float, std::dextents<int, 2>, std::layout_right,
                       host_device_protector<float, DeviceType::CPU>>{
      dev_ptr, std::dextents<int, 2>{2, 2}};
  std::cout << s[1, 2] << std::endl;
}

auto compute_strides(auto size, auto... sizes) {}

template <class T, std::size_t N> struct my_mdarray {
  std::vector<T> data_;
  std::array<int, N> sizes_;

  my_mdarray(std::convertible_to<int> auto... sizes)
      : data_((sizes * ...)), sizes_{sizes...} {
    std::iota(data_.begin(), data_.end(), 0);
  }

  operator std::mdspan<T, std::dextents<int, N>>() {
    return {data_.data(), std::dextents<int, N>{sizes_}};
  }

  std::mdspan<T, std::dextents<int, N>> mdspan() {
    return {data_.data(), std::dextents<int, N>{sizes_}};
  }
};

template <class T, class E, class L, class A>
T apply_mdspan(std::mdspan<T, E, L, A> s) {
  return s[1, 2];
}

int apply_int_matrix(std::mdspan<int, std::dextents<int, 2>> s) {
  return s[1, 2];
}

void test_my_md_array() {
  auto a = my_mdarray<int, 2>{3, 4};
  std::cout << a.data_.size() << std::endl;
  std::cout << apply_mdspan(a.mdspan()) << std::endl;
  std::cout << apply_int_matrix(a);
}

template <class WrappedAccessor, class ScaleType> struct scaled_accessor {
  ScaleType factor;
  WrappedAccessor a;
  using element_type = std::add_const_t<
      decltype(std::declval<typename WrappedAccessor::element_type>() *
               factor)>;
  using data_handle_type = WrappedAccessor::data_handle_type;
  using reference = std::remove_const_t<element_type>;
  using offset_policy = WrappedAccessor::offset_policy;

  constexpr data_handle_type offset(data_handle_type p,
                                    size_t i) const noexcept {
    return p + i;
  }
  constexpr reference access(data_handle_type p, size_t i) const noexcept {
    return p[i] * factor;
  }
};

template <class T, class E, class L, class A, class U,
          class SA = scaled_accessor<A, U>>
auto scaled(U factor, std::mdspan<T, E, L, A> m) {
  return std::mdspan<typename SA::element_type, E, L, SA>{
      m.data_handle(), m.mapping(), SA{factor, m.accessor()}};
}

namespace impl {

template <class SizeType, std::size_t E0, std::size_t E1>
constexpr std::extents<SizeType, E1, E0>
transpose_extents(std::extents<SizeType, E0, E1> const &e) {
  return std::extents<SizeType, E1, E0>{e.extent(1), e.extent(0)};
}

template <class E>
using transposed_extents_t = decltype(transpose_extents(std::declval<E>()));

void test_transpose_extents() {
  auto e1 = std::dextents<int, 2>{2, 3};
  auto e2 = std::dextents<int, 2>{3, 2};
  auto e3 = transpose_extents(e1);
  static_assert(std::is_same_v<decltype(e2), decltype(e3)>);
  assert(e2.extent(0) == e3.extent(0));
  assert(e2.extent(1) == e3.extent(1));

  auto e4 = std::extents<int, std::dynamic_extent, 2>{3};
  auto e5 = std::extents<int, 2, std::dynamic_extent>{3};
  auto e6 = transpose_extents(e4);
  static_assert(std::is_same_v<decltype(e5), decltype(e6)>);
  assert(e5.extent(0) == e6.extent(0));
  assert(e5.extent(1) == e6.extent(1));
}
} // namespace impl

template <class Layout> struct transposed_layout {
  template <class Extents>
  struct mapping
      : public Layout::template mapping<impl::transposed_extents_t<Extents>> {
    using _base_mapping =
        typename Layout::template mapping<impl::transposed_extents_t<Extents>>;
    using extents_type = Extents;

    // mapping(_base_mapping const &base)
    //     : _base_mapping(base),
    //       extents_(impl::transpose_extents(base.extents())) {}

    template <class I0, class I1>
    constexpr _base_mapping::index_type operator()(I0 i0,
                                                   I1 i1) const noexcept {
      return _base_mapping::operator()(i1, i0);
    }

    constexpr extents_type // std requires const& return
    extents() const noexcept {
      return impl::transpose_extents(_base_mapping::extents());
    }
    // constexpr const extents_type &extents() const noexcept { return extents_;
    // }

    // private:
    //   Extents extents_;
  };
};

template <class T, class E, class L, class A,
          class TE = decltype(impl::transpose_extents(std::declval<E>())),
          class TL = transposed_layout<L>>
auto transposed(std::mdspan<T, E, L, A> m) {
  // Note: this is not a standard conforming implementation, e.g.
  // std::layout_right <-> std::layout_left conversion is missing
  return std::mdspan<T, TE, TL, A>{
      m.data_handle(), {m.mapping()}, m.accessor()};
}

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

void test_transposed() {
  auto data = initialized_vector{};
  auto m = data.to_mdspan();

  auto t = transposed(m);
  for (std::size_t i : std::ranges::iota_view(0, 7)) {
    for (std::size_t j : std::ranges::iota_view(0, 9)) {
      assert((m[i, j] == t[j, i]));
    }
  }
  assert(m.extent(0) == t.extent(1));
  assert(m.extent(1) == t.extent(0));
  static_assert(decltype(m)::rank() == decltype(t)::rank());
  static_assert(std::is_trivially_copyable_v<decltype(t)>);
  static_assert(decltype(t)::rank_dynamic() == 1);
  static_assert(
      std::is_same_v<decltype(t)::index_type, decltype(m)::index_type>);
}

int main() {
  test_my_md_array();
  extents_snippets();
  layout_strides();
  test_host_device_protector();
  impl::test_transpose_extents();
  test_transposed();

  std::vector<float> v(100);

  using ext_t = std::extents<int, std::dynamic_extent, std::dynamic_extent>;
  auto v_ext = ext_t{10, 10};
  auto v_span_explicit = std::mdspan<float, ext_t>{v.data(), v_ext};

  auto v_span = std::mdspan(v.data(), std::extents{10, 10});
  for (std::size_t i : std::ranges::iota_view(0, 9)) {
    for (std::size_t j : std::ranges::iota_view(0, 9)) {
      v_span[i, j] = i * 10 + j;
    }
  }

  std::cout << v[15] << std::endl;
  std::cout << scaled(2.0, v_span)[3, 5] << std::endl;
  std::cout << transposed(v_span)[5, 3] << std::endl;
}

#else
int main() {}
#endif
