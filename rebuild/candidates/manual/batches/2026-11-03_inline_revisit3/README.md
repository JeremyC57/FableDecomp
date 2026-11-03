# Inline revisit 3 (2026-11-03): 5 of the 70 remaining `Revisit: inline-depth` rows match pragma-free (NOT yet landed)

Research only. Graded with `vland.py --recheck-landed` on a full header mirror of d0f753026 (`vland/r1` WINS 4, `vland/r2` WINS 1).
Finalised sources are `out/<addr>.cpp`. `summary.tsv` has every row's best grade, lever, attempt count and the reason it is parked.
`FINDINGS.md` has the mechanisms and `parked/` the best drafts. `rows/` keeps only each row's landed copy, its plain-inline
variant and its call-site map; the ~6,700 graded variants were deleted to save space.

## To land (final landing round)
- 0x00E504C0, 0x00E6AF20 and 0x00EDCB80 (twins): loop-head macro (lever "r"), plus the line-2 `// Inline-split:` note.
- 0x00D99A30: the loop-head macro plus one do-while(0) on the check after the inner wait loop, plus the line-2 note.
- 0x00EF5200: a plain `return;` replaces the landed `goto terminating` device. The marker is dropped and there is no wrapping.
- The tests of 0x00E504C0 and 0x00E6AF20 still copy a `#pragma inline_depth` line; drop it at landing.
- Regrade on the then-current tree first. The mirror predates landing round g.

## 65 parked
- **CScriptThing-holding special members (16+ rows).** In the CIsThingAlive/CIsThingConscious filters and the CSpawnedFunc1 ctor,
  retail calls the CScriptThing copy 0x004ABE90 and dtor 0x004AA840 out of line inside those inlined members, but expands them
  elsewhere. `/d2inlT55-65` reproduces 9 of them, but that is a diagnostic switch only, so none of them land that way.
- **UI Pointer ctors 0x005518E0 and 0x00551BC0.** The split is the inverse of the frequency model.
- **0x00CEF3B0 ~CTimer.** The out-of-line call is not a frequency effect.
- **Dead-branch rows.** 0x00D97170, 0x00E53D90, 0x00E36390 and 0x00F09950 stay 3 bytes short.
- **0x00CE1280 CTestGateSwitch::Main.** Best 872v808. Its four inner-arm exits share one tail-merged out-of-line block.
