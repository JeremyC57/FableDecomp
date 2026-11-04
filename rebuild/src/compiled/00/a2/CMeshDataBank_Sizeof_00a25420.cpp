// CMeshDataBank::Sizeof() -- the bank's tracked memory: the entry array (its own size plus a pointer per entry) and every entry's Sizeof
// Leaf: ?Sizeof@CMeshDataBank@@UBEKXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25420, 77 bytes. Name: Ego_r/FableWin PDB ?Sizeof@CMeshDataBank@@UBEKXZ (public
//   virtual; the catalog row carries this mangled name). Callee: each entry's virtual Sizeof (slot 2, +8).
// Shape: GFPContainerSizeof<const CArray<CMeshDataBankEntry*> > (FableWin.pdb ??$GFPContainerSizeof@$$CBV?$CArray@PAV
//   CMeshDataBankEntry@@@@@@YIKABV?$CArray@PAVCMeshDataBankEntry@@@@@Z) inlined: sizeof(CArray) + size() * sizeof(T*),
//   then GFPSizeof per element (the landed 0x00AC3910 model: `p ? sizeof(T*) + p->Sizeof() : sizeof(T*)`), the end
//   pointer re-read each pass (the virtual call may change it); the loop-holding template needs __forceinline.
// Layout: MeshesAndAnimations (CArray<CMeshDataBankEntry*>, STLport vector) at +0x380 (engine/CMeshDataBank.h); local
//   polymorphic model (the header flattens the vptr).
#include "engine/CArray.h"

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