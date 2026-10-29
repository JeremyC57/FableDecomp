# Chunk 575 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Quest `WatchForTermination` threads are one family (7 rows, 0x00D01290 .. 0x00D04260).** Name each one from its single
  xref: the CSpawnedFunc member-pointer store (`mov [edi+34h], <row>`) right after the "WatchForTermination" string
  0x012C84B0 inside that quest's Main (slot 2 of the quest vtable). The landed CQ_WizardBattleScript::WatchForTermination
  0x00E1AD60 form (`while (!Failed && !Succeeded)`, post-wait thread test, if/else with a thread test per arm, then
  DeactivateQuestLater) matched all seven on the first probe; only the flag the `if` tests, the bool arguments and the
  extra interface calls differ.
- **Read bool arguments push by push.** In `outer(inner(), a, b, c)` VC7.1 pushes c, b, a, then evaluates the inner
  call (its hidden-result `lea/push`), then pushes the result: the FIRST `push imm` before the inner call is the LAST
  argument (0x00D04260: SetQuestAsFailed(name, false, L"", true) is `push 1 ; ... push 0`).
- **`__uninitialized_copy` needs `__forceinline` in a speed-TU `_M_insert_overflow` that inlines both copies (0x00D18CA0,
  0x00F0D4E0).** With plain `inline`, VC7.1 inlines the prefix copy and calls the suffix copy out of line (266v280); the
  out-of-line `_M_clear()` and element copy constructor stay declarations only.
- **A scripted-thing handle local (0x00EFC300):** `{ CScriptGameResourceObjectScriptedThingBase h; ... }` gives the inline
  PImp release, the base vtable store 0x0126008C (inline user `~CScriptGameResourceObjectBase() {}`) and the out-of-line
  `??1CBaseObject@@MAE@XZ`; the ScriptedThingBase itself declares no destructor. A virtual (`UAE`) MoveToPosition is
  called directly on the local; a declared-only virtual (`GetPos`) on a by-value GetThingWithScriptName temporary stays a
  vtable call. Matched on the first grade.

Review at landing:
- No row needed more than 3 grades (00d01290 and 00efc300 regraded after name-only respellings; each grade matched).
- **0x00D18CA0 is bound with the public spelling (QAE)** by the landed push_back 0x00D18C00 (`vectorTeleportData_pushback_00d18c00.cpp`
  declares `_M_insert_overflow` public on a flat invented `vector` with three raw pointers). STLport 4.6 makes it
  protected (IAE), as the twin 0x00F0D4E0 binding already is; this chunk lands IAE, so expect a cross-row RELOC_NAME/
  CONFLICT warning until 0x00D18C00 is respelled (NAME_FIXES.tsv).
- **The landed TeleportData copy constructors 0x00D18C50 and 0x00F0CA60 emit a global `??0TeleportData@@QAE@ABU0@@Z`**
  (global `struct TeleportData`; 0x00F0CA60 even models a 16-byte Dinkumware vector inside a 0x1C retail record). Their
  callers bind the correct nested `??0TeleportData@CQ_<..>Script@NScript@@QAE@ABU012@@Z` (NAME_FIXES.tsv).
- **0x007E72F0 MoveToPosition has four wrong bound spellings** (QAE variants with `MM_N1`, `MJ_N1`, enum `_N2`, and a
  `MoveTo` name); the Ego_r / vtable-slot-4 spelling is the virtual `UAE...MW4EScriptEntityMoveType@@_N2@Z` (NAME_FIXES.tsv).
- Catalog Mains still `_gapscan::sub_*`: 0x00DC27C0 (CQ_OpeningGraveyardSecretPassageScript::Main), 0x00D67890
  (CQ_GuildTrainingWoodsWillScript::Main), 0x00D9A190 (CQ_HobbeToothContestScript::Main); 0x00D038E0 is
  CQ_BanditCampBossBattleScript::Main. All are slot 2 of the vtable their landed ctor/RegisterMain uses.
- Local models next to flattened headers: 0x00EFC300 models CBaseObject locally with a `// Layout:` line (vland warned
  SHARED_HEADER_AVAILABLE before the line was added; the generated header has its vptr as a data member). The quest rows model CScriptBase / CLevelScriptBase /
  CGameScriptInterfaceBase locally (no headers exist), as every landed quest row does.
- Slot 866 of CGameScriptInterface (0x0088FB20) is SetPrizeTavernTable per cgsi_slots_aligned.tsv and the landed row; it is
  called with `true` when the bandit camp boss battle succeeds (0x00D04260), plausibly the prize table of the camp's pub game.
