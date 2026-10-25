// std::vector::resize(unsigned int new_size) -- STLport vector<CSkeletalMorphs::CEntry>::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() default morph entries (no morph name) at the end
// Leaf: ?resize@?$vector@VCEntry@CSkeletalMorphs@@V?$allocator@VCEntry@CSkeletalMorphs@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DF50, 81 bytes (+15 int3), thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@VCEntry@CSkeletalMorphs@@...@@QAEXI@Z; sole caller
//   CSkeletalMorphs::GetAllMorphsOfType 0x0081AE50 (landed; `out->resize(count)`).
// Callees: vector<CSkeletalMorphs::CEntry>::erase(first, last) 0x0081C080 (landed as CRetailVector_pod8::erase),
//   vector<CSkeletalMorphs::CEntry>::_M_fill_insert(pos, n, const T&) 0x0081CF90 (the inline insert's body).
// Layout: Ego_r.pdb CSkeletalMorphs::CEntry (8 bytes): MorphName CDefString +0, Priority short +4,
//   SupressesLowerMorphs bool +6; FableWin.pdb lists its user default constructor ??0CEntry@CSkeletalMorphs@@QAE@XZ,
//   which only runs CDefString's `TablePos(-1)`.
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   the temporary's one store is the immediate -1, scheduled after the size compare.
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is
//   CSkeletalMorphs::CEntry.
class CDefString {
public:
    long TablePos;                                                  // +0x00
    CDefString() : TablePos(-1) { }
};

class CSkeletalMorphs {
public:
    class CEntry {                                                  // 8 bytes
    public:
        CDefString MorphName;                                       // +0x00
        short      Priority;                                        // +0x04
        bool       SupressesLowerMorphs;                            // +0x06
        CEntry() { }
    };
};

namespace std {
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
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

// 0x0081C080 erase, 0x0081CF90 _M_fill_insert (out of line in retail)
template void std::vector<CSkeletalMorphs::CEntry>::resize(unsigned int);