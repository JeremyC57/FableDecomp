// CTonyTestScriptsScript::AddQuestsToGuild() -- debug script: give the hero seven guild quest cards in one go
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFF1C0, length 544 bytes (no EH frame).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, and AddQuestCard (slot 292, +0x490)
// through the script's game script interface (+0x40).
// Shape: seven copies of the CQ_OrchardFarmRaidScript::ReplaceQuestCards statement (0x00DD0EB0): both string
// arguments are implicit conversion temporaries built right to left (quest name first), then the two `false`
// pushes, then the virtual call; the temporaries die at the end of each statement, alternating the two 4-byte slots.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

struct CTonyTestScriptsScript : CQuestScriptBase {
    void AddQuestsToGuild();
};

void CTonyTestScriptsScript::AddQuestsToGuild()
{
    GSI->AddQuestCard("OBJECT_QUEST_CARD_ARENA", "Q_Arena", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_ProtectFarm", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_ProtectFarmEvil", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_GRANNY_SCRIPT", "Q_HobbeCave", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_DARKWOOD_TRADER_ESCORT", "Q_TraderEscort", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CONTEST", "Q_HobbeContest", false, false);
}