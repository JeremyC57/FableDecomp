// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the (unsigned long, CSkeletalMorphs::CEntry) pair vector
// Leaf: ?resize@?$vector@U?$pair@KVCEntry@CSkeletalMorphs@@@std@@V?$allocator@U?$pair@KVCEntry@CSkeletalMorphs@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081DF20, 39 bytes, thiscall, `ret 4`.
// Identity: the storage vector of CVectorMap<unsigned long, CSkeletalMorphs::CEntry> (FableWin.pdb ??1?$CVectorMap@KVCEntry@
//   CSkeletalMorphs@@...; 12-byte pair: the key + CEntry {CDefString MorphName, short Priority, bool SupressesLowerMorphs}).
//   The callee 0x0081D9F0 is the two-argument resize. No direct reference (xref 0). FableWin.pdb has
//   ??0CEntry@CSkeletalMorphs@@QAE@XZ. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x0081D9F0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over `pair() : first(_T1()), second(_T2())`; CEntry() has an
//   empty body, so only the key's 0 and MorphName's -1 are stored (both immediates after the pushes).
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};
class CSkeletalMorphs {
public:
    class CEntry {
    public:
        CDefString MorphName;
        short Priority;
        bool SupressesLowerMorphs;
        CEntry() { }
    };
};
namespace std {
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x0081D9F0)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<std::pair<unsigned long, CSkeletalMorphs::CEntry> >::resize(size_type);
}