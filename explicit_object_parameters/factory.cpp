#include <print>
#include <vector>

template<typename Func>
struct factory {
    int i_;
    Func create_;
    template<typename Self>
    auto make(this Self&& self) { return std::forward<Self>(self).create_(self.i_); }
};

struct vec {
    std::vector<int> v_;
    vec(std::vector<int> v) noexcept : v_(std::move(v)) { std::println("vec::vec(v)"); }
    vec(vec const& other) noexcept : v_{other.v_} { std::println("vec::vec(vec const&)"); }
    vec(vec&& other) noexcept : v_{std::move(other.v_)} { std::println("vec::vec(A&&)"); }
};

struct A {
    int m_;
    vec v_;
};

template<typename Lambda>
void test(Lambda&& create) {
    std::println("make factory");
    factory f{42, std::forward<Lambda>(create)};

    std::println("create from lvalue");
    auto a = f.make();

    std::println("create from rvalue");
    auto b = std::move(f).make();
}

auto main() -> int {
    test([msg = vec{{1,2,3,4,5,6}}](int i)->A {
        return {i, msg};});
    std::println("");
    test([msg = vec{{1,2,3,4,5,6}}]<typename Self>(this Self&&, int i)->A {
        return {i, std::forward_like<Self>(msg)}; });
}
