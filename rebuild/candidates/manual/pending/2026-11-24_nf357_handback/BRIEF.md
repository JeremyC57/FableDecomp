# Agent NF357 (Account A naming queue, 2026-11-24): the four GetSubDef<T> pick-any debt copies

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full; every ruling at its end
applies (NF340 gate incl. the NF352 reach-flip rule and Q1b-Q3, NF348 D1/D2). Lock name NF357. Folder $S/w22/agentNF357/.

## Scope (T3 nf340 lines; audits/2026-11-24_pk_all_nf340/DEBT_BY_COPY.tsv)
| Copy (retail) | Symbol | Emitting row |
|---|---|---|
| 0x005BADE1 | ??$GetSubDef@VCInventoryItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCInventoryItemDef@@@@@Z | 0x005E4BE0 |
| 0x0042B1A6 | ??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z | 0x0076EB00 |
| 0x0065F604 | ??$GetSubDef@VCEntitySoundDef@@@... | 0x00780D80 |
| 0x0042B0A2 | ??$GetSubDef@VCAppearanceDef@@@... | 0x00667B30 |
Recount on today's HEAD first (both checkers, --leaf): the list may have moved since the audit, and other landed rows may emit the same
copies now.

Prior art to read (do not repeat): the GetDef<T> work: batches 2026-11-24_landing_nf341_r2, nf346_*, nf349_* (READMEs) and
pending/2026-11-24_nf341_handback, nf346, nf349 (NOTES). GetDef copies were fixed by FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
(engine/CDefPointer.h) or by NF349's per-type size-body opt-in in engine/CDefinitionManager.h. engine/CDefClassBase.h declares GetSubDef
(line ~71) and carries its own opt-ins. T3 lines nf306 0x0042B0A2 and nf313 0x0065F604 hold earlier findings on two of the owners.

Per copy:
1. Find the retail owner (the landed row at that address: its source, flags, pragmas, how it defines the symbol) and why the emitting
   row's copy differs (objsec / disassembly against retail).
2. Find the minimal fix: an existing opt-in first (REREAD, size-mode, an existing CDefClassBase.h opt-in). A new add-only header opt-in is
   allowed only in the per-type size-body form of NF349 / NF352, only where the owner sets optimize("s", on) with no CompilerFlags entry,
   and only with the includer-closure proof (lane_merge/equiv_check.py old v new over the catalog: 0 differing rows other than the rows that
   define the macro, plus a planted control that flags the real includer set).
3. Prove per emitting row: grade unchanged (vland --recheck-landed HEAD v new), leaf identical (objsec/leafrel), both checkers HEAD v new
   with --leaf: the target copy NEQ -> EQ, nothing turned NEQ, no new NEQ, no reach flip n -> y onto a NEQ copy; drop test on any dropped
   symbol.
A copy whose fix needs a forcing device or changes the row's bytes is STATUS skipped with the measurement. No second device.

## Paths
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; every Bash call:
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`; `mkdir -p /run/user/0` if missing.
- Retail exe "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget 3 GB.
- Checkers: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I; args: exe
  rebuild/oracles/reloc_bindings.tsv obj addr) and tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py (env FABLE_EXE,
  FABLE_ROOT=/home/user/FableDecomp; --row-length, --leaf). In awk, compare addresses as strings (`$1""==a""`; GOTCHAS 2026-11-24).
- Agent tools: tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf341..nf354.

## Hand-back
land_scripted run dir(s) under batches/bN/: rows/, RESULTS.tsv (ACCEPT only when every gate passes), head/payload.json,
head/HEAD_FROM_REPO.txt (addresses only, one per line, or omit it), orig/rows + orig/tests, tests/ if changed, hdr/engine/ +
orig/hdr_<name> + HEADERS.tsv if a header changes, TABLE_CHANGES.tsv, pk_gate.txt + reversed control. Plus NOTES.md, STATUS.tsv,
evidence/. Keep oracle files with retail bytes out of the hand-back.

Rules: read COORDINATION.md, ASM_RULES, EXTRA_RULES, GOTCHAS first. Lock each row you compile ($S/w22/nflocks/<addr>/owner = NF357),
release all before hand-back. Stay off Account B's claims / staged rows, Codex reservations, and the locks of NF355 (CThingNoiseDef) and
NF356 (owner ports 0x0042AF3C, 0x0042B040, 0x0042B312, 0x005F81BE, 0x00429C6F, 0x004291DE). Never write into the repo, never run git.
Long jobs in the background.
