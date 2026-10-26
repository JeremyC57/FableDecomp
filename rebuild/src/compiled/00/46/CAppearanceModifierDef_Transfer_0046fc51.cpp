// CAppearanceModifierDef::Transfer(CPersistContext& context) -- persist the CAppearanceModifierDef's fields (14 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CAppearanceModifierDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046FC51, 167 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CAppearanceModifierDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012330DC, whose RTTI type descriptor is .?AVCAppearanceModifierDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CAppearanceModifierDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CAppearanceModifierDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J057: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::vector<CCharString >&) 0x004331F9, Transfer(CAppearanceModifierGraphics&) 0x0046FE40, Transfer(long&) 0x00431102, Transfer(bool&) 0x0043314A, Transfer<EClothingSuitPart>(EClothingSuitPart&) 0x0046FCF8, Transfer<EHeroAttachableAppearanceModifierType>(EHeroAttachableAppearanceModifierType&) 0x0046FD9C, Transfer(float&) 0x00431061.
// Layout: Ego_r.pdb CAppearanceModifierDef members rebased to retail (def base at its PDB size, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type (the def transfers out of declaration order).
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };   // STLport, 12 bytes
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes (its _Rb_tree: header, count, compare)
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
enum EClothingSuitPart { };                            // Ego_r.pdb (enumerators not needed)
enum EHeroAttachableAppearanceModifierType { };        // Ego_r.pdb (enumerators not needed)
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    void* PStringData;
};
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
};
#pragma pack(pop)
class CAppearanceModifierGraphics {                       // Ego_r.pdb 16 bytes, retail 12 (each STLport container 12 bytes)
public:
    struct CEntry;                            // Ego_r.pdb nested class (opaque here)
    CArray<CEntry >                          Graphics;
};
class CPersistContext {
public:
    void Transfer(std::vector<CCharString >& value);             // 0x004331F9
    void Transfer(CAppearanceModifierGraphics& value);           // 0x0046FE40
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(float& value);                                 // 0x00431061
    template <class T> void Transfer(T& value);              // enum instances: EClothingSuitPart 0x0046FCF8, EHeroAttachableAppearanceModifierType 0x0046FD9C
};
class CAppearanceModifierDef : public CSubDefClassBase {    // vtable 0x012330DC
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012330DC slot 18
    std::vector<CCharString > HideMaterials;           // +0x28
    CAppearanceModifierGraphics MorphGraphics;         // +0x34 (Ego_r +0x38)
    long CoversBodyAreaFlags;                          // +0x40 (Ego_r +0x48)
    bool IsRemoveable;                                 // +0x44 (Ego_r +0x4C)
    long HeroSuit;                                     // +0x48 (Ego_r +0x50)
    EClothingSuitPart SuitPart;                        // +0x4C (Ego_r +0x54)
    EHeroAttachableAppearanceModifierType AppearanceType; // +0x50 (Ego_r +0x58)
    float SoundRadiusMultiplier;                       // +0x54 (Ego_r +0x5C)
    float VisibilityMultiplier;                        // +0x58 (Ego_r +0x60)
    long ArmourDefIndex;                               // +0x5C (Ego_r +0x64)
    float OveralProtectionWeighting;                   // +0x60 (Ego_r +0x68)
    float Scariness;                                   // +0x64 (Ego_r +0x6C)
    float Attractiveness;                              // +0x68 (Ego_r +0x70)
    float Goodstrength;                                // +0x6C (Ego_r +0x74)
};

void CAppearanceModifierDef::Transfer(CPersistContext& context)
{
    context.Transfer(HideMaterials);
    context.Transfer(MorphGraphics);
    context.Transfer(CoversBodyAreaFlags);
    context.Transfer(IsRemoveable);
    context.Transfer(HeroSuit);
    context.Transfer(SuitPart);
    context.Transfer(AppearanceType);
    context.Transfer(Scariness);
    context.Transfer(Attractiveness);
    context.Transfer(Goodstrength);
    context.Transfer(SoundRadiusMultiplier);
    context.Transfer(VisibilityMultiplier);
    context.Transfer(ArmourDefIndex);
    context.Transfer(OveralProtectionWeighting);
}