// NScript::CV_LostTraderScript::CLostTraderTrader::TraderYellForHelp() -- the lost trader's yells for help: the first six step through the lines in order, later ones pick one at random (rand() % 5); each starts a conversation between Me and the hero and adds the line
// Leaf: ?TraderYellForHelp@CLostTraderTrader@CV_LostTraderScript@NScript@@AAEXXZ
// Retail 0x00EA4980, 556 bytes incl. the 5-entry jump table (cases 0-4). Identity: fp_egor donor FUN_00f0b8a0 (string WHCIHWAY) =
//   Ego_r TraderYellForHelp; donor order YellForHelp / YellAtHero / MakeRandomComment = 0xEA4980 / 0xEA4BB0 / 0xEA4DE0.
//   Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, rand 0x00BFEB16, the game interface's conversation
//   virtuals.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
extern "C" int __cdecl rand();                          // 0x00BFEB16
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_LostTraderScript {
public:
    class CLostTraderTrader;
};
class CV_LostTraderScript::CLostTraderTrader : public CEntityScriptBase {
public:
    CV_LostTraderScript*    ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    stateBrain;                 // +0x1C
    long                    YellForHelp;                // +0x20
    long                    YellAtHero;                 // +0x24
    long                    RandomComment;              // +0x28
private:
    void TraderYellForHelp();
};

void CV_LostTraderScript::CLostTraderTrader::TraderYellForHelp()
{
    unsigned long index = YellForHelp;
    if (index > 5) {
        if (IsActiveThreadTerminating())
            return;
        index = rand() % 5;
    }
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    switch (index) {
    case 0:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_LOST", false, Me, Game->GetHero());
        break;
    case 1:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_ANYONE", false, Me, Game->GetHero());
        break;
    case 2:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_ANYBODY", false, Me, Game->GetHero());
        break;
    case 3:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_STUPID", false, Me, Game->GetHero());
        break;
    case 4:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_BROTHER", false, Me, Game->GetHero());
        break;
    default:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_WHCIHWAY", false, Me, Game->GetHero());
        break;
    }
    ++YellForHelp;
}
}