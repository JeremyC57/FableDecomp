// _STL::vector::resize(unsigned int new_size) -- resize the CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator vector: erase the surplus tail, or append default-constructed elements up to the new size
// Leaf: ?resize@?$vector@VCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@V?$allocator@VCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBDD40, 80 bytes, thiscall `ret 4`.
// Callers: 0x00BBD054, 0x00BBD16A, 0x00BBD1BD in CEngineInternalPrimitiveParticleGroup::SetChildPrimitive 0x00BBCF70 (landed; the
//   PrimitiveIDToIterator table grown to cover a new child id).
// Callees: vector::erase(first, last) 0x00BBD7B0 (thiscall, returns the iterator, dropped), vector::_M_fill_insert(pos, n, x)
//   0x00BBD950.
// Element: Ego_r.pdb CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator (8 bytes: bool Valid +0, the primitive-list
//   iterator +4); its default constructor clears only Valid (`mov byte ptr [esp+8],0`), the iterator's own default
//   constructor leaves its node word uninitialised.
// Label: catalog "vector<std::pair<CCharString ...>::resize" is propagated across the byte-identical-shaped resize bodies.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }` and `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default temporary lives in the `sub esp,8` frame, size() is recomputed after the erase/insert test.
class CEnginePrimitiveHandle;
namespace _STL {
struct _List_node_base;
template <class _Tp> struct _List_iterator {    // the primitive list's iterator: one node pointer
    _List_node_base* _M_node;
    _List_iterator() { }
};
}

class CEngineInternalPrimitiveParticleGroup {
public:
    class CPrimitiveIterator {                  // Ego_r.pdb, 8 bytes
    public:
        bool Valid;                             // +0x0
        _STL::_List_iterator<CEnginePrimitiveHandle> Iterator;   // +0x4
        CPrimitiveIterator() : Valid(false) { }
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x00BBD7B0
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00BBD950
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator >::resize(unsigned int);
}