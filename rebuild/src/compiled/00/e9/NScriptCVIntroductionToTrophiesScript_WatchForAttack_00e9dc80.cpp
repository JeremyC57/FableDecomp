// CV_IntroductionToTrophiesScript::WatchForAttack(CScriptThing briar) -- while Briar Rose lives, each time the hero hits her and she survives count the hit, re-arm the hit warning, charge the hero with a crime against Bowerstone Slums and (the first time) flag the attack
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9DC80, 0x260 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0 ("SCRIPT_NAME_HERO" 0x0125D1C8),
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line at the in-loop exits);
//   Game slots 7 NewScriptFrame, 70 GetHero, 81 GetNearestWithDefName, 264 GetHealth, 450 AddCrimeCommitted; the
//   thing's implementation slots 21 MsgIsHitBy, 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom, 75 IsAlive behind the
//   null-safe wrappers. Float 0.0001f 0x0129BA3C.
// Shape: the landed CV_TalentlessBardScript::WatchForAttack 0x00ED9790 model (b18 recipe: __forceinline ~CScriptThing,
//   in-body inline_depth(0) around the exits retail calls out of line).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, AttackedByHero +0x4a, HitByHeroCount +0x50, HitWarningComplete +0x54.
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_IntroductionToTrophiesScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    bool DoneBriarIntroSpeech;                          // +0x48
    bool DoneBriarTrophySpeech;                         // +0x49
    bool AttackedByHero;                                // +0x4a
    bool DoneApproachMessage1;                          // +0x4b
    bool DoneApproachMessage2;                          // +0x4c
    bool AttackSpeechComplete;                          // +0x4d
    long HitByHeroCount;                                // +0x50
    bool HitWarningComplete;                            // +0x54
    void WatchForAttack(CScriptThing briar);
};
} // namespace NScript
using NScript::CV_IntroductionToTrophiesScript;

void NScript::CV_IntroductionToTrophiesScript::WatchForAttack(CScriptThing briar)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!briar.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (briar.MsgIsHitBy("SCRIPT_NAME_HERO") || briar.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (Game->GetHealth(briar) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                ++HitByHeroCount;
                HitWarningComplete = false;
#pragma inline_depth(0)
                Game->AddCrimeCommitted(Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BOWERSTONE_SLUMS"), (ECrime)7, false, 0, 0, (EOpinionPostDeedType)7);
#pragma inline_depth()
                if (HitByHeroCount >= 0 && !AttackedByHero) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    AttackedByHero = true;
                }
            }
        }
        Game->NewScriptFrame();
    }
}