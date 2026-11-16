# Codex pass 23 header merge (landing p23, step 1)

Codex returned 191 headers as full files plus old/new text pairs (`hdr/PATCHES.json`), based on the reservation commit
6d0e46c677 (all 122 changed files hash-match it; 69 are new). Merged into the current tree, not copied over it:
- 80 files unchanged here since the base and the 69 new files take Codex's text (each hash-checked against `new_sha256`).
- 42 files changed on both sides: `git merge-file --diff3` (base 6d0e46c677). 33 clean, 9 with 11 conflict hunks, resolved by
  `resolve.py`: additive opt-in blocks are concatenated (ours first); CThingAICreature.h takes ours (both sides pinned the same assert
  `AT(287)`); CMatrix3x4.h keeps our new SetInverse `#ifdef` branch and takes Codex's `FABLE_MATRIX_INLINE_PASS23` spelling in its
  `#else`; libpk/CSoundEventManager.h keeps our FADER asserts (pinned `AT(24)` / `AT(25)`, their HEAD lines) inside Codex's
  `#else` of `STREAMED_PASS23`.
- One "clean" merge was wrong: CAIStateGroup_JackOfBladesStage2.h, where both sides wrapped the generated class in their own
  alternative and diff3 kept their identical `#endif` once. Fixed by hand (ours outer, Codex's nested in the `#else`, Codex's
  assert pinned `AT(25)`), found by an `#if` balance check.

Gates: every Codex `new_text` present; every line added here since the base present (the only absences are the two asserts
pinned on purpose and lines Codex respelled in place); per-line counts equal ours + Codex - base except identical pins; all 191
headers balance; no unpinned assert off its origin line; no opt-in macro introduced by both sides (Codex 242 new, ours 341).
Whole-tree equivalence (`lane_merge/equiv_check.py`, exact, HEAD's include v the merged one, 56,461 landed rows): only the
five known `__FILE__`-only rows differ (`equivalence_changed_input_rows.tsv`).
