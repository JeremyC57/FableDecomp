# NF364: the y-off sibling of NF352's Reset<CComponent> size block (Account A naming queue, 2026-11-24)

Scope: BRIEF.md (P1 approved with conditions; 19 rows; Part 2 = one diagnosis probe on 0x00603000). No repo write, no git.
Repo text used as it stood at the start of the run (header sha256 761fdbb3..., re-asserted equal at hand-back by tools/mkbatch364.py).

## Part 1: the block

### Design (engine/CCountedPointer.h, add-only)
- One new block, 38 lines, appended after the existing `FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY` block and before the
  closing `#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H`. No existing line changed (`diff` old v new: 0 `<` lines, 38 `>` lines).
- Macro: `FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU` (existing pattern + `_YOFF_TU` for "TU already in y-off mode").
- Body: `#if defined(_MSC_VER)` `#pragma optimize("y", on)` `#pragma optimize("s", on)` `template<> __declspec(noinline) inline` / `#else`
  `template<> inline` / `#endif`, then the NF352 Reset body token for token, then `#pragma optimize("y", off)` `#pragma optimize("s", on)`
  (instead of `("", on)`). Its own `namespace NUISystem { class CComponent; }` (the existing one sits under the NF352 `#if`, and widening
  that would edit an existing line).
- `#error` when defined together with `FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY` (control: a TU defining both fails clang
  at that `#error`, evidence/logs/error_control_both_macros.log; a TU defining only the new macro passes).
- Comment: `Fidelity: scaffolding, not retail`, the owner (0x00429C6F: row-head `optimize("s", on)`, catalog /O2 /Oy /W3, no CompilerFlags
  entry), why the NF352 block cannot serve these TUs, and the hard-coded-restore precondition (pragscan: state before == state after).
- File: batches/b1/hdr/engine/CCountedPointer.h (new), batches/b1/orig/hdr_CCountedPointer.h (repo), HEADERS.tsv.

### Rows: one added line each
`#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU   // NF364: ...` after the row's last
`#define FABLE_RETAIL_CCOUNTEDPOINTER*` line above its first #include (tools/mkrow364.py asserts exactly one added line, above the first
#include, and that the row defines none of RESET_SIZE_BODY / RELEASEDATA_SIZE_BODY / ORIGINAL_COMPLETE_MEMBERS_PASS22). The 4 mixed rows
(005a29f4 005a9cbe 005aaf28 005ad571) carry NF362's REREAD define in the repo text (checked).

### Gates (all 19 rows pass every one)
- **pragscan** (tools/pragscan364.py, full letter set, new TU on the new tree, HEAD TU on the pristine copy; evidence/pragscan364.txt):
  ALL_OK. Every row: BEFORE the block `y=off s=on` (all other letters at the command line), at the specialization `y=on s=on`, AFTER the
  block `y=off s=on` (== BEFORE), END-OF-TU new == HEAD (`y=off s=on`), the row's own pragma sequence unchanged (`y,off` `s,on`).
- **objsec** H v N (one tree path, header swapped between phases; hashes in evidence/logs/gate_phase.log): exactly ONE CHANGED line,
  `?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z` 62 -> 59 B; no ADDED / DROPPED / SYM lines; leaf=same.
- **Owner object**: the new copy's bytes AND relocations equal the owner's own COMDAT (H_00429c6f.obj, built here at HEAD) in all 19.
- **Grades**: scratch RELOCATION_MATCH H and N in all 19 (evidence/logs/pass_*.log); vland `--recheck-landed` HEAD (repo headers) v new
  (FABLE_HDR_OVERLAY = full new tree): see logs/ (19/19 RELOCATION_MATCH PASS both sides; WARN sets identical).
- **Both checkers** (pkcmp346 H v N): GATE PASS, totals pk_all [0 turned NEQ, 0 newly NEQ, 19 fixed, 0 dropped], pvr the same. The
  reversed control FAILs (38). **Reach flips** (reachflip.py and reachflip_coordinator.py): 19 pairs, 0 flips.
- **ReleaseData 0x004291DE** (measured, as asked): EQ 44 B reach y before and after in 17 rows; 005a9cbe / 005aaf28 define
  RELEASEDATA_DECL_ONLY, so no copy in the object (the Reset copy calls the external symbol, as the owner does). So no row needs a
  y-on ReleaseData body (the NF352 Q1b shape does not arise); no second block.
- **Debt left as it was (not changed, no flip)**: 005d64dd's DeleteData 0x00428B10 copy is NEQ at HEAD and new, reach y in both
  (T3 nf360 0x00428B10 line).

### equiv_check (tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py, unmodified, S pointing into this folder)
- Whole catalog, repo row texts, pristine tree v new tree: **57,155 rows checked (src + test), 0 differing lines** (evidence/equiv/equiv_full_old_v_new.tsv empty; log line `rows checked 57155`, evidence/logs/equiv_full.log).
- Planted control: a mirror of the repo (tools/mkmirror364.py: symlinks except the 9 directories on the changed rows' paths and the 19
  changed rows, which are real copies of rows/<a>.cpp), new tree v new tree + `typedef int NF364_PLANTED_CONTROL_TOKEN;` inside the
  block: **57,155 rows checked, exactly 19 differing lines = the 19 user rows (src), 0 tests, nothing else** (evidence/equiv/equiv_control_new_v_planted_mirror.tsv, evidence/logs/equiv_ctl.log).
