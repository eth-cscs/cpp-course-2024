#include <print>

struct base_1 {
    base_1(int m) : m_{m} {}
    template<typename Self>
    auto get(this Self const& self) { return self.base_1::m_; }
  private:
    int m_;
};

struct derived_1 : base_1 {
    derived_1(int m) : base_1{m} {}
};

struct base_2 {
    base_2(int m) : m_{m} {}
    //template<typename Self>
    //auto get(this Self const& self) { return self.base_2::m_; }
    template<typename Self>
    auto get(this Self const& self) { return ((base_2 const&)self).m_; }
    int m_;
};

struct derived_2 : private base_2 {
    derived_2(int m) : base_2{m} {}
    using base_2::get;
};

auto main() -> int {
    derived_1 d_1{42};
    std::println("d_1.get() = {}", d_1.get());

    derived_2 d_2{42};
    std::println("d_2.get() = {}", d_2.get());
}
