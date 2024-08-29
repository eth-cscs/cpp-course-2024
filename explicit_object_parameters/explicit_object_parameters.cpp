#include <type_traits>
#include <utility>

struct A {
    int m_ = 42;

    int& foo(this A& self) {
        return self.m_;
    }
    int const& foo(this A const& self) {
        return self.m_;
    }
    int&& foo(this A&& self) {
        return std::move(self.m_);
    }
    int const&& foo(this A const&& self) {
        return std::move(self.m_);
    }
};

struct B {
    int m_ = 24;
    template<typename Self>
    decltype(auto) foo(this Self&& self)  {
        return std::forward_like<Self>(self.m_);
    }
};

auto main() -> int {
    B b;
    B const cb;

    static_assert(std::is_same_v<
        decltype(b.foo()),
        int&>);
    static_assert(std::is_same_v<
        decltype(cb.foo()),
        int const&>);
    static_assert(std::is_same_v<
        decltype(std::move(b).foo()),
        int&&>);
    static_assert(std::is_same_v<
        decltype(std::move(cb).foo()),
        int const&&>);
}
