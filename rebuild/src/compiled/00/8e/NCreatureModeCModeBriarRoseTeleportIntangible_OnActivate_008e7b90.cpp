// NCreatureMode::CModeBriarRoseTeleportIntangible::OnActivate() -- Briar Rose turns intangible: read the intangibility time, theme and theme transition time from her CBriarRoseDef sub-def (defaults 6 s, no theme, 1 s), set the mode timer in frames, suspend an AI creature, take her out of the mapwho and targeting, hide her, and transition the environment theme on all six channels
// Leaf: ?OnActivate@CModeBriarRoseTeleportIntangible@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7B90, 342 bytes (virtual, `ret`); vtable 0x0128026C slot 5.
// Callees: CModeStandBase::OnActivate 0x008621A0 (qualified), CDefClassBase::GetSubDef<CBriarRoseDef> 0x008E7790 on the parent's base def
//   (CThing::GetPBaseDef, +0x70, by value), the def's virtual slot 1 (release), __ftol2 0x00BFEA70, CThingAICreature::SuspendAI 0x00833A10,
//   CVectorMap LowerBound 0x0040F020 (GetTC of TCI_MAPWHO 0x3a and TCI_TARGETED 9), CThing slot 28 SetAsDrawable (+0x70),
//   CModeBase::GetWorld 0x0073A680, CEnvironment::TransitionToTheme(long, long, long, float) 0x006B3800 (CWorld +0x18).
// Globals: ?ConstantFPS@CMainGameComponent@@1JA 0x01375550 (through the inline GetConstantFPS()).
// Layout: Ego_r.pdb CModeBriarRoseTeleportIntangible : CModeStandBase (0x68): Filter +0x68 (CAnimFilterOfGroup), Timer +0x74, PTarget +0x78;
//   CBriarRoseDef (engine/CBriarRoseDef.h) SecondsToBeIntangible +0x2c, EnvironmentThemeForIntangibleMode +0x3c,
//   SecondsForIntangibleEnvironmentThemeTransitionIn +0x40.
// Naming (NF135, 2026-11-20; q6453 / q6455): moved off the local CModeBase / CModeStandBase / CGlobalComponents / CDefPointer / CDefCopy /
//   CDefHolder / CTCFlagged / CTCType9 / CThing models and the same-TU GetSubDef stand-in onto engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE
//   + _ACTIVATE) over the shared nested CTCCreatureModeManager::CModeBase (VIRTUALS: GetWorld), engine/CThing.h (GETTC_EXPANDED, ORIGINAL_VTABLE
//   slot 28 SetAsDrawable, GetPBaseDef), engine/CThingAICreature.h (DRAINED), engine/CTCMapwho.h (SETINMAP), engine/CTCTargeted.h, engine/CBriarRoseDef.h
//   + engine/CDefClassBase_GetSubDef.h, engine/CMainGameComponent.h (GetConstantFPS), engine/CWorld.h (ENVIRONMENT) and engine/CEnvironment.h
//   (TRANSITIONTOTHEME); the same define set as 0x008E7CF0. RELOCATION_MATCH in one form. HEAD named the teleport sub-def CBriarRoseTeleportDef
//   and its fields Duration / ThemeChangeCount / ThemeScale: 0x008E7790 is GetSubDef<CBriarRoseDef> (its landed self leaf and the
//   "CBriarRoseDef" literal), so the fields are CBriarRoseDef SecondsToBeIntangible +0x2c / EnvironmentThemeForIntangibleMode +0x3c /
//   SecondsForIntangibleEnvironmentThemeTransitionIn +0x40 (Ego_r layout), and the mode member +0x74 is Ego_r Timer; the six calls are
//   TransitionToTheme(channel, 2, theme, time) and the slot-28 call is SetAsDrawable(false) (HEAD: StopMoving).
//   Self symbol ?OnActivate@CModeBriarRoseTeleportIntangible@NCreatureMode@@QAEXXZ -> ...@@UAEXXZ (Ego_r.pdb 0x00830B30, FableWin.pdb 0x0280CCA0;
//   vtable 0x0128026C slot 5; no landed row references 0x008E7B90). Row-local class: no shared header anywhere under rebuild/include; the
//   landed siblings 0x008E8940 / 0x008C3700 model it the same way; its base is the shared CModeStandBase.
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
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SETINMAP
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TRANSITIONTOTHEME
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
#include "engine/CMainGameComponent.h"
#include "engine/CAnimFilterOfGroup.h"

namespace NCreatureMode {
// Ego_r.pdb / FableWin.pdb ??0CModeBriarRoseTeleportIntangible@NCreatureMode@@QAE@AAVCThing@@AAVCThingCreatureBase@@@Z; RTTI
// CModeBriarRoseTeleportIntangible : CModeStandBase; Ego_r.pdb members Filter +0x68, Timer +0x74, PTarget +0x78. No shared header (row-local,
// as the landed siblings 0x008E8940 / 0x008C3700); only the override this row defines is declared.
class CModeBriarRoseTeleportIntangible : public CModeStandBase {
public:
    virtual void OnActivate();
private:
    CAnimFilterOfGroup                      Filter;     // +0x68
    long                                    Timer;      // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;    // +0x78
};

void CModeBriarRoseTeleportIntangible::OnActivate()
{
    CModeStandBase::OnActivate();
    float seconds = 6.0f;
    long theme = 0;
    float time = 1.0f;
    CDefPointer<const CBriarRoseDef> pdef;
    if (ParentThing.GetPBaseDef()->GetSubDef(&pdef)) {
        theme = pdef->EnvironmentThemeForIntangibleMode;
        seconds = pdef->SecondsToBeIntangible;
        time = pdef->SecondsForIntangibleEnvironmentThemeTransitionIn;
    }
    Timer = (long)(CMainGameComponent::GetConstantFPS() * seconds);
    if (ParentThing.GetType() == 1)
        ((CThingAICreature&)ParentThing).SuspendAI(true, this);
    CTCMapwho* mapwho;
    if (ParentThing.GetTC(&mapwho))
        mapwho->SetInMap(false);
    CTCTargeted* targeted;
    if (ParentThing.GetTC(&targeted))
        targeted->SetTargetable(false);
    ParentThing.SetAsDrawable(false);
    if (theme > 0) {
        for (long channel = 0; channel < 6; ++channel)
            GetWorld().GetEnvironment().TransitionToTheme(channel, 2, theme, time);
    }
}

}
