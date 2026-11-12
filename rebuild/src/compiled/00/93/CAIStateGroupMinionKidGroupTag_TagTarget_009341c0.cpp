// CAIStateGroup_MinionKidGroupTag::TagTarget() -- pick the nearest other active member of the tag group; run to it; if it is still
// within reach (planar distance <= own radius + 0.5) face it (Terminate if the look reports AI_ACTION_TERMINATE_PROCESS); if it is
// still within reach and more than two seconds of frames have passed since the last tag, make it the tagger and play the TAG
// animation, Terminate once that action reports AI_ACTION_TERMINATE_PROCESS.
// Leaf: ?TagTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009341C0, 580 bytes, thiscall, no arguments, plain `ret`; no EH frame, no frame pointer.
// Identity: Ego_r.pdb (0x009578A0) / FableWin.pdb (0x02B682E0) ?TagTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ (private);
//   only caller CAIStateGroup_MinionKidGroupTag::Update 0x00934870 (0x009348E5).
// Callees: CTCCreatureGroup::GetNearestOtherActiveMember 0x00812CA0; CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90
//   (inline CIntelligentPointer<const CThing>(T*), vtable 0x01237F1C, twice); CAIStateGroupBase::AIAction_MoveToThingProximity
//   0x0097E380, AIAction_LookAtThing 0x0097D6F0, AIAction_PerformActionAndWait 0x0097DB70, Terminate 0x008FCF40;
//   CThingPhysical::PeekPos 0x006A4D00; CWorld::GetFrame 0x0049D870; CTCCreatureGroupTag::SetTagger 0x008137B0;
//   CCharString(const char*, long) 0x0099EBF0 ("TAG" 0x012922A4) / ~CCharString 0x0099EAE0; CCreatureAction_PlayAnimation ctor
//   0x008424F0 / ~CCreatureActionBase 0x00693EF0 (implicit dtor). Data: CMainGameComponent::ConstantFPS 0x01375550, 0.5f 0x01292294,
//   2^32 0x0122DCB4 (unsigned to float).
// Shape: one exit (nested positive ifs; every early `return` spelling costs VC7.1's caller inline budget and leaves the second
//   by-value CIntelligentPointer constructor out of line). The reach test copies only the target's X/Y (C2DVector from PeekPos)
//   before re-reading the creature's position, works in plain float differences (dx spilled, dy kept: Y squared first) and loads
//   the radius after the differences; fps is read inside the frame comparison (TaggedFrame read first). The action is a temporary
//   argument (the `push 0` wait flag precedes its construction).
// Headers (overlay opt-ins, new): FABLE_RETAIL_CAISTATEGROUP_MINIONKIDGROUPTAG (the class on its CAIStateGroupBase base),
//   FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR (both by-value IP sites inline in retail, past the caller inline budget).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDGROUPTAG
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureGroup.h"
#include "engine/CAIStateGroup_MinionKidGroupTag.h"

// CTCCreatureGroupTag has no shared header (modelled as in the landed SetTagger 0x008137B0): Ego_r.pdb TaggerUID +0x28,
// TaggedFrame +0x30 (sizeof 0x38) on the retail CTCCreatureGroup (36 bytes).
class CTCCreatureGroupTag : public CTCCreatureGroup {
public:
    bool SetTagger(const CThingAICreature* tagger);     // 0x008137B0
    unsigned long GetTaggedFrame() const { return TaggedFrame; }   // FableWin ?GetTaggedFrame@CTCCreatureGroupTag@@QBEKXZ (inline)
protected:
    unsigned __int64 TaggerUID;                          // +0x28
    unsigned long TaggedFrame;                           // +0x30
};

void CAIStateGroup_MinionKidGroupTag::TagTarget()
{
    const CThingAICreature* target = PTCGroupTag->GetNearestOtherActiveMember(&GetCreature());
    if (target != 0 && !target->IsDead()) {
        bool reached = false;
        if (AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, target, 0.5f, reached, AI_MOVEMENT_NON_CONSTANT_RUNNING,
                true, false, true, true, 0.0f) == AI_ACTION_FINISHED) {
            C2DVector targetPos(target->PeekPos());
            C2DVector here(GetCreature().PeekPos());
            float dx = targetPos.X - here.X;
            float dy = targetPos.Y - here.Y;
            float reach = GetCreature().GetPhysicsTC()->GetRadius() + 0.5f;
            if (dy * dy + dx * dx <= reach * reach) {
                if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, target, 1, 0.013888889f) == AI_ACTION_TERMINATE_PROCESS) {
                    Terminate();
                } else {
                    C2DVector targetPos(target->PeekPos());
                    C2DVector here(GetCreature().PeekPos());
                    float dx = targetPos.X - here.X;
                    float dy = targetPos.Y - here.Y;
                    float reach = GetCreature().GetPhysicsTC()->GetRadius() + 0.5f;
                    if (dy * dy + dx * dx <= reach * reach) {
                        if ((float)(unsigned long)(CWorld::GetFrame() - PTCGroupTag->GetTaggedFrame()) > 2.0f * CMainGameComponent::GetConstantFPS()) {
                            PTCGroupTag->SetTagger(target);
                            if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "TAG", CSoundPair(),
                                    false, true, false, 0, 0), false) == AI_ACTION_TERMINATE_PROCESS)
                                Terminate();
                        }
                    }
                }
            }
        }
    }
}