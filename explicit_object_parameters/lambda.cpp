#include <print>

auto main() -> int {

    // recursive call
    auto fib = [](this auto self, int n) {
        if (n<2) return n;
        return self(n-1) + self(n-2);
    };
    for (int n=1; n<10; ++n) {
        std::println("fib({}) = {}", n, fib(n));
    }

    // lambda type
    auto lambda1 = [](this auto&& self, int n) {
        using type = std::remove_cvref_t<decltype(self)>;
    };
    auto lambda2 = []<typename Lambda>(this Lambda&& self, int n) {
        using type = std::remove_cvref_t<Lambda>;
    };

    // constrain value category
    auto lambda3 = [](this auto& self, int n) {
        std::println("{}", n);
    };
    //decltype(lambda3){}(42); // compile error

    // mutable
    int m = 0;
    auto lambda4 = [m](this auto& self, int n) {
        m = n;
        std::println("{}", m);
    };
    lambda4(42);
}