- What neither run can see, checked by grep instead: a landed file that `#include`s one of the 19 row sources (the folded
  `../src/compiled/...` form) would be a further user of the block at landing. 0 hits over rebuild/src, rebuild/tests and
  rebuild/include for the 19 basenames (evidence/includers_of_rows.txt, evidence/row_basenames.txt).
- Macro-name census: `FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU` occurs nowhere in rebuild/ or tools/ before this
  hand-back. The block defines no new symbol (the specialization is the same `?Reset@...` COMDAT the rows already emit).

## Part 2 (diagnosis only): 0x00603000, the NF352 size body WITHOUT `__declspec(noinline)`
- Scratch tree work/ptree (full copy, probe block appended under `NF364_PROBE_RESET_SIZE_INLINE`: `optimize("s", on)`, `template<> inline`
  Reset body, `optimize("", on)`; evidence/part2_probe_block.diff). Row text = HEAD + one `#define NF364_PROBE_RESET_SIZE_INLINE`. The row
  has no optimize pragma and no Reset opt-in (default in-class Reset), so the `("", on)` restore is right for it.
- Result: **fails**. The Reset copy does become 59 B, EQ under both checkers, bytes + relocations equal to the owner object (103 -> 59).
  But the leaf moves: scratch DIFFER(9794v9571) (HEAD RELOCATION_MATCH 9571); leaf relocations to Reset 3 -> 1 (two more sites expanded
  inline), DeleteData 10 -> 12. And the already-NEQ ReleaseData copy (53 v 44 B) flips reach n -> y (the NF352 hold). objsec: 2 CHANGED
  (leaf, Reset). Hypothesis (one probe, not proven): c2 prices an inline callee by the size of its own COMDAT (as the header's
  RESET_ELSE_KEEPS_INFO comment records); the 59 B size-mode body is cheaper than the 103 B speed one, so more Reset sites expand.
  Measured fact: dropping `noinline` does not keep the leaf; the leaf moves with it (NF361: Reset calls 3 -> 13) and without it (here
  3 -> 1). Not scaled; no hand-back text. rows/probe/00603000_p2.cpp is the probe text only, NOT a candidate.
- evidence/part2/: objsec_H_v_P2.txt, owner_cmp.txt, H_ / P2_ pk files.

## Run dirs
- batches/b1 (19 rows): rows/, rows.txt, RESULTS.tsv, head/payload.json (HEAD payload), head/HEAD_FROM_REPO.txt (bare addresses),
  TABLE_CHANGES.tsv (header only), orig/rows + orig/tests (asserted equal to the repo at batch build), hdr/engine/CCountedPointer.h +
  orig/hdr_CCountedPointer.h + HEADERS.tsv, evidence/ (pk_gate H/N, objsec, check364, pragscan364, pk_gate.txt, reversed control,
  reachflip, equiv/), logs/recheck_head.log + recheck_new.log.
- Landing notes: (1) the header must be applied before land_scripted's recheck (it compiles with repo headers); (2) COORDINATION_CLAIMS has
  the open NF364 claim line naming every row (land_scripted holds rows named by an open claim); (3) T3 nf358 0x00429C6F: these 19 emitters
  are fixed; the four mixed rows' Reset debt (NF362 notes) closes too; (4) 0x00429C6F owner row unchanged (compiled read-only once);
  (5) every one of the 19 rows is named on T3_PORT_QUEUE (evidence/t3_mentions.txt): all on nf358 0x00429C6F, 005a29f4 / 005a9cbe /
  005aaf28 / 005ad571 also on nf358 0x0042B312, 005a9cbe / 005aaf28 / 005ad571 on nf357 0x0042B1A6, 005d64dd on nf360 0x00428B10.
  land_scripted holds them as T3_PORT_QUEUE unless the coordinator releases those lines (`--release-t3` or a manual override); only the
  nf358 0x00429C6F mention is about this copy, the other lines stay open.
- Account B screen (evidence/acctb_screen.txt): no B claim line, no UNMATCHED status, no Codex RESERVED line for any row. The
  pending/*acctB* hits are 2026-11-11..13 authoring packages of these since-landed rows, plus two comments inside B's 2026-11-24_acctB_b24
  header overlays (CIVCountedPointer.h names 0x005A8C66, NGameText_CDataBank.h names 0x005AD571): no staged row.

## Locks and process
- Locks: 19 rows + owner 0x00429C6F (released right after the owner build) + 0x00603000 (released after the Part 2 probe). evidence/locks.log.
- Prechecks: tests first (clang; 11 failed only inside VC7.1 headers -> `cl /Zs` PASS, evidence/logs/pre_zs.log, D1), then locks, then HEAD and new
  sources by clang on the pristine / new tree (38/38 PASS) and the owner / 00603000 sources (PASS), evidence/logs/pre.log.
- Retail bytes: the vland oracle and objects stay in work/ (cleaned at the end). No byte dumps in any hand-back file.

## Tools (tools/)
mkrow364.py (row texts + orig), pragscan364.py (full-letter optimize replay, before / spec / after / end), gate364.sh (one-path H / N phases
with header swap + both checkers), check364.py (objsec one-COMDAT + owner compare + grades + copy verdicts), mkmirror364.py (equiv mirror),
part2_364.sh (Part 2 probe), pretests364.sh / presrc364.sh (prechecks), vlandchain364.sh, mkbatch364.py; copies from NF362 / NF352 with
paths repointed: nf.py pass.py pkrun352.py objsec352.py cmpcomdat.py pkcmp346.py reachflip.py reachflip_coordinator.py zs_tests.sh.
- Locks: all released at hand-back (21 claims: 19 rows, owner 0x00429C6F, 0x00603000); scan of nflocks/*/owner finds 0 NF364.
