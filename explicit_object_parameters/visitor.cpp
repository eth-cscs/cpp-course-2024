#include <print>
#include <utility>
#include <stack>
#include <memory>
#include <variant>

namespace traditional {

struct expression;
struct number_expression;
struct binary_expression;
struct sum_expression;
struct difference_expression;

struct visitor {
    virtual void visit(number_expression&) = 0;
    virtual void visit(binary_expression&) = 0;
    virtual void visit(sum_expression&) = 0;
    virtual void visit(difference_expression&) = 0;
};

struct expression {
    virtual void accept(visitor&) = 0;
    virtual ~expression() = default;
};

using expression_ptr = std::unique_ptr<expression>;

template<typename T, typename... Args>
auto make_expression(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

struct number_expression : expression {
    number_expression(double v) :  value_{v} {}
    virtual void accept(visitor& v) override { v.visit(*this); }
    double value_;
};

struct binary_expression : expression {
    binary_expression(expression_ptr lhs, expression_ptr rhs) : lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}
    expression_ptr lhs_;
    expression_ptr rhs_;
    double left_value_ = 0;
    double right_value_ = 0;
};

struct sum_expression : binary_expression {
    using binary_expression::binary_expression;
    virtual void accept(visitor& v) override { v.visit(*this); }
};

struct difference_expression : binary_expression {
    using binary_expression::binary_expression;
    virtual void accept(visitor& v) override { v.visit(*this); }
};

struct calculator : visitor {
    void visit(number_expression& e) override {
        stack_.push(e.value_);
    }

    void visit(binary_expression& e) override {
        e.lhs_->accept(*this);
        e.rhs_->accept(*this);
        e.right_value_ = stack_.top();
        stack_.pop();
        e.left_value_ = stack_.top();
        stack_.pop();
    }

    void visit(sum_expression& e) override {
        visit(static_cast<binary_expression&>(e));
        stack_.push(e.left_value_ + e.right_value_);
    }

    void visit(difference_expression& e) override {
        visit(static_cast<binary_expression&>(e));
        stack_.push(e.left_value_ - e.right_value_);
    }

    std::stack<double> stack_;
};

void calculate() {
    // 3 + 4 - 5 = 2
    auto expr =
        make_expression<sum_expression>(
            make_expression<number_expression>(3),
            make_expression<difference_expression>(
                make_expression<number_expression>(4),
                make_expression<number_expression>(5)
            )
        );

    calculator calc;
    calc.visit(*expr);
    std::println("result = {}", calc.stack_.top());
}
} // namespace traditional

namespace deduce_this {

struct number_expression;
struct sum_expression;
struct difference_expression;

using expression = std::variant<number_expression, sum_expression, difference_expression>;
using expression_ptr = std::unique_ptr<expression>;

template<typename T, typename... Args>
auto make_expression(Args&&... args) { return std::make_unique<expression>(std::in_place_type<T>, std::forward<Args>(args)...); }

struct number_expression {
    number_expression(double v) :  value_{v} {}
    double value_;
};

struct binary_expression {
    binary_expression(expression_ptr lhs, expression_ptr rhs) : lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}
    expression_ptr lhs_;
    expression_ptr rhs_;
    double left_value_ = 0;
    double right_value_ = 0;
};

struct sum_expression {
    sum_expression(expression_ptr lhs, expression_ptr rhs) : impl_{std::move(lhs), std::move(rhs)} {}
    binary_expression impl_;
};

struct difference_expression {
    difference_expression(expression_ptr lhs, expression_ptr rhs) : impl_{std::move(lhs), std::move(rhs)} {}
    binary_expression impl_;
};

template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };

void calculate() {
    std::stack<double> stack_;

    overloaded calculator {
        [&](number_expression& e) { stack_.push(e.value_); },
        [&](this auto& self, sum_expression& e) {
            self(e.impl_);
            stack_.push(e.impl_.left_value_ + e.impl_.right_value_);
        },
        [&](this auto& self, difference_expression& e) {
            self(e.impl_);
            stack_.push(e.impl_.left_value_ - e.impl_.right_value_);
        },
        [&](this auto& self, binary_expression& e) {
            std::visit(self, *e.lhs_);
            std::visit(self, *e.rhs_);
            e.right_value_ = stack_.top();
            stack_.pop();
            e.left_value_ = stack_.top();
            stack_.pop();
        },
    };

    auto expr =
        make_expression<sum_expression>(
            make_expression<number_expression>(3),
            make_expression<difference_expression>(
                make_expression<number_expression>(4),
                make_expression<number_expression>(5)
            )
        );

    std::visit(calculator, *expr);

    std::println("result = {}", stack_.top());
}

} // namespace deduce_this

auto main() -> int {
    traditional::calculate();
    deduce_this::calculate();
}
