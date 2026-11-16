// NScript::CQ_TraderConflictEvilScript::Main() -- the trader conflict (evil) quest main: bind the five entity scripts, wait for Barrow Fields, set the village up for the raid (sick-child quest off, repopulate, hero as opinion source, guards ignore crimes and disabled, slum beds unusable), play the intro cut-scene from the hero's side, start the quest screen and music, spawn WatchTimeLimit, put every live enemy into the monster faction (replacing the shopkeepers with fake ones), then until 25 guards are down send the guards near a hit or provoked creature after the hero / the nearest bandit fighter, and finally play the outro, restore the village and complete the quest.
// Grade: RELOCATION_MATCH (6493v6493) with /d2inlT72 (wfE1d; device c replaced by device d in wfE1 h1). The last residue (six SIB base/index flips 0x00DF6839 / 6B86 / 6F0A / 6F1D / 7157 / 7169 and the GiveThingBestEnemyTarget block 0x00DF6F6E..6F90, calls and structure already equal) is a c2 symbol-record-count position (see Inferred). How it got here: ~vector<CScriptThing> left INLINE (no diagnostic declaration); the beds loop as a guarded do-while and the hero-side n loop as `while (n < size()) { ...; ++n; }` (FITTED, byte-identical to the for: c2 then gives their bodies no loop multiplier, which makes the beds-loop and loop-region ~vector / ~CScriptThing return sites CALLED as in retail; the plain beds `for` is DIFFER 8237v6493); FABLE_FORCEINLINE on the row-local GetDataString (retail expands it at 0x00DF64D1); `#pragma inline_depth(0)` round the k-loop top terminate check (retail calls its pad 0x00DF75EC, c2 score 67); /d2inlT72 (R23 = n2-loop top: retail expands the three ~CScriptThing (sc 68) and calls ~vector (sc 73) at 0x00DF75C7, while the guard-top / post-loop ~vector (sc 71) stay expanded); CScriptThing::operator== in the landed spelling `PImp->IsEqualTo(*other.PImp)`. Behaviour fixes: MissionSucceeded = true, signed guard count, AllCreatures[j] in the k loop.
// Inferred: 6 dead release asserts (FABLE_RETAIL_ASSERT, overlay fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM), each one CCharString text compare on a const-ref GetName() (no class temporary, so no conditional-temporary flag bit), all in the guard loop or after it (from the fighter lookup on). The COUNT and the region are EVIDENCED by grading, the conditions are INFERRED/FITTED (retail emits no byte for them). The dead arms number c2 symbol records before c2 deletes them, and the last residue (six SIB flips + the GiveThingBestEnemyTarget block) closes only at one record position, the one the earlier 224-unused-locals ruler hit. Count sweep (h1/subs*.log, every subset of 7 candidate sites x k = 0..7 extra locals): 1..5 asserts DIFFER for every k (5: 3-7 hunks), 6 asserts RELOCATION_MATCH at k = 0 for 5 of the 7 six-subsets (k = 1: 6AC9 flip, 2..7: 66B8 band), 7 and 8 also match at k = 0; asserts placed before the guard loop (worth 35-36 records each instead of 32 to the loop records) need k = 23 with 6. No unused locals. Under FABLE_PORTABLE / other compilers the asserts are ((void)0); they cannot change behaviour.
// Inline-budget: /d2inlT72 (per-site ~vector / ~CScriptThing split, see Grade line) + scoped inline_depth(0) at the k-loop top check + FABLE_FORCEINLINE GetDataString (row-local inline helper). All three are needed (measured with the asserts in place): without the FORCEINLINE DIFFER 6477v6493 at T72/T73, without the inline_depth region DIFFER 6605v6493 (T73) / 6653v6493 (T72), without both 6589v6493 (T73); no T in 50..100 and no /d2inlS 8..30 replaces either; with no flag 6541v6493.
// Leaf: ?Main@CQ_TraderConflictEvilScript@NScript@@UAEXXZ
// Flags: /d2inlT72
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF6010, 6493 bytes, thiscall, no EH frame, ends in its `ret` at 0x00DF796C.
// Name: slot 2 (Main) of the quest-script vtable 0x012DE6A4 (slot 1 the landed RegisterMain 0x00DF5F50); Ego_r.pdb name set
//   ?Main@CQ_TraderConflictEvilScript@NScript@@UAEXXZ (public virtual); FableWin.pdb holds the same spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_TraderConflictEvilScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_distance.h"
#include "engine/EHeroAbility.h"
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
FABLE_FORCEINLINE CCharString CScriptThing::GetDataString() const { if (!PImp.Data) return NullString; return PImp.Data->GetDataString(); }


