// CVertexGroupBlock::SharesGroupWith(const CVertexGroupBlock& other) -- true when the two blocks have at least one bone group in common (lock-step walk of both sorted group maps)
// Grade: DIFFER(188v188) -- pass 3: the STLport 4.6 model (static _Rb_global<bool>::_M_increment, _Const_traits iterators) puts the iterators in retail's edx/ecx; left: retail loads `other` first and keeps this/other headers in ebp/ebx, the lane loads this first into ebx/ebp (tried: forceinline helper, named ends, b declared first)
// Leaf: ?SharesGroupWith@CVertexGroupBlock@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD8AF0, 0xBA bytes (+ int3 padding), thiscall, `ret 4`, bool in al.
// Identity: Ego_r.pdb ?SharesGroupWith@CVertexGroupBlock@@QBE_NABV1@@Z; both operands are walked through
// GroupToIndexMap (map<unsigned long, long>) at +0x0C (retail layout of the landed CVertexGroupBlock::IsSubsetOf
// 0x00AD8C30: Groups vector +0x00, GroupToIndexMap +0x0C). Caller: CVertexGroupBlockOptimiser::PaintCurrentBlockSegment
// 0x00AD9380. The catalog's "TreeNodes_HaveIntersection" is descriptive.
// Callees: none (the STLport iterator increment is inline).
// Shape: a `bool shares = false` result set before the loop (retail's early `xor al,al`), `shares = true; break;` on a hit;
// two const_iterators from begin() walk in step while neither is at its end(): the smaller key advances
// (`cmp edi,esi ; jbe` unsigned), equal keys return true (`jae` -> `mov al,1`); the false result is the
// `xor al,al` set before the first end test. STLport 4.6 iterators over `_Const_traits` with ++ as the static `_Rb_global<bool>::_M_increment(node)` (inline) -- retry
//   pass 3: the real 4.6 layering fixes the register swap of the earlier member `_M_increment()` model.
namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    const_iterator end() const { return const_iterator(_M_header._M_data); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;                                     // +0x00
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
};

} // namespace _STL

struct CGroupArray {                                         // CArray<unsigned long> (retail STLport vector, 12 bytes)
    unsigned long* m_first;
    unsigned long* m_last;
    unsigned long* m_end;
};

class CVertexGroupBlock {                                    // retail 0x18 bytes (PDB 32: debug containers)
public:
    CGroupArray                         Groups;              // +0x00
    _STL::map<unsigned long, long>      GroupToIndexMap;     // +0x0C
    bool SharesGroupWith(const CVertexGroupBlock& other) const;
};

bool CVertexGroupBlock::SharesGroupWith(const CVertexGroupBlock& other) const
{
    bool shares = false;
    _STL::map<unsigned long, long>::const_iterator a = GroupToIndexMap.begin();
    _STL::map<unsigned long, long>::const_iterator b = other.GroupToIndexMap.begin();
    while (a != GroupToIndexMap.end() && b != other.GroupToIndexMap.end()) {
        if (a->first < b->first)
            ++a;
        else if (b->first < a->first)
            ++b;
        else {
            shares = true;
            break;
        }
    }
    return shares;
}
