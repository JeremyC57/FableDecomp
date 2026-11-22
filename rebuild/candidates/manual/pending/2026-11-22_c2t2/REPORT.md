# C2T2 report (2026-11-22): the committed lk tools on 0x00B4D370 and 0x00BD3070

(Written by the coordinator from C2T2's hand-back; the agent's sandbox could not write .md files. Its LOG.md, scripts and small outputs are here.)

**0. Control:** the final lka on C2T1's p8_all v p8b: 34,291 sites aligned, 0 unaligned, **0 flips** (`out/lka_p8all_p8b.txt`); p8b v p9
reproduces C2T1's 132 sorts in 13 trees.

**1. 0x00B4D370 (p9, no new run): every residue SUM's tool order equals p9's emitted order** (operand order within products not covered).
Addressable by order: SetInverse det (0x00B4D464), both GetSquaredMagnitude sums (0x00B4E5E0 / 0x00B4E5F1: tool X^2 > Y^2, retail Y^2 first),
diag GetMagnitude (0x00B4E748), the 26-point loop X / Y / W rows (both copies), end L225 / L226 column 0 (retail computes D0 FIRST; AUTH20's
ESCALATE.md has this backwards). Not addressable by order: perspective columns 1-2 (retail keeps a CSE'd `P1+P0`: a different tree, not a
permutation). Pattern (data, not a lever): in 4 of 7 addressable trees the moving operand is the product over a struct's offset-0 field
(s602, s1069, s1166), which points to an early-record difference, not a late-record shift. X/Y labels are inferred (field-number model).

**2. 0x00BD3070 (one lk run on AUTH21's c6, stock flags, `--syms`; 1967 s; lk check 56,925 keys ok; objcheck 11,288 B SAME; zero-shift replay
reproduces all 27,475 sorts):** 39 SetRenderState expansions, each an AND tree and an OR tree over C = CurrentBookmarkMask (base+0x2814) and
M = BookmarkMask (base+8); bases are kind-5 inline locals allocated before the first sort (so only number insertions can move them).
- test / or orientation = the comparator bit at 30 of 38 sites; 8 (sites 1, 2, 10, 19, 34-37) are decided by register allocation too.
- Residue sites: 9 bookmark sites agree (tool-addressable): 0x00BD3433, 0x00BD3734, 0x00BD37AB, 0x00BD381B, 0x00BD3888, 0x00BD38F5,
  0x00BD43B2, 0x00BD441F, 0x00BD448C. 0x00BD4C30 (site 34) disagrees: the tool already has retail's order, ours emits the other form.
  x87 0x00BD32F4 (Radius * scale) and SIB 0x00BD5712 agree.
- **Solver: EMPTY.** No uniform shift F in -2048..2047 gives the 9 flips + 21 keeps; the 9 flips alone are already EMPTY (p3 needs F 78..715,
  p20-22 need -1060..-425 or 988..1623); one insertion point at each of the 79 base numbers: EMPTY (best F = -180 holds 26 of 30 sites;
  p3 and p20-22 fail); two insertion points: 9 of 36 pairs scanned, 0 solutions (stopped by the coordinator: a staircase of insertion
  points would be fitting record counts, which the ruling forbids).

**3. Terminal standard:** no forced oracle (nothing to force: not a uniform shift). Conflicting sites: p3 v p20-22; inside L25 p3 flip +
p4 keep allow F 713..715 while p11-15 flip + p16-18 keep need -180..-178 or 1868..1870.

**Tool notes:** no patches needed. README additions: objcheck.py needs the base address as a 4th argument for other rows; `fsolve.py
first..last` forces every unlisted sort in the range to keep its order (over-constrains); `scripts/psolve.py` is a per-site alternative.
