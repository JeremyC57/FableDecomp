// NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main() -- Maze's guild trigger: alive filter on Me, keep Me persistent; after the post-save point drop the "TravHero Maze-Scythe" rumour and remove Me, otherwise set up the guild gossip; mark PreMeleeMaze as having information and, until the loop exits, wait for the hero to come within 2 units (or a guild warning): past the post-save point just exit, during a guild warning wait it out, clear the information, wait for the hero to walk 40 units away and set it again, otherwise play the maze-argument cut-scene; then drop the rumour, flag the maze cut-scene done, clear PreMeleeMaze's information and remove Me
// Leaf: ?Main@CTH_Trigger_Guild_Maze@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE3B20, 952 bytes; entity-script vtable 0x012F42CC slot 1 (Main), stored by the
//   landed CTH_Trigger_Guild_Maze alloc 0x00EE03E0; PDB ?Main@CTH_Trigger_Guild_Maze@CV_TravellingHeroesScript@NScript@@EAEXXZ
//   (Ego_r.pdb + FableWin.pdb). Catalog label is a propagated sub_; Init 0x00EE0490 clears ExitThisLoop (+0x1c).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, ~CIsThingAlive 0x00E241B0 (predicate vtable 0x012C32F0),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall),
//   CV_TravellingHeroesScript::GossipSetup 0x00EE07E0 / PlayCutscene 0x00EE2DB0; CGameScriptInterface slots 7, 70, 72, 108, 360, 361,
//   477, 719.
// Strings: "TravHero Maze-Scythe" 0x012F4670, "FACTION_GUILD" 0x012F4354, "VILLAGE_GUILD_COMPLEX_INSIDE" 0x012C5CA0,
//   "TEXT_AI_GOSSIP_MAZE_SCYTHE_GUILD" 0x012F464C, "PreMeleeMaze" 0x012CD694, "CS_TRAVHERO_MAZEARGUMENT" 0x012F4630.
// Layout: Ego_r.pdb CTH_Trigger_Guild_Maze (0x20): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, ExitThisLoop +0x1c. CQ_SunnyvaleMasterData: PostSavePosition +4, GuildWarningOccuring +0xc0.
//   CV_TravellingHeroesScript sits 0x14 below the Ego_r.pdb offsets: MazeCutsceneDone +0x7b.
// Shape: the landed CTH_Maze::Main 0x00EE1E40 (same TU) model; VC7.1 expands ~CScriptThing inline at one return.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
namespace NScript {
class __declspec(novtable) CPredicateBase { public:
    virtual ~CPredicateBase();                         // Ego_r.pdb ??1CPredicateBase@NScript@@UAE@XZ
};
class CIsThingAlive : public CPredicateBase { public:   // vtable 0x012C32F0
    CScriptThing Thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                          // 0x00E241B0
};
}
typedef long EGameflowPosition;
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad0[4];
    EGameflowPosition PostSavePosition;                // +0x04
    unsigned char _pad8[0xb8];
    bool GuildWarningOccuring;                         // +0xc0
};
class CV_TravellingHeroesScript { public:
    unsigned char _pad[0x78];
    bool ThunderKidsTalking;                           // +0x78
    bool ThunderSceneStarted;                          // +0x79
    bool ThunderSceneDone;                             // +0x7a
    bool MazeCutsceneDone;                             // +0x7b
    void GossipSetup(CCharString category, CCharString rumour, CCharString village, CCharString faction);   // 0x00EE07E0
    void PlayCutscene(CCharString name);               // 0x00EE2DB0
    class CTH_Trigger_Guild_Maze;
};
class CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;            // +0x18
    bool                       ExitThisLoop;           // +0x1c
private:
    virtual void Main();                                // slot 1 (+0x04)
};
}

void NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetThingPersistent(Me, true);
    if (PMasterData->PostSavePosition > 100) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveRumourCategory("TravHero Maze-Scythe");
        Game->RemoveThing(Me, false, true);
    }
    else {
        if (IsActiveThreadTerminating()) return;
        ParentClass->GossipSetup("TravHero Maze-Scythe", "TEXT_AI_GOSSIP_MAZE_SCYTHE_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE", "FACTION_GUILD");
    }
    CScriptThing preMelee = Game->GetThingWithScriptName("PreMeleeMaze");
    Game->SetThingHasInformation(preMelee, false, true, false);
    while (!ExitThisLoop) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f) && !PMasterData->GuildWarningOccuring) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (PMasterData->PostSavePosition > 100) {
            if (IsActiveThreadTerminating()) return;
            ExitThisLoop = true;
        }
        else if (PMasterData->GuildWarningOccuring) {
            if (IsActiveThreadTerminating()) return;
            while (PMasterData->GuildWarningOccuring) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->ClearThingHasInformation(preMelee);
            while (!IsDistanceBetweenThingsOver(Me, Game->GetHero(), 40.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->SetThingHasInformation(preMelee, false, true, false);
        }
        else {
            if (IsActiveThreadTerminating()) return;
            ParentClass->PlayCutscene("CS_TRAVHERO_MAZEARGUMENT");
            ExitThisLoop = true;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveRumourCategory("TravHero Maze-Scythe");
    ParentClass->MazeCutsceneDone = true;
    Game->ClearThingHasInformation(preMelee);
    Game->RemoveThing(Me, false, true);
}