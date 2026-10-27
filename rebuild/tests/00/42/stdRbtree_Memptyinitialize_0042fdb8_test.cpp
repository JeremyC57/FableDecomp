// behaviour test for 0x0042fdb8 std::_Rb_tree::_M_empty_initialize(): header colour red (0), root null, leftmost and
// rightmost point back at the header
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; };
struct Tree { Node* header; unsigned int count;
    void EmptyInitialize() { header->color = false; header->parent = 0; header->left = header; header->right = header; } };
int main()
{
    int fails = 0;
    Node h; h.color = true; h.parent = &h; h.left = 0; h.right = 0;
    Tree t; t.header = &h; t.count = 0;
    t.EmptyInitialize();
    if (h.color != false) fails |= 1;
    if (h.parent != 0) fails |= 2;
    if (h.left != &h || h.right != &h) fails |= 4;
    if (fails) { std::printf("0042fdb8_TEST FAIL %d\n", fails); return 1; }
    std::printf("0042fdb8_TEST PASS\n");
    return 0;
}