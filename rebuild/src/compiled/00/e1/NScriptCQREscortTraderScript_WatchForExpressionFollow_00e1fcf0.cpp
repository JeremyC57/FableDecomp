// Levers: for(;;) with the terminating return first, then `if (alive) { ... } else { if (term) return; break; }` (the dead branch last, so its exits are laid out after the hit-block call); every terminate check is `do { if (term) return; } while (0);` with a plain-inline ~CScriptThing, so VC7.1's per-site frequency rule (docs/pipeline/INLINE_DEPTH_REVISIT.md) leaves the hit-block exit out of line; the CCharString == is the nested CBasicString<char>::operator== (result byte homed in the frame) with `strcmp(text, "")` in the null arm; IsAlive is the `Data ? Data->IsAlive() : false` ternary (test al,al).
// CQR_EscortTraderScript::WatchForExpressionFollow(CScriptThing trader) -- spawned per trader: each frame, stop when the trader dies; when the hero performs EXPRESSION_FOLLOW to it, restart the random-speech timer and set ExpressionDetectedFollow
// Leaf: ?WatchForExpressionFollow@CQR_EscortTraderScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1FCF0, 435 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, the thing's
//   implementation slots 75 IsAlive (+0x12c) and 29 MsgExpressionPerformedTo(CCharString&) (+0x74, wrapper 0x004AABF0),
//   Compare 0x00411570 (__fastcall, the inlined CCharString == const char*; the null-string arm is the strcmp intrinsic
//   against "" 0x0122D70E), EXPRESSION_FOLLOW 0x012757EC, RestartRandomSpeechTimer 0x00E1FEB0 (sets TraderRandomSpeechTimer +0x84 from
//   rand()), NewScriptFrame (interface slot 7), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840.
// Layout (retail = Ego_r.pdb - 0x14): ExpressionDetectedFollow +0x70.
#include <string.h>
#pragma intrinsic(strcmp)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE   // Compare(PData,text) ? false : true, expanded in the quest TUs
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#include "fable_string.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
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
#include "engine/NScript_CQR_EscortTraderScript.h"   // Ego_r/FableWin owner, retail base 0x48: ExpressionDetectedFollow +0x70, ExpressionDetectedWait +0x71

void NScript::CQR_EscortTraderScript::WatchForExpressionFollow(CScriptThing trader)
{
    CCharString expression;
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (trader.IsAlive()) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            if (trader.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW") {
                do { if (IsActiveThreadTerminating()) return; } while (0);
                ResetTraderRandomSpeechTime();
                ExpressionDetectedFollow = true;
            }
        } else {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            break;
        }
        Game->NewScriptFrame();
    }
}