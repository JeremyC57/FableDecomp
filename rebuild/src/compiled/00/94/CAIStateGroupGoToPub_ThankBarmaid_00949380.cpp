// CAIStateGroup_GoToPub::ThankBarmaid() -- say the creature's thanking phrase (waiting while the speech is busy), look at the
// barmaid who brought the tankard, play the ACKNOWLEDGE_BARMAID animation and clear the tankard's barmaid; false on any failure,
// else !IsStopping().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00949380, 498 bytes (`ret`). Ego_r.pdb / FableWin.pdb: ?ThankBarmaid@CAIStateGroup_GoToPub@@IAE_NXZ.
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined CThing::GetTC<CTCTalk> / <CTCAIScratchpad> / <CTCTavernTankard>),
// CTCTalk::PlayAISpeech 0x006E3B60, CAIStateGroupBase::AIAction_DoNothing 0x0097E200, CBaseIntelligentPointer ctor 0x00A01B10 /
// GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C),
// CAIStateGroupBase::AIAction_LookAtThing 0x0097D6F0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
// ("ACKNOWLEDGE_BARMAID" 0x0129304C), CCreatureAction_PlayAnimation ctor 0x008424F0, AIAction_PerformActionAndWait 0x0097DB70,
// ~CCreatureActionBase 0x00693EF0 (implicit action dtor), CTCTavernTankard::SetBarmaid 0x0094B400 (Ego_r.pdb + FableWin.pdb
// ?SetBarmaid@CTCTavernTankard@@QAEXPAVCThingAICreature@@@Z),
// CAIStateGroupBase::IsStopping 0x008FD010.
// Shape: the stack-byte HasTC (`const bool&`) with the expanded GetTC; the miss paths leave the out pointers in a stale frame word.
// The phrase is read through the inline accessor CTCAIScratchpad::GetThankingPhrase() const (FableWin.pdb ?GetThankingPhrase@
// CTCAIScratchpad@@QBEKXZ): reading the member directly moves three temps from eax/edx to edx/ecx/eax. The tail is the
// conditional `IsStopping() ? false : true` (byte `neg al ; sbb al,al ; inc al`; `!IsStopping()` widens to eax).
// Layout: CAIStateGroup_GoToPub (Ego_r.pdb, no shared header: local) PTankard +0x34; CTCTavernTankard (Ego_r.pdb, no shared
// header: local) PBarmaid +0x34.
// Leaf: ?ThankBarmaid@CAIStateGroup_GoToPub@@IAE_NXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCAISCRATCHPAD_GETTHANKINGPHRASE
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
class CThingBuilding;
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"
#include "engine/CTCTalk.h"
#include "engine/CTCAIScratchpad.h"

// CTCTavernTankard (Ego_r.pdb sizeof 60; no shared header): PBarmaid +0x34.
class CTCTavernTankard : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_TAVERN_TANKARD; }
    unsigned char _pad[0x34 - sizeof(CTCBase)];
    CIntelligentPointer<CThingAICreature> PBarmaid;     // +0x34
    void SetBarmaid(CThingAICreature* barmaid);          // 0x0094B400
};

class CAIStateGroup_GoToPub : public CAIStateGroupBase {
protected:
    bool ThankBarmaid();
    CIntelligentPointer<CThingBuilding> PPub;            // +0x14
    CIntelligentPointer<CThingObject> PBar;              // +0x1c
    CIntelligentPointer<CThingObject> PTable;            // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;       // +0x2c
    CIntelligentPointer<CThing> PTankard;                // +0x34
};

bool CAIStateGroup_GoToPub::ThankBarmaid()
{
    CTCTalk* talk;
    GetCreature().GetTC(&talk);
    CTCAIScratchpad* scratchpad;
    GetCreature().GetTC(&scratchpad);
    for (;;) {
        CTCTalk::ESpeechPlaybackResult result = talk->PlayAISpeech(scratchpad->GetThankingPhrase(), false, NSpeechGainManager::EDialogueLayer(2), false, true);
        if (result == 2) {
            if (AIAction_DoNothing() == 4)
                continue;
            return false;
        }
        if (result != 0)
            return false;
        break;
    }
    if (!IS_THING_ALIVE(PTankard))
        return false;
    CTCTavernTankard* tankard;
    PTankard.Get()->GetTC(&tankard);
    if (AIAction_LookAtThing(EAIActionRepeat(2), CIntelligentPointer<const CThing>(tankard->PBarmaid.Get()), 0x16, 0.0027777f) == 1)
        return false;
    bool failed;
    {
        CCharString anim("ACKNOWLEDGE_BARMAID");
        failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), anim, CSoundPair(),
                                                                             false, true, false, 0, 0), false) == 1;
    }
    if (failed)
        return false;
    tankard->SetBarmaid(0);
    return IsStopping() ? false : true;
}