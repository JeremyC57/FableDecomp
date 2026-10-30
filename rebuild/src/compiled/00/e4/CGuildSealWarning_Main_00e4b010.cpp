// CGuildSealWarning::Main() -- guild seal warning: the guildmaster tells the hero about the guild seal, then (once per quest) the active quest hands the hero the bandit-toll quest card and sets its objective; finally this marker removes itself
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4B010, length 359 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("" @ 0x0122D70E,
//   "TEXT_QST_011_GUILDSEAL_MESSAGE" @ 0x012E50EC, "OBJECT_QUEST_CARD_BANDIT_TOLL" @ 0x012E50CC, "GreatwoodBanditToll"
//   @ 0x012E4CA8, "TEXT_QUEST_BANDIT_TOLL_OBJECTIVE_01" @ 0x012E50A8), IsDone @ 0x00F35B30; CGameScriptInterface (vtable
//   0x01260F0C) slots 108 RemoveThing, 274 HeroReceiveMessageFromGuildMaster, 295 GiveHeroQuestCardDirectly, 296
//   SetQuestCardObjective, 376 Pause, 655 GetActiveQuestName (CCharString by value).
// Layout: the entity-script base {vptr, Game +4, CScriptThing Me +8} as the landed CHTG_AmbushPoint::Main 0x00D73610;
//   ParentClass +0x14 is the owning quest script whose bool at +0x58 records that the quest card was given.
// Shape: every literal is an implicit CCharString conversion (built right to left before the call's other arguments,
//   GetActiveQuestName last); the IsDone exit skips the closing RemoveThing.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_BanditTollOwner {
    unsigned char _pad[0x58];
    bool          QuestCardGiven;                  // +0x58
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CGuildSealWarning : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CQ_BanditTollOwner*   ParentClass;             // +0x14
    void Main();
};

void CGuildSealWarning::Main()
{
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_011_GUILDSEAL_MESSAGE", "", true, true);
    Game->Pause(2.0f);
    if (!ParentClass->QuestCardGiven) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BANDIT_TOLL", Game->GetActiveQuestName(), false);
        ParentClass->QuestCardGiven = true;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BANDIT_TOLL_OBJECTIVE_01", "GreatwoodBanditToll", "");
    }
    Game->RemoveThing(Me, false, true);
}