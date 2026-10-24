// CVertexGroupBlock::SharesGroupWith(const CVertexGroupBlock& other) -- true when the two blocks have at least one bone group in common (lock-step walk of both sorted group maps)
// Grade: DIFFER(188v188) -- register assignment only: retail loads `other` first (eax) and keeps this/other headers in ebp/ebx with the iterators in edx/ecx; the lane keeps this in ebx/ecx and other in ebp/edx (same instructions otherwise)
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
// `xor al,al` set before the first end test. STLport `_M_increment()` inline: right subtree's leftmost, else climb
// while a right child and step to the parent unless the header's right link points back.
namespace _STL {

struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                   // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};

template <class _Value> struct _Rb_tree_const_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_const_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &(operator*()); }
    _Rb_tree_const_iterator& operator++() { _M_increment(); return *this; }
    bool operator==(const _Rb_tree_const_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_const_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                           // +0x00 (alloc proxy)
    unsigned int        _M_node_count;                       // +0x04
    char                _M_key_compare;                      // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
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
