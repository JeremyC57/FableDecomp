# Agent NF359 (Account A, 2026-11-24): the CThingNoiseDef def TU follow-ups (T3 nf355 lines)

Lock name NF359. Folder $S/w22/agentNF359/. Read first: batches/2026-11-24_landing_nf355_u1/README.md, pending/2026-11-24_nf355_handback/
(NOTES.md, u1_agent/), the new headers engine/CThingNoiseDef.h, CThingComponentSet.h and the opt-ins in CThingBaseDef.h / CTCNoiseDef.h.

Scope (T3_PORT_QUEUE lines nf355):
1. **Constructor ports (PRIORITY):** 0x00455EF0 ??0CThingNoiseDef@@QAE@XZ and 0x0044C1BB ??0CTCNoiseDef@@QAE@XZ are landed STRONG on
   stand-in models with one-slot vftables; the 0x0045C5F6 object now emits byte-identical pick-any copies, so the link has an LNK2005. Port
   both onto the shared headers so each emits the retail body with the full vftable. Note the NF356 rule: an owner port is a move gated on
   the COMDAT set; measure exactly what the set change is (one-slot -> full vftable, selection 1 -> 2) and report it; do not invent devices.
2. **0x0045C5C1 Alloc and 0x0045C5DA's self name** (??_GCThingNoiseDef@@UAEPAXI@Z; today the stand-in
   ?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z): both come from the def TU of row 0x0045C5F6, which ALREADY defines Alloc strongly.
   First check whether the precedent's Alloc 0x00432C40 (CUIMiscThingsDef, defined by landed row 0x004317D0) has its own landed row and how
   it is covered; propose the same form for 0x0045C5C1 (a new row must not define Alloc a second time). For 0x0045C5DA, respell its self onto
   ??_GCThingNoiseDef (Ego_r + FableWin publics; RTTI slot 0) if a non-forcing text emits it; measure.
Gates per row: grade not worse HEAD v new; objsec leaf equal; both checkers with --leaf (no new NEQ, no NEQ reach flip); retail compare of
every emitted function with a retail home; drop test; whole-catalog equiv_check (lane_merge/equiv_check.py) for any header change, with a
planted control.

## Paths and rules (all NF agents)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; every Bash call: `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`; `mkdir -p /run/user/0` if missing.
- Retail exe "$S/dl/game/Fable The Lost Chapters/Fable.exe". Disk budget 3 GB.
- Checkers: rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I; args: exe rebuild/oracles/reloc_bindings.tsv obj addr) and tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py (env FABLE_EXE, FABLE_ROOT=/home/user/FableDecomp; ALWAYS --row-length and --leaf=<row // Leaf:>). Neither follows vftables; a copy with no table line reads NOADDR: compare such copies with retail directly (tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf355/retcmp_coordinator.py + objcmp.py). In awk compare addresses as strings (`$1""==a""`).
- vland payloads: set each entry's `leaf` to the row's // Leaf: symbol (recheck.py reads it from the source).
- Read COORDINATION.md, ASM_RULES, EXTRA_RULES, GOTCHAS and BRIEF_NF_BATCH.md (rulings at its end, incl. NF352 reach flips and the NF356 rule: an owner port onto a shared header is a MOVE, gated on the COMDAT set staying the same) before the first compile.
- Lock each row you compile ($S/w22/nflocks/<addr>/owner = your lock name), release all before hand-back. Stay off Account B claims / staged rows, Codex reservations, other agents' locks. Never write into the repo, never run git (not even read-only). Long jobs in the background. Keep oracle files with retail bytes out of the hand-back.
- Hand-back: land_scripted run dirs under batches/bN/ (rows/, RESULTS.tsv with ACCEPT only when every gate passes, head/payload.json, orig/rows + orig/tests, tests/ if changed, hdr/engine + orig/hdr_<name> + HEADERS.tsv if a header changes, TABLE_CHANGES.tsv, RENAMES.tsv, NAME_FIXES.tsv, pk_gate.txt + reversed control); do NOT write a prose head/HEAD_FROM_REPO.txt (addresses only, or omit). Plus NOTES.md, STATUS.tsv, evidence/.
