// behaviour test for 0x00BD8B30 (STLport red-black tree model)
#include <cstdio>
#include <cstdlib>
#include <new>

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
template <class V> struct Node : NodeBase { V value; };

template <class V> static Node<V>* CloneNode(Node<V>* x)
{
    Node<V>* t = (Node<V>*)std::malloc(sizeof(Node<V>));
    new (&t->value) V(x->value);
    t->color = x->color; t->left = 0; t->right = 0;
    return t;
}
template <class V> static Node<V>* Copy(Node<V>* x, Node<V>* p)
{
    Node<V>* top = CloneNode(x);
    top->parent = p;
    if (x->right) top->right = Copy((Node<V>*)x->right, top);
    p = top;
    x = (Node<V>*)x->left;
    while (x != 0) {
        Node<V>* y = CloneNode(x);
        p->left = y; y->parent = p;
        if (x->right) y->right = Copy((Node<V>*)x->right, y);
        p = y;
        x = (Node<V>*)x->left;
    }
    return top;
}
template <class V> static bool SameShape(NodeBase* a, NodeBase* b, NodeBase* pa, NodeBase* pb)
{
    if (!a || !b) return a == b;
    if (a->parent != pa || b->parent != pb || a->color != b->color) return false;
    return SameShape<V>(a->left, b->left, a, b) && SameShape<V>(a->right, b->right, a, b);
}

static int g_arrayCopies = 0;
struct Vec3 { float X, Y, Z; };
struct Array {
    Vec3* first; Vec3* last; Vec3* end;
    Array() : first(0), last(0), end(0) { }
    Array(const Array& o) { ++g_arrayCopies; unsigned n = (unsigned)(o.last - o.first); first = n ? (Vec3*)std::malloc(n * sizeof(Vec3)) : 0; last = first; end = first + n; for (Vec3* p = o.first; p != o.last; ++p, ++last) *last = *p; }
};
struct Value { unsigned long key; Array scalings; Value(const Value& o) : key(o.key), scalings(o.scalings) { } Value() : key(0) { } };

int main()
{
    Vec3 v[2] = { { 1, 2, 3 }, { 4, 5, 6 } };
    Node<Value> n[3]; NodeBase head;
    for (int i = 0; i < 3; ++i) { n[i].value.key = (unsigned long)i; n[i].left = n[i].right = 0; n[i].color = false; }
    n[1].value.scalings.first = v; n[1].value.scalings.last = v + 2; n[1].value.scalings.end = v + 2;
    n[1].parent = &head; n[1].left = &n[0]; n[1].right = &n[2]; n[0].parent = &n[1]; n[2].parent = &n[1];
    NodeBase head2;
    Node<Value>* top = Copy(&n[1], (Node<Value>*)&head2);
    if (!SameShape<Value>(&n[1], top, &head, &head2) || g_arrayCopies != 3) { std::printf("00bd8b30_TEST FAIL\n"); return 1; }
    if (top->value.key != 1 || top->value.scalings.last - top->value.scalings.first != 2 || top->value.scalings.first == v || top->value.scalings.first[1].Z != 6.0f) { std::printf("00bd8b30_TEST FAIL\n"); return 1; }
    std::printf("00bd8b30_TEST PASS\n");
    return 0;
}