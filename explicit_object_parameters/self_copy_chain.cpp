#include <concepts>
#include <vector>
#include <algorithm>
#include <print>

namespace traditional_free_v1 {

struct my_vector {
    std::vector<int> m_;

    my_vector(std::vector<int> m) noexcept : m_{std::move(m)} { std::println("my_vector::my_vector()"); }
    my_vector(my_vector const& other) noexcept : m_{other.m_} { std::println("my_vector::my_vector(my_vector const&)"); }
    my_vector(my_vector&& other) noexcept : m_{std::move(other.m_)} { std::println("my_vector::my_vector(my_vector&&)"); }
};

template<typename Compare>
auto sorted_by(my_vector v, Compare comp) {
    std::sort(v.m_.begin(), v.m_.end(), comp);
    return v;
}

auto negated(my_vector v) {
    std::transform(v.m_.begin(), v.m_.end(), v.m_.begin(), [](this auto, auto const& x) noexcept { return -x; });
    return v;
}

} // namespace traditional_free_v1

namespace traditional_free_v2 {

struct my_vector {
    std::vector<int> m_;

    my_vector(std::vector<int> m) noexcept : m_{std::move(m)} { std::println("my_vector::my_vector()"); }
    my_vector(my_vector const& other) noexcept : m_{other.m_} { std::println("my_vector::my_vector(my_vector const&)"); }
    my_vector(my_vector&& other) noexcept : m_{std::move(other.m_)} { std::println("my_vector::my_vector(my_vector&&)"); }
};

template<typename Compare, typename Vector>
auto sorted_by(Vector&& v, Compare comp) {
    auto tmp = std::forward<Vector>(v);
    std::sort(tmp.m_.begin(), tmp.m_.end(), comp);
    return tmp;
}

template<typename Vector>
auto negated(Vector&& v) {
    auto tmp = std::forward<Vector>(v);
    std::transform(tmp.m_.begin(), tmp.m_.end(), tmp.m_.begin(), [](this auto, auto const& x) noexcept { return -x; });
    return tmp;
}

} // namespace traditional_free_v2

namespace traditional {

struct my_vector {
    std::vector<int> m_;

    my_vector(std::vector<int> m) noexcept : m_{std::move(m)} { std::println("my_vector::my_vector()"); }
    my_vector(my_vector const& other) noexcept : m_{other.m_} { std::println("my_vector::my_vector(my_vector const&)"); }
    my_vector(my_vector&& other) noexcept : m_{std::move(other.m_)} { std::println("my_vector::my_vector(my_vector&&)"); }

    template<typename Compare>
    auto sorted_by(Compare comp) & {
        auto tmp = *this;
        std::println("  sorted_by on lvalue");
        std::sort(tmp.m_.begin(), tmp.m_.end(), comp);
        return tmp;
    }

    template<typename Compare>
    auto sorted_by(Compare comp) && {
        auto tmp = std::move(*this);
        std::println("  sorted_by on rvalue");
        std::sort(tmp.m_.begin(), tmp.m_.end(), comp);
        return tmp;
    }

    auto negated() & {
        auto tmp = *this;
        std::println("  negated on lvalue");
        std::transform(tmp.m_.begin(), tmp.m_.end(), tmp.m_.begin(), [](this auto, auto const& x) noexcept { return -x; });
        return tmp;
    }

    auto negated() && {
        auto tmp = std::move(*this);
        std::println("  negated on lvalue");
        std::transform(tmp.m_.begin(), tmp.m_.end(), tmp.m_.begin(), [](this auto, auto const& x) noexcept { return -x; });
        return tmp;
    }
};

} // namespace traditional

namespace deduce_this_v1 {

struct my_vector {
    std::vector<int> m_;

    my_vector(std::vector<int> m) noexcept : m_{std::move(m)} { std::println("my_vector::my_vector()"); }
    my_vector(my_vector const& other) noexcept : m_{other.m_} { std::println("my_vector::my_vector(my_vector const&)"); }
    my_vector(my_vector&& other) noexcept : m_{std::move(other.m_)} { std::println("my_vector::my_vector(my_vector&&)"); }

