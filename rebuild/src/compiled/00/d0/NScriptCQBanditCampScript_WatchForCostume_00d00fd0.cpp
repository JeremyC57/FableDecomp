// CQ_BanditCampScript::WatchForCostume() -- once the camp path level is loaded, show a 5-piece quest counter and track how many bandit-costume pieces the hero carries until he has all five
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D00FD0, length 702 bytes (padded to 704).
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// CGameScriptInterface slots 7 NewScriptFrame, 11 IsLevelLoaded, 70 GetHero, 184 IsObjectInThingsPossession,
// 321 DisplayQuestInfo, 327 AddQuestInfoCounter, 335 UpdateQuestInfoCounter, 338 RemoveQuestInfoElement.
// Layout: CScriptBase body 0x40, Game +0x40 (Ego_r.pdb - 0x14).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_BanditCampScript {
public:
    unsigned char           _body[0x40];               // CQuestScriptBase / CScriptBase
    CGameScriptInterface*   Game;                      // +0x40
    bool IsActiveThreadTerminating();                  // 0x00CB7940
    void WatchForCostume();
};
} // namespace NScript
using NScript::CQ_BanditCampScript;

void NScript::CQ_BanditCampScript::WatchForCostume()
{
    while (!Game->IsLevelLoaded("BanditCampPath_1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    long counter = Game->AddQuestInfoCounter("HUD_BANDIT_CLOTHES_ICON", 5, 1.0f);
    Game->DisplayQuestInfo(true);
    long pieces;
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        pieces = 0;
        if (Game->IsObjectInThingsPossession("OBJECT_HERO_HAT_BANDITCAMP", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            ++pieces;
        }
        if (Game->IsObjectInThingsPossession("OBJECT_HERO_GLOVES_BANDITCAMP", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            ++pieces;
        }
        if (Game->IsObjectInThingsPossession("OBJECT_HERO_BOOTS_BANDITCAMP", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            ++pieces;
        }
        if (Game->IsObjectInThingsPossession("OBJECT_HERO_SHIRT_BANDITCAMP", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            ++pieces;
        }
        if (Game->IsObjectInThingsPossession("OBJECT_HERO_TROUSERS_BANDITCAMP", Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            ++pieces;
        }
        Game->UpdateQuestInfoCounter(counter, pieces, -1);
    } while (pieces != 5);
    if (IsActiveThreadTerminating()) return;
    Game->RemoveQuestInfoElement(counter);
    Game->DisplayQuestInfo(false);
}