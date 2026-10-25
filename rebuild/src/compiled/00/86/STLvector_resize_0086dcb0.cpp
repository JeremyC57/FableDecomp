// std::vector::resize(unsigned int new_size) -- STLport vector<pair<long, CRandomAppearanceMorph::CSkeletalMorph> >::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() default (id, skeletal morph) pairs at the end
// Leaf: ?resize@?$vector@U?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@std@@V?$allocator@U?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086DCB0, 99 bytes (+13 int3), thiscall on the vector, `ret 4`.
// Identity: the element vector of CRandomAppearanceMorph::SkeletalMorphs (CVectorMap<long, CSkeletalMorph>, +0x18 in the
//   landed ctor 0x0086C4D0); no direct call site in the image (xref: 0 calls / 0 references).
// Callees: vector<pair<long, CSkeletalMorph> >::erase(first, last) 0x0086C720, ::_M_fill_insert(pos, n, const T&)
//   0x0086CD70 (the inline insert's body).
// Layout: Ego_r.pdb CRandomAppearanceMorph::CSkeletalMorph (12 bytes): GroupID long +0, Name CDefString +4,
//   AllowVariation bool +8; FableWin.pdb lists its default constructor ??0CSkeletalMorph@CRandomAppearanceMorph@@QAE@XZ.
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   pair's `first(long())` and the morph's GroupID / AllowVariation zeroes share one zero register, CDefString's -1 is
//   an immediate scheduled after the size compare.
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is
//   pair<long, CRandomAppearanceMorph::CSkeletalMorph>.
class CDefString {
public:
    long TablePos;                                                  // +0x00
    CDefString() : TablePos(-1) { }
};

class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {                                          // 12 bytes
    public:
        long       GroupID;                                         // +0x00
        CDefString Name;                                            // +0x04
        bool       AllowVariation;                                  // +0x08
        CSkeletalMorph() : GroupID(0), AllowVariation(false) { }
    };
};

namespace std {
template <class _T1, class _T2> struct pair {                       // STLport pair
    _T1 first;                                                      // +0x00
    _T2 second;                                                     // +0x04
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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

// 0x0086C720 erase, 0x0086CD70 _M_fill_insert (out of line in retail)
template void std::vector<std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> >::resize(unsigned int);