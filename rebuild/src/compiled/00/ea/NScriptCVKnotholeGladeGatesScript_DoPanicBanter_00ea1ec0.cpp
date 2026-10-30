// CV_KnotholeGladeGatesScript::DoPanicBanter() -- once the hero comes within 30 units of the gate, until he is alerted to the problem have the gate guard now and then (1 frame in 300) complain to him in a one-line conversation
// Leaf: ?DoPanicBanter@CV_KnotholeGladeGatesScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA1EC0, 448 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, IsDistanceBetweenThingsUnder 0x00CBE2FF (`__fastcall`),
//   rand 0x00BFEB16, CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame
//   (slot 7), GetHero (slot 70), GetThingWithScriptName (slot 72, hidden result), AddNewConversation (slot 364),
//   AddPersonToConversation (slot 365), AddLineToConversation (slot 366).
// Strings: "KG_GateGuard" 0x012EDBD8, "TEXT_QST_050_GUARD_STOP_COMPLAINING" 0x012EDC7C; 30.0f immediate.
// Shape: two WAIT_UNTIL-style loops; the guard handle's inline destructor is expanded at the first in-loop return only,
//   the out-of-line ~CScriptThing called at the later exits.
// Layout (retail = Ego_r.pdb - 0x14): HeroAlertedToProblem +0x4c, TheGate +0x5c; interface pointer +0x40.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <stdlib.h>
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance); // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_KnotholeGladeGatesScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    bool Cutscene1Start;                                    // +0x4b
    bool HeroAlertedToProblem;                              // +0x4c
    bool GatesOpened;                                       // +0x4d
    bool SaidBefore;                                        // +0x4e
    bool HeroKilledAVillager;                               // +0x4f
    bool IntroductionFinished;                              // +0x50
    bool BalvsKilled;                                       // +0x51
    bool CutsceneStarting;                                  // +0x52
    bool Mentioned;                                         // +0x53
    bool ToldSuccess;                                       // +0x54
    long BalvKillCount;                                     // +0x58
    CScriptThing TheGate;                                   // +0x5c
    void DoPanicBanter();
};
} // namespace NScript
using NScript::CV_KnotholeGladeGatesScript;

void NScript::CV_KnotholeGladeGatesScript::DoPanicBanter()
{
    CScriptThing guard = Game->GetThingWithScriptName("KG_GateGuard");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), TheGate, 30.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!HeroAlertedToProblem) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (rand() % 300 == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(guard, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_050_GUARD_STOP_COMPLAINING", false, guard, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
}