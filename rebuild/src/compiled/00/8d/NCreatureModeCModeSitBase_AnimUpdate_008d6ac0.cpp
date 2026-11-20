// NCreatureMode::CModeSitBase::AnimUpdate(CAnimateThing&) -- while no remove-action is pending, count the idle timer down; when the sitting animation (type 0x15) is playing, start a one-off ST_IDLE action once the timer runs out, otherwise (re)start ST_IDLE_SUBTLE and, after an idle, re-seed the timer with a random frame count
// Leaf: ?AnimUpdate@CModeSitBase@NCreatureMode@@UAEXAAVCAnimateThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6AC0, 444 bytes (padded to 448), `ret 4`; CModeSitChair vtable 0x01267B74 slot 2
//   (inherited; also CModeSleepBed's sibling slot).
// Name: FableWin.pdb `?AnimUpdate@CModeSitBase@NCreatureMode@@UAEXAAVCAnimateThing@@@Z` -- CModeSitChair has no AnimUpdate of
//   its own, so the catalog's CModeSitChair::vfunc_2 is the base (as the landed CModeSitBase::OnActivate 0x008D6580, slot 5).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("ST_IDLE" 0x0127138C, "ST_IDLE_SUBTLE" 0x01276DBC) / ~CCharString
//   @ 0x0099EAE0, CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, const CSoundPair&, bool, bool, bool,
//   long, long) @ 0x008424F0, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0,
//   ~CCreatureAction_PlayAnimation @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated),
//   CThingCreatureBase::GetAnimFromName @ 0x00662A00, CAnimationEntry::GetAnimateThingFlags @ 0x005DC390 / GetTimeToBlendIn
//   @ 0x005DC230, CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim @ 0x0070D580, virtual GetType (slot 1),
//   CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims @ 0x0073A6F0, _ftol @ 0x00BFEA70, CModeBase::GetWorld @ 0x0073A680,
//   the world's random seed accessor @ 0x0049D810, GFROR13 @ 0x00497890.
// Globals: 0.75f @ 0x01238174, -1.5f @ 0x01285670.
// Layout: Ego_r.pdb CModeSitBase (0x2c, engine/NCreatureMode_CModeSitFloor.h): IdleCounter +0x1c, PlayingIdle +0x20, PRemoveAction +0x24.
// Shape: the creature is a named local read after the counter decrement; both names are named locals in their own scopes; the parent thing
//   is a named reference read right after the second name's destructor (retail `mov ebx,[esi+4]` before GetAnimateThingFlags); the
//   re-seed is the landed CModeSitBase::OnActivate 0x008D6580 body; `PlayingIdle = false` is unconditional.
// Naming (NF128, 2026-11-20; q6453 / q6455): the local CModeBase / CModeSitBase / CThingCreatureBase / CWorld / CAnimInit / CAnimateThing /
//   CAnimationEntry / CCreatureAction_PlayAnimation / CSoundPair stand-ins replaced by the shared headers: engine/NCreatureMode_CModeSitFloor.h
//   (overlay opt-in FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ANIMUPDATE) over the shared nested CModeBase, the landed sibling 0x008D6580 define
//   set (GetNoFramesBeforePlayingCombinationIdleAnims, CWorld::GetWorldSeed, GFRandom), engine/CThingCreatureBase.h (ANIMATION: the const&
//   GetAnimFromName), engine/CAnimSet.h / CAnimateThing.h (PeekMainAnim().IsFinished() / GetType()), engine/CAnimInit.h, engine/CAnimationEntry.h
//   and engine/CCreatureAction_PlayAnimation.h (IMPLICIT_DTOR: the temporary is destroyed through ~CCreatureActionBase 0x00693EF0, the self
//   leaf there). Own leaf QAE -> UAE (the slot-2 override). RELOCATION_MATCH in two forms (the first loaded ParentThing late: 444v446).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ANIMUPDATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/NCreatureMode.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_PlayAnimation.h"

void NCreatureMode::CModeSitBase::AnimUpdate(CAnimateThing& anim)
{
    if (PRemoveAction.Data == 0) {
        --IdleCounter;
        CThingCreatureBase& creature = (CThingCreatureBase&)ParentThing;
        if (!anim.PeekMainAnim().IsFinished() && anim.PeekMainAnim().GetType() == 0x15) {
            if (!PlayingIdle && IdleCounter < 0) {
                bool started;
                {
                    CCharString name("ST_IDLE");
                    started = creature.SetCurrentAction(CCreatureAction_PlayAnimation(creature, name, CSoundPair(),
                        false, true, false, 0, 0));
                }
                if (started)
                    PlayingIdle = true;
            }
        }
        else {
            const CAnimationEntry* entry;
            {
                CCharString name("ST_IDLE_SUBTLE");
                entry = &creature.GetAnimFromName(name, 0L);
            }
            CThing& thing = ParentThing;
            anim.SetMainAnim(CAnimInit(0x15, 0, entry->BankIndex, (EAnimUpdateMode)1, true, entry->GetAnimateThingFlags(), 100),
                entry->GetTimeToBlendIn(thing), 0);
            if (PlayingIdle) {
                float frames = (float)GetNoFramesBeforePlayingCombinationIdleAnims((NCreatureMode::EMode)GetType());
                long minFrames = (long)(frames * 0.75f);
                unsigned long* seed = GetWorld().GetWorldSeed();
                unsigned long range = -minFrames - (long)(frames * -1.5f);
                IdleCounter = GFRandom(range, seed) + minFrames;
            }
            PlayingIdle = false;
        }
    }
}
