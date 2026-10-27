// Behaviour test for 0x00AD01F0 std::_Rb_tree_node_base::_S_minimum -- the leftmost node of a subtree
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; };
static Node* S_minimum(Node* x) { while (x->left != 0) x = x->left; return x; }
int main()
{
    Node n[4];
    for (int i = 0; i < 4; ++i) { n[i].color = false; n[i].parent = n[i].left = n[i].right = 0; }
    n[0].left = &n[1]; n[1].left = &n[2]; n[2].right = &n[3];
    if (S_minimum(&n[0]) != &n[2] || S_minimum(&n[3]) != &n[3]) { std::printf("00ad01f0_TEST FAIL\n"); return 1; }
    std::printf("00ad01f0_TEST PASS\n");
    return 0;
}