// NCreatureMode::CModeBriarRoseTeleportIntangible::OnDeactivate() -- resume AI, re-enable mapwho/targeting/drawability, restore environment themes.
// Leaf: ?OnDeactivate@CModeBriarRoseTeleportIntangible@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7CF0, 273 bytes (virtual, `ret`).
// Callees: CModeStandBase::OnDeactivate 0x00861D00 (qualified), CThingAICreature::SuspendAI 0x00833A10 (when the parent's type is 1),
//   CVectorMap LowerBound 0x0040F020 (GetTC of TCI_MAPWHO 0x3a and TCI_TARGETED 9), CThing slot 28 SetAsDrawable (+0x70),
//   CDefClassBase::GetSubDef<CBriarRoseDef> 0x008E7790 on the parent's base def (CThing::GetPBaseDef, +0x70, by value: AddRef / Release
//   around the call), CModeBase::GetWorld 0x0073A680, CEnvironment::TransitionToDefaultThemeAllChannels 0x006B3950 (CWorld +0x18).
// Data: 1.0f (the default transition time, stored in the frame).
// Layout: Ego_r.pdb CModeBriarRoseTeleportIntangible : CModeStandBase (0x68); CBriarRoseDef SecondsForIntangibleEnvironmentThemeTransitionOut
//   +0x44 (engine/CBriarRoseDef.h); CTCMapwho InMapwho +0x50 bit 0 (SetInMap, inline: `or byte [eax+50h],1`); CTCTargeted Targetable +0x10
//   (SetTargetable, inline).
// Naming (NF135, 2026-11-20; q6453 / q6455): moved off the ThingNative / ModeStandNative / ModeBriar / NativeDefinition / BriarDefinitionPointer /
//   EnvironmentMethods stand-ins and the quarantine CThing (included under a renaming macro) onto engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE
//   + _DEACTIVATE) over the shared nested CTCCreatureModeManager::CModeBase (VIRTUALS: GetWorld), engine/CThing.h (GETTC_EXPANDED,
//   ORIGINAL_VTABLE slot 28 SetAsDrawable, GetPBaseDef), engine/CThingAICreature.h (DRAINED: SuspendAI), engine/CTCMapwho.h (SETINMAP),
//   engine/CTCTargeted.h (SetTargetable), engine/CBriarRoseDef.h + engine/CDefClassBase_GetSubDef.h (GetSubDef<CBriarRoseDef>), engine/CWorld.h
//   (ENVIRONMENT) and engine/CEnvironment.h (DEFAULTTHEME); the define set of the landed 0x008D8CE0 / 0x00956E70. RELOCATION_MATCH in one form.
//   Self symbol ?OnDeactivate@ModeBriar@@QAEXXZ -> ?OnDeactivate@CModeBriarRoseTeleportIntangible@NCreatureMode@@UAEXXZ (Ego_r.pdb 0x00830CB0,
//   FableWin.pdb 0x0280D0A0; vtable 0x0128026C slot 6; no landed row references 0x008E7CF0). The row-local class has no shared header
//   anywhere under rebuild/include; the landed siblings 0x008E8940 / 0x008C3700 model it the same way; its base is the shared CModeStandBase.
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
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SETINMAP
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
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
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCTargeted.h"
#include "engine/CBriarRoseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CEnvironment.h"
#include "engine/CWorld.h"

namespace NCreatureMode {
// Ego_r.pdb / FableWin.pdb ??0CModeBriarRoseTeleportIntangible@NCreatureMode@@QAE@AAVCThing@@AAVCThingCreatureBase@@@Z; RTTI
// CModeBriarRoseTeleportIntangible : CModeStandBase. No shared header (row-local, as the landed siblings 0x008E8940 / 0x008C3700);
// only the override this row defines is declared.
class CModeBriarRoseTeleportIntangible : public CModeStandBase {
public:
    virtual void OnDeactivate();
};

void CModeBriarRoseTeleportIntangible::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
    if (ParentThing.GetType() == 1)
        ((CThingAICreature&)ParentThing).SuspendAI(false, this);
    CTCMapwho* mapwho;
    if (ParentThing.GetTC(&mapwho))
        mapwho->SetInMap(true);
    CTCTargeted* targeted;
    if (ParentThing.GetTC(&targeted))
        targeted->SetTargetable(true);
    ParentThing.SetAsDrawable(true);
    float seconds = 1.0f;
    CDefPointer<const CBriarRoseDef> pdef;
    if (ParentThing.GetPBaseDef()->GetSubDef(&pdef))
        seconds = pdef->SecondsForIntangibleEnvironmentThemeTransitionOut;
    GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2, seconds);
}

}
