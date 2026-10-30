// Levers: twin of 0x00E1FCF0 (EXPRESSION_WAIT, ExpressionDetectedWait): for(;;) with the terminating return first, alive/else-dead branches, in-body `#pragma inline_depth(0)` around the hit-block terminate return, nested CBasicString<char>::operator==, ternary IsAlive.
// CQR_EscortTraderScript::WatchForExpressionWait(CScriptThing trader) -- spawned per trader: each frame, stop when the trader dies; when the hero performs EXPRESSION_WAIT to it, restart the random-speech timer and set ExpressionDetectedWait
// Leaf: ?WatchForExpressionWait@CQR_EscortTraderScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1FEF0, 435 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, the thing's
//   implementation slots 75 IsAlive (+0x12c) and 29 MsgExpressionPerformedTo(CCharString&) (+0x74, wrapper 0x004AABF0),
//   Compare 0x00411570 (__fastcall, the inlined CCharString == const char*; the null-string arm is the strcmp intrinsic
//   against "" 0x0122D70E), EXPRESSION_WAIT 0x012757DC, RestartRandomSpeechTimer 0x00E1FEB0 (sets TraderRandomSpeechTimer +0x84 from
//   rand()), NewScriptFrame (interface slot 7), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840.
// Layout (retail = Ego_r.pdb - 0x14): ExpressionDetectedWait +0x71.
// Label: the manifest calls 0x00E1FEF0 WatchForExpressionFollow (propagated); the literal EXPRESSION_WAIT and the
//   ExpressionDetectedWait store make it WatchForExpressionWait (Ego_r.pdb ?WatchForExpressionWait@CQR_EscortTraderScript).
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
inline bool CScriptThing::MsgExpressionPerformedTo(CCharString& expression) const { if (!PImp.Data) return false; return PImp.Data->MsgExpressionPerformedTo(expression); }
inline bool CScriptThing::IsAlive() const { return PImp.Data ? PImp.Data->IsAlive() : false; }
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
struct CQR_EscortTraderScript : CQuestScriptBase {
    unsigned char _pad48[0x70 - 0x48];
    bool          ExpressionDetectedFollow;                 // +0x70
    bool          ExpressionDetectedWait;                   // +0x71
    void RestartRandomSpeechTimer();                        // 0x00E1FEB0 (TraderRandomSpeechTimer +0x84)
    void WatchForExpressionWait(CScriptThing trader);
};

void CQR_EscortTraderScript::WatchForExpressionWait(CScriptThing trader)
{
    CCharString expression;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (trader.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (trader.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_WAIT") {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                RestartRandomSpeechTimer();
                ExpressionDetectedWait = true;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        Game->NewScriptFrame();
    }
}