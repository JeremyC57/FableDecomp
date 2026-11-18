# 2026-11-17 landing nf87: name-fix round 7, NF87 (54 rows: the nf85 follow-ups, q11121..q11149)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF87 took the 29 entries queued at the nf85 landing (25 landed rows that still bound NF85's addresses under stand-ins, 0x008AA480 and
0x005A45F6). Brief, list and records: `naming/2026-11-17_agentNF87/`.

## Rows (STATUS.tsv, 91 = 83 locked + 8 claimed through the lock): 54 ported, 34 not-a-binder, 3 blocked
- **Self renames (42), among them:** 0x009A8150 `CBankFileManager::SetBankAlias` (was `CStringTable_009a8150::SetEntry`; Ego_r and FableWin give the
  member, Ego_d the parameter names), 0x006223C0 `CTCInventoryQuests::ProcessQuestActivateAction` (was `vfunc_70`), 0x005BC4C4
  `NInventory::CTCInventoryBase::GetBuildingThing` (was `_global::GetMember_dword0xb8_005bc4c4`), the CVectorMap `Sort` / `Update` of
  `<CCharString, CSoundBankLH *>` and `<CCharString, C3DVector>` (were `_global::CVectorMap_*` stand-ins), the STLport `sort` / `sort_heap` /
  `__partial_sort` / `__make_heap` / `__pop_heap` families of the CCharString-keyed vectors (each now an explicit STLport instantiation), tree
  iterator `operator++` / `operator--` rows (were `CRetailTreeIterator::...`), and the CGameScriptInterface gossip accessors.
- **Script rows:** five `operator[]` binders of 0x009AC700 moved onto shared headers with STLport maps (0x00D7AE80, 0x00E6DA80, 0x00ED0B10,
  0x00E57020, 0x009EB0D0); three took their nested or private PDB names.
- **0x00EE8870 NScript::CCS_OakValeRevisitedScript::OakValeFire** is on the shared OakVale script header. Retail inlines the implicit
  `CScriptThing::operator=` at one site, which VC7.1 does from `/d2inlT76`: the row carries `// Flags: /d2inlT76` and an `// Inline-budget:`
  line (owner's 2026-11-08 inline-budget rule; 76 is the smallest value tested). Coordinator correction: module / name split like its siblings.
- **0x005A45F6** is on the retail branch of CThingPlayerCreature.h (q11149). **0x008AA480** (q11148) rechecked and ported.
- **Blocked:** 0x004B4770 `~CQuestManager` (CQuestManager.h pads the members the implicit destruction needs), 0x006BC510 `CRegion::LoadBinary`
  (DIFFER 1386 v 1480), 0x0071D3B0 `UpdateTattoos` (CTCHeroMorph.h has no layout).

## Entries (APPLIED.tsv, 29): 26 done, 3 blocked (q11122, q11127, q11128); REJECTED.tsv: q11135 / 0x008AE510

## Rows and checks
- **Section 1: 42 self renames (58 file moves).** Modules match their siblings' catalog spellings.
- **Headers (13 opt-ins in 9 files):** stlport/fable_containers.h, engine/CTCAIScratchpad.h, engine/CTCInventoryQuests.h, engine/CTCHeroCentre.h (8
  asserts pinned), engine/CCS_OakValeRevisitedScript.h (1 pinned), engine/CQuestManager.h, engine/CVectorMap.h, engine/NInventory_CTCInventoryBase.h,
  fable_retail_banks.h; details in `naming/2026-11-17_agentNF87/hdr/README.md`. With the opt-ins removed, all 56 row files preprocess
  identically with and without the overlays (`tokid2.log`); `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 54 rows graded from the repo after landing: 50 RELOCATION_MATCH and 4 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 82 binding lines and added 42; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 14)
NF87's NAME_FIXES.tsv, among them the landed `__introsort_loop` rows 0x00A35490 / 0x00A73910 / 0x00A73A00, which still bind the old
`__partial_sort` spellings and need a whole-family port.

## Rule slips
- Two Bash calls exited 1 (a false `[ -n ]` test; a helper run outside the repo directory).
- 0x00EE8870 used 16 grades against the 15-grade budget; its assert pin came after its first grade (no byte change).
- STATUS was written late for 0x009EB0D0 and 0x00A35260.
