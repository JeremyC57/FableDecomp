// Behaviour test for 0x005BAF94 std::set<EClothingSuitPart>::insert: forward to the tree's insert_unique and
// return its (node, inserted) pair converted to the set's const-iterator pair
#include <stdio.h>
enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE, CLOTHING_SUIT_PART_A, CLOTHING_SUIT_PART_B, CLOTHING_SUIT_PART_C };
void* TestNodeFor(int key);
bool TestFirstInsert(int key);
static int gInsertCalls = 0;
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Const_traits { };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
};
template <class _Value, class _Alloc> struct _Rb_tree_base {
    _Rb_tree_node<_Value>* _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> >    const_iterator;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    pair<iterator, bool> insert_unique(const _Value& __x)       // stand-in: one fixed node per key
    {
        ++gInsertCalls;
        iterator __it;
        __it._M_node = (_Rb_tree_node_base*)TestNodeFor((int)__x);
        bool __fresh = TestFirstInsert((int)__x);
        if (__fresh) ++_M_node_count;
        return pair<iterator, bool>(__it, __fresh);
    }
};
template <class _Tp> struct _Identity { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Key value_type;
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;
    pair<iterator, bool> insert(const value_type& __x) {
        typedef typename _Rep_type::iterator _Rep_iterator;
        pair<_Rep_iterator, bool> __p = _M_t.insert_unique(__x);
        return pair<iterator, bool>(__p.first, __p.second);
    }
};
}
typedef std::_Rb_tree_node<EClothingSuitPart> CPartNode;
static CPartNode gNodes[4];
static bool gSeen[4];
void* TestNodeFor(int key) { return &gNodes[key]; }
bool TestFirstInsert(int key) { bool fresh = !gSeen[key]; gSeen[key] = true; return fresh; }
typedef std::set<EClothingSuitPart> CPartSet;
int main()
{
    CPartSet s;
    s._M_t._M_header = 0;
    s._M_t._M_node_count = 0;
    bool ok = true;
    std::pair<CPartSet::iterator, bool> r1 = s.insert(CLOTHING_SUIT_PART_B);
    ok = ok && r1.second && r1.first._M_node == &gNodes[2];
    std::pair<CPartSet::iterator, bool> r2 = s.insert(CLOTHING_SUIT_PART_A);
    ok = ok && r2.second && r2.first._M_node == &gNodes[1];
    std::pair<CPartSet::iterator, bool> r3 = s.insert(CLOTHING_SUIT_PART_B);   // duplicate: existing node, not inserted
    ok = ok && !r3.second && r3.first._M_node == &gNodes[2];
    ok = ok && gInsertCalls == 3 && s._M_t._M_node_count == 2;
    if (ok) { printf("005baf94_TEST PASS\n"); return 0; }
    printf("005baf94_TEST FAIL\n");
    return 1;
}