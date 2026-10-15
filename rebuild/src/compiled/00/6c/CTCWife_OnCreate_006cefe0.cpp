// CTCWife::OnCreate() -- fetch the hero-marriage def, then stagger the gift-culling timer with a world-random frame below the culling interval
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CEFE0, length 92 bytes (+ int3 padding).
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (the global definitions holder [0x013B8A1C]+0x20; manifest
// "_Cons_val<...>" is propagated), CDefClassBase::GetSubDef<CHeroMarriageDef> @ 0x006D3D00 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>" is the propagated template name), CTCBase::GetPRandomSeed() @ 0x00686DA0
// ([0x013B8A1C]+0x2c; manifest "CViewBase::GetDisplayEngine", propagated), GFROR13 @ 0x00497890 (__fastcall).
// Ego_r.pdb: ?OnCreate@CTCWife@@UAEXXZ. Layout: engine/CTCWife.h PHeroMarriageDef +0x0c; past the GiftsReceived map the
// retail object sits 4 bytes below the PDB (12-byte retail map), so FrameLastCulledGiftsReceived is +0x88 (header +0x8c):
// the local model below carries the retail offsets. engine/CHeroMarriageDef.h FramesBetweenCullingOldGifts +0x50.
// The definitions holder keeps the hero-marriage defs' container at +0xd0 (as 0x004C79A0 returns).
// Shape: the definitions holder is a named local so its call precedes the `push edi` of the def pointer's address
// (a bare chain pushes the argument first); the def object is read into a local right after GetSubDef (ebx), its interval read after the seed call; the
// inline world-random step stores the LCG value then the rotated seed, then `if (range != 0) return seed % range; return 0;` (the
// zero case placed second; the early `return 0` polarity puts it first).
#include "engine/CHeroMarriageDef.h"

extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}

struct CDefPointer_CHeroMarriageDef {
    const CHeroMarriageDef* Object;
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer_CHeroMarriageDef* out);      // 0x006D3D00
};

struct CDefinitions {
    unsigned char  _pad_00[0xd0];
    CDefClassBase* m_pMarriageDefs;                         // +0xd0
};

struct CTCWife {                                            // retail layout (see above)
    void*                        __vftable;                 // +0x00
    void*                        ParentThing;               // +0x04
    bool                         ToBeRemoved;               // +0x08
    bool                         IsInGlobalUpdateTCs;       // +0x09
    CDefPointer_CHeroMarriageDef PHeroMarriageDef;          // +0x0c
    unsigned char                _pad_0x10[0x78];
    unsigned long                FrameLastCulledGiftsReceived;   // +0x88 (PDB +0x8c)
    CDefinitions*  GetDefinitions();                        // 0x006869D0 (CTCBase)
    unsigned long* GetPRandomSeed();                        // 0x00686DA0 (CTCBase)
    void OnCreate();
};

void CTCWife::OnCreate()
{
    CDefinitions* definitions = GetDefinitions();
    definitions->m_pMarriageDefs->GetSubDef(&PHeroMarriageDef);
    const CHeroMarriageDef* def = PHeroMarriageDef.Object;
    unsigned long* seed = GetPRandomSeed();
    FrameLastCulledGiftsReceived = WorldRandomRange(*seed, def->FramesBetweenCullingOldGifts);
}