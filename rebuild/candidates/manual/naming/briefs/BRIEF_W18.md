# Naming pass, wave 18 (one agent beside two chunk agents on the lane)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it** -- a proven wrong spelling in a landed source is respelled in this wave, not held. Resolved decisions:
`naming/2026-11-01_wave17/DECISIONS_W17_RESOLVED.tsv` (+ waves 13-16); canonical globals / members in `naming/ESTABLISHED_SPELLINGS.tsv`
(changed 2026-11-01: 0x013B86A0 `?PMainGameComponent@CMainGameComponent@@1PAV1@A`, 0x0143E8F8 `?pDSTGame@@3PAVCGameScriptInterfaceBase@@A`,
0x0143E90C the const `$$CB` CDefPointer form -- these are no longer gated: respell their users). Rules, mechanics and hand-back as
`briefs/BRIEF_W17.md` / `BRIEF_W16.md` (read both, the wave 17 README + AGENT_README, DEFERRED.tsv, QUEUE_W17.tsv and the wave 17 `scripts/` --
compose17.py, fam_q17.py, hand17/ -- reused as compose18.py etc.). RENAMES.tsv paths repo-rooted; new_name the catalog's bare spelling.
Labels the chunks proved are in `ghidra_out/labels_chunk678..683_*.tsv` and `labels_chatgpt_pass10_2026-11-01.tsv`.

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family)
1. **Wave 17 DEFERRED.tsv** (`$S/naming/w18/DEFERRED_W17_INPUT.tsv`, 397 rows) and the partial/deferred rows of `QUEUE_W17_outcomes.tsv`:
   the four [recheck-fail] rows (0x00401F30: keep the Close call non-virtual -- find the model that spells the name without the vtable call;
   0x006A2520 / 0x009E1D00 / 0x009E1D50: fix the tests' stand-in names, then respell), the STL stand-in callers (00ad3a10, 00514c10) and the
   placeholder-class own rows (007acc50, 00579435, 00b2ef20, 00b2ea90, 009c2220, 004e34b2, 00b7fd00, 00b7ffc0 with std::__false_type, 00caf680),
   PeekThingSearchTools 008fd000 (31 rows) / PeekTypeList 0049c780 / 00947780 on const or std::list models, HasTC 006a80a0 with reference-taking
   filter templates, the 13 GetPWorldSeed stand-in-class rows.
2. **The c678 / c681 own rows that need re-authoring** (`QUEUE_W18_input.tsv`, c678/c681 rows; byte-identical results only): the seven
   quest-script ctors on `(CScriptDataBase*, CGameScriptInterfaceBase&)` (0x00E7EF70 nested CFishCompOrganiser, 0x00E1DB00, 0x00E755F0,
   0x00D9E210, 0x00CF88C0, 0x00DE0320, 0x00E398F0), Alloc 0x00CF91C0, 0x00DEC7F0, and 0x00994B40 `CCreatureActionBase_GetFramesRemaining_00994b40.cpp`
   -> `CMemoryDataOutputStream::GetLength` const (file rename + ps1 + the 56 callers' spellings, DECISIONS_W17 row); 0x0074EE90's
   `g_pMainGameComponent` -> `CMainGameComponent::PMainGameComponent` (established).
3. **PMainGameComponent 013b86a0 and the PClass singleton 013b89fc** (~60 rows: `?PClass@?$CTBaseSingletonNoDefaultAlloc@V<T>@@@@2V?$CCountedPointer@V<T>@@@@A`
   per T; CQuestManager's is private `0V` per two raw-PDB readings -- the landed 005fd410 binds `2V`, respell it), then the destructor families
   (0099a300 / 0099a2e0 / 0099a410 / 009fc550 / 00aa75c0 / 00f35b40 / 00cbd510) where the bytes hold.
4. **The remaining cg10 targets** (119 of 155 not driven in wave 17; `CHATGPT_NAMING_QUEUE.tsv` lists the 1,236 stand-in spellings at 170
   addresses with their binders) and then the mega families, biggest first: ConstantFPS 0x01375550, `bitset::test`, `CBasicString::Compare`,
   GetPItem, ~CCharString, GetSubDef, GetInstance, `Transfer<>`, AddEvent.
The coordinator appends c680 / c682 / c683 rows to QUEUE_W18_input.tsv as those chunks land -- re-read it before each scope step.
Not in this wave: byte-moving re-authoring beyond scope 2, 0x00C28690 (libpk name unknown), VIRTUAL_SCREEN_* (gated), the engine header
changes (migration wave 10). 0x00440730 stays on HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W18.tsv (every queue row with its outcome), the per-family rows
files, logs and scripts, as wave 17 did; leave no lv_* directory and no background job.
