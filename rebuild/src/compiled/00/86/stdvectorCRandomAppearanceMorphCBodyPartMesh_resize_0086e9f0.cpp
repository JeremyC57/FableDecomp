// std::vector<CRandomAppearanceMorph::CBodyPartMesh>::resize(unsigned int, const CBodyPartMesh&) -- shrink by erasing the tail or grow by fill-inserting copies of x
// Leaf: ?resize@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@VCBodyPartMesh@CRandomAppearanceMorph@@@std@@@std@@QAEXIABVCBodyPartMesh@CRandomAppearanceMorph@@@Z
// Retail: Fable.exe @ 0x0086E9F0, 100 bytes, thiscall `ret 8`.
// Identity (proven): STLport 4.6 `resize(n, x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }`
// over a 24-byte element. Its erase callee 0x0086E150 is the landed (proven) std::vector<CRandomAppearanceMorph::CBodyPartMesh>
// ::erase(iterator, iterator), whose header already names this row as that vector's resize(n, x); the insert is the inline
// `insert(pos, n, x)` = _M_fill_insert 0x0086E5C0 (24-byte stride). Its caller 0x0086EB08 is the one-argument resize(n).
// Label correction: catalog "_gapscan::sub_0086e9f0".
// Callees: 0x0086E150 ?erase@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@...@@QAEPAVCBodyPartMesh@CRandomAppearanceMorph@@PAV34@0@Z;
// 0x0086E5C0 ?_M_fill_insert@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@...@@QAEXPAVCBodyPartMesh@CRandomAppearanceMorph@@IABV34@@Z
// (landed under the stand-in N0086e5c0::vector<Element>::_M_fill_insert).
// Layout: CBodyPartMesh is 24 bytes (MeshIndex, Priority, a 16-byte CVectorMap at +8: the landed erase's model); only its
// size matters here. STLport vector = { _M_start, _M_finish, _M_end_of_storage }.
// Shape: size() is recomputed after the branch (the else arm reloads _M_finish); the compare is unsigned (`jae`).
class CRandomAppearanceMorph {
public:
    class CBodyPartMesh {
    public:
        long MeshIndex;                                     // +0x00
        long Priority;                                      // +0x04
        unsigned char Morphs[0x10];                         // +0x08 CVectorMap<..>
    };
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
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
};

template void vector<CRandomAppearanceMorph::CBodyPartMesh, allocator<CRandomAppearanceMorph::CBodyPartMesh> >::resize(
    unsigned int, const CRandomAppearanceMorph::CBodyPartMesh&);
}