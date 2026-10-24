#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test: STLport _Rb_tree::_M_copy for map<unsigned long, CCharString (string data pointer copied)> -- the right subtree recursively, the left spine iteratively,
// every clone keeping its colour and value with null children until linked.
typedef unsigned long Key;
typedef const char* Val;
struct Node {
    bool Color; Node* _M_parent; Node* _M_left; Node* _M_right;
    Key First; Val Second;
};
static int g_allocs = 0;
static Node* _M_clone_node(Node* x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node));
    ++g_allocs;
    tmp->First = x->First;
    tmp->Second = x->Second;
    tmp->Color = x->Color;
    tmp->_M_left = 0;
    tmp->_M_right = 0;
    return tmp;
}
static Node* _M_copy(Node* x, Node* p)
{
    Node* top = _M_clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = _M_copy(x->_M_right, top);
    p = top;
    x = x->_M_left;
    while (x != 0) {
        Node* y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = _M_copy(x->_M_right, y);
        p = y;
        x = x->_M_left;
    }
    return top;
}
static bool Same(const Node* a, const Node* b, const Node* bParent)
{
    if (!a || !b) return a == b;
    if (a == b || b->_M_parent != bParent || a->First != b->First || !(a->Second == b->Second) || a->Color != b->Color) return false;
    return Same(a->_M_left, b->_M_left, b) && Same(a->_M_right, b->_M_right, b);
}
int main()
{
    Node n[7];
    std::memset(n, 0, sizeof(n));
    // keys 0..6 in a balanced shape rooted at 3
    for (int i = 0; i < 7; ++i) { n[i].First = (Key)(i * 10); n[i].Second = "s"; n[i].Color = (i & 1) != 0; }
    n[3]._M_left = &n[1]; n[3]._M_right = &n[5];
    n[1]._M_left = &n[0]; n[1]._M_right = &n[2]; n[1]._M_parent = &n[3];
    n[5]._M_left = &n[4]; n[5]._M_right = &n[6]; n[5]._M_parent = &n[3];
    n[0]._M_parent = n[2]._M_parent = &n[1];
    n[4]._M_parent = n[6]._M_parent = &n[5];
    Node header;
    std::memset(&header, 0, sizeof(header));
    Node* copy = _M_copy(&n[3], &header);
    if (g_allocs != 7 || !Same(&n[3], copy, &header)) { std::printf("006310e0_TEST FAIL\n"); return 1; }
    Node leaf;
    std::memset(&leaf, 0, sizeof(leaf));
    leaf.First = (Key)7;
    Node* single = _M_copy(&leaf, &header);
    if (g_allocs != 8 || single->_M_left || single->_M_right || single->_M_parent != &header || single->First != (Key)7) { std::printf("006310e0_TEST FAIL\n"); return 1; }
    std::printf("006310e0_TEST PASS\n");
    return 0;
}