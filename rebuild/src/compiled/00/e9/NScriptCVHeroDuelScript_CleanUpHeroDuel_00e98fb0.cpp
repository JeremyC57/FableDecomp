// CV_HeroDuelScript::CleanUpHeroDuel() -- after the duel: wait for GibbetWoods, re-enable teleporting, hand back the Hanging Tree quest cards late in the game, open Thunder's helmet door and retire the quest
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E98FB0, 0x2ac bytes, plain `ret`. PDB ?CleanUpHeroDuel@CV_HeroDuelScript@NScript@@QAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   CBaseClass() 0x0099A2D0, CScriptThing::operator= 0x008AB980 (out of line here), ~CScriptThing 0x004AA840; the thing's
//   IsAlive (slot 75, inline null test); CGameScriptInterface slots 7 NewScriptFrame, 11 IsLevelLoaded, 72
//   GetThingWithScriptName, 281 DeactivateQuestLater, 286 IsQuestCompleted, 292 AddQuestCard, 425 OpenDoor, 565
//   SetTeleportingAsActive, 655 GetActiveQuestName.
// Shape: `!A && !B` over two IsQuestCompleted calls (cards only while neither ending is done) keeps conditional-destruction bits (1, 3) for the two name temporaries and
//   the result byte in the frame; the helmet is a default-constructed local assigned through the out-of-line operator=.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, the shared master data +0x44 (its +0x04 is the unnamed game clock the
//   landed CTH_TriggerOakvale 0x00EE0A20 also reads).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CMasterScriptData {
    long _unk00;
    long Clock;                                          // +0x04 (unnamed in the PDB views)
};
namespace NScript {
class CV_HeroDuelScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    CMasterScriptData* PMasterData;                      // +0x44
    void CleanUpHeroDuel();
};
} // namespace NScript
using NScript::CV_HeroDuelScript;

void NScript::CV_HeroDuelScript::CleanUpHeroDuel()
{
    while (!Game->IsLevelLoaded("GibbetWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetTeleportingAsActive(true);
    if (PMasterData->Clock >= 1100) {
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsQuestCompleted("Q_HangingTreeEvil") && !Game->IsQuestCompleted("Q_HangingTreeGood")) {
            if (IsActiveThreadTerminating())
                return;
            Game->AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_EVIL", "Q_HangingTreeEvil", false, false);
            Game->AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_GOOD", "Q_HangingTreeGood", false, false);
        }
    }
    CScriptThing helmet;
    helmet = Game->GetThingWithScriptName("DD_ThunderHelmet");
    if (helmet.IsAlive())
        Game->OpenDoor(helmet);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}