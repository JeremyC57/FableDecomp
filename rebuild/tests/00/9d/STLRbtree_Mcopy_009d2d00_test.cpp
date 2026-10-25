// Behaviour test for std::_Rb_tree<CCharString, pair<const CCharString, unsigned long>, ...>::_M_copy (0x009D2D00).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct CCharString {
    int Key;
    CCharString(int k) : Key(k) {}
    CCharString(const CCharString& other) : Key(other.Key) { ++g_copies; }
};
struct Value { CCharString first; unsigned long second; Value(int k, unsigned long v) : first(k), second(v) {} };
struct Node { bool colour; Node* parent; Node* left; Node* right; Value value; Node(int k, unsigned long v) : value(k, v) {} };

static Node* CloneNode(const Node* x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node));
    new (&tmp->value.first) CCharString(x->value.first);
    tmp->value.second = x->value.second;
    tmp->colour = x->colour;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
}

static Node* Copy(Node* x, Node* p)
{
    Node* top = CloneNode(x);
    top->parent = p;
    if (x->right) top->right = Copy(x->right, top);
    p = top;
    x = x->left;
    while (x != 0) {
        Node* y = CloneNode(x);
        p->left = y;
        y->parent = p;
        if (x->right) y->right = Copy(x->right, y);
        p = y;
        x = x->left;
    }
    return top;
}

static int Same(const Node* a, const Node* b, const Node* bParent)
{
    if (!a || !b) return a == b;
    if (a == b || b->parent != bParent) return 0;
    if (a->value.first.Key != b->value.first.Key || a->value.second != b->value.second || a->colour != b->colour) return 0;
    return Same(a->left, b->left, b) && Same(a->right, b->right, b);
}

int main()
{
    Node n1(1, 10), n2(2, 20), n3(3, 30), n4(4, 40), n5(5, 50), n6(6, 60);
    Node* all[6] = { &n1, &n2, &n3, &n4, &n5, &n6 };
    for (int i = 0; i < 6; ++i) { all[i]->left = all[i]->right = all[i]->parent = 0; all[i]->colour = (i & 1) != 0; }
    // root 4: left 2 (1, 3), right 5 (-, 6)
    n4.left = &n2; n2.parent = &n4; n4.right = &n5; n5.parent = &n4;
    n2.left = &n1; n1.parent = &n2; n2.right = &n3; n3.parent = &n2;
    n5.right = &n6; n6.parent = &n5;
    Node header(0, 0);
    Node* copy = Copy(&n4, &header);
    if (copy->parent != &header || !Same(&n4, copy, &header) || g_copies != 6) { std::printf("009d2d00_TEST FAIL\n"); return 1; }
    std::printf("009d2d00_TEST PASS\n");
    return 0;
}