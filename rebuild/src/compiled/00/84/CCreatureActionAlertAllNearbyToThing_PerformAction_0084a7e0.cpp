// CCreatureAction_AlertAllNearbyToThing::PerformAction() -- alert everything within Radius of the creature to the stored thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A7E0, length 51 bytes. Vtable slot 3 (PerformAction) of
// CCreatureAction_AlertAllNearbyToThing (0x012736A4).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CCreatureActionBase::GetThingManager
// @ 0x00692980 (`this+8` GetPItem then the global @ 0x013B8A1C), the manager's getter @ 0x0051E490 (`[this+24h]->[+20h]`;
// the manifest label "CPixelShader::CountReferences" is propagated onto this two-load getter), and the __fastcall
// member @ 0x0084A720 (this in ecx, the thing in edx, the alerting creature and the float radius on the stack; the
// manifest "CTCTrapBase::UpdateTriggerThings" label is propagated -- the body reads both stack arguments as a
// creature position and a float).
// Shape: the creature is a named local fetched BEFORE the radius conversion (`fild [this+0A8h]` sits between the
// GetPItem call and the pushes: a memory read that retail keeps ahead of the manager calls); arguments right to left:
// the float, the creature, then the object chain. No engine header exists for this class: Radius (long, +0xa8) and
// PThing (plain pointer, +0xac) follow the 0xa8-byte retail CCreatureActionBase.
class CThing;
class CThingCreatureBase;

class CThingSearchTools;
// NF155 (q5074): 0x0051E490 is Ego_r.pdb / FableWin.pdb ?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ (engine/
// CThingManager.h; `[this+24h]->[+20h]`, tail-called by CThing::GetThingSearchTools 0x004C7A50) and 0x0084A720 the landed static
// ?AlertAllNearby@CCreatureAction_AlertAllNearbyToThing@@SIXABVCThingSearchTools@@ABVCThing@@AAVCThingCreatureBase@@M@Z (Ego_r.pdb
// 0x00525C90 + FableWin.pdb 0x01DDBE80; was the stand-ins CThingManager::GetNearbyAlerter / CNearbyAlerter::AlertAllNearbyToThing).
class CThingManager { public:
    CThingSearchTools& GetThingSearchTools();     // 0x0051E490
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
// agentNF16: derive from the shared-header CCreatureActionBase (retail branch: PCreature +0x08, sizeof 0xa8) so the
//   protected accessor 0x00692980 binds its Ego_r / FableWin spelling ?GetThingManager@CCreatureActionBase@@IAEAAVCThingManager@@XZ
//   (was a local CCreatureAction_AlertAllNearbyToThing::GetThingManager returning a pointer).
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETTHINGMANAGER
#include "engine/CCreatureActionBase.h"
class CThing : public CBaseObject { };
class CThingCreatureBase : public CThing { };

class CCreatureAction_AlertAllNearbyToThing : public CCreatureActionBase { public:
    virtual void PerformAction();           // slot 3
    static void __fastcall AlertAllNearby(const CThingSearchTools& tools, const CThing& perceived, CThingCreatureBase& alerter,
                                          float radius);   // 0x0084A720
    long    Radius;                         // +0xa8
    CThing* PThing;                         // +0xac
};

void CCreatureAction_AlertAllNearbyToThing::PerformAction()
{
    CThingCreatureBase* creature = PCreature.Get();
    AlertAllNearby(GetThingManager().GetThingSearchTools(), *PThing, *creature, (float)Radius);
}