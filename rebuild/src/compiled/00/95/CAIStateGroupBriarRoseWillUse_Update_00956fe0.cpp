// CAIStateGroup_BriarRoseWillUse::Update() -- use Briar Rose's will on a live enemy: force-push it when close, shoot far into it otherwise; then schedule the next use
// Leaf: ?Update@CAIStateGroup_BriarRoseWillUse@@UAEXXZ
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956FE0, length 389 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, CAIStateGroupBase::Terminate @ 0x008FCF40,
//          CDefClassBase::GetSubDef<CBriarRoseDef> @ 0x008E7790 (landed), the def's slot 1 release,
//          CThing::GetPos @ 0x004C73D0, CCreatureAction_BriarRoseForcePush(creature, target) @ 0x009572A0 (vtable 0x01294CD4),
//          CCreatureAction_BriarRoseShotFarInto(creature, target) @ 0x00957170 (landed), AIAction_PerformActionAndWait @ 0x0097DB70,
//          the combat action base dtor @ 0x0062E900, CAIStateGroupBase::GetWorld @ 0x008FCFC0, the static frame counter @ 0x0049D870, __ftol2.
// Globals: frames per second @ 0x01375550 (int 15).
// Shape: IS_THING_ALIVE on PEnemy (two GetPItem); the creature's base def is copied into a CDefPointer temporary around GetSubDef and the
//          sub-def is released after Terminate (the visible noinline GetSubDef body); the push threshold and the use interval are copied
//          to float locals; when the enemy's type flags (+0x6C) bit 3 is set, the XY distance is compared inline against threshold^2 and
//          the two action calls share their hoisted `push 0 ; push target`; each arm reads the creature through GetCreature().
// NF137 (2026-11-20, q5021): moved onto the shared headers (the sibling 0x00956E70 block): AIAction_PerformActionAndWait 0x0097DB70 now binds
//   the PDB ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z (Ego_r.pdb 0x00848940 +
//   FableWin.pdb 0x02892570; engine/CAIStateGroupBase.h PERFORMACTION), was the CAIStateGroupBriarRoseWillUseMethods stand-in over the
//   namespaced N_0062e900 action base. The state group is the retail class (overlay opt-in FABLE_RETAIL_CAISTATEGROUP_BRIARROSEWILLUSE);
//   own leaf now the PDB public virtual ?Update@CAIStateGroup_BriarRoseWillUse@@UAEXXZ (Ego_r.pdb 0x0097BC70 + FableWin.pdb 0x02C027D0).
//   The row-local CCreatureAction_BriarRoseForcePush (no shared header) now derives from the shared CCombatActionBase (RTTI) and its
//   constructor binds the spelling its self row 0x009572A0 defines (the PDB reference form). ShotFarInto is the shared
//   engine/CCreatureAction_BriarRoseShotFarInto.h class; its constructor (overlay opt-in FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CTOR) now binds the PDB reference form
//   ??0CCreatureAction_BriarRoseShotFarInto@@QAE@AAVCThingCreatureBase@@0@Z (Ego_r.pdb 0x0097BE30 + FableWin.pdb 0x02C02D20), was the
//   PAVCThing@@0 stand-in (its self row 0x00957170 still defines a pointer form: NAME_FIXES). The hand-written same-TU GetSubDef stand-in is gone for engine/CDefClassBase_GetSubDef.h
//   (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, GOTCHAS agentNF25b).
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CLASS
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUP_BRIARROSEWILLUSE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#include "engine/EAIAction.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_BriarRoseWillUse.h"
#include "engine/CAIBrain.h"
#include "engine/CBriarRoseDef.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_BriarRoseShotFarInto.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/C2DVector.h"

class CCreatureAction_BriarRoseForcePush : public CCombatActionBase {   // RTTI : CCombatActionBase (no shared header)
public:
    CCreatureAction_BriarRoseForcePush(CThingCreatureBase& creature, CThingCreatureBase& enemy);   // 0x009572A0
    CIntelligentPointer<CThingCreatureBase> PEnemy;                    // +0x114
};

void CAIStateGroup_BriarRoseWillUse::Update()
{
    if (!IS_THING_ALIVE(PEnemy)) {
        Terminate();
        return;
    }
    CDefPointer<const CBriarRoseDef> pdef;
    GetCreature().GetPBaseDef()->GetSubDef(&pdef);
    float threshold = pdef->DistanceForcePushThreshold;
    float interval = pdef->SecondsBetweenWillUse;
    if (PEnemy->GetTypeFlags() & 8) {
        CThing* enemy = PEnemy.Get();
        CThingAICreature& creature = GetCreature();
        CThing* target = PEnemy.Get();
        const C3DVector& creaturePos = creature.GetPos();
        const C3DVector& pos = target->GetPos();
        C2DVector delta(pos.X - creaturePos.X, pos.Y - creaturePos.Y);
        if (delta.GetSquaredMagnitude() < threshold * threshold)
            AIAction_PerformActionAndWait(CCreatureAction_BriarRoseForcePush(GetCreature(), *(CThingCreatureBase*)enemy), false);
        else
            AIAction_PerformActionAndWait(CCreatureAction_BriarRoseShotFarInto(GetCreature(), *(CThingCreatureBase*)enemy), false);
    }
    NextWillUseFrame = (long)(GetWorld().GetFrame() + CMainGameComponent::GetConstantFPS() * interval);
    Terminate();
}
