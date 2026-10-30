// Levers: ~CScriptThing __forceinline; for(;;) with the terminating return first, the dead branch (`if (term) return; if (term) return; break;`) before the hit test, in-body `#pragma inline_depth(0)` around the attacked terminate return (the one out-of-line ~CScriptThing).
// CV_BeardyBaldyScript::WatchForAttack(CScriptThing) -- while the watched creature lives, flag AttackedByHero whenever the hero hits it (melee or an aggressive special ability) and it survives the hit
// Leaf: ?WatchForAttack@CV_BeardyBaldyScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E53D90, 464 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SCRIPT_NAME_HERO" 0x0125D1C8),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   ~CScriptThing 0x004AA840 (out of line); through the interface (+0x40) NewScriptFrame (slot 7), GetHealth
//   (slot 264, +0x420); the thing's implementation slots 21 MsgIsHitBy (+0x54), 43
//   MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac) and 75 IsAlive (+0x12c) behind the inlined null-safe wrappers.
//   Float 0.0001f 0x0129BA3C.
// Layout (retail = Ego_r.pdb - 0x14): AttackedByHero +0x70; interface pointer +0x40.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
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
class CV_BeardyBaldyScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x70 - 0x48];                      // QuestFinished +0x48 .. IncorrectHairComboCount +0x6c
    bool          AttackedByHero;                           // +0x70
    void WatchForAttack(CScriptThing thing);
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;

void NScript::CV_BeardyBaldyScript::WatchForAttack(CScriptThing thing)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!thing.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        bool attacked = (thing.MsgIsHitBy("SCRIPT_NAME_HERO") || thing.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO"))
            && Game->GetHealth(thing) > 0.0001f;
        if (attacked) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            AttackedByHero = true;
        }
        Game->NewScriptFrame();
    }
}