    template<typename Compare>
    auto sorted_by(this my_vector self, Compare comp) {
        std::sort(self.m_.begin(), self.m_.end(), comp);
        return self;
    }

    auto negated(this my_vector self) {
        std::transform(self.m_.begin(), self.m_.end(), self.m_.begin(), [](this auto, auto const& x) noexcept { return -x; });
        return self;
    }
};

} // namespace deduce_this_v1

namespace deduce_this_v2 {

struct my_vector {
    std::vector<int> m_;

    my_vector(std::vector<int> m) noexcept : m_{std::move(m)} { std::println("my_vector::my_vector()"); }
    my_vector(my_vector const& other) noexcept : m_{other.m_} { std::println("my_vector::my_vector(my_vector const&)"); }
    my_vector(my_vector&& other) noexcept : m_{std::move(other.m_)} { std::println("my_vector::my_vector(my_vector&&)"); }

    template<typename Compare, typename Self>
    auto sorted_by(this Self&& self, Compare comp) {
        my_vector tmp = std::forward<Self>(self);
        std::sort(tmp.m_.begin(), tmp.m_.end(), comp);
        return tmp;
    }

    template<typename Self>
    auto negated(this Self&& self) {
        my_vector tmp = std::forward<Self>(self);
        std::transform(tmp.m_.begin(), tmp.m_.end(), tmp.m_.begin(), [](this auto, auto const& x) noexcept { return -x; });
        return tmp;
    }
};

} // namespace deduce_this_v2

auto main() -> int {
    {
        using namespace traditional_free_v1;
        std::println("traditional_free_v1");
        std::println("");
        std::println("v1:");
        auto v1 = sorted_by(my_vector{{1,2,3,4,5,6,7,8}},
            [](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } );
        std::println("");
        std::println("v2:");
        auto v2 = negated(
            sorted_by(my_vector{{1,2,3,4,5,6,7,8}},
                [](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } ));
        std::println("");
        std::println("v3:");
        auto v3 = negated(v1);
    }
    std::println("");
    std::println("");
    {
        using namespace traditional_free_v2;
        std::println("traditional_free_v2");
        std::println("");
        std::println("v1:");
        auto v1 = sorted_by(my_vector{{1,2,3,4,5,6,7,8}},
            [](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } );
        std::println("");
        std::println("v2:");
        auto v2 = negated(
            sorted_by(my_vector{{1,2,3,4,5,6,7,8}},
                [](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } ));
        std::println("");
        std::println("v3:");
        auto v3 = negated(v1);
    }
    std::println("");
    std::println("");
    {
        using namespace traditional;
        std::println("traditional");
        std::println("");
        std::println("v1:");
        auto v1 = my_vector{{1,2,3,4,5,6,7,8}}
            .sorted_by([](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } );
        std::println("");
        std::println("v2:");
        auto v2 = my_vector{{1,2,3,4,5,6,7,8}}
            .sorted_by([](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } )
            .negated();
        std::println("");
        std::println("v3:");
        auto v3 = v1.negated();
    }
    std::println("");
    std::println("");
    {
        using namespace deduce_this_v1;
        std::println("deduce_this_v1");
        std::println("");
        std::println("v1:");
        auto v1 = my_vector{{1,2,3,4,5,6,7,8}}
            .sorted_by([](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } );
        std::println("");
        std::println("v2:");
        auto v2 = my_vector{{1,2,3,4,5,6,7,8}}
            .sorted_by([](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } )
            .negated();
        std::println("");
        std::println("v3:");
        auto v3 = v1.negated();
    }
    std::println("");
    std::println("");
    {
        using namespace deduce_this_v2;
        std::println("deduce_this_v2");
        std::println("");
        std::println("v1:");
        auto v1 = my_vector{{1,2,3,4,5,6,7,8}}
            .sorted_by([](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } );
        std::println("");
        std::println("v2:");
        auto v2 = my_vector{{1,2,3,4,5,6,7,8}}
            .sorted_by([](this auto, auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; } )
            .negated();
        std::println("");
        std::println("v3:");
        auto v3 = v1.negated();
    }
}
