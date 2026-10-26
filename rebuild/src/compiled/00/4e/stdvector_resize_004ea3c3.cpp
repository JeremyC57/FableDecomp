// std::vector::resize(size_type __new_size, const _Tp& __x) -- (vector<CEngineGraphic >) STLport resize: erase the surplus [begin() + n, end()) when shrinking, else insert n - size() copies of x at the end
// Leaf: ?resize@?$vector@VCEngineGraphic@@V?$allocator@VCEngineGraphic@@@std@@@std@@QAEXIABVCEngineGraphic@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EA3C3, 72 bytes (size-optimised TU, odd entry), thiscall, `ret 8`.
// Identity (proven): std::vector<CEngineGraphic >::resize(size_type, const CEngineGraphic&); evidence: its only caller is vector<CEngineGraphic>::resize(n) 0x004EA398, which builds T() field by field (dword 0 at +0, fldz into the floats at +4 / +8, bytes 0 at +0x0C / +0x0D of a 14-byte frame object: the Ego_r.pdb CEngineGraphic layout) and passes it on; that resize(n) is called by CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryIn 0x004EA2C9 on the meshes' Vector (+0, Ego_r.pdb CReplaceableMeshes::Vector, CArray<CEngineGraphic>), reached from CPersistContext::Transfer 0x004EA21E of CReplaceableMeshDef::Transfer 0x004EA20E on this+0x28 (Ego_r.pdb Meshes)
// Label: the manifest's "push_back" is a behavioural / propagated label; this row is vector<CEngineGraphic >::resize(n, x) (family J069: five 72-byte bodies, each its own element).
// Callees: erase(iterator, iterator) 0x004EA40B, _M_fill_insert(iterator, size_type, const CEngineGraphic&) 0x004EA446 (both out of line, thiscall).
// Shape: STLport 4.6 vector.h `resize` verbatim with insert(pos, n, x) inlined to _M_fill_insert; size() is the exact
//   pointer difference (`idiv 0xE`, the divisor loaded by push/pop under /Os) and is recomputed on the insert path.
// Element: CEngineGraphic (Ego_r.pdb, 14 bytes packed: BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x004EA40B
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x004EA446
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}   // namespace std
#pragma pack(push, 1)
class CEngineGraphic {                                   // Ego_r.pdb 14 bytes (packed)
public:
    unsigned long BankIndex;                             // +0x00
    float         AnimStep;                              // +0x04
    float         RenderSizeX;                           // +0x08
    bool          AdditiveAlpha;                         // +0x0C
    unsigned char Type;                                  // +0x0D
};
#pragma pack(pop)
namespace std {
template void vector<CEngineGraphic >::resize(unsigned int, const CEngineGraphic&);
}