void NScript::CQ_TraderConflictEvilScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictEvilScript>("TC_GuardSpawnPoint", this, &CTC_GuardSpawnPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictEvilScript>("TC_BanditFollower", this, &CTC_BanditFollower::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictEvilScript>("TC_BanditFighter", this, &CTC_BanditFighter::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictEvilScript>("TC_Villager", this, &CTC_Villager::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictEvilScript>("IsAGuard", this, &CIsAGuard::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("BarrowFields")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuest("V_SickChildBarrowFields", 0);
    Game->RepopulateVillage(Game->GetThingWithScriptName("VILLAGE_BARROWFIELDS"));
    Game->EntitySetAsOpinionSource(Game->GetHero(), "OPINION_SOURCE_VILLAGER_TRADER_CONFLICT_EVIL");
    Game->SetGuardsIgnoreCrimes(true);
    Game->EnableGuards(Game->GetThingWithScriptName("VILLAGE_BARROWFIELDS"), false);
    std::vector<CScriptThing> beds;
    long bed_count = Game->GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01", beds);
    long b = 0;
    if (b < bed_count) {
        do {
            if (IsActiveThreadTerminating())
                return;
            Game->SetThingAsUsable(beds[b], false);
        } while (++b < bed_count);
    }
    if (IsActiveThreadTerminating())
        return;
    UpdateLiveEnemies();
    CScriptThing intro_pos = Game->GetNearestWithScriptName(Game->GetHero(), "TC_IntroCSHeroPos");
    if (intro_pos.GetDataString() == "SOUTH") {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_TRADERCON_EVIL_INTRO_SOUTH");
    }
    else {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_TRADERCON_EVIL_INTRO_NORTH");
    }
    if (!QuestStartScreened) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        QuestStartScreened = true;
    }
    Game->FadeScreenIn();
    Game->OverrideMusic((EMusicSetType)23, false, false);
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderConflictEvilScript>("WatchTimeLimit", 0, this, &CQ_TraderConflictEvilScript::WatchTimeLimit), CCharString(""));
    long fakes_created = 0;
    for (unsigned long i = 0; i < AllCreatures.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetCombatNearbyBreakOffRange(AllCreatures[i], 10.0f);
        Game->EntitySetInFaction(AllCreatures[i], "FACTION_MONSTERS");
        Game->MiniMapAddMarker(AllCreatures[i], "HUD_ORB_RED_SMALL");
        Game->EntitySetSleepEnabled(AllCreatures[i], false);
        if (!(AllCreatures[i].GetName() == "IsAGuard")) {
            if (IsActiveThreadTerminating())
                return;
            if (AllCreatures[i].GetDefName() == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER") {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing fake = Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_FAKE_SHOPKEEPER", AllCreatures[i].GetPos(), "TC_Villager", false);
                ++fakes_created;
                Game->SetCombatNearbyBreakOffRange(fake, 10.0f);
                Game->EntitySetInFaction(fake, "FACTION_MONSTERS");
                Game->MiniMapAddMarker(fake, "HUD_ORB_RED_SMALL");
                Game->EntitySetSleepEnabled(fake, false);
                Game->EntitySetStategroupEnabled(fake, "SG_MINION_SLEEP", false);
            }
            Game->EntitySetStategroupEnabled(AllCreatures[i], "SG_MINION_HAWKING_FROM_STALL", false);
            Game->EntitySetStategroupEnabled(AllCreatures[i], "SG_SELL_TO_BUYER", false);
            Game->EntitySetStategroupEnabled(AllCreatures[i], "SG_OPEN_AND_CLOSE_SHOP", false);
            Game->EntitySetStategroupEnabled(AllCreatures[i], "SG_SETUP_WARES", false);
            Game->EntitySetStategroupEnabled(AllCreatures[i], "SG_MINION_SLEEP", false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    for (long j = 0; j < fakes_created; ++j) {
        if (IsActiveThreadTerminating())
            return;
        for (unsigned long k = 0; k < AllCreatures.size(); ++k) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (AllCreatures[j].GetDefName() == "CREATURE_OAKVALE_VILLAGER_MALE_SHOPKEEPER") {
                if (IsActiveThreadTerminating())
                    return;
                for (std::vector<CScriptThing>::iterator it = AllCreatures.begin(); it != AllCreatures.end(); ++it) {
                    if (AllCreatures[j] == *it) {
                        AllCreatures.erase(it);
                        break;
                    }
                }
                break;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    UpdateLiveEnemies();
    InitialNumberInRegion = AllCreatures.size();
    Game->DisplayQuestInfo(true);
    CounterID = Game->AddQuestInfoCounter("HUD_QUEST_ICON_GUARD", 25, 1.0f);
    Game->UpdateQuestInfoCounter(CounterID, AllCreatures.size() - InitialNumberInRegion - NumberSpawned + 25, -1);
    CScriptThing follower = Game->GetThingWithScriptName("TC_BanditFollower");
    while ((long)AllCreatures.size() - InitialNumberInRegion - NumberSpawned + 25 > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        for (unsigned long m = 0; m < AllCreatures.size(); ++m) {
            if (IsActiveThreadTerminating())
                return;
            if (AllCreatures[m].MsgIsHitBy("SCRIPT_NAME_HERO") || (AllCreatures[m].MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !AllCreatures[m].MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) || IsDistanceBetweenThingsUnder(AllCreatures[m], follower, 10.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                unsigned long n = 0;
                while (n < AllCreatures.size()) {
                        if (IsActiveThreadTerminating())
                            return;
                        if (IsDistanceBetweenThingsUnder(AllCreatures[m], AllCreatures[n], 10.0f) && AllCreatures[n].GetName() == "IsAGuard") {
                            if (IsActiveThreadTerminating())
                                return;
                            Game->GiveThingBestEnemyTarget(AllCreatures[n], Game->GetHero());
                            // Inferred: dead release assert (1 of 6, see header).
                            FABLE_RETAIL_ASSERT(AllCreatures[n].GetName() == "IsAGuard");
                        }
                    ++n;
                }
            }
            if (AllCreatures[m].MsgIsHitBy("TC_BanditFighter") || (AllCreatures[m].MsgIsHitByAnySpecialAbilityFrom("TC_BanditFighter") && !AllCreatures[m].MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing fighter = Game->GetNearestWithScriptName(AllCreatures[m], "TC_BanditFighter");
                // Inferred: dead release assert (1 of 6, see header).
                FABLE_RETAIL_ASSERT(fighter.GetName() == "TC_BanditFighter");
                for (unsigned long n = 0; n < AllCreatures.size(); ++n) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (IsDistanceBetweenThingsUnder(AllCreatures[m], AllCreatures[n], 10.0f) && AllCreatures[n].GetName() == "IsAGuard") {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->GiveThingBestEnemyTarget(AllCreatures[n], fighter);
                        // Inferred: dead release assert (1 of 6, see header).
                        FABLE_RETAIL_ASSERT(AllCreatures[n].GetName() == "IsAGuard");
                    }
                }
            }
        }
        UpdateLiveEnemies();
        // Inferred: dead release assert (1 of 6, see header).
        FABLE_RETAIL_ASSERT(follower.GetName() == "TC_BanditFollower");
        Game->UpdateQuestInfoCounter(CounterID, AllCreatures.size() - InitialNumberInRegion - NumberSpawned + 25, -1);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(CounterID);
    // Inferred: dead release assert (1 of 6, see header).
    FABLE_RETAIL_ASSERT(follower.GetName() == "TC_BanditFollower");
    Game->StopOverrideMusic(false);
    MissionSucceeded = true;
    PlayCutscene("CS_TRADERCON_EVIL_OUTRO");
    // Inferred: dead release assert (1 of 6, see header).
    FABLE_RETAIL_ASSERT(Game->GetHero().GetName() == "SCRIPT_NAME_HERO");
    Game->EntityUnsetAsOpinionSource(Game->GetHero(), true);
    Game->SetGuardsIgnoreCrimes(false);
    for (b = 0; b < bed_count; ++b) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingAsUsable(beds[b], true);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    CCharString unused_name;
    if (Game->IsQuestActive("V_SickChild")) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_SickChildBarrowFields");
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}