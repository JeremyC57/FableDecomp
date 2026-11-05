// CTCHero::CTCHero(CThing*) -- CTCBase(thing), the CTCHero vtable, the four use flags / title / title def / def pointer / podium flag zeroed, and the two intelligent pointers (LastWeaponEquipped +0x1d, PTeleportParticle +0x30) with the 64-bit weapon id between them
// Leaf: ??0CTCHero@@QAE@PAVCThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9FE0, length 93 bytes (+ int3 padding). Vtable 0x0125EB54 = CTCHero.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice; each followed by the CIntelligentPointer<CThing>
// vtable store 0x01238C6C from its inline derived constructor).
// Shape: Ego_r.pdb layout (0x38 bytes): WeaponsUsable +0xc, WillUsable +0xd, WeaponUseOverriddenByScripts +0xe,
// WillUseOverriddenByScripts +0xf, Title +0x10, HeroTitleObjectDef +0x14, PHeroTitleSubDef +0x18 (CDefPointer, inline
// constructor = null), OnBoastingPodium +0x1c, LastWeaponEquipped +0x1d (UNALIGNED 8-byte intelligent pointer: the pointer
// classes are declared under pack(1)), LastWeaponEquippedID +0x28 (unsigned __int64), PTeleportParticle +0x30.
// All initialisers share ebx = 0; the 64-bit id is zeroed in the initialiser list and scheduled between the two pointer
// constructors, in declaration order.
// Local model: no engine header exists for CTCHero; engine/CTCBase.h is a flattened POD (vptr as a data member), so the
// polymorphic CTCBase is modelled locally.
class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

class CDefPointeeBase;
struct CDefPointer {                                        // CDefPointer<CHeroTitleDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CTCHero : public CTCBase { public:                                  // vtable 0x0125EB54
    bool WeaponsUsable;                                     // +0x0c
    bool WillUsable;                                        // +0x0d
    bool WeaponUseOverriddenByScripts;                      // +0x0e
    bool WillUseOverriddenByScripts;                        // +0x0f
    long Title;                                             // +0x10 EHeroTitle
    long HeroTitleObjectDef;                                // +0x14
    CDefPointer PHeroTitleSubDef;                           // +0x18
    bool OnBoastingPodium;                                  // +0x1c
    CIntelligentPointer<CThing> LastWeaponEquipped;                 // +0x1d
    unsigned __int64 LastWeaponEquippedID;                  // +0x28
    CIntelligentPointer<CThing> PTeleportParticle;                  // +0x30
    virtual void v00();
    CTCHero(CThing* thing);
};

CTCHero::CTCHero(CThing* thing)
    : CTCBase(thing), WeaponsUsable(false), WillUsable(false), WeaponUseOverriddenByScripts(false),
      WillUseOverriddenByScripts(false), Title(0), HeroTitleObjectDef(0), OnBoastingPodium(false),
      LastWeaponEquippedID(0)
{
}