#include <print>

struct base {
    int m_;

    auto foo(this base const& self) { return self.m_; }

    template<typename Self>
    auto bar(this Self const& self) { return self.m_; }

    template<typename Self>
    auto baz(this Self const& self) { return self.base::m_; }
};

struct derived : base {
    int m_;
};

auto main() -> int {
    base b{42};
    std::println("b.foo() = {}", b.foo());
    std::println("b.bar() = {}", b.bar());
    std::println("b.baz() = {}", b.baz());

    derived d{{42}, 99};
    std::println("d.foo() = {}", d.foo());
    std::println("d.bar() = {}", d.bar());
    std::println("d.baz() = {}", d.baz());
}
