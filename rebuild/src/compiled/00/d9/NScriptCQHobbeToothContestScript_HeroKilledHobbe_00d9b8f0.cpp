// CQ_HobbeToothContestScript::HeroKilledHobbe() -- count a tooth for the hero and, unless Whisper commented within the last five seconds, have her comment on the score (ahead / equal / behind, or one of two random lines)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9B8F0, length 608 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, rand 0x00BFEB16, ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC; through the interface (+0x40): GetHero (70), GetThingWithScriptName (72),
//   AddNewConversation (364), AddPersonToConversation (365), AddLineToConversation (366); the timer service (global
//   0x0143E8F8) SetTimer (89) / GetTimer (90).
// Strings: "Q_HobbeToothWhisper" 0x012D6788, "TEXT_QST_035_WHISPER_HERO_SCORE_{AHEAD,EQUAL,BEHIND,RANDOM}{1,2}_10".
// Layout (retail = Ego_r.pdb - 0x14): NumberOfHobbeTeethCollectedByHero +0x4c, ...ByWhisper +0x50, WhisperCommentTimer
//   +0x60.
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340 (inline
//   destructor at the single exit, zero kept in ebp); the eight AddLineToConversation calls tail-merge into one. The
//   comment-timer test is an early `return` (the `if (<= 0) { ... }` form keeps the zero as immediates, 621 bytes).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { const long& index = TimerIndex; g_pGameScriptInterface->SetTimer(index, time); return *this; }
inline long CTimer::GetTime() const { const long& index = TimerIndex; return g_pGameScriptInterface->GetTimer(index); }
extern "C" int __cdecl rand();                                     // 0x00BFEB16
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
};
namespace NScript {
class CQ_HobbeToothContestScript : public CQuestScriptBase {
public:
    long NumberOfHobbesInTotal;                         // +0x48
    long NumberOfHobbeTeethCollectedByHero;             // +0x4c
    long NumberOfHobbeTeethCollectedByWhisper;          // +0x50
    long NumberOfHobbeTeethNeeded;                      // +0x54
    bool CS_IntroDone;                                  // +0x58
    bool CS_OutroBegin;                                 // +0x59
    bool DrinkingHobbesNotHit;                          // +0x5a
    bool MissionSucceeded;                              // +0x5b
    bool MissionFailed;                                 // +0x5c
    bool WhisperOrFarmerHit;                            // +0x5d
    CTimer WhisperCommentTimer;                         // +0x60
    bool QuestStartScreenShown;                         // +0x64
    void HeroKilledHobbe();
};
} // namespace NScript
using NScript::CQ_HobbeToothContestScript;

void NScript::CQ_HobbeToothContestScript::HeroKilledHobbe()
{
    NumberOfHobbeTeethCollectedByHero++;
    CScriptThing whisper = Game->GetThingWithScriptName("Q_HobbeToothWhisper");
    long conversation = Game->AddNewConversation(whisper, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    if (WhisperCommentTimer.GetTime() > 0)
        return;
    WhisperCommentTimer = 5;
    if (rand() % 3 == 0) {
        if (NumberOfHobbeTeethCollectedByHero > NumberOfHobbeTeethCollectedByWhisper) {
            if (rand() % 2 == 0)
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_AHEAD1_10", false, whisper, Game->GetHero());
            else
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_AHEAD2_10", false, whisper, Game->GetHero());
        } else if (NumberOfHobbeTeethCollectedByHero == NumberOfHobbeTeethCollectedByWhisper) {
            if (rand() % 2 == 0)
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_EQUAL1_10", false, whisper, Game->GetHero());
            else
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_EQUAL2_10", false, whisper, Game->GetHero());
        } else {
            if (rand() % 2 == 0)
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_BEHIND1_10", false, whisper, Game->GetHero());
            else
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_BEHIND2_10", false, whisper, Game->GetHero());
        }
    } else {
        if (rand() % 2 == 0)
            Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_RANDOM1_10", false, whisper, Game->GetHero());
        else
            Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_HERO_SCORE_RANDOM2_10", false, whisper, Game->GetHero());
    }
}