// NScript::CQ_NewOakValeIntroScript::AddGoodDeed() -- count a good deed: give the hero the per-deed morality, show the first/next good-deed message and wait until it is clicked past (the first one also adds the map logbook tutorial), set the gold objective while the hero has under 3 gold and no sweets yet, then update the good-deed counter
// Leaf: ?AddGoodDeed@CQ_NewOakValeIntroScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB0660, 544 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, AddLogbookTutorialEntry 0x00CBE9EE (__fastcall, as landed),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame
//   (+0x1c), 40 MsgIsGameInfoClickedPast (+0xa0), 115 DisplayGameInfo (+0x1cc), 127 GetHeroGold (+0x1fc), 156
//   GiveHeroMorality(float) (+0x270), 296 SetQuestCardObjective (+0x4a0), 335 UpdateQuestInfoCounter (+0x53c), 655
//   GetActiveQuestName (+0xa3c, CCharString by value).
// Data: the script def at 0x0143E90C: OVI_MoralityChangePerDeed (PDB +0xE14, retail +0xD64; the same -0xB0 shift as the
//   OFE_ boasts of the orchard raid Init 0x00DD2020).
// Layout (retail = Ego_r PDB -0x14): GoodDeedsPerformed +0x54, BadDeedsPerformed +0x58, GUIGoodDeedCounter +0x5C,
//   GivenSweets +0x94.
// Shape: the click-past waits are the rotated script idiom; the objective call is the landed WatchForGotGold
//   0x00DBE2E0 nested expression.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
void __fastcall AddLogbookTutorialEntry(const CCharString& tag);   // 0x00CBE9EE

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad0[0xd64];
    float OVI_MoralityChangePerDeed;                    // +0xD64
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_NewOakValeIntroScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0xc];
    long                  GoodDeedsPerformed;           // +0x54
    long                  BadDeedsPerformed;            // +0x58
    long                  GUIGoodDeedCounter;           // +0x5C
    unsigned char         _pad60[0x34];
    bool                  GivenSweets;                  // +0x94
    void AddGoodDeed();
};

void CQ_NewOakValeIntroScript::AddGoodDeed()
{
    ++GoodDeedsPerformed;
    GSI->GiveHeroMorality(pGlobalScriptDef->OVI_MoralityChangePerDeed);
    if (GoodDeedsPerformed == 1 && BadDeedsPerformed == 0) {
        if (IsActiveThreadTerminating())
            return;
        GSI->DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED");
        while (!GSI->MsgIsGameInfoClickedPast()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP");
    } else {
        if (IsActiveThreadTerminating())
            return;
        GSI->DisplayGameInfo("TEXT_QST_048_SCRMSG_DID_GOOD_DEED");
        while (!GSI->MsgIsGameInfoClickedPast()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (GSI->GetHeroGold() < 3 && !GivenSweets) {
        if (IsActiveThreadTerminating())
            return;
        GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02", "", "");
    }
    GSI->UpdateQuestInfoCounter(GUIGoodDeedCounter, GoodDeedsPerformed, -1);
}
}