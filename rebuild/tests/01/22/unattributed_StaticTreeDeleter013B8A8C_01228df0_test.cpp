// Behaviour test for 0x01228DF0 the static STLport map's atexit destructor: clear the tree (erase every node from
// the root, relink the empty header, zero the count), then free the header node
#include <stdio.h>
#include <stdlib.h>
static int gMallocs = 0;
static int gFrees = 0;
static void* test_malloc(unsigned int n) { ++gMallocs; return malloc(n); }
static void test_free(void* p) { ++gFrees; free(p); }
namespace _STL {
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)test_malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { if (p != 0) test_free(p); }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef allocator<_Node> _M_node_allocator_type;
    _Rb_tree_base(const _Alloc&) : _M_header(_M_node_allocator_type(), 0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x)                 // stand-in for the out-of-line erase: free the subtree
    {
        while (__x != 0) {
            _M_erase((_Link_type)__x->_M_right);
            _Link_type __y = (_Link_type)__x->_M_left;
            this->_M_header.deallocate(__x, 1);
            __x = __y;
        }
    }
    void _M_empty_initialize() {
        this->_M_header._M_data->_M_color = 0;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    void clear() {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
struct SStaticTreeValue013B8A8C {
    long m_a;
    long m_b;
    long m_c;
};
typedef _STL::map<long, SStaticTreeValue013B8A8C> CStaticTree;
typedef _STL::_Rb_tree_node<CStaticTree::value_type> CTreeNode;
static void Link3(CStaticTree& m)                   // root with a left and a right child
{
    CTreeNode* r = m._M_t._M_header.allocate(1);
    CTreeNode* a = m._M_t._M_header.allocate(1);
    CTreeNode* b = m._M_t._M_header.allocate(1);
    CTreeNode* h = m._M_t._M_header._M_data;
    r->_M_parent = h; r->_M_left = a; r->_M_right = b;
    a->_M_parent = r; a->_M_left = 0; a->_M_right = 0;
    b->_M_parent = r; b->_M_left = 0; b->_M_right = 0;
    h->_M_parent = r; h->_M_left = a; h->_M_right = b;
    m._M_t._M_node_count = 3;
}
int main()
{
    bool ok = true;
    {
        CStaticTree m;                                  // header node allocated
        ok = ok && gMallocs == 1 && m._M_t._M_root() == 0 && m._M_t._M_node_count == 0;
        Link3(m);
        ok = ok && gMallocs == 4;
        m._M_t.clear();
        CTreeNode* h = m._M_t._M_header._M_data;
        ok = ok && gFrees == 3 && m._M_t._M_node_count == 0;
        ok = ok && h->_M_parent == 0 && h->_M_left == h && h->_M_right == h;
        m._M_t.clear();                                 // already empty: nothing erased
        ok = ok && gFrees == 3;
    }
    ok = ok && gFrees == 4;                             // the header went at destruction
    {
        CStaticTree m;                                  // the atexit path: destruction alone
        Link3(m);
    }
    ok = ok && gMallocs == 8 && gFrees == 8;            // three nodes then the header
    if (ok) { printf("01228df0_TEST PASS\n"); return 0; }
    printf("01228df0_TEST FAIL\n");
    return 1;
}