// CQ_ArenaScript::GivePrizeFund() -- the arena keeper's prize conversation: add the round's prize to the gold total, then play the "prize" and "prize later" lines and wait for the conversation to end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F21300, length 612 bytes + an 8-entry jump table (padded to 656).
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0, operator=(const char*)
// 0x0099EFE0, ~CCharString 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 364 AddNewConversation,
// 365 AddPersonToConversation, 366 AddLineToConversation, 368 IsConversationActive.
// Layout: see the CQ_ArenaScript Init row (0x00CFA700): Ego_r.pdb - 0x14, then a further -4 after the 12-byte
// Rounds vector, so GoldMultiplier is +0xb0 and GoldTotal +0xb8.
// Shape: the switch is a jump table over GoldMultiplier 0..7; VC7.1 cross-jumps the shared "later =" tail.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const char* text);          // 0x0099EFE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                   // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};

namespace NScript {
class CQ_ArenaScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    unsigned char CrowdLoopTags[0x50];                 // +0x48 CCharString[4][5]
    long Rounds[3];                                    // +0x98 vector<CArenaRoundDef>
    long ArenaState;                                   // +0xa4
    long ArenaRound;                                   // +0xa8
    long ArenaRoundWave;                               // +0xac
    long GoldMultiplier;                               // +0xb0
    bool ChamLeaving;                                  // +0xb4
    long GoldTotal;                                    // +0xb8
    void GivePrizeFund();
};
} // namespace NScript
using NScript::CQ_ArenaScript;

void NScript::CQ_ArenaScript::GivePrizeFund()
{
    long conversation = Game->AddNewConversation(Game->GetHero(), true, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    CCharString prize("");
    CCharString later("");
    switch (GoldMultiplier) {
    case 0:
        GoldTotal += 100;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE_LATER";
        break;
    case 1:
        GoldTotal += 500;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE_LATER";
        break;
    case 2:
        GoldTotal += 1000;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE_LATER";
        break;
    case 3:
        GoldTotal += 2000;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE_LATER";
        break;
    case 4:
        GoldTotal += 3000;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE_LATER";
        break;
    case 5:
        GoldTotal += 4000;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE_LATER";
        break;
    case 6:
        GoldTotal += 8000;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE_LATER";
        break;
    case 7:
        GoldTotal += 10000;
        prize = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE";
        later = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE_LATER";
        break;
    }
    Game->AddLineToConversation(conversation, prize, false, Game->GetHero(), Game->GetHero());
    Game->AddLineToConversation(conversation, later, false, Game->GetHero(), Game->GetHero());
    while (Game->IsConversationActive(conversation)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
}