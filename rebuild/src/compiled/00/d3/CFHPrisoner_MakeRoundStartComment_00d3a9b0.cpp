// CFH_Prisoner::MakeRoundStartComment() -- the prophet's "TEXT_QST_B01_PROPHET<round + 1>_START" line, suffixed GOOD / DEFAULT / BAD by the quest's mood once the quest is under way, said to the hero in a new conversation (bailing out whenever the script is done)
// Leaf: MakeRoundStartComment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3A9B0, length 327 bytes (`ret`).
// Callees: GFIntToCharString(long) @ 0x0099F830, operator+(const char*, const CCharString&) @ 0x0099F690 and
//   operator+(const CCharString&, const char*) @ 0x0099F600 (all `__fastcall` with the hidden result in ecx),
//   CCharString::operator+=(const char*) @ 0x0099F100, ~CCharString @ 0x0099EAE0, the entity script's IsDone @ 0x00F35B30;
//   CGameScriptInterface slots 70 GetHero, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation.
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8, the owning quest
//   script +0x14 (its started counter +0x88, mood +0x184), the round counter +0x20.
// Shape: every mood arm re-tests IsDone before appending; the callee-saved pushes for the conversation sit after the tests.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString { public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const char* text);          // 0x0099F100
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQuestScript_Retail {
    unsigned char _pad000[0x88];
    long          RoundsStarted;                        // +0x88
    unsigned char _pad08c[0xf8];
    long          Mood;                                 // +0x184
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CFH_Prisoner : public CEntityScriptBase {
public:
        CQuestScript_Retail*    Quest;                      // +0x14
    long                    _pad18[2];                  // +0x18
    long                    Round;                      // +0x20
    void MakeRoundStartComment();
};

void CFH_Prisoner::MakeRoundStartComment()
{
    CCharString comment = "TEXT_QST_B01_PROPHET" + GFIntToCharString(Round + 1) + "_START";
    if (Quest->RoundsStarted > 0) {
        if (IsActiveThreadTerminating())
            return;
        long mood = Quest->Mood;
        if (mood == 1) {
            if (IsActiveThreadTerminating())
                return;
            comment += "GOOD";
        } else if (mood == 2) {
            if (IsActiveThreadTerminating())
                return;
            comment += "DEFAULT";
        } else if (mood == 3) {
            if (IsActiveThreadTerminating())
                return;
            comment += "BAD";
        } else if (IsActiveThreadTerminating())
            return;
    }
    long conversation = Game->AddNewConversation(Me, true, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, comment, false, Me, Game->GetHero());
}