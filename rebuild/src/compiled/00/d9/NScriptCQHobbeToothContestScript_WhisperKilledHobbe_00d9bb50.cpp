// CQ_HobbeToothContestScript::WhisperKilledHobbe() -- count a tooth for Whisper and, unless she commented within the last five seconds, have her comment to the hero on the score (enemy ahead / equal / behind, or one of two random lines)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9BB50, length 621 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, rand 0x00BFEB16, ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC; through the interface (+0x40): GetHero (70), GetThingWithScriptName (72),
//   AddNewConversation (364), AddPersonToConversation (365), AddLineToConversation (366); the timer service (global
//   0x0143E8F8) SetTimer (89) / GetTimer (90).
// Strings: "Q_HobbeToothWhisper" 0x012D6788, "TEXT_QST_035_WHISPER_ENEMY_SCORE_{AHEAD,EQUAL,BEHIND,RANDOM}{1,2}_10".
// Layout (retail = Ego_r.pdb - 0x14): NumberOfHobbeTeethCollectedByHero +0x4c, ...ByWhisper +0x50, WhisperCommentTimer
//   +0x60.
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340 (inline
//   destructor at the single exit); the eight AddLineToConversation calls tail-merge; the comment timer is tested before
//   the conversation is opened (twin HeroKilledHobbe 0x00D9B8F0 opens it first) and is an early `return` -- the
//   `if (<= 0) { ... }` form schedules `pop esi` into the inline destructor and swaps the SetTimer registers.
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
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { const long& index = Index; g_pGameScriptInterface->SetTimer(index, time); }
    long GetTime() { const long& index = Index; return g_pGameScriptInterface->GetTimer(index); }
};
int __cdecl rand();                                     // 0x00BFEB16
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
    void WhisperKilledHobbe();
};
} // namespace NScript
using NScript::CQ_HobbeToothContestScript;

void NScript::CQ_HobbeToothContestScript::WhisperKilledHobbe()
{
    NumberOfHobbeTeethCollectedByWhisper++;
    CScriptThing whisper = Game->GetThingWithScriptName("Q_HobbeToothWhisper");
    if (WhisperCommentTimer.GetTime() > 0)
        return;
    WhisperCommentTimer.SetTime(5);
    long conversation = Game->AddNewConversation(whisper, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    if (rand() % 3 == 0) {
        if (NumberOfHobbeTeethCollectedByHero > NumberOfHobbeTeethCollectedByWhisper) {
            if (rand() % 2 == 0)
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_AHEAD1_10", false, whisper, Game->GetHero());
            else
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_AHEAD2_10", false, whisper, Game->GetHero());
        } else if (NumberOfHobbeTeethCollectedByHero == NumberOfHobbeTeethCollectedByWhisper) {
            if (rand() % 2 == 0)
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_EQUAL1_10", false, whisper, Game->GetHero());
            else
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_EQUAL2_10", false, whisper, Game->GetHero());
        } else {
            if (rand() % 2 == 0)
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_BEHIND1_10", false, whisper, Game->GetHero());
            else
                Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_BEHIND2_10", false, whisper, Game->GetHero());
        }
    } else {
        if (rand() % 2 == 0)
            Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_RANDOM1_10", false, whisper, Game->GetHero());
        else
            Game->AddLineToConversation(conversation, "TEXT_QST_035_WHISPER_ENEMY_SCORE_RANDOM2_10", false, whisper, Game->GetHero());
    }
}