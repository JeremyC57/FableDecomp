// CQ_GuildTrainingScript::KeepTabsOnWhisper() -- once the woods-will quest is running and the hero is back in the guild complex with the melee apprentice still alive, remove the apprentice, wait for that quest to end and, unless the departure quest is active, recreate Whisper's teen-apprentice at the melee opponent stand
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3CBD0, length 1167 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing::GetPos (slot 6) / IsAlive (slot 75); the game script
//   interface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded, 72 GetThingWithScriptName, 91 CreateCreature, 108 RemoveThing,
//   284 IsQuestActive.
// Strings: "GuildWoods" 0x012CD7DC, "Q_GuildTrainingWoodsWill" 0x012C5AC4, "HeroGuildComplex" 0x012C5AFC, "MeleeApprentice"
//   0x012CD6F8, "Q_GuildTrainingDeparture" 0x012C5A4C, "M_MeleeOpponentStand" 0x012CD7C4,
//   "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE" 0x012CD798.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement); the `&&` condition's
//   name temporaries are guarded by a flag word; the "still active" wait is written `!= false` (test/setne), the
//   "not loaded" waits `!f()` (neg/sbb/inc), as the landed CQ_GuildTrainingScript::KeepBookcaseExitRemoved 0x00D3C8F0.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingScript : public CScriptBase { public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    long                  GameState;                        // +0x48
    void KeepTabsOnWhisper();
};
void CQ_GuildTrainingScript::KeepTabsOnWhisper()
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->IsLevelLoaded("GuildWoods") && Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsLevelLoaded("HeroGuildComplex")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsQuestActive("Q_GuildTrainingWoodsWill")) {
                if (IsActiveThreadTerminating())
                    return;
                if (Game->GetThingWithScriptName("MeleeApprentice").IsAlive())
                    break;
            }
        }
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Game->GetThingWithScriptName("MeleeApprentice"), false, true);
    while (Game->IsQuestActive("Q_GuildTrainingWoodsWill") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!Game->IsQuestActive("Q_GuildTrainingDeparture")) {
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("HeroGuildComplex")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", Game->GetThingWithScriptName("M_MeleeOpponentStand").GetPos(), "MeleeApprentice", false);
    }
}
}