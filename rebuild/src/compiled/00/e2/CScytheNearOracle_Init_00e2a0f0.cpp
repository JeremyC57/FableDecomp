// CScytheNearOracle::Init() -- make the scythe-near-oracle entity untouchable: not force-pushable, not pushable by the hero, not killable, no combo multiplier, no information marker
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2A0F0, length 128 bytes (120 + int3 padding).
// Identity: vtable 0x012E23B8 slot 2 (Init); the factory at 0x00E2A020 allocates 0x1c bytes (PDB
// CScytheNearOracle is 28 bytes: ParentClass +0x14, PMasterData +0x18) and stores this vtable.
// Callees (CGameScriptInterface virtuals, names from the vtable's mangled slots): SetIsThingForcePushable
// slot 745 (+0xba4), SetIsPushableByHero slot 844 (+0xd30, CScriptThing BY VALUE), EntitySetAsKillable
// slot 517 (+0x814), EntitySetAsToAddToComboMultiplierWhenHit slot 526 (+0x838), SetThingHasInformation
// slot 360 (+0x5a0).
// Shape: the entity-script base keeps the interface at +4 (re-read for every call) and the entity's
// CScriptThing at +8; the by-value argument is built in the argument area by CScriptThing's inline copy
// ctor (CBaseClass vtable 0x01231710, then CScriptThing 0x01238C8C) over an embedded CCountedPointer PImp
// member with its own inline copy ctor -- copying Data/Info as two loose fields CSEs `&Thing` into edi and
// reads [edi+4]/[edi+8] where retail reads [esi+0Ch]/[esi+10h].
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CCPPointerInfo { public: long m_count; };

class CGameScriptThing;
template <class T> class CCountedPointer { public:             // CCountedPointer<CGameScriptThing>
    CGameScriptThing* Data;
    CCPPointerInfo*   Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& o) : CBaseClass(o), PImp(o.PImp) {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CScytheNearOracle {
    virtual void v00();
    CGameScriptInterface* GSI;                       // +0x04
    CScriptThing          Thing;                     // +0x08
    void*                 ParentClass;               // +0x14
    void*                 PMasterData;               // +0x18
    void Init();
};

void CScytheNearOracle::Init()
{
    GSI->SetIsThingForcePushable(Thing, false);
    GSI->SetIsPushableByHero(Thing, false);
    GSI->EntitySetAsKillable(Thing, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Thing, false);
    GSI->SetThingHasInformation(Thing, false, false, false);
}