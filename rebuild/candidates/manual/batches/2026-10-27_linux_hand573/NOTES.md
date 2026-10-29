# Hand chunk 573 (NScript::CQ_TraderEscortScript .. NScript::CQ_AmbushTradersScript): 5 of 6 byte-exact

All five matches are RELOCATION_MATCH, compiler-emitted C++ (no retail_asm), each with a behaviour test printing
`<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). Dry vland (`tmp/vland.log`): WINS 5, no RELOC_IDENTITY
(warnings only: SHARED_HEADER_AVAILABLE CSpawnedFuncBase, two RELOC_NAME label disagreements listed under Label corrections).
Every string literal was paired with retail by hand (`rel.py`: relocation offset -> retail target, `$SG`/`??_C` contents
compared; 0 mismatches) because the verifier skips string relocations. Rows 00e04bd0, 00e2ad50, 00f08590 and the parked
00e29b20 were drafted by the interrupted previous agent; I re-graded, re-checked names and wrote all tests. 00e9c4b0 and
00ee6e90 are new. Every row's first/last instruction checked (entry `sub esp`/`push`, exit `ret` / `ret 4` / `ret 0Ch`;
the full-length match proves the extent).

## What matched (5)
- **0x00E04BD0 NScript::CQ_TraderEscortScript::CDarkwoodTrader::Init()** (826) -- reset BrainState/AI states (brain state 1
  when Me's data string is "INFECTED"), Infected, the incubation / follow-comment / banter (60) timers and three flags; green
  mini-map orb, persistent, not following through teleporters; narrator INFECTED_TRADER / SCARED_TRADER / FRIENDLY_TRADER from
  the data string; spawn the parent's WatchForPickpocketing on a copy of Me as "ParentClass." + "WatchForPickpocketing".
  Shape: inline CCharString::operator==(const char*) (strcmp intrinsic on the null arm, protected static Compare otherwise),
  the landed CAT_Sentry::Init CSpawnedFunc1 block (`__forceinline` ctor, InlineCopy of Me, out-of-line ~CScriptThing).
- **0x00E2AD50 NScript::CV_AmbushScamScript::OnPersist(CPersistContext&)** (819) -- 32 CPersistContext::Transfer calls
  (bool 0x004045C0 / float 0x00410620 / long 0x00410BE0) under each member's own name, zero defaults built in the dead slot.
- **0x00E9C4B0 NScript::CV_IntroductionToTrophiesScript::Main()** (926) -- bind BriarRose / TrophiesIntroWatcher /
  TrophiesBriarTrophy / TrophiesIntroTable, spawn WatchForInsideTavern / WatchForCrowdVolume / WatchForQuestComplete, wait for
  QuestComplete (+0x56), ActivateQuest("V_TalentlessBard"), DeactivateQuestLater(active quest, 0). First probe on the landed
  CV_RockTrollFirstEncounterScript::Main 0x00EC3D40 model (the conditional-destruction flag bits for the binding-name
  temporaries come out by themselves).
- **0x00EE6E90 NScript::CV_TrophyDealerScript::Main()** (767) -- SetCreatureGeneratorsEnabled("Witchwood2", false), bind
  TrophyDealerInCave / DemonDoorFace, ActivateQuestWithoutLoadingResources("V_SingingStones"), spawn HilightGuildTeleporter,
  wait for Witchwood1, SetQuestCardObjective(active quest, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02", "Witchwood2", ""),
  KickOffQuestStartScreen("V_TrophyDealer", true, false). First probe, same model; `while (!IsRegionLoaded(..))` gives the
  retail `neg/sbb/inc` bool.
- **0x00F08590 NScript::CQ_AmbushTradersScript::WatchForEnemyDeath(CScriptThing)** (839) -- keep the enemy's def name, each
  frame ask the enemy MsgWhoKilledMe; when a killer exists stop, and if it is SCRIPT_NAME_HERO call EntityCountDecrease(def).
  Shape: the landed WatchForAssassinDead 0x00E39140 model (`__forceinline` ~CScriptThing, `for(;;)` with the terminate test
  first, `#pragma inline_depth(0)` around the one return where retail calls 0x004AA840).

