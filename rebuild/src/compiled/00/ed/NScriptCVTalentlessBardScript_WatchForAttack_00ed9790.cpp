// Levers: ~CScriptThing __forceinline; after the predicate + yield prologue a for(;;) with the terminating return first; in-body `#pragma inline_depth(0)` around both in-loop terminate returns (out-of-line ~CScriptThing), so only the prologue return and the loop-top return expand the dtor.
// CV_TalentlessBardScript::WatchForAttack(CScriptThing bard) -- with an is-alive predicate on the bard, flag the hero for hitting him while he lives
// Leaf: ?WatchForAttack@CV_TalentlessBardScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED9790, 453 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's copy), CScriptBase::SetPredicate 0x00CB7920,
//   ~CIsThingAlive 0x00E241B0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0 ("" 0x0122D70E, "SCRIPT_NAME_HERO" 0x0125D1C8), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line); through the interface (+0x40) NewScriptFrame
//   (slot 7), GetHealth (slot 264, +0x420); the thing's implementation slots 18 MsgIsKilledBy (+0x48), 21 MsgIsHitBy
//   (+0x54), 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac) behind the inlined null-safe wrappers.
//   Float 0.0001f 0x0129BA3C (`test ah,5 ; jnp` = ordered less-than).
// Layout: interface pointer +0x40 (CQuestScriptBase); no members are touched.
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
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
namespace NScript {
class __declspec(novtable) CPredicateBase { public:               // vtable 0x012C2FF4 (never stored: novtable)
    virtual bool IsTrue() = 0;
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:            // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                       // 0x00E241B0
    virtual bool IsTrue();
};
}
using NScript::CIsThingAlive;
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
class CV_TalentlessBardScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x61 - 0x48];
    bool          AttackedByHero;                           // +0x61
    void WatchForAttack(CScriptThing bard);
};
} // namespace NScript
using NScript::CV_TalentlessBardScript;

void NScript::CV_TalentlessBardScript::WatchForAttack(CScriptThing bard)
{
    SetPredicate(CIsThingAlive(bard));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (bard.IsAlive()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            bool hit = bard.MsgIsHitBy("SCRIPT_NAME_HERO") || bard.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
            if (hit) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                AttackedByHero = true;
            }
        }
        Game->NewScriptFrame();
    }
}