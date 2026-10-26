// CTavernTableDef::Transfer(CPersistContext& context) -- persist the CTavernTableDef's fields (2 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CTavernTableDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE9D2, 33 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CTavernTableDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123C964, whose RTTI type descriptor is .?AVCTavernTableDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CTavernTableDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CTavernTableDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J080: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(bool&) 0x0043314A.
// Layout (packed): the two bools follow the 0x25-byte packed CSubDefClassBase directly (+0x25, +0x26), as Ego_r.pdb places them; Ego_r.pdb CTavernTableDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type.
//   engine/CTavernTableDef.h is the flattened data-only view (vptr as a member, no virtual Transfer to define), so the class is local, on the real base chain.
// Shape: two members, so no edi: this in esi, the context re-read from its stack slot (`mov ecx,[esp+8]`) before each call,
//   the first address through eax, the last formed in place (`add esi,N`).
#pragma optimize("s", on)
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
class CPersistContext {
public:
    void Transfer(bool& value);                                  // 0x0043314A
};
class CTavernTableDef : public CSubDefClassBase {    // vtable 0x0123C964
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123C964 slot 18
    bool AddTankards;                                  // +0x25
    bool HighQualityTankards;                          // +0x26
};

void CTavernTableDef::Transfer(CPersistContext& context)
{
    context.Transfer(AddTankards);
    context.Transfer(HighQualityTankards);
}