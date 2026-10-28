// CMeshDataBank::Sizeof() -- the bank's tracked memory: the entry array (its own size plus a pointer per entry) and every entry's Sizeof
// Leaf: ?Sizeof@CMeshDataBank@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25420, 77 bytes. Name: Ego_r/FableWin PDB ?Sizeof@CMeshDataBank@@UBEKXZ (public
//   virtual; the catalog row carries this mangled name). Callee: each entry's virtual Sizeof (slot 2, +8).
// Shape: GFPContainerSizeof<const CArray<CMeshDataBankEntry*> > (FableWin.pdb ??$GFPContainerSizeof@$$CBV?$CArray@PAV
//   CMeshDataBankEntry@@@@@@YIKABV?$CArray@PAVCMeshDataBankEntry@@@@@Z) inlined: sizeof(CArray) + size() * sizeof(T*),
//   then GFPSizeof per element (the landed 0x00AC3910 model: `p ? sizeof(T*) + p->Sizeof() : sizeof(T*)`), the end
//   pointer re-read each pass (the virtual call may change it); the loop-holding template needs __forceinline.
// Layout: MeshesAndAnimations (CArray<CMeshDataBankEntry*>, STLport vector) at +0x380 (engine/CMeshDataBank.h); local
//   polymorphic model (the header flattens the vptr).
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

template <class T> inline bool GFPIsNull(T* p) { return p == 0; }
template <class T> inline unsigned long GFSizeof(T& object) { return object.Sizeof(); }
template <class T> inline unsigned long __fastcall GFPSizeof(T& pointer)
{
    if (!GFPIsNull(pointer))
        return sizeof(T) + GFSizeof(*pointer);
    return sizeof(T);
}
template <class C> __forceinline unsigned long __fastcall GFPContainerSizeof(C& container)
{
    unsigned long size = sizeof(C) + container.size() * sizeof(typename C::value_type);
    for (typename C::const_iterator it = container.begin(); it != container.end(); ++it)
        size += GFPSizeof(*it);
    return size;
}

class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry();
    virtual void v04();
    virtual unsigned long Sizeof() const;   // slot 2 (+8)
};

class CMeshDataBank {
public:
    virtual ~CMeshDataBank();
    unsigned char _pad004[0x380 - 4];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;   // +0x380
    virtual unsigned long Sizeof() const;
};

unsigned long CMeshDataBank::Sizeof() const
{
    return GFPContainerSizeof(MeshesAndAnimations);
}