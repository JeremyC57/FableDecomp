// CAIStateGroup_MinionReactToApology::Update() -- speak the accepted or rejected apology response to the hero, then terminate
// Leaf: ?Update@CAIStateGroup_MinionReactToApology@@UAEXXZ
// Retail 0x0091D190, 270 bytes. Ego_r.pdb 0x009408F0 / FableWin.pdb 0x02C092A0 ?Update@CAIStateGroup_MinionReactToApology@@UAEXXZ (the
//   CAIStateGroupBase slot-12 override; the landed leaf was QAE).
// Callees: the creature's entity-events component through CThing::GetTC (Components lower_bound 0x0040F020), CTCEntityEvents::GetEvent
//   <NEntityEvents::CEventApologyReceived, NEntityEvents::CEventFilter_None> 0x0091D0A0 (Ego_r.pdb / FableWin.pdb, QBE), GetPlayerManager
//   0x008FCF90, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, the CIntelligentPointer temporaries
//   (ctor 0x00A01B10 + SetPItem 0x00A01B90, vtables 0x01237F1C / 0x0123E7D8), AIAction_SpeakAISpeechToTarget 0x00981EA0 and Terminate
//   0x008FCF40.
// Shape: the event pointer is copied out of GetEvent's out slot in an inner scope (retail loads it straight into edi, `mov edi,[esp+10h]`,
//   as an inlined accessor's return value; a plain out-slot local is reloaded into eax first: NF88 grades 2..3).
// Objects: 0x013BB684 SUCCESS_STRING / 0x013BB688 FAILURE_STRING (anonymous namespace; agentNF26 / agentFAM09 names).
// Port (NF88, round 7, q11160 binder: it bound 0x00981EA0 under the derived-class stand-in
//   ?AIAction_SpeakAISpeechToTarget@CAIStateGroup_MinionReactToApology@@QAEJJUCIntelligentCreature@@UCIntelligentConstThing@@ABVCCharString@@_NJ@Z):
//   the `#define CAIStateGroupBase CAIStateGroupBaseLayout` / `#define CPlayer CPlayerPdb` includes and the local CBaseIntelligentPointer /
//   CIntelligentConstThing / CIntelligentCreature / CThingPlayerCreature / CTCEntityEventsMethods models are gone: the row is on
//   engine/CAIStateGroupBase.h (MEMBERS, AIACTIONS, SPEECH, GETCREATURE), CAIBrain.h, CIntelligentPointer.h (CONVERT), CThing.h (GETTC_EXPANDED),
//   CTCEntityEvents.h, CPlayerManager.h and CPlayer.h, as the landed 0x0092FAB0 (which binds 0x00981EA0 under the PDB spelling). The event and
//   filter classes and the state group itself have no shared header and stay declared here (Ego_r.pdb CEventApologyReceived {Accepted +8}).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/EAIAction.h"

namespace NEntityEvents {
class CEventApologyReceived : public CEventBase {               // Ego_r.pdb, 12 bytes
public:
    bool GetAccepted() const { return Accepted; }               // FableWin.pdb ?GetAccepted@CEventApologyReceived@NEntityEvents@@QBE_NXZ
private:
    bool Accepted;                                              // +0x08
};
class CEventFilter_None {
public:
    CEventFilter_None() {}
};
}
namespace { extern CCharString SUCCESS_STRING; }                // 0x013BB684
namespace { extern CCharString FAILURE_STRING; }                // 0x013BB688

class CAIStateGroup_MinionReactToApology : public CAIStateGroupBase {
public:
    virtual void Update();
};

void CAIStateGroup_MinionReactToApology::Update()
{
    CTCEntityEvents* events;
    if (GetCreature().GetTC(TCI_ENTITY_EVENTS, &events)) {
        const NEntityEvents::CEventApologyReceived* apology;
        {
            NEntityEvents::CEventApologyReceived* p_event = 0;
            events->GetEvent(&p_event, NEntityEvents::CEventFilter_None());
            apology = p_event;
        }
        if (apology) {
            CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (apology->GetAccepted())
                AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), hero, SUCCESS_STRING, true, 0x93);
            else
                AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED, &GetCreature(), hero, FAILURE_STRING, true, 0x93);
        }
    }
    Terminate();
}
