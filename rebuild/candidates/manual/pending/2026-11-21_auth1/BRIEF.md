# AUTH1: Account A authoring agent (2026-11-21)

Owner (2026-11-21): "start an authoring agent ... It should not park functions, but keep working until they match."
Owner: "if you are having a lot of trouble matching a function, you can send it to main chat to be looked at by the fable 5.2 advisor."

You are AUTH1, the only authoring agent. One naming agent (NF215) runs at the same time on landed rows; you never touch its rows
or its overlay files. Account A = branch FullDecomp. Read `/home/user/FableDecomp/docs/COORDINATION.md`.

## Paths
- Repo (READ-ONLY for you): `/home/user/FableDecomp`. Never edit, commit or push anything in it. You work in scratch; the coordinator lands.
- Lane: `export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh`, cwd = repo root.
- Your work dir: `$S/w22/auth1/` (put drafts in `$S/w22/auth1/<addr>/`). Your rows: `$S/w22/auth1/rows.tsv` (retail lengths).
  Their oracles: `$S/batch/auth1_oracle.tsv` (scratch only: retail bytes never go into the repo).
- Locks: `$S/w22/nflocks/<addr>/owner`. Your ten rows are locked `AUTH1`. Before you edit a landed row or a helper row, claim it
  (`mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo AUTH1 > $S/w22/nflocks/<addr>/owner`). If someone else owns it, leave it and say so.
- Disk is tight (about 6 GB free on a fixed allowance). Delete your own big intermediates (objects, sweep dirs) when done with them; check `df -h /`
  now and then.

## Tools (all in `$S/tools/`, current copies from the repo's `tools/decomp_pipeline/linux_lane/lane_tools/`)
- `python3 $S/tools/q.py <addr8> <src.cpp> [leaf] [--flags /EHsc ...]`: grade one source (RELOCATION_MATCH / MATCH good, DIFFER(ours v retail)
  with side-by-side disassembly). About 4 s for a small row. `udiff.py` shows only the differing instructions.
- `python3 $S/tools/fg.py` grades up to 3 variants per call (fast/README in the repo), `vt.py` runs a behaviour test, `addmatch.py` writes a matched row.
- `python3 $S/tools/xdis.py <va> <len-hex>`; `xref.py <va>` (callers / absolute references); `fun.py 0x<va>` (EH frame); `rstr.py`, `iat.py`.
- Repo tool dirs worth knowing: `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/` (the c2 symbol-order / priority hooks B used),
  `lane_tools/retry_tools/`, `lane_tools/lane_merge/pin_asserts.py`, the p25R compile-order harness (commit 0163501389).
- Types: `rebuild/include/engine/<Class>.h` (PDB layouts), `ghidra_out/struct_layouts_egor.tsv`; Ego_d.pdb type dump at `/tmp/claude-0/egod_pdb/types.txt`.

## Read first
- `docs/pipeline/GOTCHAS.md` (2026-11-1x / 11-2x lines at least), `docs/pipeline/ASM_RULES.md`,
  `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`, `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/brief_tools_rules.md`
  (its "4 grading attempts" cap does NOT apply to you; see below).
