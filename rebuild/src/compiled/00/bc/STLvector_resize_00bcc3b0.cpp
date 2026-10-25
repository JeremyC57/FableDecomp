// _STL::vector::resize(unsigned int new_size) -- resize the CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry vector: erase the surplus tail, or append default-constructed elements up to the new size
// Leaf: ?resize@?$vector@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@V?$allocator@VCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCC3B0, 77 bytes, thiscall `ret 4`.
// Callers: one, 0x00BCBDCD in CEngineInternalPrimitiveSpriteTrail::Update 0x00BCBC00 (landed: `Trail.resize(trail.Trail.size())`).
// Callees: vector::erase(first, last) 0x00BCBF20 (thiscall, returns the iterator, dropped), vector::_M_fill_insert(pos, n, x)
//   0x00BCC080.
// Element: Ego_r.pdb CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry (16 bytes: C3DVector Position, float Width) with its
//   empty default constructor, so the default value is an uninitialised 16-byte temporary (no stores).
// Label: catalog "vector<std::pair<CCharString ...>::resize" is propagated across the byte-identical-shaped resize bodies.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` with `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }` and `insert(pos, n, x)` forwarding to the out-of-line _M_fill_insert: the
//   default temporary lives in the `sub esp,16` frame, size() is recomputed after the erase/insert test.
class C3DVector {
public:
    float X;                                    // +0x0
    float Y;                                    // +0x4
    float Z;                                    // +0x8
    C3DVector() { }
};

class CEngineInternalPrimitiveSpriteTrail {
public:
    class CSpriteTrailEntry {                   // Ego_r.pdb, 16 bytes
    public:
        C3DVector Position;                     // +0x0
        float     Width;                        // +0xC
        CSpriteTrailEntry() { }
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
    iterator erase(iterator __first, iterator __last);                          // 0x00BCBF20
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x00BCC080
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

template void vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry >::resize(unsigned int);
}