## Names and evidence (two sources each; spellings confirmed with objdump -t)
- 0x00E04BD0 `?Init@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@EAEXXZ`: slot 2 (Init) of entity vtable 0x012DF424
  (slot 5 is the landed CDarkwoodTrader::OnPredicateFail 0x00E01760); Ego_r 0x00E47950 / FableWin 0x038CC5D0; Ego_r layout
  (ParentClass +0x14 .. RegulateBanterComment +0x3C, no STL members, 0x40 bytes) matches every store.
- 0x00E29B20 `?Main@CQS_ScytheInfoScript@NScript@@UAEXXZ` (parked): slot 2 of quest vtable 0x012E22C8 (slot 1 the landed
  RegisterMain 0x00E29A80); Ego_r 0x00E76400 / FableWin 0x0391FDB0; binds the landed CScytheMarker::Alloc 0x00E29E80 and
  CScytheNearOracle::Alloc 0x00E2A020 by their name strings.
- 0x00E2AD50 `?OnPersist@CV_AmbushScamScript@NScript@@UAEXAAVCPersistContext@@@Z`: slot 4 of quest vtable 0x012E2428 (slot 1
  landed RegisterMain 0x00E2AC90, slot 3 landed Init 0x00E2ABC0); Ego_r 0x00E7B7E0 / FableWin 0x03921E90; every persisted name
  string is the Ego_r member at that offset - 0x14.
- 0x00E9C4B0 `?Main@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ`: slot 2 of quest vtable 0x012ED558 (slot 1 landed
  RegisterMain 0x00E9C410, slot 3 landed Init 0x00E9C3B0); Ego_r 0x00F05240; QuestComplete = Ego_r +0x6A - 0x14. Spawned members
  WatchForQuestComplete 0x00E9C850 / WatchForCrowdVolume 0x00E9C8D0 / WatchForInsideTavern 0x00E9CB20 (PDB QAEXXZ, each paired
  with its own string; the last is landed under that name).
- 0x00EE6E90 `?Main@CV_TrophyDealerScript@NScript@@UAEXXZ`: slot 2 of quest vtable 0x012F51F4 (slot 1 landed RegisterMain
  0x00EE6DF0, slot 3 landed Init 0x00EE6D60); Ego_r 0x00F5FA70. Allocs 0x00EE7340 / 0x00EE7430 landed under the PDB names.
- 0x00F08590 `?WatchForEnemyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z`: Ego_r 0x00F87410 / FableWin
  0x0362FB70; six spawners (incl. the landed CAT_Sentry::Init 0x00F08E90) store 0x00F08590 in a CSpawnedFunc1 named
  "WatchForEnemyDeath"; `ret 0Ch` = this + by-value CScriptThing.
