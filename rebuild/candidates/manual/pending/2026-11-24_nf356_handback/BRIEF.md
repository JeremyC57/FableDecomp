# Agent NF356 (Account A naming queue, 2026-11-24): port six owner rows onto their landed size-body opt-ins

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full; every ruling at its end
applies (NF340 gate incl. the NF352 reach-flip rule, NF348 D1/D2, NF351 "no interim made-up names"). Lock name NF356. Folder $S/w22/agentNF356/.

## Scope (T3_PORT_QUEUE.tsv lines nf349 / nf352, "port (owner onto opt-in)")
| Owner | Copy | Opt-in (already landed) |
|---|---|---|
| 0x0042AF3C | CDefinitionManager::GetDef<CThingBaseDef> | engine/CDefinitionManager.h FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGBASEDEF_SIZE_BODY |
| 0x0042B040 | GetDef<CThingObjectDef> | ..._GETDEF_CTHINGOBJECTDEF_SIZE_BODY |
| 0x0042B312 | GetDef<NUISystem::CUIMiscThingsDef> | ..._GETDEF_CUIMISCTHINGSDEF_SIZE_BODY |
| 0x005F81BE | GetDef<CExpressionDef> | ..._GETDEF_CEXPRESSIONDEF_SIZE_BODY |
| 0x00429C6F | CCountedPointer<NUISystem::CComponent>::Reset | engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY |
| 0x004291DE | CCountedPointer<NUISystem::CComponent>::ReleaseData | ..._CCOMPONENT_RELEASEDATA_SIZE_BODY |

Each owner today is a local model / explicit instantiation under its own row-head `#pragma optimize("s",on)` (0x0042AF3C even uses a
self stand-in name `GetDef_0042af3c` with no Leaf line). Goal: the owner row includes the shared header, defines the opt-in, and emits the
retail symbol from the shared specialization (exact PDB-spelled Leaf line), with no local model of the class. Read the opt-in blocks' own
comments first: a row using them must not carry a row-head optimize pragma before the include (VC7.1 has no push/pop; the block restores
command-line settings). Prior art: landed batches 2026-11-24_landing_nf349_* and _nf352_r1 (READMEs), pending/2026-11-24_nf349_handback,
pending/2026-11-24_nf352_handback.

Per owner, prove:
- grade not worse (vland --recheck-landed, HEAD v new); the Leaf symbol is exactly the retail mangled name the pick-any rows reference
  (reloc_bindings line, self_symbols);
- objsec HEAD v new: the leaf COMDAT's bytes and relocations equal; list every other section / symbol / COMDAT-selection change (a strong
  definition becoming a pick-any COMDAT is a change to report, with the link consequence: does any LNK2005 against the pick-any copies go away?);
- both pick-any checkers HEAD v new with --leaf (no copy turned NEQ, no new NEQ, no reach flip onto NEQ);
- drop test for any dropped symbol (reloc_bindings witnesses, references in compiled landed objects, retail-address EQ losses);
- catalog rename if the row's file/function name changes (hand back RENAMES.tsv: address, old base, new base, class), and NAME_FIXES for
  any stand-in symbol the port retires (0x0042AF3C's GetDef_0042af3c: list every landed binder of it).
An owner whose port changes its leaf bytes or needs a new device is STATUS skipped with the measurement. Do not edit headers.

## Paths
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; every Bash call:
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`; `mkdir -p /run/user/0` if missing.
- Retail exe "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget 3 GB.
- Checkers: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I; args: exe reloc_bindings.tsv obj addr)
  and tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py (env FABLE_EXE, FABLE_ROOT=/home/user/FableDecomp; --row-length, --leaf).
- Agent tools: tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf341..nf352 (nf352/reachflip_coordinator.py).

## Hand-back
land_scripted run dir(s) under batches/bN/: rows/, RESULTS.tsv (ACCEPT only when every gate passes), head/payload.json,
head/HEAD_FROM_REPO.txt, orig/rows + orig/tests, tests/ if changed, TABLE_CHANGES.tsv, RENAMES.tsv, NAME_FIXES.tsv, pk_gate.txt + reversed
control. Plus NOTES.md, STATUS.tsv, evidence/. Do not copy oracle files with retail bytes into the hand-back.

Rules: read COORDINATION.md, ASM_RULES, EXTRA_RULES, GOTCHAS first. Lock each row you compile ($S/w22/nflocks/<addr>/owner = NF356),
release all before hand-back. Stay off Account B's claims / staged rows, Codex reservations, NF355's locks. Never write into the repo, never
run git. Long jobs in the background.
