# Agent NF353 (Account A naming queue, 2026-11-24): the map<CCharString,STB> operator[] family port (T3 nf340 0x00CD3D2E)

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full, especially the
NF340 entry's scope line on drop-test vs move rules. Lock name: NF353. Folder: $S/w22/agentNF353/.

Read first:
- batches/2026-11-24_landing_nf350_b1/README.md (b1 and b2) and pending/2026-11-24_nf350_handback/ (NOTES, evidence/census_emitters_00cd3d2e.tsv,
  droptest_*, drop_census_*). Tools are in tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf350/ and .../nf347/.
- batches/2026-11-24_landing_nf347_b1/README.md: the pilot ruling and the resolve test.
- T3_PORT_QUEUE.tsv lines nf340 0x00CD3D2E and nf350 0x00CD3CAE.

## Scope
The owners 0x00CD3D2E / 0x00CD3CAE now define the 4-argument STLport symbols. NF350's census found 70 emitters that define the exact
4-argument `??A` through stlport/fable_map.h under FABLE_RETAIL_STLP_MAP and pass the resolve test. Three of them have landed already:
0x00D58490, 0x00EA59E0, 0x00E8EF80. Port the rest.

For each row, add `#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY` before the includes, under a `// Fidelity: scaffolding, not retail`
comment that gives the reason. Then prove all of the following per row:
- grade unchanged (vland HEAD v new);
- leaf identical (leafrel.py by section name, from agent_tools/nf349; do not trust index-keyed LEAF flags);
- both pick-any checkers HEAD v new: no copy turned NEQ and no new NEQ copy;
- the resolve test: the new `??A` reference resolves to the landed owner.

The drop test must count definers OUTSIDE the batch. Every row in a batch drops the same symbols, so for every dropped EQ copy with a
retail address, count the landed definers that are NOT in any of your batches (simulate all your batches landed).
- The risk is `??8_Rb_tree_base_iterator@std@@QBE_NABU01@@Z` 0x0041C820: its owner row defines a stand-in (IsEquivalent).
- Also check the STB copy ctor 0x00905330 and anything else EQ at a retail address.
- If porting a batch would leave such a copy with no EQ definer at all, stop that batch and report the count.

Rows whose leaf changes, whose grade changes, or where something turns NEQ are STATUS skipped, with the measurement. /d2inl-flagged
rows (S15 and the T flags) are included only if they pass; NF343 found that S15 rows keep a NEQ copy.

Exclude these rows:
- Account B's claimed or staged rows (the 4 rows NF350 did not compile);
- 0x00626750, which belongs to agent NF352;
- Codex reservations;
- T3 column-2 holes, which you must list.

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad. In every Bash call:
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`. Run `mkdir -p /run/user/0` if missing.
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe".
- Checkers:
  - rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I);
  - tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py, with env FABLE_EXE and FABLE_ROOT=/home/user/FableDecomp set BEFORE
    any run, plus --row-length.
- Never run two compile runners on one row path.

## Hand-back
land_scripted run dirs, batches/bN/ (at most 25 rows each). Each contains:
- rows/, RESULTS.tsv, head/payload.json, orig/rows + orig/tests, rows.txt;
- evidence/pk_gate, plus pk_gate.txt and a reversed control.

Also hand back NOTES.md, STATUS.tsv (one line per candidate) and evidence/ (with the cross-batch drop census).

Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS before the first compile. Stay off agent NF352's locks. Never write into the
repo, never run git, and run long jobs in the background.
