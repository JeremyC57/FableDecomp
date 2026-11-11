// NScript::CGlobal_TeleportToHeroGuildScript::Main() -- the guild-seal teleport: when a teleport is under way, either send the hero back to the saved recall spot (he is at the guild marker, or the guild interior is loaded) or make his followers wait, save his spot and teleport him to the guild marker; then switch the script off
// Shape: one recall block for both "at the marker" and "inside the guild" (0x00CDD80B, reached from 0x00CDD7AC and 0x00CDD7FD); the
//   followers branch leaves past it with `goto done` (a flag or an inline helper twice graded 1006 / 788 bytes).
// Leaf: ?Main@CGlobal_TeleportToHeroGuildScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD6B0, 1012 bytes, thiscall, plain `ret`; slot 2 (Main) of the script vtable 0x012C2FB8
//   (cell 0x012C2FC0). Identity: Ego_r.pdb ?Main@CGlobal_TeleportToHeroGuildScript@NScript@@UAEXXZ; the landed RegisterMain 0x00CDD5F0
//   of the same vtable spawns it as "Main"; the body deactivates "Global_TeleportToHeroGuild".
// Callees: CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940, CScriptThing copy 0x004ABE90 / dtor 0x004AA840,
//   CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF, CScriptGameResourceObjectScriptedThingBase ctor
//   0x007E72A0 / PerformExpression 0x007E73C0 / dtor 0x007E74D0, ~vector<CScriptThing> 0x008AC970; CGameScriptInterfaceBase slots 7
//   NewScriptFrame, 8 StartScriptingEntity, 12 IsRegionLoaded, 70 GetHero, 72 GetThingWithScriptName, 231 GetFollowingEntityList,
//   281 DeactivateQuestLater, 472 EntityTeleportToThing, 473 EntityTeleportToPosition, 474 EntitySetFacingAngle, 566
//   IsTeleportingActive, 710 SetGuildSealRecallLocation, 711 GetGuildSealRecallPos, 712 GetGuildSealRecallAngleXY; CScriptThing slots
//   6 GetPos, 10 GetAngleXY, 75 IsAlive (on the implementation).
// Data: CIsThingConscious vtable 0x012C2FE8; DELTA_NOTIONAL_ZERO 0x0129BA3C.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/fable_script_distance.h"

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2, SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };

// Retail expands the predicate's constructor and destructor in place (vtable 0x012C2FE8 stored at 0x00CDD6CC and 0x00CDD6E5).
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& t) : Thing(t) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
// The follower list's destructor is the out-of-line COMDAT 0x008AC970 in this TU.
template<> std::vector<CScriptThing>::~vector();

// A script handle is usable when it has an implementation and that implementation is alive (0x00CDD757..0x00CDD76F).
static inline bool IsThingImpAlive(const CScriptThing& thing)
{
    CScriptThing* imp = thing.PImp.Data;
    return imp && imp->IsAlive();
}

namespace NScript {
class CGlobal_TeleportToHeroGuildScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    virtual void RegisterMain();                        // 0x00CDD5F0
    virtual void Main();                                // 0x00CDD6B0
};
}

void NScript::CGlobal_TeleportToHeroGuildScript::Main()
{
    SetPredicate(CIsThingConscious(Game->GetHero()));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    if (Game->IsTeleportingActive()) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing marker = Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER");
        if (IsThingImpAlive(marker)) {
            if (IsActiveThreadTerminating()) return;
            if (IsDistanceBetweenThingsUnder(marker, Game->GetHero(), 5.0f)) {
                // already at the guild marker
                if (IsActiveThreadTerminating()) return;
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->IsRegionLoaded("HeroGuildComplexInside")) {
                    // inside the guild
                    if (IsActiveThreadTerminating()) return;
                } else {
                    // away from the guild: the followers wait, the spot is remembered and the hero goes to the marker
                    if (IsActiveThreadTerminating()) return;
                    std::vector<CScriptThing> followers;
                    Game->GetFollowingEntityList(Game->GetHero(), followers);
                    CScriptGameResourceObjectScriptedThingBase hero;
                    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    for (unsigned int i = 0; i < followers.size(); ++i) {
                        if (IsActiveThreadTerminating()) return;
                        hero.PerformExpression(followers[i], "EXPRESSION_WAIT");
                    }
                    if (IsActiveThreadTerminating()) return;
                    const CScriptThing& angleThing = Game->GetHero();
                    const CScriptThing& posThing = Game->GetHero();
                    Game->SetGuildSealRecallLocation(posThing.GetPos(), angleThing.GetAngleXY());
                    Game->EntityTeleportToThing(Game->GetHero(), marker, true);
                    goto done;
                }
            }
            // back to where the seal was used (when a spot is remembered), and forget the spot
            if (IsActiveThreadTerminating()) return;
            if (Game->GetGuildSealRecallPos().IsNonZero()) {
                if (IsActiveThreadTerminating()) return;
                Game->EntityTeleportToPosition(Game->GetHero(), Game->GetGuildSealRecallPos(), 0.0f, true, true);
                Game->EntitySetFacingAngle(Game->GetHero(), Game->GetGuildSealRecallAngleXY(), true);
            }
            Game->SetGuildSealRecallLocation(C3DVector(0.0f, 0.0f, 0.0f), 0.0f);
        }
    done:;
    }
    Game->DeactivateQuestLater("Global_TeleportToHeroGuild", 0);
}