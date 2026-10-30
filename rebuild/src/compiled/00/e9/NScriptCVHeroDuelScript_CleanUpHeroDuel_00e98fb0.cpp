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
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074();
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing() {}
    virtual ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
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