- Callees (all bound spellings reused; new ones PDB-spelled): EntityCountDecrease 0x00F088E0
  `?EntityCountDecrease@CQ_AmbushTradersScript@NScript@@QAEXVCCharString@@@Z` (the only CQ_AmbushTradersScript member taking a
  by-value CCharString; called with the enemy's def name; its body compares def-name strings); CGSI slots from
  ghidra_out/cgsi_slots_aligned.tsv (278 ActivateQuestWithoutLoadingResources, 309 KickOffQuestStartScreen(const CCharString&,
  bool, bool), 558 SetCreatureGeneratorsEnabled, 276 ActivateQuest, 296 SetQuestCardObjective ...).
- Descriptive: `g_pGameScriptInterface` (0x0143E8F8, the CTimer service in 00e04bd0 / 00e29b20) -- the PDB name sets carry no
  data symbols and no static accessor; type proven CGameScriptInterface* (RegisterTimer/DeregisterTimer/SetTimer/GetTimer are
  its vtable slots 87-90); the spelling is the one landed CTimer rows already bind. CGSI / CScriptThing padding virtuals
  (gNNN / tNNN / vNNN) are slot fillers, not emitted.

## Parked for Fable 5.1 (1)
- 00e29b20 NScript::CQS_ScytheInfoScript::Main DIFFER(856v846): retail keeps the edx-register `DeregisterTimer + ret` copy inline right after the post-region-loop IsActiveThreadTerminating test (`je C ; B ; C:`, the in-loop and mission-loop terminates jump back into it); ours emits `jne B` and places that copy after the function-end copy, plus a `jmp` over the mission loop's alignment pad (all other instructions identical, relocation-masked) | tried: mission wait as while / for(;;) / do-while / if+do-while / for(;;){...break;}, post-check as if-return, if-else, `if (!IATT()) {rest}`, `do{}while(0)`, `while(!IATT())` folding the final block, region loop as for(;;)+break and with the post-check inside its exit arm, inner block scopes for the timer and the tail, CTimer::operator== (PDB ??8CTimer@@QBE_NJ@Z), `const long&` handle accessors in GetTime / dtor / operator=, `__forceinline` and out-of-class inline ~CTimer, explicit trailing `return;`, QFE 4035 compiler (~30 compile-only probes, none moved the block) | next: find a landed or retail sibling with an inline-dtor CTimer local whose post-loop return block sits inline (e.g. other CQS_* / CQ_* Main rows near 0x00E29xxx) and copy its statement form; possibly an inline helper (WAIT_UNTIL-style macro body) or a `goto`-free form that makes B2 the cross-jump survivor; else lane gap: cross-jump survivor choice.

## Skipped
- none.

## Label corrections
| Address | Manifest / catalog label | Correct name |
|---|---|---|
| 0x00E04BD0 | Script_Trader_Infection_State | NScript::CQ_TraderEscortScript::CDarkwoodTrader::Init |
| 0x00E29B20 | sub_00e29b20 | NScript::CQS_ScytheInfoScript::Main |
| 0x00E2AD50 | sub_00e2ad50 | NScript::CV_AmbushScamScript::OnPersist |
| 0x00E9C4B0 | sub_00e9c4b0 | NScript::CV_IntroductionToTrophiesScript::Main |
| 0x00EE6E90 | sub_00ee6e90 | NScript::CV_TrophyDealerScript::Main |
| 0x00F08590 | Script_Hero_Morphs_Thread | NScript::CQ_AmbushTradersScript::WatchForEnemyDeath |
| 0x00F088E0 (callee) | CScriptBase_AmbushTradersFilter (vland RELOC_NAME) | NScript::CQ_AmbushTradersScript::EntityCountDecrease |
| 0x00CB8930 (callee) | CTCVillage::OnInitialActivate (Ghidra label, vland RELOC_NAME) | CScriptBase::PostAddScriptedEntities (the bound spelling) |
| 0x00EE71A0 (landed) | CV_TrophyDealerScript::HilightGuildTeleporter | NScript::CV_TrophyDealerScript::HilightGuildTeleporter (NAME_FIXES.tsv) |

## Header debt
- engine/CSpawnedFuncBase.h, engine/CScriptThing.h and engine/CGameScriptInterface.h are flattened data views without the
  vtables these rows call through, so every row models them locally (as the landed chunk 570/571 siblings do); vland warns
  SHARED_HEADER_AVAILABLE CSpawnedFuncBase for 00e04bd0 / 00e9c4b0 / 00ee6e90.
- No engine headers exist for CScriptBase, CEntityScriptBase, CTimer, CPersistContext or the five script classes; all are local
  models at retail offsets (Ego_r.pdb - 0x14 for the quest scripts; CDarkwoodTrader has no shift).
