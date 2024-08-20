#include <variant>
#include <print>

struct node;
using sentinel = std::monostate;
using tree = std::variant<sentinel, node*>;
struct node {
    int data;
    tree left;
    tree right;
};

template <typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };

auto preorder_traversal = [](tree const& t, auto&& f) {
    std::visit(
        overloaded{
            [ ] (sentinel const&) {}, // do nothing
            [&] (this auto self, node const* n) {
                f(*n);
                std::visit(self, n->left);
                std::visit(self, n->right);
            },
        },
        t
    );
};

auto main() -> int {
    auto n6 = node{6, {   }, {   }};
    auto n3 = node{3, {   }, {&n6}};
    auto n4 = node{4, {   }, {   }};
    auto n5 = node{5, {   }, {   }};
    auto n2 = node{2, {&n4}, {&n5}};
    auto n1 = node{1, {&n2}, {&n3}};
    preorder_traversal(tree{&n1}, [](node const& n) { std::println("{}", n.data); });
}
