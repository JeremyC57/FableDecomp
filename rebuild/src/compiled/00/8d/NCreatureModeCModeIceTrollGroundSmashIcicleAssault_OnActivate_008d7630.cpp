// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::OnActivate() -- fetch the troll's CSnowTrollDef sub-def into the mode, reset the timer, suspend an AI creature, and collect the viable icicle positions around the live target (else around the troll) within the def's distance range
// Leaf: ?OnActivate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7630, 242 bytes (virtual, `ret`).
// Callees: CModeStandBase::OnActivate 0x008621A0 (qualified), CDefClassBase::GetSubDef<CSnowTrollDef> 0x008C7A70 on the parent's base def
//   (CThing::GetPBaseDef, +0x70, by value), CThingAICreature::SuspendAI 0x00833A10 (type 1), CThing::GetPos 0x004C73D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (IS_THING_ALIVE on PTarget, then the target's position), CModeBase::GetWorld 0x0073A680
//   (twice), CWorld slot 12 GetWorldMap (+0x30), NCreatureHiding::GetValidPositions 0x0074A5D0 (__fastcall).
// Layout: Ego_r.pdb CModeIceTrollGroundSmashIcicleAssault : CModeStandBase (0x68): PTarget +0x68, Filter +0x70, Timer +0x7c, PDef +0x80,
//   Icicles +0x84, ViablePoints +0x90 (retail 12-byte STLport vectors); CSnowTrollDef DistanceRange +0x48; CWorld PThingSearchTools +0x20.
// Naming (NF135, 2026-11-20; q6453 / q6455): moved off the ThingNative / ModeStandNative / ModeIceTroll / WorldNative / NativeDefinition /
//   SnowDefinitionPointer / IntelligentThingPointer / PositionVector stand-ins and the quarantine CThing (included under a renaming macro) onto
//   engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + _ACTIVATE) over the shared nested CTCCreatureModeManager::CModeBase (VIRTUALS: GetWorld),
//   engine/CThing.h, engine/CThingAICreature.h (DRAINED), engine/CIntelligentPointer.h + engine/ThingAlive.h (IS_THING_ALIVE on PTarget),
//   engine/CSnowTrollDef.h (new NF135 opt-in FABLE_RETAIL_CSNOWTROLLDEF: the retail class over CSubDefClassBase) + engine/CDefClassBase_GetSubDef.h,
//   engine/CWorld.h (GetThingSearchTools, slot 12 GetWorldMap) and engine/NCreatureHiding.h. RELOCATION_MATCH in the second form (the first:
//   C2440, the flat CSnowTrollDef is not a CDefPointee). Self symbol ?OnActivate@ModeIceTroll@@QAEXXZ ->
//   ?OnActivate@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UAEXXZ (Ego_r.pdb 0x008189B0, FableWin.pdb 0x02846FB0; vtable 0x01268774
//   slot 5; no landed row references 0x008D7630). Row-local class: no shared header anywhere under rebuild/include; the landed constructor rows
//   0x00743BE0 / 0x00743FE0 / 0x008C7C00 model it locally; its base is the shared CModeStandBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CSNOWTROLLDEF
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CSnowTrollDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CWorld.h"
#include "engine/NCreatureHiding.h"

namespace NCreatureMode {
// Ego_r.pdb / FableWin.pdb ??0CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@QAE@AAVCThing@@@Z; RTTI CModeIceTrollGroundSmashIcicleAssault
// : CModeStandBase; Ego_r.pdb members (offsets above). No shared header (row-local); only the override this row defines is declared; the nested
// CIcicleData (Ego_r vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>) is declared only.
class CModeIceTrollGroundSmashIcicleAssault : public CModeStandBase {
public:
    virtual void OnActivate();
private:
    class CIcicleData;
    CIntelligentPointer<CThing>      PTarget;        // +0x68
    CAnimFilterOfGroup               Filter;         // +0x70
    long                             Timer;          // +0x7c
    CDefPointer<const CSnowTrollDef> PDef;           // +0x80
    std::vector<CIcicleData>         Icicles;        // +0x84
    std::vector<C3DVector>           ViablePoints;   // +0x90
};

void CModeIceTrollGroundSmashIcicleAssault::OnActivate()
{
    CModeStandBase::OnActivate();
    ParentThing.GetPBaseDef()->GetSubDef(&PDef);
    Timer = 0;
    if (ParentThing.GetType() == 1)
        ((CThingAICreature&)ParentThing).SuspendAI(true, 0);
    C3DVector pos = ParentThing.GetPos();
    if (IS_THING_ALIVE(PTarget))
        pos = PTarget->GetPos();
    CWorld& world = GetWorld();
    const CSnowTrollDef* def = PDef.Get();
    const CThingSearchTools& search = GetWorld().GetThingSearchTools();
    NCreatureHiding::GetValidPositions(ViablePoints, world.GetWorldMap(), search, pos, def->DistanceRange, false, 0);
}

}
