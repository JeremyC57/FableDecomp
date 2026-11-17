// CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString& text) -- speak the named AI speech to the (alive, creature-typed) thing
//   this reaction is about, from the brain's creature, priority 0x93; a speaker allowed non-pure-AI speech takes the seven-argument form with
//   wait = false. Returns the action result, or AI_ACTION_FAILED when there is nothing to speak to.
// Leaf: ?SayToThingReactingTo@CAIStateGroup_OpinionReactionRoot@@AAE?AW4EAIActionReturn@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095EE70, 262 bytes, `ret 4`. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (ThingReactingTo +0x6B,
//   four times), the by-value CIntelligentPointer<const CThing> / <CThingCreatureBase> arguments built in place (ctor 0x00A01B10, vtables
//   0x01237F1C / 0x0123E7D8, SetPItem 0x00A01B90), CAIStateGroupBase::AIAction_SpeakAISpeechToTarget seven-argument 0x00981350 (descriptive,
//   NF84) / six-argument 0x00981EA0 (Ego_r.pdb).
// Names: Ego_r.pdb 0x00983400 / FableWin.pdb 0x02C285B0 (NF59); the class members from engine/CAIStateGroup_OpinionReactionRoot.h (WIFEUPDATE:
//   AllowSpeechOnNonPureAISpeaker +0x8A, ThingReactingTo +0x6B), the speaker CAIStateGroupBase::GetCreature() (PBrain->Creature, FableWin inline).
// Port (NF86, round 7, q11108 binder; NF64/NF84-blocked): was a local CAIStateGroupBase / CAIStateGroup_OpinionReactionRoot model with local
//   intelligent-pointer stand-ins binding both callees as ?AIAction_SpeakAISpeechToTarget@CAIStateGroup_OpinionReactionRoot@@QAEJJ...; now the
//   shared headers, with the NF86 overlay opt-in FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO (ThingReactingTo typed as
//   CIntelligentPointer<CThing>; same layout). Both calls now bind the CAIStateGroupBase spellings 0x00981350 / 0x00981EA0 define.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEAKAISPEECH_WAIT
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

EAIActionReturn CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString& text)
{
    if (IS_THING_ALIVE(ThingReactingTo) && (ThingReactingTo.Get()->GetTypeFlags() & 8)) {
        const CThing* target = ThingReactingTo.Get();
        if (AllowSpeechOnNonPureAISpeaker)
            return AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), target, text, false, 0x93, false);
        return AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), target, text, false, 0x93);
    }
    return AI_ACTION_FAILED;
}
