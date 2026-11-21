// CThingManager::ActivateThings(const vector<CThing*>& activate_these_things) -- bring freshly loaded things through their initialisation stages with a progress line per thing: OnCreate for the ENTITY_INIT_NONE ones, InitialActivate for ENTITY_INIT_ON_CREATE, OnInitialActivate for ENTITY_INIT_INITIAL_ACTIVATE; the player creature is held back while the world is serialising an about-to-load hero state and is initially activated last (when alive)
// Leaf: ?ActivateThings@CThingManager@@QAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E2F0, length 395 bytes (+5 int3), `ret 4`. thing_manager.obj.
// Identity: Ego_r.pdb / FableWin.pdb (0x01D3B850) ?ActivateThings@CThingManager@@QAEXABV?$vector@PAVCThing@@..@std@@@Z (public). Ego_d.pdb
//   (debug length 957): parameter activate_these_things; phero; one `it` (const_iterator) and one pthing per loop.
// Callees: CCharString(const char*, long) 0x0099EBF0, NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) 0x009E9F40
//   (`__fastcall`), ~CCharString 0x0099EAE0; CThing virtual slots 8 OnCreate / 9 InitialActivate / 10 OnInitialActivate (+0x20 / +0x24 /
//   +0x28; shared CThing header, FABLE_RETAIL_CTHING_ORIGINAL_VTABLE). Inline: CThing::GetEntityInitLevel (+0x94 bits 0..2), CClass::GetType
//   (+0x10), CThing::IsDead (+0x91 bit 0), CThingManager::GetWorld (+0x24), CWorld::IsSerialisingAboutToLoadHeroState (+0x101; FableWin
//   lists it right after this function). Strings "Activate Things" 0x01244E24, "Activate Things: OnCreate" 0x01244E08, "Activate Things:
//   Initial Activate" 0x01244DE4, "Activate Things End" 0x01244DD0.
// Ego_d lines (VC10 /Od): 2054 / 2070 / 2088 the for loops (114 / 114 / 110 B); 2056 / 2072 / 2090 the progress calls (string temporary,
//   EH states, the fastcall with dl = false); 2058 / 2074 / 2092 (13 B) pthing = *it; 2059 (12 B) / 2075 / 2093 (13 B) the
//   GetEntityInitLevel() call tested against ENTITY_INIT_NONE / ON_CREATE / INITIAL_ACTIVATE; 2061 / 2077 (34 B) the player-creature and
//   world-flag test; 2063 / 2079 (5 B) continue; 2066 / 2082 (13 B) and 2102 (13 B) the virtual calls; 2086 (7 B) phero = NULL; 2095 (13 B)
//   the GetType() test, 2098 (6 B) phero = pthing, 2100 (2 B) else; 2108..2112 the held-back hero; 2116 the closing progress call.
// Inferred: the held-back hero's condition as one `if` (`phero && !phero->IsDead() && !GetWorld().IsSerialisingAboutToLoadHeroState()`):
//   Ego_d splits it over lines 2108 (15 B) / 2110 (21 B), whose exact split is not decoded; retail tests the three in that order
//   (0x0051E423..0x0051E43F).
// Re-port (p25Q, 2026-11-21): the landed local models (CThing with slots named Initialise / OnCreate / Activate, a flat vector, a
//   CMainGameComponent at +0x24 with a "HoldHero" byte) and the made-up ShowProgress helper replaced by the shared headers. The +0x24 member is
//   CThingManager::World (CWorld&), and +0x101 is CWorld::SerialisingAboutToLoadHeroState. The text is Ego_d's.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_ALLOCATECLASS
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_DEAD
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ONACTIVATEENTITY
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CLEVELLOADER_LOADALL
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CTHING_ENTITYINITLEVEL
#define FABLE_RETAIL_CTHINGMANAGER_GETWORLD
#define FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE
#define FABLE_RETAIL_CTHINGMANAGER_LOADMAINPLAYERCREATURESTATE
#define FABLE_RETAIL_THING_TYPE_IDS
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "fable_game.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CQuestManager.h"
#include "engine/CLevelLoader.h"
#include "engine/CASuspendableProcess.h"
namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}
void CThingManager::ActivateThings(const std::vector<CThing*>& activate_these_things)
{
    for (std::vector<CThing*>::const_iterator it = activate_these_things.begin(); it != activate_these_things.end(); ++it) {
        NProgressDisplay::DisplayProgress("Activate Things", -1.0f, false, false);
        CThing* pthing = *it;
        if (pthing->GetEntityInitLevel() == CThing::ENTITY_INIT_NONE) {
            if (pthing->GetType() == THING_TYPE_PLAYER_CREATURE && GetWorld().IsSerialisingAboutToLoadHeroState())
                continue;
            pthing->OnCreate();
        }
    }
    for (std::vector<CThing*>::const_iterator it = activate_these_things.begin(); it != activate_these_things.end(); ++it) {
        NProgressDisplay::DisplayProgress("Activate Things: OnCreate", -1.0f, false, false);
        CThing* pthing = *it;
        if (pthing->GetEntityInitLevel() == CThing::ENTITY_INIT_ON_CREATE) {
            if (pthing->GetType() == THING_TYPE_PLAYER_CREATURE && GetWorld().IsSerialisingAboutToLoadHeroState())
                continue;
            pthing->InitialActivate();
        }
    }
    CThing* phero = NULL;
    for (std::vector<CThing*>::const_iterator it = activate_these_things.begin(); it != activate_these_things.end(); ++it) {
        NProgressDisplay::DisplayProgress("Activate Things: Initial Activate", -1.0f, false, false);
        CThing* pthing = *it;
        if (pthing->GetEntityInitLevel() == CThing::ENTITY_INIT_INITIAL_ACTIVATE) {
            if (pthing->GetType() == THING_TYPE_PLAYER_CREATURE)
                phero = pthing;
            else
                pthing->OnInitialActivate();
        }
    }
    if (phero && !phero->IsDead() && !GetWorld().IsSerialisingAboutToLoadHeroState())
        phero->OnInitialActivate();
    NProgressDisplay::DisplayProgress("Activate Things End", -1.0f, false, false);
}
