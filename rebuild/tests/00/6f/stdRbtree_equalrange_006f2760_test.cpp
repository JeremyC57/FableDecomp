// Behaviour test for 006f2760 (std::_Rb_tree::equal_range of the UID -> CThingCreatureBase* tree): a hand-built three-node tree, the
// inlined upper-bound walk and the lower bound (STLport body here) give the expected ranges.
#include <cstdio>
#if defined(_MSC_VER)
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

class CThingCreatureBase;

namespace tst {
template<class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template<class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template<class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
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
template<class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node<Value>* x) : _M_node(x) {}
};
template<class Key, class Value, class KeyOfValue, class Compare>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Link_type _M_header;
    unsigned int _M_node_count;
    Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)_M_header->_M_parent; }
    static _Link_type& _S_left(_Link_type x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Link_type x) { return (_Link_type&)(x->_M_right); }
    static const Key& _S_key(_Link_type x) { return KeyOfValue()(x->_M_value_field); }
    _Link_type _M_lower_bound(const Key& k) const      // STLport body (out of line in retail)
    {
        _Link_type y = _M_header;
        _Link_type x = _M_root();
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = _S_left(x);
            else
                x = _S_right(x);
        return y;
    }
    _Link_type _M_upper_bound(const Key& k) const
    {
        _Link_type y = _M_header;
        _Link_type x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = _S_left(x);
            else
                x = _S_right(x);
        return y;
    }
    iterator lower_bound(const Key& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const Key& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const Key& k) { return pair<iterator, iterator>(lower_bound(k), upper_bound(k)); }
};
}

typedef tst::pair<const u64, CThingCreatureBase*> SEntry;
typedef tst::_Rb_tree<u64, SEntry, tst::_Select1st<SEntry>, tst::less<u64> > STree;
typedef tst::_Rb_tree_node<SEntry> SNode;

static SNode* MakeNode(u64 key)
{
    SNode* n = (SNode*)new char[sizeof(SNode)];
    *(u64*)&n->_M_value_field.first = key;
    n->_M_color = false;
    n->_M_parent = n->_M_left = n->_M_right = 0;
    return n;
}

int main()
{
    // keys 0x100000005 (left), 0x200000000 (root), 0x200000007 (right): the high dwords decide the first two
    SNode* header = MakeNode(0);
    SNode* a = MakeNode(((u64)1 << 32) | 5);
    SNode* b = MakeNode((u64)2 << 32);
    SNode* c = MakeNode(((u64)2 << 32) | 7);
    header->_M_parent = b; header->_M_left = a; header->_M_right = c;
    b->_M_parent = header; b->_M_left = a; b->_M_right = c;
    a->_M_parent = b; c->_M_parent = b;
    STree tree;
    tree._M_header = header;
    tree._M_node_count = 3;
    int ok = 1;
    tst::pair<STree::iterator, STree::iterator> r = tree.equal_range((u64)2 << 32);
    if (r.first._M_node != b || r.second._M_node != c) ok = 0;
    r = tree.equal_range(((u64)1 << 32) | 5);
    if (r.first._M_node != a || r.second._M_node != b) ok = 0;
    r = tree.equal_range(((u64)1 << 32) | 9);
    if (r.first._M_node != b || r.second._M_node != b) ok = 0;
    r = tree.equal_range((u64)3 << 32);
    if (r.first._M_node != header || r.second._M_node != header) ok = 0;
    r = tree.equal_range(0);
    if (r.first._M_node != a || r.second._M_node != a) ok = 0;
    if (!ok) {
        std::printf("006f2760_TEST FAIL\n");
        return 1;
    }
    std::printf("006f2760_TEST PASS\n");
    return 0;
}