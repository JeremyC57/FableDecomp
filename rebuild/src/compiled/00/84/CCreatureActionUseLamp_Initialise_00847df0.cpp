// CCreatureAction_UseLamp::Initialise() -- choose the creature's "LIGHT_LAMP" animation, filtered by the animation group the lamp's
// action-use component names (when the lamp is alive and has one), play it, and for the player character with a camera manager put the
// camera into a NCameraModes::CScriptModuleUseLamp scripted mode lasting the action's total time; then the base initialisation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847DF0, length 442 bytes (vtable 0x0127486C slot 12).
// Leaf: ?Initialise@CCreatureAction_UseLamp@@UAEXXZ
// Identity: Ego_r.pdb ?Initialise@CCreatureAction_UseLamp@@UAEXXZ + rtti-vtables.tsv (CCreatureAction_UseLamp slot 12).
// Callees: CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap::LowerBound const 0x004365B0 (the inline CThing::PeekTC, TCI_ON_ACTION_USE
//   13 = bit 13 of UsingComponents +0x20); CTCActionUseBase::GetActionUseAnimationGroupName const 0x007EABA0; CCRC::Calc 0x004014A0
//   (the inline CCharString::GetCRC inside the inline CAnimFilterOfGroup constructor; filter vtables 0x01267568 / base 0x0122FFCC);
//   CThingCreatureBase::GetRandomAnimFromNameWithFilter 0x00662EF0; CCreatureActionBase::SetAnimation 0x00693030; CThingCreatureBase::
//   IsPlayerCharacter (vtable slot 77, +0x134); CCreatureActionBase::GetWorld 0x00692940; CAIGameCameraBase::GetCameraManager (slot 9,
//   +0x24) on CWorld::PCurrentGameCamera +0x34; operator new 0x00BFEA1A (0xa0 bytes); CCreatureActionBase::GetTotalTimeWorldFrames
//   0x006925B0; NCameraModes::CScriptModuleUseLamp(const CThing&, const CThing&, long) 0x00853120; CCountedPointer<CScriptModuleBase>(T*)
//   0x005ED270 (called COMDAT, built in the argument area); CGameCameraManager::SetScriptedCameraMode 0x00699E60;
//   CCreatureActionBase::Initialise 0x00694D10.
// Layout: PCreature +0x08 (CIntelligentPointer<CThingCreatureBase>), PObjectToUse +0xa8 (CIntelligentPointer<CThing>).
// Shape: the animation entry is bound to a reference local (the "LIGHT_LAMP" temporary dies before SetAnimation's arguments are
//   pushed); the module's reference arguments are operator* dereferences (VC7.1 then evaluates lamp, creature, duration and pushes
//   them afterwards); CAnimFilterOfGroup has no user-declared destructor (no derived vptr store on the way out, only the base's).
// Local declarations: NCameraModes::CScriptModuleBase / CScriptModuleUseLamp have no shared header (sizes from the landed 0x00853120:
//   base state to +0x4c, module 0xa0 bytes); only the constructor is used.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_ANIMGROUPNAME
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETTOTALTIMEWORLDFRAMES
#define FABLE_RETAIL_CCREATUREACTION_USELAMP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBase.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UseLamp.h"
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
    unsigned char _state[0x4c - 4];
};
class CScriptModuleUseLamp : public CScriptModuleBase {   // 0xa0 bytes
public:
    CScriptModuleUseLamp(const CThing& creature, const CThing& lamp, long duration);   // 0x00853120
    unsigned char _state2[0xa0 - 0x4c];
};
}

void CCreatureAction_UseLamp::Initialise()
{
    CCharString anim_group;
    if (PObjectToUse.Get() != 0 && !PObjectToUse.Get()->IsDead()) {
        const CTCActionUseBase* action_use;
        if (PObjectToUse.Get()->PeekTC(TCI_ON_ACTION_USE, &action_use))
            anim_group = action_use->GetActionUseAnimationGroupName();
    }
    CAnimFilterOfGroup filter(anim_group);
    const CAnimationEntry& anim = PCreature.Get()->GetRandomAnimFromNameWithFilter("LIGHT_LAMP", filter);
    SetAnimation(anim, -1.0f, 1.0f);
    if (PCreature.Get()->IsPlayerCharacter()) {
        CGameCameraManager* camera_manager;
        if (GetWorld().GetGameCamera()->GetCameraManager(&camera_manager))
            camera_manager->SetScriptedCameraMode(new NCameraModes::CScriptModuleUseLamp(*PCreature, *PObjectToUse, GetTotalTimeWorldFrames()));
    }
    CCreatureActionBase::Initialise();
}