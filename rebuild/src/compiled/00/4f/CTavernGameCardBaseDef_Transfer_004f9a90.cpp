// CTavernGameCardBaseDef::Transfer(CPersistContext& context) -- persist the CTavernGameCardBaseDef's fields (18 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CTavernGameCardBaseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9A90, 211 bytes, virtual thiscall `ret 4`; size-optimised def TU (every member address through eax; the entry is 16-aligned by chance).
// Identity (proven): CTavernGameCardBaseDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0124258C, whose RTTI type descriptor is .?AVCTavernGameCardBaseDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CTavernGameCardBaseDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CTavernGameCardBaseDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J063: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer<ETavernGameControlType>(ETavernGameControlType&) 0x004F9B63, Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(CRGBColour&) 0x004595A3, Transfer(C2DVector&) 0x004568BC, Transfer(std::vector<CCardPositionsDef >&) 0x004F9C07, Transfer(std::vector<CCardDef >&) 0x004FA0A9.
// Layout: Ego_r.pdb CTavernGameCardBaseDef members rebased to retail (def base at its PDB size, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
//   engine/CTavernGameCardBaseDef.h is the flattened data-only view (vptr as a member, no virtual Transfer to define), so the class is local, on the real base chain.
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
enum ETavernGameControlType { };                       // Ego_r.pdb (enumerators not needed)
class CCardDef;
class CCardPositionsDef;
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
class C2DVector {                                         // Ego_r.pdb 8 bytes
public:
    float                                    X;
    float                                    Y;
};
class CRGBColour {                                        // Ego_r.pdb 4 bytes (opaque here)
public:
    unsigned char _body[4];
};
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(CRGBColour& value);                            // 0x004595A3
    void Transfer(C2DVector& value);                             // 0x004568BC
    void Transfer(std::vector<CCardPositionsDef >& value);       // 0x004F9C07
    void Transfer(std::vector<CCardDef >& value);                // 0x004FA0A9
    template <class T> void Transfer(T& value);              // enum instances: ETavernGameControlType 0x004F9B63
};
class CTavernGameCardBaseDef : public CSubDefClassBase {    // vtable 0x0124258C
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0124258C slot 18
    ETavernGameControlType ControlType;                // +0x28
    float CardWidth;                                   // +0x2C
    float CardHeight;                                  // +0x30
    float ImpulseScaleXbox;                            // +0x34
    float ImpulseScalePC;                              // +0x38
    long PointerModel;                                 // +0x3C
    float MoveSpeed;                                   // +0x40
    float TurnSpeed;                                   // +0x44
    float PointerHeightOffset;                         // +0x48
    long PointerParticleEffect;                        // +0x4C
    float HighlightWidth;                              // +0x50
    CRGBColour HighlightColourGood;                    // +0x54
    CRGBColour HighlightColourBad;                     // +0x58
    float CardSeparation;                              // +0x5C
    float TableSeparation;                             // +0x60
    C2DVector PackOffset;                              // +0x64
    std::vector<CCardPositionsDef > CardOrders;        // +0x6C
    std::vector<CCardDef > CardPack;                   // +0x78 (Ego_r +0x7C)
};

void CTavernGameCardBaseDef::Transfer(CPersistContext& context)
{
    context.Transfer(ControlType);
    context.Transfer(CardWidth);
    context.Transfer(CardHeight);
    context.Transfer(ImpulseScaleXbox);
    context.Transfer(ImpulseScalePC);
    context.Transfer(PointerModel);
    context.Transfer(MoveSpeed);
    context.Transfer(TurnSpeed);
    context.Transfer(PointerHeightOffset);
    context.Transfer(PointerParticleEffect);
    context.Transfer(HighlightWidth);
    context.Transfer(HighlightColourGood);
    context.Transfer(HighlightColourBad);
    context.Transfer(CardSeparation);
    context.Transfer(TableSeparation);
    context.Transfer(PackOffset);
    context.Transfer(CardOrders);
    context.Transfer(CardPack);
}