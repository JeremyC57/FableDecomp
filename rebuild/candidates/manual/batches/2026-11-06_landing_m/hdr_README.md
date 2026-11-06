# Chunk 812 header overlay

All four headers were copied fresh from `rebuild/include/engine/` and diffed against the repo copy again right before hand-back
(the repo copies had not changed). Every edit is an **additive opt-in**: new text only, inside a new `#ifdef FABLE_RETAIL_...` block;
no existing line was changed or reflowed, so the default path is textually identical. No landed source defines any of these macros
(grep of `rebuild/`), so the landed includers listed in SIBLINGS.tsv cannot change; they were not regraded.

| Header | Opt-in macro | What it adds | Evidence | Used by |
|---|---|---|---|---|
| engine/CQ_ArenaHoldingScriptScript.h | `FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_FANMARKER` | `class CFanMarker : CEntityScriptBase` (Alloc 0x00CF4DF0, GetParentScript 0x00CF4E90, private Main 0x00CF4EC0; ParentClass +0x14, PMasterData +0x18) + size assert 0x1C | Ego_r.pdb layout (28 bytes); vtable 0x012C657C stored by the Alloc; Ego_r + FableWin `?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ` | differ/00cf4ec0.cpp |
| engine/CQ_BanditCampBossBattleScript.h | `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GATE2GUARD1` | `OpenGate(float, CCharString)` 0x00D0EBC0; `class CGate2Guard1` (Alloc 0x00D071F0, GetParentScript, Main 0x00D0D910, Init 0x00D072C0; ParentClass/PMasterData/AIState) + size assert 0x20 | Ego_r.pdb layout (32 bytes); vtable 0x012C8E80; Ego_r + FableWin names incl. `?OpenGate@CQ_BanditCampBossBattleScript@NScript@@QAEXMVCCharString@@@Z`; functor store 0x00D0E495 / SuspendableProcess 0x00D0EBA0 | differ/00d0d910.cpp |
| engine/CQ_BanditCampBossBattleScript.h | `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CAMPHOSTAGEGUARD` | `class CCampHostageGuard` (Alloc 0x00D08E50, GetParentScript, Main 0x00D09010, Init 0x00D08F00, OnPredicateFail 0x00D08FB0; ParentClass/PMasterData/DropPass) + size assert 0x20 | Ego_r.pdb layout (32 bytes); vtable 0x012C9130 (7 slots); Ego_r + FableWin names | differ/00d09010.cpp |
| engine/CQ_BountyHuntScript.h | `FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_MAIN` | in TeleportData: `TeleportData()` 0x00D15140 (only when chunk 797's GANGDEPUTY2 is not defined, which already declares it) and `operator=` 0x00D15170; in the class: WatchForQuestFailure 0x00D15090, WatchForQuestSuccess 0x00D150F0, WatchForEnableFisherCreekHutDoor 0x00D151D0, WatchForAllGruntsKilled 0x00D17340, WatchForBoasts 0x00D17A80; nested forward declarations CBH_GangGruntSpawn / CBH_GangGrunt / CBH_GangDeputy1 / CBH_Hostage1 / CBH_Hostage2, plus CBH_GangLeader (guarded by `#ifndef ..._GANGLEADER`, chunk 810's pending opt-in) and CBH_GangDeputy2 (guarded by `#ifndef ..._GANGDEPUTY2`, chunk 797) so that combining the macros never redeclares | Ego_r + FableWin `??0`/`??4TeleportData@CQ_BountyHuntScript@NScript@@...`, `?Watch...@CQ_BountyHuntScript@NScript@@QAEXXZ`, `?Alloc@CBH_...@CQ_BountyHuntScript@NScript@@SI...`; call and functor-store targets in Main 0x00D13580 | differ/00d13580.cpp |
| engine/CV_LostTraderScript.h | `FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BROTHER` | `WatchForQuestFailureState(CScriptThing)` 0x00EA7460 | CSpawnedFunc1<CV_LostTraderScript,CScriptThing> vtable 0x012EE360, Func +0x34 = 0x00EA7460; Ego_r + FableWin `?WatchForQuestFailureState@CV_LostTraderScript@NScript@@QAEXVCScriptThing@@@Z` | src/00ea59e0.cpp (RELOCATION_MATCH) |

Only `CV_LostTraderScript.h` is needed by a matched row (00ea59e0). The other three serve the parked rows in differ/; land them with those rows or
now (they are inert for every other includer). The overlay copy is the one used: each row declares classes or members that exist only in the
overlay opt-in (CFanMarker, CGate2Guard1, CCampHostageGuard, WatchForQuestFailureState, the BountyHunt threads), and it compiles only with
FABLE_HDR_OVERLAY set.
