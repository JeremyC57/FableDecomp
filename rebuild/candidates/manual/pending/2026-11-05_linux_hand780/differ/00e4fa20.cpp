// NScript::CV_BeardyBaldyScript::Main() -- bind the BB_BeardyBaldyMan entity script, wait until BowerstoneSlumsWarehouses reports loaded, spawn the seven watcher threads, mark the quest activated and point the hero at Beardy Baldy; then wait until the quest is finished (or complete and the region unloaded), clear the markers and information on the barber and Beardy Baldy, and complete, remove and deactivate the quest card
// Grade: DIFFER(1763v1756) -- VC7.1 pushes edi lazily after the first terminate check (a private epilogue copy without `pop edi`, `push edi ; mov edi,edi` at the region-loop head, frame slots +4 until then); retail pushes it at entry. Every other byte matches, including the inline (barber) / out-of-line (beardyBaldy) IsAlive split
// Next idea: the forms that keep the entry push (first check inside the region loop, probes c/e/h; or the body nested under `if (!IsActiveThreadTerminating())`) cool the barber IsAlive site (T* 150-170 against retail's 74) and every do-while(0) lever jumps past the window (probes q..D: either site cold or both inline); find a spelling with the a-form frequencies (plain early return, region loop at function scope) in which edi is needed before the first check
// Leaf: ?Main@CV_BeardyBaldyScript@NScript@@UAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4FA20, 1756 bytes, thiscall virtual (CLevelScriptBase slot 2), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb name set ?Main@CV_BeardyBaldyScript@NScript@@UAEXXZ; the landed same-TU
//   RegisterMain 0x00E4F730 spawns "Main" on this member; literals "BB_BeardyBaldyMan" 0x012E5AD8, "BowerstoneSlumsWarehouses"
//   0x012E5AB4, the seven thread names 0x012E27B4 / 0x012E5A98 / 0x012E5A80 / 0x012E5A64 / 0x012E5A50 / 0x012E5A3C / 0x012E5A2C,
//   "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER" 0x012E59F8, "" 0x0122D70E; control flow = the FSE Lua port
//   refs/script_recovery/seed_corpus/sources/BeardyBaldy/FSE/BeardyBaldy/BeardyBaldy.lua (Main).
// Callees: operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940 /
//   AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450, CBasicString<char>::Compare
//   0x00411570 (the inline text equality, null arm `repe cmpsb` of the literal against ""), GoTalkToBeardyBaldy 0x00E50650,
//   CScriptThing::IsAlive 0x004AB130 (out of line at the second site, inline at the first), ~CScriptThing 0x004AA840; the spawned
//   members WatchForQuestFinished 0x00E50100 (manifest label CV_AmbushScamScript:: is a byte-twin propagation),
//   WatchForQuestCardConditions 0x00E50910, WatchForBarberLadyDeath 0x00E504C0, WatchForBeardyBaldyDeath 0x00E503D0,
//   WatchForNewHairdo 0x00E50150, WatchForNewBeard 0x00E50210, WatchForNewTash 0x00E502E0 (Ego_r / FableWin name sets);
//   CBB_BeardyBaldyMan::Alloc 0x00E507E0; vtables CEntityScriptBinding<CV_BeardyBaldyScript> 0x012E5AD0, CSpawnedFunc<CV_BeardyBaldyScript>
//   0x012E5894. Interface slots 7 NewScriptFrame, 12 IsRegionLoaded, 22 MsgOnRegionLoaded, 64 PostAddScriptedEntities, 70 GetHero,
//   72 GetThingWithScriptName, 77 GetNearestWithScriptName, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 294
//   RemoveQuestCardFromHero, 352 MiniMapRemoveMarker, 361 ClearThingHasInformation, 655 GetActiveQuestName; thing slot 75 IsAlive.
// Layout (retail = Ego_r.pdb - 0x14, the landed OnPersist 0x00E4F7F0 list): QuestFinished +0x48, QuestComplete +0x49, QuestActivated +0x4a.
// Shape: the landed quest Mains (CQ_SecretPassageScript::Main 0x00DECAD0, CV_HeroDuelScript::Main 0x00E987C0): one ebx flag word
//   guards the name temporaries of the binding and the seven spawns; the region wait is the generated MsgOnRegionLoaded poll with
//   the inline CCharString == "literal" (hdr overlay opt-in FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE); the finish wait evaluates its
//   condition into a bool before the IsRegionLoaded temporary dies.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include <string.h>
#pragma intrinsic(strcmp)
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
class CEntityScriptBase;
namespace NScript {
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFinished;                                 // +0x48
    bool QuestComplete;                                 // +0x49
    bool QuestActivated;                                // +0x4a
    class CBB_BeardyBaldyMan;
    virtual void Main();                                // slot 2
    void WatchForQuestFinished();                       // 0x00E50100
    void WatchForQuestCardConditions();                 // 0x00E50910
    void WatchForBarberLadyDeath();                     // 0x00E504C0
    void WatchForBeardyBaldyDeath();                    // 0x00E503D0
    void WatchForNewHairdo();                           // 0x00E50150
    void WatchForNewBeard();                            // 0x00E50210
    void WatchForNewTash();                             // 0x00E502E0
    void GoTalkToBeardyBaldy();                         // 0x00E50650
};
class CV_BeardyBaldyScript::CBB_BeardyBaldyMan { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BeardyBaldyScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E507E0
};
}
void NScript::CV_BeardyBaldyScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BeardyBaldyScript>("BB_BeardyBaldyMan", this, &CBB_BeardyBaldyMan::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString regionName;
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        while (!Game->MsgOnRegionLoaded(regionName)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (regionName == "BowerstoneSlumsWarehouses") break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (IsActiveThreadTerminating()) return;
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForQuestFinished", 0, this, &CV_BeardyBaldyScript::WatchForQuestFinished), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForQuestCardConditions", 0, this, &CV_BeardyBaldyScript::WatchForQuestCardConditions), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForBarberLadyDeath", 0, this, &CV_BeardyBaldyScript::WatchForBarberLadyDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForBeardyBaldyDeath", 0, this, &CV_BeardyBaldyScript::WatchForBeardyBaldyDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewHairdo", 0, this, &CV_BeardyBaldyScript::WatchForNewHairdo), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewBeard", 0, this, &CV_BeardyBaldyScript::WatchForNewBeard), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewTash", 0, this, &CV_BeardyBaldyScript::WatchForNewTash), CCharString(""));
    if (!QuestActivated) {
        if (IsActiveThreadTerminating()) return;
        QuestActivated = true;
    }
    GoTalkToBeardyBaldy();
    while (!QuestFinished && (!QuestComplete || Game->IsRegionLoaded("BowerstoneSlumsWarehouses"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing beardyBaldy = Game->GetThingWithScriptName("BB_BeardyBaldyMan");
    CScriptThing barber = Game->GetNearestWithScriptName(Game->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    if (barber.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->MiniMapRemoveMarker(barber);
        Game->ClearThingHasInformation(barber);
    }
    if (beardyBaldy.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->MiniMapRemoveMarker(beardyBaldy);
        Game->ClearThingHasInformation(beardyBaldy);
    }
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
