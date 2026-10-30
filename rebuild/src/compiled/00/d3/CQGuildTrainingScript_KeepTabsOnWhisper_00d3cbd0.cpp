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
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes {vptr, PImp.Data, PImp.Info}
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18) 0x004AA980
    virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                           // slot 75 (+0x12c)
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                        // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                        // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
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