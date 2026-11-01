// CCRCSymbolMap::~CCRCSymbolMap() -- the compiler-generated destructor: destroy the NamesUsed set (clear + free the header node), free the SLONGMap entry block, then run ~CBaseClassNonCopyable
// Leaf: ??1CCRCSymbolMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D1D50, 93 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): callers are the slot-0 `??_G` 0x00A399E0 of vtable 0x0129CF84 (RTTI .?AVCCRCSymbolMap@@ :
//   CBaseClassNonCopyable; the landed ctor 0x00A38500 stores it) and the atexit destructor 0x01228DD0 of the global
//   CCRCSymbolMap at 0x013B8A54. Ego_r.pdb (0x004F0700) and FableWin.pdb (0x01B829A0) spell ??1CCRCSymbolMap@@UAE@XZ, both far
//   from the class's own TU (0x00B94xxx / 0x031DAxxx), as retail's copy is far from 0x00A38xxx: the implicit destructor,
//   emitted where it is first needed. No 0x0129CF84 store confirms it.
// Callees: the set's _Rb_tree<unsigned long,...>::_M_erase 0x004CF810 (bound PDB spelling, private STLport member), free
//   0x00BFEA14 (the header node, the entry block), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE).
// Shape: members in reverse order: NamesUsed (STLport 4.6 ~_Rb_tree = clear(), then ~_Rb_tree_base's null-tested header
//   free), then SLONGMap (CVectorMap -> CArray -> STLport vector of pairs: trivial _Destroy, ~_Vector_base's null-tested
//   free). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1` (chunks 656 / 657).
// Layout: the landed ctor 0x00A38500 / Ego_r.pdb: SLONGMap +0x04 (CVectorMap 0x10: Data +0, Compare +0xC, Dirty +0xD),
//   NamesUsed +0x14 (header +0x14, count +0x18, compare +0x1C), CheckForDuplicateSymbols +0x20.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"                   // ~ 0x0099A300 (MAE)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                   // +0x00
    _Base_ptr   _M_parent;                                  // +0x04
    _Base_ptr   _M_left;                                    // +0x08
    _Base_ptr   _M_right;                                   // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;                             // +0x04
    _Compare _M_key_compare;                                // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                          // 0x004CF810 (this instantiation)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
private:
    _Rep_type _M_t;
};

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > {   // Fable's CArray over the STLport vector
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                // +0x00
    TCompare           Compare;                             // +0x0c (empty comparator)
    bool               Dirty;                               // +0x0d
};

class CCRCSymbolMap : public CBaseClassNonCopyable {        // vtable 0x0129CF84, 0x24 bytes
public:
    CCRCSymbolMap();
    CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> > SLONGMap;   // +0x04
    std::set<unsigned long> NamesUsed;                      // +0x14
    bool CheckForDuplicateSymbols;                          // +0x20
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CCRCSymbolMap* map)
{
    map->CCRCSymbolMap::~CCRCSymbolMap();
}