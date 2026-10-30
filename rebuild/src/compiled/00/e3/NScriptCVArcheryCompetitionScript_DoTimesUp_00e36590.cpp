// CV_ArcheryCompetitionScript::DoTimesUp(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&) -- time is up: stop the judge, show the times-up title, pause, then (each time the judge is still healthy) have him speak the two game-over lines to the hero and wait for him to finish
// Leaf: ?DoTimesUp@CV_ArcheryCompetitionScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E36590, 446 bytes (448 with padding), `ret 8`.
// Callees: CScriptThing::IsNull (slot 77, +0x134), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   ~CScriptThing 0x004AA840, CScriptBase::IsActiveThreadTerminating 0x00CB7940; the scripted-thing handle's
//   IsScripting 0x00CD23B9 and virtuals ClearCommands (slot 10), GetScriptThing (slot 12), Speak(thing, const char*, ...)
//   (slot 13), IsPerformingScriptTask (slot 26) (vtable 0x0127094C); through the interface (+0x40) NewScriptFrame (slot 7),
//   GetHero (slot 70), AddScreenTitleMessage (slot 113), GetHealth (slot 264), Pause (slot 376).
// Strings: "TEXT_QST_004_TIMES_UP" 0x012E3438, "TEXT_QST_004_GAME_OVER1" 0x012E3420, "TEXT_QST_004_GAME_OVER2" 0x012E3408;
//   floats 5.0f (immediate), 0.0f 0x0122DEDC.
// Layout: CScriptBase 0x40 bytes, interface pointer +0x40; the handle as the landed CUndeadRisingZombieAttacker::Main.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                                // 0x004AA840 (out of line)
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074(); virtual void t075(); virtual void t076();
    virtual bool IsNull() const;                            // slot 77 (+0x134) 0x004AB150
    void* PImpData;
    void* PImpInfo;
};
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE = 0, GROUP_SELECT_RANDOM = 1 };
class CScriptGameResourceObjectScriptedThingBase { public:         // vtable 0x0127094C, 0x10 bytes
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009();
    virtual void ClearCommands();                                                       // slot 10 (+0x28)
    virtual void s011();
    virtual CScriptThing GetScriptThing() const;                                        // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c); // slot 13 (+0x34)
    virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019();
    virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025();
    virtual bool IsPerformingScriptTask() const;                                        // slot 26 (+0x68)
    void* m_pIntelligentPointer;
    void* PImpData;
    void* PImpInfo;
    bool IsScripting();                                     // 0x00CD23B9
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_ArcheryCompetitionScript : public CQuestScriptBase {
public:
    void DoTimesUp(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& judge);
};
} // namespace NScript
using NScript::CV_ArcheryCompetitionScript;

void NScript::CV_ArcheryCompetitionScript::DoTimesUp(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& judge)
{
    if (thing.IsNull()) {
        IsActiveThreadTerminating();
        return;
    }
    if (!judge.IsScripting())
        return;
    if (IsActiveThreadTerminating())
        return;
    judge.ClearCommands();
    Game->AddScreenTitleMessage("TEXT_QST_004_TIMES_UP", 5.0f, false);
    Game->Pause(5.0f);
    if (Game->GetHealth(judge.GetScriptThing()) > 0.0f) {
        judge.Speak(Game->GetHero(), "TEXT_QST_004_GAME_OVER1", GROUP_SELECT_RANDOM, false, true, false);
        while (judge.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (Game->GetHealth(judge.GetScriptThing()) > 0.0f) {
        judge.Speak(Game->GetHero(), "TEXT_QST_004_GAME_OVER2", GROUP_SELECT_RANDOM, false, true, false);
        while (judge.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
}