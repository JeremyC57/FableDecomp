// Behaviour test for 0x0051AA00 _M_insert of map<CThingBuilding*, list<CThingAICreature*> >: an insert under the header
// becomes root, leftmost and rightmost; smaller keys go left (updating leftmost), larger right (updating rightmost), a
// non-null x or the header as y forces left, a non-null w forces right; every new node is a copy of v with parent y and
// null children, is handed to _Rebalance with the root link, bumps the count and is returned.
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc;
static struct _Rb_tree_node_base* g_rebal; static struct _Rb_tree_node_base** g_root; static int g_copies;
class CThingBuilding;
class CThingAICreature;

struct _Rb_tree_node_base {
    char                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static void _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root) { g_rebal = x; g_root = &root; }
};

struct _List_node_base;
template <class T> class list {
public:
    _List_node_base* _M_node;                  // sentinel (alloc proxy data)
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { ++g_copies; }
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

class CBuildingCreatureMapTree {               // STLport _Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >, ...>
public:
    typedef pair<CThingBuilding* const, list<CThingAICreature*> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    };
    _Node*                          _M_header;       // +0x0
    unsigned long                   _M_node_count;   // +0x4
    less<CThingBuilding*>      _M_key_compare;  // +0x8

    static _Node* allocate(unsigned int n) { return (_Node*)malloc(n * sizeof(_Node)); }
    _Node* _M_create_node(const value_type& x)
    {
        _Node* tmp = allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    static CThingBuilding* const& _S_key(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field.first; }

    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const value_type& v, _Rb_tree_node_base* w_);
};

CBuildingCreatureMapTree::iterator CBuildingCreatureMapTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_,
                                                                       const value_type& v, _Rb_tree_node_base* w_)
{
    _Node* w = (_Node*)w_;
    _Node* x = (_Node*)x_;
    _Node* y = (_Node*)y_;
    _Node* z;

    if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(v.first, _S_key(y))))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

typedef CBuildingCreatureMapTree T;
static CThingBuilding* B(int i) { static char c[16]; return (CThingBuilding*)&c[i]; }
int main()
{
    T t; static double hraw[8]; T::_Node& hdr = *(T::_Node*)hraw; hdr._M_color = 0; hdr._M_parent = 0; hdr._M_left = &hdr; hdr._M_right = &hdr;
    t._M_header = &hdr; t._M_node_count = 0;
    list<CThingAICreature*> l; l._M_node = (_List_node_base*)&l;
    int ok = 1;
    T::value_type v5(B(5), l);
    g_copies = 0;
    T::iterator it = t._M_insert(0, &hdr, v5, 0);
    T::_Node* n5 = (T::_Node*)it._M_node;
    ok &= g_copies == 1 && n5->_M_value_field.first == B(5) && n5->_M_value_field.second._M_node == l._M_node;
    ok &= hdr._M_parent == n5 && hdr._M_left == n5 && hdr._M_right == n5 && n5->_M_parent == &hdr;
    ok &= n5->_M_left == 0 && n5->_M_right == 0 && g_rebal == n5 && g_root == &hdr._M_parent && t._M_node_count == 1;
    T::value_type v2(B(2), l);
    T::_Node* n2 = (T::_Node*)t._M_insert(0, n5, v2, 0)._M_node;           // 2 < 5: left, new leftmost
    ok &= n5->_M_left == n2 && hdr._M_left == n2 && hdr._M_right == n5 && n2->_M_parent == n5 && t._M_node_count == 2;
    T::value_type v9(B(9), l);
    T::_Node* n9 = (T::_Node*)t._M_insert(0, n5, v9, 0)._M_node;           // 9 > 5: right, new rightmost
    ok &= n5->_M_right == n9 && hdr._M_right == n9 && hdr._M_left == n2 && n9->_M_parent == n5;
    T::value_type v7(B(7), l);
    T::_Node* n7 = (T::_Node*)t._M_insert((_Rb_tree_node_base*)n9, n9, v7, 0)._M_node;   // x != 0: left of y
    ok &= n9->_M_left == n7 && hdr._M_right == n9 && hdr._M_left == n2;
    T::value_type v1(B(1), l);
    T::_Node* n1 = (T::_Node*)t._M_insert(0, n2, v1, (_Rb_tree_node_base*)n2)._M_node;  // w != 0: right of y
    ok &= n2->_M_right == n1 && hdr._M_left == n2 && hdr._M_right == n9 && t._M_node_count == 5 && g_copies == 5;
    std::free(n5); std::free(n2); std::free(n9); std::free(n7); std::free(n1);
    if (!ok) { std::printf("0051aa00_TEST FAIL\n"); return 1; }
    std::printf("0051aa00_TEST PASS\n");
    return 0;
}