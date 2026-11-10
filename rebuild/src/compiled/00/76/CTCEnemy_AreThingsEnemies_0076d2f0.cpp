// CTCEnemy::AreThingsEnemies(const CThing& thing1, const CThing& thing2) -- whether two things are enemies: each side's speaking enemy component must exist and not be friends with everything; enemies when either component counts the other side's proxy thing as an enemy, else (two non-type-2 things) when one side is allied with the faction of the player's live controlled creature and that creature is, recursively, an enemy of the other side
// Leaf: ?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076D2F0, 356 bytes (+12 int3 pad), public static `__fastcall` (thing1 in ecx, thing2
//   in edx), plain `ret`.
// Name: Ego_r.pdb `?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z` (0x0077B430, between CalculateEnemyTC 0x0077B080 = retail
//   0x0076CFE0 and IsAllyOf 0x0077B5B0 = retail 0x0076D460) and FableWin.pdb (same spelling); the body calls CalculateEnemyTC twice
//   and itself twice. 17 landed callers; five bind non-PDB spellings (NAME_FIXES.tsv).
// Callees: CTCEnemy::CalculateEnemyTC 0x0076CFE0 (private static), CTCEnemy::IsEnemyOf(const CThing&) const 0x0076CCE0 (private),
//   CPlayerManager::GetMainPlayer() const 0x00449960, CPlayer::PeekPControlledCreature() const 0x00487DD0,
//   IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0, CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0
//   (inside the forced-inline CThing::PeekTC<const CTCEnemy>, FableWin.pdb ??$PeekTC@$$CBVCTCEnemy@@@CThing@@QBE_NPAPBVCTCEnemy@@@Z),
//   CTCEnemy::IsAlliedWithFaction(const CFaction*) const 0x0076C7F0. Data: CMainGameComponent::PMainGameComponent 0x013B86A0.
// Headers: opt-ins FABLE_RETAIL_CTCENEMY_QUERIES (engine/CTCEnemy.h: the query declarations and the FableWin inline accessors) and
//   FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (engine/ThingAlive.h: the real template body kept out of line, as this TU calls it).
// Shape: the two out-parameter lookups live in their own scope and their results are copied out: the first component's address
//   escapes to the out-of-line CalculateEnemyTC, and only the end of its scope frees its frame word, which retail shares with the
//   player pointer and PeekTC's key (`mov [esp+1Ch],edi`, 0x0076D3C6; `mov [esp+20h],49h`, 0x0076D3E5; a 4-word frame). Both
//   IsEnemyOf answers are taken before the test; `enemies` is their `||` (bl, with the zero of the null tests hoisted into ebx),
//   set to true by the player-faction rule, and returned at the single late exit (`mov bl,1 ; ... mov al,bl`, 0x0076D43E).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ThingAlive.h"

bool FABLE_FASTCALL CTCEnemy::AreThingsEnemies(const CThing& thing1, const CThing& thing2)
{
    // The components (and the things they speak for) that decide for each side. The lookup for a side waives the follower proxy
    // when the OTHER side is of class type 2.
    const CTCEnemy* tc1;
    const CTCEnemy* tc2;
    const CThing* other1;
    const CThing* other2;
    {
        const CTCEnemy* enemy_tc1 = 0;
        const CTCEnemy* enemy_tc2 = 0;
        const CThing* enemy_thing1 = &thing1;
        const CThing* enemy_thing2 = &thing2;
        CalculateEnemyTC(thing1, &enemy_tc1, enemy_thing1, thing2.GetType() == 2);
        CalculateEnemyTC(thing2, &enemy_tc2, enemy_thing2, thing1.GetType() == 2);
        tc1 = enemy_tc1; tc2 = enemy_tc2; other1 = enemy_thing1; other2 = enemy_thing2;
    }
    if (tc1 == 0 || tc1->IsFriendsWithEverything() || tc2 == 0 || tc2->IsFriendsWithEverything())
        return false;
    bool enemies1 = tc1->IsEnemyOf(*other2);
    bool enemies2 = tc2->IsEnemyOf(*other1);
    bool enemies = enemies1 || enemies2;
    if (!enemies && thing1.GetType() != 2 && thing2.GetType() != 2) {
        const CThingPlayerCreature* player = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const CTCEnemy* player_tc;
        if (IS_THING_ALIVE(player) && player->PeekTC(&player_tc)) {
            const CFaction* player_faction = player_tc->GetPFaction();
            if ((tc1->IsAlliedWithFaction(player_faction) && AreThingsEnemies(*player, *other2)) ||
                (tc2->IsAlliedWithFaction(player_faction) && AreThingsEnemies(*player, *other1)))
                enemies = true;
        }
    }
    return enemies;
}