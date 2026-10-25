// std::vector::resize(size_type new_size) -- forward to resize(new_size, value_type()) for the bank file runtime records
// Leaf: ?resize@?$vector@VCRuntimeData@CBankFile@@V?$allocator@VCRuntimeData@CBankFile@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D4300, 40 bytes, thiscall, `ret 4`.
// Identity: vector<CBankFile::CRuntimeData> (12 bytes: DataOffset, DataSize, DataType byte, Valid -- Ego_r.pdb): the
//   callee 0x009D4010 is the two-argument resize. No direct reference (xref 0). FableWin.pdb has
//   ??0CRuntimeData@CBankFile@@QAE@XZ. The element is modelled locally inside a stub CBankFile (engine/CBankFile.h is
//   the Ego_r layout of the outer class and does not declare the nested record's constructor).
//   Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: vector::resize(size_type, const value_type&) @ 0x009D4010.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }`; CRuntimeData() zeroes each member: two eax dwords, two al bytes.
class CBankFile {
public:
    class CRuntimeData {
    public:
        unsigned long DataOffset;
        unsigned long DataSize;
        unsigned char DataType;
        bool Valid;
        CRuntimeData() : DataOffset(0), DataSize(0), DataType(0), Valid(false) { }
    };
};
namespace std {
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
    void resize(size_type __new_size, const _Tp& __x);                    // out of line (0x009D4010)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CBankFile::CRuntimeData>::resize(size_type);
}