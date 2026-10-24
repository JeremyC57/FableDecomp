// behaviour test for 0x00B97980 (STLport red-black tree model)
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

struct Info { long count; };
struct Counted { void* Data; Info* Inf; Counted(const Counted& o) : Data(o.Data), Inf(o.Inf) { if (Inf) ++Inf->count; } Counted() : Data(0), Inf(0) { } };
struct Value { long key; Counted ptr; Value(const Value& o) : key(o.key), ptr(o.ptr) { } Value() : key(0) { } };

int main()
{
    Info info = { 1 };
    Node<Value> n[5]; NodeBase head;
    // root 2, left 1 (left 0), right 4 (left 3)
    for (int i = 0; i < 5; ++i) { n[i].value.key = i; n[i].value.ptr.Data = &n[i]; n[i].value.ptr.Inf = (i % 2) ? &info : 0; n[i].left = n[i].right = 0; n[i].color = (i & 1) != 0; }
    n[2].parent = &head; n[2].left = &n[1]; n[2].right = &n[4];
    n[1].parent = &n[2]; n[1].left = &n[0]; n[0].parent = &n[1];
    n[4].parent = &n[2]; n[4].left = &n[3]; n[3].parent = &n[4];
    NodeBase head2;
    Node<Value>* top = Copy(&n[2], (Node<Value>*)&head2);
    if (!SameShape<Value>(&n[2], top, &head, &head2)) { std::printf("00b97980_TEST FAIL\n"); return 1; }
    if (info.count != 3) { std::printf("00b97980_TEST FAIL\n"); return 1; }   // keys 1 and 3 hold the counted info
    if (((Node<Value>*)top->right->left)->value.key != 3 || ((Node<Value>*)top->left->left)->value.key != 0) { std::printf("00b97980_TEST FAIL\n"); return 1; }
    std::printf("00b97980_TEST PASS\n");
    return 0;
}