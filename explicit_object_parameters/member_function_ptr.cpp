#include <functional>
#include <print>
#include <type_traits>

struct A {
    int m_;
    int foo(int n) const noexcept { return m_*n; }
    int bar(this A const& self, int n) noexcept { return self.m_*n; }
    template<typename Self>
    int baz(this Self& self, int n) noexcept { return self.m_*n; }
};

auto main() -> int {

    auto a = A{42};

    auto foo_ptr = &A::foo;
    static_assert(std::is_same_v<decltype(foo_ptr), int (A::*)(int) const noexcept>);
    std::println("{}", (a.*foo_ptr)(2));
    std::println("{}", std::invoke(foo_ptr, a, 2));

    auto bar_ptr = &A::bar;
    static_assert(std::is_same_v<decltype(bar_ptr), int (*)(A const&, int) noexcept>);
    std::println("{}", bar_ptr(a, 2));
    std::println("{}", std::invoke(bar_ptr, a, 2));

    auto baz_ptr = &A::template baz<A const&>;
    static_assert(std::is_same_v<decltype(baz_ptr), int (*)(A const&, int) noexcept>);
    std::println("{}", baz_ptr(a, 2));
    std::println("{}", std::invoke(baz_ptr, a, 2));
}
