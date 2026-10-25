// Behaviour test for _Rb_tree<long, pair<const long, CAStarState>>::_M_insert (0x00A21D90): every new A* state node is
// linked as a leaf under its parent, signed keys stay in order, leftmost/rightmost/root track the extremes, the count
// grows, the hint forces a right link, and each stored state is copied whole (counted in by its copy constructor).
#include <cstdio>
#include <cstdlib>
#include <new>

class CANavQuadTreeNode { public: int Id; };
static long g_states = 0, g_open = 0, g_closed = 0;
class CAStarState {
public:
    CAStarState*       ParentState;
    CANavQuadTreeNode* Node;
    float              CostToThisState;
    float              TotalCost;
    bool               OnOpenList;
    bool               OnClosedList;
    CAStarState(CANavQuadTreeNode* node, float cost, bool open)
        : ParentState(0), Node(node), CostToThisState(cost), TotalCost(cost * 2.0f), OnOpenList(open), OnClosedList(!open) {}
    CAStarState(const CAStarState& o)
        : ParentState(o.ParentState), Node(o.Node), CostToThisState(o.CostToThisState), TotalCost(o.TotalCost),
          OnOpenList(o.OnOpenList), OnClosedList(o.OnClosedList)
    {
        ++g_states;
        if (OnOpenList) ++g_open;
        if (OnClosedList) ++g_closed;
    }
};
static CANavQuadTreeNode g_node;

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
static int g_rebalances = 0;
struct _Rb_global_inst {
    // Test stand-in: an unbalanced tree is still a valid search tree; the real rebalance is 0x0042971D.
    static void _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root) { ++g_rebalances; if (!__root) __root = __x; }
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    typedef _Value value_type;
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;

    _Rb_tree() : _M_node_count(0)
    {
        _M_header._M_data = (_Link_type)std::malloc(sizeof(_Rb_tree_node<_Value>));
        _M_header._M_data->_M_color = 0;
        _M_header._M_data->_M_parent = 0;
        _M_header._M_data->_M_left = _M_header._M_data;
        _M_header._M_data->_M_right = _M_header._M_data;
    }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const value_type& __v, _Rb_tree_node_base* __w = 0);
    // Test driver: the STLport insert_equal descent, then _M_insert.
    iterator insert_equal(const value_type& __v)
    {
        _Rb_tree_node_base* __y = this->_M_header._M_data;
        _Rb_tree_node_base* __x = _M_root();
        while (__x != 0) {
            __y = __x;
            __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x) : _S_right(__x);
        }
        return _M_insert(__x, __y, __v);
    }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}

// In-order walk collecting keys (no parent links needed for an unbalanced tree).
template <class _Value, class _Key>
static void walk(std::_Rb_tree_node_base* n, _Key* out, int& k)
{
    if (!n) return;
    walk<_Value, _Key>(n->_M_left, out, k);
    out[k++] = ((std::_Rb_tree_node<_Value>*)n)->_M_value_field.first;
    walk<_Value, _Key>(n->_M_right, out, k);
}

typedef std::pair<const long, CAStarState> V;
typedef std::_Rb_tree<long, V, std::_Select1st<V>, std::less<long> > Tree;

int main()
{
    int ok = 1;
    Tree t;
    long keys[] = { 5, -3, 12, -40, 0, 7, 99, -3 };
    const int n = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < n; ++i) {
        V v(keys[i], CAStarState(&g_node, 1.5f, keys[i] >= 0));
        Tree::iterator it = t.insert_equal(v);
        Tree::_Link_type z = (Tree::_Link_type)it._M_node;
        if (!(z->_M_value_field.first == keys[i]) || z->_M_left != 0 || z->_M_right != 0 || z->_M_parent == 0) ok = 0;
    }
    if (t._M_node_count != (unsigned int)n || std::g_rebalances != n) ok = 0;
    long got[16]; int k = 0;
    walk<V, long>(t._M_root(), got, k);
    if (k != n) ok = 0;
    for (int i = 1; i < k; ++i) if (got[i] < got[i - 1]) ok = 0;
    if (!(((Tree::_Link_type)t._M_leftmost())->_M_value_field.first == got[0])) ok = 0;
    if (!(((Tree::_Link_type)t._M_rightmost())->_M_value_field.first == got[k - 1])) ok = 0;
    // A non-null hint w forces a right link even for a smaller key (else a smaller key links left); y == header always links left as the root/leftmost.
    {
        Tree h;
        V a(keys[2], CAStarState(&g_node, 1.5f, keys[1] >= 0));
        Tree::iterator r = h._M_insert(0, h._M_header._M_data, a);
        if (h._M_root() != r._M_node || h._M_leftmost() != r._M_node || h._M_rightmost() != r._M_node) ok = 0;
        V b(keys[3], CAStarState(&g_node, 1.5f, keys[0] >= 0));
        Tree::iterator r2 = h._M_insert(0, r._M_node, b, r._M_node);
        if (r._M_node->_M_right != r2._M_node || r._M_node->_M_left != 0 || h._M_rightmost() != r2._M_node || r2._M_node->_M_parent != r._M_node) ok = 0;
        V c(keys[3], CAStarState(&g_node, 1.5f, keys[0] >= 0));
        Tree::iterator r3 = h._M_insert(0, r._M_node, c);
        if (r._M_node->_M_left != r3._M_node || h._M_leftmost() != r3._M_node || h._M_node_count != 3u) ok = 0;
    }
    if (g_states < (long)(n + 3) || g_open + g_closed != g_states) ok = 0;
    {
        Tree::_Link_type m = (Tree::_Link_type)t._M_leftmost();
        if (m->_M_value_field.first != -40 || m->_M_value_field.second.Node != &g_node || m->_M_value_field.second.OnOpenList) ok = 0;
        float d = m->_M_value_field.second.TotalCost - 3.0f;
        if (d > 0.0001f || d < -0.0001f) ok = 0;
    }

    if (!ok) { std::printf("00a21d90_TEST FAIL\n"); return 1; }
    std::printf("00a21d90_TEST PASS\n");
    return 0;
}