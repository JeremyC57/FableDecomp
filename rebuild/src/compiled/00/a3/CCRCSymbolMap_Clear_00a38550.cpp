// CCRCSymbolMap::Clear() -- empty the CRC symbol map: clear the CRC-keyed vector-map and the set of used CRCs
// Leaf: ?Clear@CCRCSymbolMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A38550, 62 bytes (+2 int3), `__fastcall`, plain `ret`. No callers
//   (an unreferenced copy kept by the linker).
// Identity: FableWin.pdb `?Clear@CCRCSymbolMap@@QAEXXZ` (031DA820, right after the constructor and scalar deleting
//   destructor, matching retail's 0x00A38500 constructor (agent329) followed by this body).
// Layout (agent329's constructor model: Ego_r.pdb with the retail 16-byte CVectorMap and 12-byte STLport set):
//   CBase vptr +0, SLONGMap CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> > +0x04,
//   NamesUsed set<unsigned long> +0x14 (header +0x14, node count +0x18), CheckForDuplicateSymbols +0x20.
// Callees: CVectorMap::Clear @ 0x00A39900 (out of line), the set's _Rb_tree::_M_erase @ 0x004CF810.
// Shape: byte-identical to CSymbolMap::Clear 0x009E5250 except the two callees: the out-of-line vector-map clear,
//   then STLport `set::clear()` inline (the tree address formed once, each header store re-reads the header
//   pointer, the count zeroed last).
extern "C" void __cdecl free(void* block);               // 0x00BFEA14 (import thunk)

struct CBase {
    CBase();                                             // 0x0099A2F0
    virtual ~CBase();
};

namespace _STL {
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* p, unsigned int) { if (p != 0) free(p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                   // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef allocator<_Node> _M_node_allocator_type;
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                           // +0x04
    _Compare     _M_key_compare;                          // +0x08
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                        // out of line (recursive)
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
template <class _Tp> struct _Identity { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;   // 12 bytes
    void clear() { _M_t.clear(); }
};
}

template <class T> struct CVector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
template <class K, class V> struct CKeyValuePair { K first; V second; };
template <class K, class V, class C> struct CVectorMap { // retail 16 bytes
    CVector<CKeyValuePair<K, V> > Data;                  // +0x00
    C                             Compare;               // +0x0c empty comparator
    bool                          Dirty;                 // +0x0d
    void Clear();                                        // out of line
};
template <class K, class V> struct CKeyPairCompareLess { };

struct CCRCSymbolMap : CBase {                           // vtable 0x0129CF84
    CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> > SLONGMap;   // +0x04
    _STL::set<unsigned long>                                                  NamesUsed;  // +0x14
    bool                                                                      CheckForDuplicateSymbols;   // +0x20
    virtual ~CCRCSymbolMap();
    void Clear();
};

void CCRCSymbolMap::Clear()
{
    SLONGMap.Clear();
    NamesUsed.clear();
}