#include <print>
#include <concepts>

namespace traditional {

template<class T>
struct add_postfix_increment {
    auto operator++(int) {
        auto& this_ = static_cast<T&>(*this);
        auto tmp = this_;
        ++this_;
        return tmp;
    }
};

struct A : add_postfix_increment<A> {
    using add_postfix_increment<A>::operator++;
    A& operator++() { ++m_; return *this; }
    int m_ = 0;
};

} // namespace traditional

namespace deduce_this {

struct add_postfix_increment {
    template<typename Self>
    requires requires(Self&& self) { {++self} -> std::convertible_to<std::remove_cvref_t<Self>>; }
    auto operator++(this Self&& self, int) {
        auto tmp = self;
        ++self;
        return tmp;
    }
};

struct A : add_postfix_increment {
    using add_postfix_increment::operator++;
    A& operator++() { ++m_; return *this; }
    int m_ = 0;
};

} // namespace deduce_this

auto main() ->int {
    {
        using namespace traditional;
        A a0;
        ++a0;
        auto a1 = a0++;
        std::println("a0.m_ = {}", a0.m_);
        std::println("a1.m_ = {}", a1.m_);
    }

    {
        using namespace deduce_this;
        A a0;
        ++a0;
        auto a1 = a0++;
        std::println("a0.m_ = {}", a0.m_);
        std::println("a1.m_ = {}", a1.m_);
    }
}