- `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` (rulings that still bind: fitted dead temporaries, redundant reads, assert arms, ...).
- `rebuild/candidates/manual/pending/2026-11-20_p25_open/README.md` and each row's folder there (best sources, notes, hdr.diff).
- `rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/LESSONS_EGOD.md` (the Ego_d rule: the source must not contradict Ego_d's line table / code).
- For EVERY row, before writing anything: `grep -n -i <addr> docs/journal/FINDINGS_LOG.md docs/pipeline/GOTCHAS.md docs/journal/2026-11/*.md`
  and `git -C /home/user/FableDecomp log --oneline --all -S <addr-lowercase-or-0xUPPER> | head` to find the newest draft and every measured
  negative. The UNMATCHED_ROWS `closest_version` path is often NOT the newest draft (B's p25Q/p25R/p25S notes are newer for several rows,
  e.g. commit ffa9d70019 for 0x00502E90 / 0x00BD3070, d2e07efc14 / 0163501389 / d7dfc77b48 / b1d965cae6 for the large acctC rows).
  Do not repeat a variant a note already measured negative.

## Rows (work in this order; smallest first)
| row | function | retail len | best on record |
|---|---|---|---|
| 0x0093D270 | CAIStateGroup_MeleeCombatNearbyBase::IsRunnable | 295 | DIFFER(297v295) (the Q8 draft in the repo grades 305v295 here; B's p25R work is newer) |
| 0x00C73050 | Audio_PlaySourceAndProcessQueue | 672 | DIFFER(672v672), register edi v edx (p25N notes) |
| 0x007602F0 | NShape::IsShapeConvex | 1126 | bracket match only with texts Ego_d contradicts: NOT acceptable |
| 0x00763060 | NShape::FractureShape | 1146 | DIFFER(1132v1146) |
| 0x008E18D0 | CModeThunderLightningStorm::UpdateLightningBeams | 2073 | DIFFER(2045v2073) |
| 0x00A4EFC0 | C3DAnimationSequenceData::InitialiseFromFileData | 3491 | DIFFER(3491v3491) one residue |
| 0x00CF4EC0 | CQ_ArenaHoldingScriptScript::CFanMarker::Main | 6164 | DIFFER(6244v6164) |
| 0x00B4D370 | CEngineShadowRenderer::UpdateShadowSpace | 7176 | DIFFER(7162v7176) |
| 0x00BD3070 | CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive | 11288 | DIFFER(11288v11288) |
| 0x00502E90 | CWorldMap::PopulateRegionWithVillagers | 13622 | DIFFER(13587v13622) |

## No parking: keep working until it matches
- There is no attempt cap and no "park". A row ends only as MATCHED (RELOCATION_MATCH or MATCH under the rules, test PASS) or as ESCALATED.
- Work by hypothesis: before each grade write one line in `<addr>/LOG.md` (what you change and why, from the diff), then the grade. Measure the
  residue (differing instruction count / length) after each grade so progress is visible.
- **Escalate** when a row stalls: 25 grades in a row with no reduction in the residue, or every idea you have left contradicts a rule or Ego_d.
  Then write `$S/w22/auth1/<addr>/ESCALATE.md` (at most ~120 lines): the best source path and its grade line, the remaining diff (udiff
  output, trimmed), what you tried (grouped, with grades), your hypotheses for what retail did, and the specific question for the advisor.
  Leave the row reserved, append `escalated` to STATUS.tsv, and go on to the next row.
- When every remaining row is matched or escalated, stop and return. Your final message lists the ESCALATE.md files. The coordinator takes them
  to the advisor and sends you the answers (SendMessage); you then resume those rows with the advice. That is not parking: the rows stay yours.
- Also return early (with the same packets) if you are at risk of running out of context: write STATUS.tsv and ESCALATE/NOTES files first.

## Hard rules (unchanged by "no parking")
- Plain C++ that VC7.1 compiles to the retail bytes. No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline`; `volatile` only where
  retail used it; no `#error` probes; no cast addresses; no `Unproven` / `VirtualNN_` / `FUN_` placeholder names. Asm only per ASM_RULES.
- The source must not contradict Ego_d (LESSONS_EGOD). A match reached only with texts that Ego_d contradicts (like the 0x007602F0 bracket) is not
  a match: keep it as evidence, keep working, or escalate it.
- Shared headers: model `this` with `#include "engine/<Class>.h"`. No new local model of a class that has a shared header. Header changes go in
  your overlay `$S/w22/auth1/hdr/` as `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>` opt-ins only (default path token-identical), base copies in
  `$S/w22/auth1/orig/`. Copy from the CURRENT repo header. After editing an overlay header run
  `python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py rebuild/include $S/w22/auth1/hdr <header>` and record in
  `hdr/README.md` what it pinned. Never move a `FABLE_STATIC_ASSERT`. A rename inside a shared header is not yours: note it as "needs header claim".
- Names right the first time (EXTRA_RULES): exact PDB spellings; descriptive names only where none exists anywhere, marked descriptive in NOTES.
  Name fixes you find go in `$S/w22/auth1/NAME_FIXES.tsv` (address, correct symbol, evidence x2, wrong spelling); do not apply them.
- Pick-any COMDAT copies must equal the owner row's bytes AND relocation targets (nf201).
- Behaviour test per matched row (`<addr>_test.cpp` printing `<addr>_TEST PASS`), checked with `vt.py`.
- Loop prevention for TOOLS: never repeat a failing command more than twice with the same arguments.
- The first grade after a container restart can time out while wineserver starts: retry once.

## Hand-back (keep current after EVERY row: the container can restart without warning)
- `$S/w22/auth1/STATUS.tsv`: address, state (working / matched / escalated), best grade, grades used, best source path, note.
- Per matched row: `<addr>/<addr>.cpp` (header comment: what it does, address, length, callees by address, flags / pragmas, `// Leaf:` full
  mangled line), `<addr>/<addr>_test.cpp`, `<addr>/NOTES.md` (route to the match, opt-ins used), plus `hdr/` changes.
- `MATCHES.tsv` (address, leaf, flags, source, test) for the coordinator's `frommatches.py`.
