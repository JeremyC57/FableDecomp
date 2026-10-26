// NInventory::CCategory::~CCategory() -- the compiler-generated (implicit) destructor, kept out of line: release the def pointer PDef (+0x14), destroy ItemsInCategory (+0x8), then the first member PAssociatedThing (+0x00) as a tail jump
// Leaf: ??1CCategory@NInventory@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2739, 40 bytes (+ int3 padding), thiscall, no return value; size-optimised TU (odd entry,
//   `and dword ptr [esi+14h],0`); callers 0x005C0121, 0x005C024A, 0x005C297A, 0x005C29CE, 0x005C2A8E.
// Identity (proven): NInventory::CCategory::~CCategory(); evidence: non-polymorphic, so identified by its users: the landed CTCInventoryBase::AddCategory 0x005C024A destroys its temporary NInventory::CCategory with it (that file names 0x005C2739 ~CCategory); its scalar deleting destructor 0x005C297A (this body, then operator delete 0x00BFE9BC on flags & 1) and the __destroy_aux 0x005C2A8E (a 0x2C-stride loop = retail sizeof(CCategory), Ego_r 0x30 minus one 4-byte container) call it, as do the inventory OnCreate 0x005C0121 and 0x005C29CE; FableWin.pdb holds ??1CCategory@NInventory@@QAE@XZ (non-virtual, as here); the released / destroyed offsets +0x14 / +0x08 / +0x00 are Ego_r.pdb CCategory::PDef (rebased) / ItemsInCategory / PAssociatedThing.
// Label: the manifest's "~CCreatureAction_FireMissileWeapon" is propagated across byte-identical bodies (family J081: five implicit destructors of five
//   classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CIVCountedPointeeBase::Delete() const (the def's vtable slot 1, on the last release); std::vector<NInventory::CItem>::~vector 0x005C1E30 (FableWin.pdb ??1?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@QAE@XZ; the landed GetTimeWieldedWeapon 0x005C99BE calls it on a vector<CItem>); CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (tail jmp: the implicit ~CIntelligentPointer<CThing> of PAssociatedThing +0x00).
// Layout: Ego_r.pdb NInventory::CCategory members rebased after the 12-byte retail STLport vector (PDef Ego_r +0x18 -> retail +0x14, as the body's release shows; sizeof 0x2C, the __destroy_aux 0x005C2A8E stride).
// Shape: the J072 model (no vtable store -- VC7.1 resets the vptr only in user-written destructors; members destroyed in reverse
//   declaration order, the last one the tail `jmp`) with a CDefPointer member whose inline destructor is the IV release
//   (`p = Object; if (p) { if (--p->IVRefCount == 0) p->Delete(); Object = 0; }`, agent952 H021 / J075), `__forceinline` because
//   under /Os VC7.1 keeps the class-body destructor out of line (grade 1: a call, 27 bytes) and without /Os the clear is a 7-byte
//   `mov` (grade 2, 43 bytes); retail has both the size-TU `and` and the inlined release. KillOne's explicit
//   destructor call is a modelling device that makes VC7.1 emit the implicit destructor out of line.
#pragma optimize("s", on)
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CIVCountedPointeeBase {                            // Ego_r.pdb 8 bytes: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1 (FableWin.pdb ?Delete@CIVCountedPointeeBase@@UBEXXZ)
    unsigned long IVRefCount;                            // +0x04
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {         // Ego_r.pdb 0x1C
public:
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource { };            // Ego_r.pdb 0x1C, no members of its own
template <class T> class CDefPointer {                   // Ego_r.pdb 4 bytes
public:
    __forceinline ~CDefPointer()                         // the IV release, inline (FableWin.pdb has it out of line, debug)
    {
        CDefPointeeBase* p = Object;
        if (p) {
            if (--p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                             // +0x0
};
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {            // STLport 4.6, 12 bytes
public:
    ~vector();                                                  // out of line in retail (0x005C1E30 for NInventory::CItem)
};
}   // namespace std
class CThing;
class CInventoryCategoryDef;
namespace NInventory {
class CItem;                                             // Ego_r.pdb NInventory::CItem (only named here)
class CTCInventoryBase;
class CCategory {                                        // Ego_r.pdb 0x30 bytes, retail 0x2C (12-byte vector); non-polymorphic
public:
    CIntelligentPointer<CThing>              PAssociatedThing;     // +0x00
    std::vector<CItem>                       ItemsInCategory;      // +0x08
    CDefPointer<const CInventoryCategoryDef> PDef;                 // +0x14 (Ego_r +0x18)
    CTCInventoryBase*                        PInventory;           // +0x18 (Ego_r +0x1C)
    long                                     GlobalDefIndex;       // +0x1C
    long                                     SelectedItemIndex;    // +0x20
    long                                     HighlightedItemIndex; // +0x24
    bool                                     Selected;             // +0x28
    bool                                     DisplayItems;         // +0x29
};
}   // namespace NInventory
void KillOne(NInventory::CCategory* p) { p->~CCategory(); }