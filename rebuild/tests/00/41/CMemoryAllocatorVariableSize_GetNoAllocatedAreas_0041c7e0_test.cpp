// Behaviour test for 0x0041C7E0 std::_Rb_tree_node_base::_S_minimum.
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right;
    static Node* Minimum(Node* x) { while (x->left != 0) x = x->left; return x; } };
int main()
{
    Node a = { false, 0, 0, 0 }, b = { false, 0, 0, 0 }, c = { false, 0, 0, 0 };
    c.left = &b; b.left = &a; c.right = &a;
    if (Node::Minimum(&c) != &a || Node::Minimum(&a) != &a || Node::Minimum(&b) != &a) { std::printf("0041c7e0_TEST FAIL\n"); return 1; }
    std::printf("0041c7e0_TEST PASS\n");
    return 0;
}
