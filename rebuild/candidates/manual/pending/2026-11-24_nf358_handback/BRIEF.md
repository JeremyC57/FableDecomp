# Agent NF358 (Account A, 2026-11-24): retail-caller census for three pick-any copy lines closed today (read-only audit)

Lock name NF358 (only if you compile). Folder $S/w22/agentNF358/.

The T3 lines below were closed today on DEBT_BY_COPY.tsv (rebuild/candidates/manual/naming/audits/2026-11-24_pk_all_nf340/), but that audit
SAMPLED 300 non-/d2inl rows (SUMMARY.md line 15), so its per-copy emitter list is a floor, not a count (NF357 found 34 emitters of a copy
the audit listed with 1). Re-establish each with a full census:
| Copy | Symbol | Closed by |
|---|---|---|
| 0x00429C6F | ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z | batches/2026-11-24_landing_nf352_r1 |
| 0x0042B312 | ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z | batches/2026-11-24_landing_nf356_b1 (T3_CLOSED) |
| 0x0042B040 | ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z | same |
Method (NF354's): scan retail for every call / jmp rel32 to the copy's address, map each caller address to its catalog row
(rebuild/oracles/auto-re-candidates.tsv, rebuild/build_candidates.ps1), keep the landed (compiled) rows, compile each read-only and run both
checkers with --leaf: is the copy present, EQ / NEQ, leaf-reached? Also grep landed sources for rows that emit the copy without retail calling
it (a same-TU copy). Hand back CENSUS_<addr>.tsv per copy (row, compiled?, copy present, verdict, leafref, reach) and a verdict per line:
CLOSE CONFIRMED or REOPEN with the NEQ emitters. No batches, no fixes.

## Paths and rules (all NF agents)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; every Bash call: `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`; `mkdir -p /run/user/0` if missing.
- Retail exe "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget 3 GB.
- Checkers: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I; args: exe rebuild/oracles/reloc_bindings.tsv obj addr) and tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py (env FABLE_EXE, FABLE_ROOT=/home/user/FableDecomp; ALWAYS --row-length and --leaf=<row // Leaf:>). Neither follows vftables; a copy with no table line reads NOADDR: compare such copies with retail directly (tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf355/retcmp_coordinator.py + objcmp.py). In awk compare addresses as strings (`$1""==a""`).
- vland payloads: set each entry's `leaf` to the row's // Leaf: symbol (recheck.py reads it from the source).
- Read COORDINATION.md, ASM_RULES, EXTRA_RULES, GOTCHAS and BRIEF_NF_BATCH.md (rulings at its end, incl. NF352 reach flips and the NF356 rule: an owner port onto a shared header is a MOVE, gated on the COMDAT set staying the same) before the first compile.
- Lock each row you compile ($S/w22/nflocks/<addr>/owner = your lock name), release all before hand-back. Stay off Account B claims / staged rows, Codex reservations, other agents' locks. Never write into the repo, never run git (not even read-only). Long jobs in the background. Keep oracle files with retail bytes out of the hand-back.
- Hand-back: land_scripted run dirs under batches/bN/ (rows/, RESULTS.tsv with ACCEPT only when every gate passes, head/payload.json, orig/rows + orig/tests, tests/ if changed, hdr/engine + orig/hdr_<name> + HEADERS.tsv if a header changes, TABLE_CHANGES.tsv, RENAMES.tsv, NAME_FIXES.tsv, pk_gate.txt + reversed control); do NOT write a prose head/HEAD_FROM_REPO.txt (addresses only, or omit). Plus NOTES.md, STATUS.tsv, evidence/.
