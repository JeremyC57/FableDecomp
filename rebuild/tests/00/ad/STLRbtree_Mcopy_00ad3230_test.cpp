// Behaviour test for _STL::_Rb_tree<unsigned long, unsigned long, _Identity, less, allocator>::_M_copy (set<unsigned long>) @ 0x00AD3230
// Structural copy of a red-black subtree: same shape, colours and values, fresh nodes, parent links into the new tree.
#include <cstdio>
#include <cstdlib>
#include <new>

template <class V> struct TNode {
    bool Color;
    TNode* Parent;
    TNode* Left;
    TNode* Right;
    V Value;
};

static int gClones = 0;

template <class V> TNode<V>* CloneNode(const TNode<V>* x)
{
    TNode<V>* n = (TNode<V>*)std::malloc(sizeof(TNode<V>));
    new (&n->Value) V(x->Value);
    n->Color = x->Color;
    n->Left = 0;
    n->Right = 0;
    ++gClones;
    return n;
}

// STLport _Rb_tree::_M_copy: structural copy of the subtree at x under parent p (x and p non-null).
template <class V> TNode<V>* CopyTree(const TNode<V>* x, TNode<V>* p)
{
    TNode<V>* top = CloneNode(x);
    top->Parent = p;
    if (x->Right)
        top->Right = CopyTree(x->Right, top);
    p = top;
    x = x->Left;
    while (x != 0) {
        TNode<V>* y = CloneNode(x);
        p->Left = y;
        y->Parent = p;
        if (x->Right)
            y->Right = CopyTree(x->Right, y);
        p = y;
        x = x->Left;
    }
    return top;
}

template <class V> void Link(TNode<V>* n, TNode<V>* parent, TNode<V>* left, TNode<V>* right, bool color)
{
    n->Parent = parent; n->Left = left; n->Right = right; n->Color = color;
}

// Same shape, colours and parent links, distinct nodes; values compared by the caller.
template <class V> int SameShape(const TNode<V>* a, const TNode<V>* b, const TNode<V>* pa, const TNode<V>* pb)
{
    if (a == 0 || b == 0) return a == b;
    if (a == b || a->Color != b->Color || a->Parent != pa || b->Parent != pb) return 0;
    return SameShape(a->Left, b->Left, a, b) && SameShape(a->Right, b->Right, a, b);
}

template <class V> void FreeTree(TNode<V>* n)
{
    if (!n) return;
    FreeTree(n->Left);
    FreeTree(n->Right);
    n->Value.~V();
    std::free(n);
}

typedef unsigned int VALUE;
static VALUE MAKE(int i) { return (VALUE)(i * 3 + 1); }
static int SAMEVAL(VALUE a, VALUE b) { return a == b; }

int main()
{
    TNode<VALUE> header;

    TNode<VALUE> t[7];
    for (int i = 0; i < 7; ++i) new (&t[i].Value) VALUE(MAKE(i));
    Link(&t[3], (TNode<VALUE>*)&header, &t[1], &t[5], false);
    Link(&t[1], &t[3], &t[0], &t[2], true);
    Link(&t[5], &t[3], &t[4], &t[6], false);
    Link(&t[0], &t[1], (TNode<VALUE>*)0, (TNode<VALUE>*)0, false);
    Link(&t[2], &t[1], (TNode<VALUE>*)0, (TNode<VALUE>*)0, false);
    Link(&t[4], &t[5], (TNode<VALUE>*)0, (TNode<VALUE>*)0, true);
    Link(&t[6], &t[5], (TNode<VALUE>*)0, (TNode<VALUE>*)0, true);
    TNode<VALUE> newHeader;
    TNode<VALUE>* copy = CopyTree((const TNode<VALUE>*)&t[3], &newHeader);
    int ok = gClones == 7 && SameShape((const TNode<VALUE>*)&t[3], (const TNode<VALUE>*)copy,
                                       (const TNode<VALUE>*)&header, (const TNode<VALUE>*)&newHeader);
    // in-order walk of the copy must give the source values 0..6 in order
    const TNode<VALUE>* order[7] = { copy->Left->Left, copy->Left, copy->Left->Right, copy,
                                     copy->Right->Left, copy->Right, copy->Right->Right };
    for (int i = 0; i < 7 && ok; ++i) if (!SAMEVAL(order[i]->Value, MAKE(i))) ok = 0;
    // a single node with a right child only
    TNode<VALUE> a, b;
    new (&a.Value) VALUE(MAKE(10)); new (&b.Value) VALUE(MAKE(11));
    Link(&a, (TNode<VALUE>*)&header, (TNode<VALUE>*)0, &b, false);
    Link(&b, &a, (TNode<VALUE>*)0, (TNode<VALUE>*)0, true);
    TNode<VALUE>* copy2 = CopyTree((const TNode<VALUE>*)&a, &newHeader);
    if (copy2->Left != 0 || copy2->Right == 0 || copy2->Right->Parent != copy2 || !copy2->Right->Color
        || !SAMEVAL(copy2->Right->Value, MAKE(11)) || gClones != 9) ok = 0;

    FreeTree(copy);
    FreeTree(copy2);
    if (!ok) { std::printf("00ad3230_TEST FAIL\n"); return 1; }
    std::printf("00ad3230_TEST PASS\n");
    return 0;
}