// COracleMinigameDef::Transfer(CPersistContext& context) -- persist the COracleMinigameDef's fields (13 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@COracleMinigameDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF658, 156 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): COracleMinigameDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123E504, whose RTTI type descriptor is .?AVCOracleMinigameDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@COracleMinigameDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@COracleMinigameDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J058: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(unsigned long&) 0x00431020.
// Layout: Ego_r.pdb COracleMinigameDef members at their PDB offsets; every transferred offset hits the PDB member of the asserted type (the def transfers out of declaration order).
//   engine/COracleMinigameDef.h is the flattened data-only view (vptr as a member, no virtual Transfer to define), so the class is local, on the real base chain.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
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
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(unsigned long& value);                         // 0x00431020
};
class COracleMinigameDef : public CSubDefClassBase {    // vtable 0x0123E504
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123E504 slot 18
    float WaitBetweenRoundsSeconds;                    // +0x28
    float PauseBetweenOutputSeconds;                   // +0x2C
    float PauseBetweenOutputsLevelMultiplier;          // +0x30
    long MaxLevel;                                     // +0x34
    unsigned long TintRed;                             // +0x38
    unsigned long TintGreen;                           // +0x3C
    unsigned long TintBlue;                            // +0x40
    unsigned long TextLose;                            // +0x44
    unsigned long TextWin;                             // +0x48
    unsigned long TextWatch;                           // +0x4C
    unsigned long TextRepeat;                          // +0x50
    unsigned long TextOfferGame;                       // +0x54
    unsigned long TextWonRound;                        // +0x58
};

void COracleMinigameDef::Transfer(CPersistContext& context)
{
    context.Transfer(WaitBetweenRoundsSeconds);
    context.Transfer(PauseBetweenOutputSeconds);
    context.Transfer(PauseBetweenOutputsLevelMultiplier);
    context.Transfer(MaxLevel);
    context.Transfer(TintRed);
    context.Transfer(TintGreen);
    context.Transfer(TintBlue);
    context.Transfer(TextWin);
    context.Transfer(TextLose);
    context.Transfer(TextWatch);
    context.Transfer(TextRepeat);
    context.Transfer(TextOfferGame);
    context.Transfer(TextWonRound);
}