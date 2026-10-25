// Link test for 0x00AD3720: the source object's std::_Rb_tree<unsigned long, unsigned long, _Identity<unsigned long>, ...>::operator=
// runs against stand-in _M_erase / _M_copy specialisations (counted; _M_copy clones the node shape under the new parent).
#include <cstdio>
#include <cstdlib>

namespace std {
template <class _Tp> struct _Identity { };
template <class _Tp> struct less { };
template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    char      _M_color;                              // +0x00
    _Base_ptr _M_parent;                             // +0x04
    _Base_ptr _M_left;                               // +0x08
    _Base_ptr _M_right;                              // +0x0C
    static _Base_ptr _S_minimum(_Base_ptr __x) { while (__x->_M_left != 0) __x = __x->_M_left; return __x; }
    static _Base_ptr _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};

template <class _Tp, class _Alloc>
struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                      // +0x04
    _Compare     _M_key_compare;                     // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type _S_minimum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(__x); }
    static _Link_type _S_maximum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(__x); }
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    void _M_erase(_Link_type __x);
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = (_Link_type)this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = (_Link_type)this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    _Rb_tree& operator=(const _Rb_tree& __x);
};

typedef _Rb_tree<unsigned long, unsigned long, _Identity<unsigned long>,
                 less<unsigned long>, allocator<unsigned long > > _CULongSetTree;
}
using std::_CULongSetTree;
typedef _CULongSetTree::_Link_type Link;
static int g_erase = 0, g_copy = 0;
static Link g_erased = 0;
template <> void _CULongSetTree::_M_erase(Link x) { ++g_erase; g_erased = x; }
template <> Link _CULongSetTree::_M_copy(Link x, Link p)
{
    ++g_copy;
    if (!x) return 0;
    Link n = (Link)std::malloc(sizeof(*n));
    n->_M_color = x->_M_color; n->_M_value_field = x->_M_value_field; n->_M_parent = p;
    n->_M_left = _M_copy((Link)x->_M_left, n); n->_M_right = _M_copy((Link)x->_M_right, n);
    return n;
}
static Link Mk(unsigned long v) { Link n = (Link)std::malloc(sizeof(*n)); n->_M_color = 0; n->_M_value_field = v; return n; }
static void Init(_CULongSetTree& t, std::_Rb_tree_node_base* h)
{
    h->_M_color = 0; h->_M_parent = 0; h->_M_left = h; h->_M_right = h;
    t._M_header._M_data = h; t._M_node_count = 0;
}
int main()
{
    int fail = 0;
    std::_Rb_tree_node_base ha, hb;
    _CULongSetTree a, b;
    Init(a, &ha); Init(b, &hb);
    // source b: root 2.0 with left 1.0 and right 3.0
    Link n1 = Mk(1u), n2 = Mk(2u), n3 = Mk(3u);
    n2->_M_parent = &hb; n2->_M_left = n1; n2->_M_right = n3;
    n1->_M_parent = n2; n1->_M_left = n1->_M_right = 0;
    n3->_M_parent = n2; n3->_M_left = n3->_M_right = 0;
    hb._M_parent = n2; hb._M_left = n1; hb._M_right = n3; b._M_node_count = 3;
    // destination a: one old node, must be erased first
    Link old = Mk(9u);
    old->_M_parent = &ha; old->_M_left = old->_M_right = 0;
    ha._M_parent = old; ha._M_left = old; ha._M_right = old; a._M_node_count = 1;
    if (&(a = b) != &a) fail |= 1;
    if (g_erase != 1 || g_erased != old) fail |= 2;
    Link r = (Link)ha._M_parent;
    if (!r || r == n2 || r->_M_parent != &ha || r->_M_value_field != 2u) fail |= 4;
    if (((Link)ha._M_left)->_M_value_field != 1u || ((Link)ha._M_right)->_M_value_field != 3u) fail |= 8;
    if (a._M_node_count != 3) fail |= 16;
    // self-assignment is a no-op
    int e = g_erase, c = g_copy;
    a = a;
    if (g_erase != e || g_copy != c || a._M_node_count != 3) fail |= 32;
    // empty source: a is cleared (erase once) and its header reset
    _CULongSetTree z; std::_Rb_tree_node_base hz; Init(z, &hz);
    a = z;
    if (g_erase != e + 1 || g_copy != c || ha._M_parent != 0 || ha._M_left != &ha || ha._M_right != &ha || a._M_node_count != 0) fail |= 64;
    // empty destination is not erased
    a = b;
    if (g_erase != e + 1 || a._M_node_count != 3) fail |= 128;
    if (fail) { std::printf("00ad3720_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ad3720_TEST PASS\n");
    return 0;
}
