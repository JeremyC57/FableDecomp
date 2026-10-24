// Behaviour test for 006f2870 (std::_Rb_tree::_M_copy of the UID -> CThingCreatureBase* map): structurally copy a
// four-node tree under a new parent; keys, values, colours and parent links must be reproduced.
#include <cstdio>
#include <cstdlib>
#include <new>
#if defined(_MSC_VER)
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

class CThingCreatureBase;
static char gCreatures[8];

namespace tst {
template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;
};
template<class Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
    static bool& _S_color(_Link_type x) { return x->_M_color; }
    _Link_type _M_create_node(const Value& x)
    {
        _Link_type tmp = (_Link_type)std::malloc(sizeof(_Rb_tree_node<Value>));
        new (&tmp->_M_value_field) Value(x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        _S_color(tmp) = _S_color(x);
        _S_left(tmp) = 0;
        _S_right(tmp) = 0;
        return tmp;
    }
    _Link_type _M_copy(_Link_type x, _Link_type p)
    {
        _Link_type top = _M_clone_node(x);
        top->_M_parent = p;
        if (x->_M_right)
            top->_M_right = _M_copy(_S_right(x), top);
        p = top;
        x = _S_left(x);
        while (x != 0) {
            _Link_type y = _M_clone_node(x);
            p->_M_left = y;
            y->_M_parent = p;
            if (x->_M_right)
                y->_M_right = _M_copy(_S_right(x), y);
            p = y;
            x = _S_left(x);
        }
        return top;
    }
};
}

typedef tst::pair<const u64, CThingCreatureBase*> SEntry;
typedef tst::_Rb_tree_node<SEntry> SNode;

static SNode* MakeNode(u64 key, long tag, bool color)
{
    SNode* n = (SNode*)std::malloc(sizeof(SNode));
    new (&n->_M_value_field) SEntry(key, (CThingCreatureBase*)(gCreatures + tag));
    n->_M_color = color;
    n->_M_parent = n->_M_left = n->_M_right = 0;
    return n;
}

static int Same(const SNode* a, const SNode* b, const SNode* expectedParent)
{
    if (!a || !b)
        return a == b;
    if (a == b || a->_M_value_field.first != b->_M_value_field.first || a->_M_value_field.second != b->_M_value_field.second ||
        a->_M_color != b->_M_color || b->_M_parent != expectedParent)
        return 0;
    return Same((const SNode*)a->_M_left, (const SNode*)b->_M_left, b) && Same((const SNode*)a->_M_right, (const SNode*)b->_M_right, b);
}

int main()
{
    //        40
    //      20   50
    //    10  30
    SNode* n40 = MakeNode(40, 4, true);
    SNode* n20 = MakeNode(20, 2, false);
    SNode* n50 = MakeNode((u64)5 << 33, 5, true);
    SNode* n10 = MakeNode(10, 1, true);
    SNode* n30 = MakeNode(30, 3, true);
    n40->_M_left = n20; n40->_M_right = n50; n20->_M_parent = n40; n50->_M_parent = n40;
    n20->_M_left = n10; n20->_M_right = n30; n10->_M_parent = n20; n30->_M_parent = n20;
    SNode* header = MakeNode(0, 0, false);
    tst::_Rb_tree<SEntry> tree;
    SNode* copy = tree._M_copy(n40, header);
    int ok = Same(n40, copy, header);
    if (!ok) {
        std::printf("006f2870_TEST FAIL\n");
        return 1;
    }
    std::printf("006f2870_TEST PASS\n");
    return 0;
}