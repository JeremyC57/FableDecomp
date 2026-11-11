// CTCDummyVillager::SetupFromData(unsigned long defType, CThingBuilding* home, CThingBuilding* work, bool respawnable) -- record the def type and the home / work buildings; a respawnable dummy is due back tomorrow, or (when its def has a creature def) after a random wait between the creature's RespawnWaitInDays min and max from today; otherwise never
// Leaf: ?SetupFromData@CTCDummyVillager@@QAEXKPAVCThingBuilding@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072F170, 384 bytes (`ret 10h`). Ego_r.pdb
// `?SetupFromData@CTCDummyVillager@@QAEXKPAVCThingBuilding@@0_N@Z`.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 (then the CIntelligentPointer<CThingBuilding> vtable 0x01244344), SetPItem
// @ 0x00A01B90, GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10, CTCBase::GetWorld() const @ 0x00686A70,
// CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CCreatureDef> @ 0x00485815, CTCBase::GetPWorldSeed() const @ 0x00686DA0, GFROR13 @ 0x00497890 (__fastcall).
// Layout: engine/CTCDummyVillager.h (Ego_r.pdb): DefType +0x0c, HomeBuilding +0x10, WorkBuilding +0x20, DayToBeRecreated +0x40.
// CWorld PGameTimeManager +0x1c, CGameTimeManager DayCount +0x14 (engine/CGameTimeManager.h). CCreatureDef RespawnWaitInDaysMin / Max
// at +0xb8 / +0xbc (engine/CCreatureDef.h FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT).
// Shape: the building pointers are assigned from converted temporaries; the random wait is the inline GFRandom.
// Name fix (agentNF10 2026-11-11): moved off the local CTCBase / CTCDummyVillager / CDefinitionManager / CDefClassBase / CDefPointer
// stand-ins onto the shared headers. The thing def handle is CDefPointer<const CThingBaseDef>, so 0x0042AF3C binds GetDef<CThingBaseDef>
// (was GetDef<CDefClassBase>, which belongs to 0x0042B144: agentNF9 NAME_FIXES); the row's own symbol takes the PDB spelling.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/GFRandom.h"
#include "engine/CTCDummyVillager.h"

void CTCDummyVillager::SetupFromData(unsigned long def_type, CThingBuilding* home_building, CThingBuilding* work_building, bool respawnable)
{
    DefType = def_type;
    HomeBuilding = CIntelligentPointer<CThingBuilding>(home_building);
    WorkBuilding = CIntelligentPointer<CThingBuilding>(work_building);
    if (respawnable) {
        DayToBeRecreated = GetWorld().GetGameTimeManager().GetDayCount() + 1;
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(DefType, &def);
        CDefPointer<const CCreatureDef> creature_def;
        if (def->GetSubDef(&creature_def)) {
            long today = GetWorld().GetGameTimeManager().GetDayCount();
            DayToBeRecreated = creature_def->RespawnWaitInDaysMin
                + GFRandom(creature_def->RespawnWaitInDaysMax - creature_def->RespawnWaitInDaysMin, GetPWorldSeed()) + today;
        }
    } else
        DayToBeRecreated = 0x7fffffff;
}
