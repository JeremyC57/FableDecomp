// CV_LostTraderScript::WatchForQuestFailureState(CScriptThing) -- wait for the quest-failed flag, then fail the active quest with the reason's text (trader died / brother died / none), deactivate it and add the Lost Trader quest card
// Leaf: ?WatchForQuestFailureState@CV_LostTraderScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA7460, 419 bytes (432 with padding), `ret 0Ch` (the by-value CScriptThing).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, ~CScriptThing 0x004AA840 (out of line), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   DeactivateQuestLater (slot 281), SetQuestAsFailed (slot 289), AddQuestCard (slot 292), GetActiveQuestName (slot 655).
// Strings: L"TEXT_QST_042_QUEST_FAILED_TRADER_DIED" 0x012EE3F8, L"TEXT_QST_042_QUEST_FAILED_TRADERS_BROTHER_DIED"
//   0x012EE398, L"" 0x0122D70C, "V_LostTrader" 0x012C4F98, "OBJECT_QUEST_CARD_LOST_TRADER" 0x012C4F78.
// Layout (retail = Ego_r.pdb - 0x14): g_bQuestFailed +0x4a, ReasonForFailure +0x54; interface pointer +0x40.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);                    // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
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
struct CV_LostTraderScript : CQuestScriptBase {
    bool g_bLostTraderKilled;                               // +0x48
    bool g_bTraderBrotherKilled;                            // +0x49
    bool g_bQuestFailed;                                    // +0x4a
    bool g_bTraderHit;                                      // +0x4b
    bool ReachedTrader;                                     // +0x4c
    long DistanceForBrotherToReunite;                       // +0x50
    long ReasonForFailure;                                  // +0x54
    void WatchForQuestFailureState(CScriptThing trader);
};

void CV_LostTraderScript::WatchForQuestFailureState(CScriptThing trader)
{
    while (!g_bQuestFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (ReasonForFailure == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_042_QUEST_FAILED_TRADER_DIED", true);
    } else if (ReasonForFailure == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_042_QUEST_FAILED_TRADERS_BROTHER_DIED", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->AddQuestCard("OBJECT_QUEST_CARD_LOST_TRADER", "V_LostTrader", false, false);
}