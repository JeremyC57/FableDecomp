// NScript::CV_LostTraderScript::CLostTraderTrader::TraderYellAtHero() -- the lost trader's yells at the hero: the first six step through the lines in order, later ones pick one at random (rand() % 5); each starts a conversation between Me and the hero and adds the line
// Leaf: ?TraderYellAtHero@CLostTraderTrader@CV_LostTraderScript@NScript@@AAEXXZ
// Retail 0x00EA4BB0, 556 bytes incl. the 5-entry jump table (cases 0-4); the YellAtHero twin of 0x00EA4980 (strings HEAR .. HELP),
//   same donor-order evidence. Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, rand 0x00BFEB16.
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
    void TraderYellAtHero();
};

void CV_LostTraderScript::CLostTraderTrader::TraderYellAtHero()
{
    unsigned long index = YellAtHero;
    if (index > 5) {
        if (IsActiveThreadTerminating())
            return;
        index = rand() % 5;
    }
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    switch (index) {
    case 0:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_HEAR", false, Me, Game->GetHero());
        break;
    case 1:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_HERE", false, Me, Game->GetHero());
        break;
    case 2:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_CAN_HEAR", false, Me, Game->GetHero());
        break;
    case 3:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_THISWAY", false, Me, Game->GetHero());
        break;
    case 4:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_WHO", false, Me, Game->GetHero());
        break;
    default:
        Game->AddLineToConversation(conversation, "TEXT_QST_042_LOSTTRADER_HELP", false, Me, Game->GetHero());
        break;
    }
    ++YellAtHero;
}
}