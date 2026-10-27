// Behaviour test for 0x0099A140 std::_Rb_tree<CUnprovenKey_00999e50, ...>::_M_copy: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
#define private public
#define protected public
struct CUnprovenKey_00999e50 { char _M_unproven; };                     // one byte: the node value copy is a byte move
inline bool operator<(const CUnprovenKey_00999e50& __x, const CUnprovenKey_00999e50& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenValue_00999e50 { CUnprovenKey_00999e50 _M_key; };
struct CUnprovenKeyOfValue_00999e50 { const CUnprovenKey_00999e50& operator()(const CUnprovenValue_00999e50& __v) const { return __v._M_key; } };

namespace std {

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                           // +0x00
    _Base_ptr   _M_parent;                          // +0x04
    _Base_ptr   _M_left;                            // +0x08
    _Base_ptr   _M_right;                           // +0x0C
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class T> struct _Identity { const T& operator()(const T& x) const { return x; } };
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Node* _Link_type;

    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;     // +0x00
    unsigned int _M_node_count;                                 // +0x04
    Compare _M_key_compare;                                     // +0x08

    static _Link_type& _S_left(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_right); }

protected:
    _Link_type _M_create_node(const Value& x)
    {
        _Link_type tmp = _M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
private:
    _Link_type _M_copy(_Link_type x, _Link_type p);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Link_type
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_copy(_Link_type x, _Link_type p)
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

typedef _Rb_tree<CUnprovenKey_00999e50, CUnprovenValue_00999e50, CUnprovenKeyOfValue_00999e50, less<CUnprovenKey_00999e50>,
                 allocator<CUnprovenValue_00999e50> > _Tree_00999e50;
template _Tree_00999e50::_Link_type _Tree_00999e50::_M_copy(_Tree_00999e50::_Link_type x, _Tree_00999e50::_Link_type p);

}

typedef tstd::_Rb_tree_node<CUnprovenValue_00999e50> Node;
static Node* N(char v, bool red, Node* l, Node* r)
{
    Node* n = (Node*)malloc(sizeof(Node)); n->_M_value_field._M_key._M_unproven = v; n->_M_color = red;
    n->_M_parent = 0; n->_M_left = l; n->_M_right = r;
    if (l) l->_M_parent = n;
    if (r) r->_M_parent = n;
    return n;
}
static int Same(const tstd::_Rb_tree_node_base* a, const tstd::_Rb_tree_node_base* b, const tstd::_Rb_tree_node_base* parent)
{
    if (!a || !b) return a == b;
    if (a == b || b->_M_parent != parent || a->_M_color != b->_M_color) return 0;
    if (((const Node*)a)->_M_value_field._M_key._M_unproven != ((const Node*)b)->_M_value_field._M_key._M_unproven) return 0;
    return Same(a->_M_left, b->_M_left, b) && Same(a->_M_right, b->_M_right, b);
}
int main()
{
    // 8 with a left spine 4 -> 2 -> 1, right children 6 (under 4), 3 (under 2), 10 (under 8, with a left 9)
    Node* root = N(8, false, N(4, true, N(2, false, N(1, true, 0, 0), N(3, true, 0, 0)), N(6, false, 0, 0)),
                   N(10, false, N(9, true, 0, 0), 0));
    Node parent; memset(&parent, 0, sizeof(parent));
    tstd::_Tree_00999e50* tree = (tstd::_Tree_00999e50*)malloc(sizeof(tstd::_Tree_00999e50));   // _M_copy reads no member
    Node* copy = tree->_M_copy(root, &parent);
    int ok = copy != root && Same(root, copy, &parent) && copy->_M_parent == &parent;
    Node* leaf = tree->_M_copy((Node*)root->_M_right->_M_left, root);
    ok &= leaf->_M_value_field._M_key._M_unproven == 9 && leaf->_M_left == 0 && leaf->_M_right == 0 && leaf->_M_parent == root;
    ok &= leaf->_M_color == true;
    if (ok) printf("0099a140_TEST PASS\n");
    return ok ? 0 : 1;
}