# Agent NF352 (Account A naming queue, 2026-11-24): size-mode CCountedPointer<NUISystem::CComponent> copies (T3 nf340 0x00429C15 family)

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full, including the NF340
entry with its scope line, and the NF348 entry. Lock name: NF352. Folder: $S/w22/agentNF352/.

## Read first
- rebuild/candidates/manual/batches/2026-11-24_landing_nf349_b1/README.md, plus pending/2026-11-24_nf349_handback/ (NOTES, evidence).
  NF349 proved the working device: a per-type explicit specialization, `template<> __declspec(noinline) inline`, wrapped in
  `optimize("y", off)` / `("s", on)` ... `optimize("", on)`. It is an add-only header opt-in marked `Fidelity: scaffolding, not retail`.
  Wrapping a template itself does nothing, because VC7.1 takes an instance's optimize mode at the end of the TU.
- Tools: tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf349/ (leafrel.py compares the leaf by stable section
  names; use it instead of objsec346's LEAF flag).
- pending/2026-11-24_nf347_handback/ (NOTES, Part 2): NF347's measurements on this family.
- T3_PORT_QUEUE.tsv, the lines nf340 0x00429C15 / 0x00429C53 / 0x00429C6F / 0x00429F43.

## Scope
The leaf-called NEQ copies are CCountedPointer<NUISystem::CComponent> ctor / dtor / Reset (0x00429C15 / 0x00429C53 / 0x00429C6F) in
rows 0x00626110, 0x00626750 and 0x00605570. Retail's copies come from a size-optimised TU (the owners set `optimize("s")`).
- Apply the 2026-11-20 size-pragma rule in NF349's per-type form, in the shared header that defines these members. Prove all three
  conditions per row:
  - (a) grade unchanged, leaf identical, and only the target COMDATs change;
  - (b) the owner rows set the same pragmas: read their sources and cite them with their catalog flags;
  - (c) no optimize pragma before the block.
- Run both pick-any checkers HEAD v new, with a reversed control.
- Run the drop test on any dropped symbol.
- Prove the includer closure with equiv_check over the header's reverse include closure plus a planted control. hdrcheck2 can be
  vacuous; NF349 found that.
- DeleteData<CAction> 0x00429F43: `DELETEDATA_DECL_ONLY` was REJECTED by the advisor, so do not use it. If the size-body form fixes
  it without dropping the EQ DeleteData<CComponent> 0x00428B10, include it.
- If a copy cannot reach retail's body this way, STATUS it skipped with the measurement. Do not invent another device.
- 0x00626750 also carries a map operator[] item that agent NF350 owns. Do not touch that item; if your change and NF350's meet in one
  row, say so in NOTES.

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad. In every Bash call:
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`. Run `mkdir -p /run/user/0` if it is missing.
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe".
- Checkers:
  - rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I);
  - tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py, with env FABLE_EXE and FABLE_ROOT=/home/user/FableDecomp set BEFORE
    any run, plus --row-length.

## Hand-back
- land_scripted run dirs (batches/bN/). Each holds:
  - rows/, RESULTS.tsv, head/payload.json;
  - orig/rows + orig/tests, and tests/ wherever a test changes;
  - hdr/engine/ + orig/hdr_<name>, HEADERS.tsv;
  - evidence/pk_gate + pk_gate.txt with a reversed control.
- NOTES.md, STATUS.tsv and evidence/.

Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS before the first compile. Stay off Account B's claimed or staged rows,
Codex reservations, and the locks of agents NF350 and NF351. Never write into the repo, never run git, run long jobs in the
background, and never run two compile runners on one row path at once.
