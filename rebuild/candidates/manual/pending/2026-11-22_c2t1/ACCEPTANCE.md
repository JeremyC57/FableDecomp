# ACCEPTANCE: 0x00B4D370 p8 v p9 (C2T1, 2026-11-22)
**Result in one line: the 0x00B4E556 site's comparator bit flips between p8 and p9 (hash-decided, IND-temp `n mod 4`), but 12 other
trees' bits flip as well with identical code, so "no other bit changes" does not hold (pre-registered outcome c).**

Inputs: AUTH20's `x/p8.cpp` / `x/p9.cpp` copied to `in/` (sha256 in LOG.md), flags `/d2inlT314`, overlay `in/ov` (= auth20 `ov/`).
p8 and p9 differ in one source line (185): `Y / fabs(X) * max` becomes `Y * max / fabs(X)`.

## Verdict (strict): the test as set FAILS its second half -- outcome (c) of the pre-registration
- The advisor's test: "that site's bit flips between p8 and p9, and no other bit changes". **The site's bit flips (first half holds);
  12 other trees' comparator bits flip too (second half fails).** 132 flipped sorts in 13 trees (c2 re-sorts one tree 4..15 times).
  The pre-registered PASS needed every flip on the one tree, so this is outcome (c): extra flips with identical code.
- **The site:** 0x00B4E556 (last column of the PostMultiply(perspective_transform) expansion; p8 `fadd [eax-14h]` second, p9 last)
  is the output order of ONE comparator tree, the 4-operand sum `@1287a44` (line 203, rel 159): p8 `D3*ps > IND(t9655) > t9682 >
  t9571`, p9 `D3*ps > t9683 > t9572 > IND(t9656)` -- the emitted order in both builds. The deciding compare is hash-decided
  (IND(temp) v CSE temp): p8 IND key 0x1C008 >= t9682 0x17480 (stays 2nd), p9 IND 0x10008 < t9572 0x15900 (drops to last).
  INFERRED: that the IND over temp 9655/9656 is the `[eax-14h]` operand (from the operand order matching the listing and the
  operand being an IND over a pointer temp; c2 records do not carry the x86 operand).
- **The 12 other trees** are all binary products `IND(temp) * symbol` (lines 193 x4, 203 x4, 233 x2, 268 x2 = rel 149 / 159 / 189 /
  224) whose IND `n mod 4` term crossed the symbol's hash. NONE changes the code: the two traced objects differ only in the three
  lines at 0x00B4E556 (unmasked disassembly diff of logs/p8b.obj v logs/p9.obj). E.g. tree `@1287a14` = `D3 * perspective_shift`:
  keys flip (p8 IND 0x1C008 > ps 0x13440; p9 ps > IND 0x10008), both builds emit `fld [esp+10h] ; fmul [eax-0Ch]`.
- **Consequence for the "x87 sites follow the id rule" claim on this row:** the one multi-operand SUM followed its key order; 12 of 12
  flipped binary PRODUCTS did not change code (the wfA6g pattern). Product operand order is part of several remaining sites (the
  26-point loop, GetSquaredMagnitude / GetMagnitude, the end PostMultiply columns), so for those the key readout below is data, not
  a prediction of the emitted order.
- **Rule form:** the bit is record-number driven, but this site's operand is an IND over a temp, hashed `fold(+30h) + op - 0x24A +
  (hash(temp) << 8)`: only the low byte of the temp's `64n` survives, so the compare is `[0x4000*(X mod 4) + 8 >= 64Y mod 65536]`
  (X 9655 -> 9656, mod 4: 3 -> 0), not `[2X mod 2048 >= 2Y mod 2048]`. For two plain CSE-temp operands (both kind 1/2, symbol kind 3,
  no value node) the brief's rule holds: counted on every hash-decided temp-v-temp compare in the logs, 297 / 297 agree in p8 and
  297 / 297 in p9, 0 disagreements.

## The cause: one extra record, every later number +1
- In the symbol-allocation log (`--syms`), p8 and p9 first diverge at allocation #1587, number 1174 (the edited statement).
- Across all aligned operands the leaf-number delta is +1 for 24886 operands and 0 for 10582 (`lka --shifts`).
- **fsolve check, whole function:** shifting every number >= 1174 by F = +1 (T = 0) in p8's log and replaying all 34291 sorts predicts:
  - all 132 actual flips (0 missed);
  - 10 extra flips (8 sorts at line 195 and 2 at line 172, rel 151 / 128). INFERRED (not traced): some numbers >= 1174 there did not
    move by +1 in p9 (numbers come from free lists, not in allocation order; lka's delta histogram also shows 0 / +2 / +4 deltas).
- **fsolve check, the target sort only** (`fsolve.py logs/p8b.json 32091 32091 32091=0231 --F -4 4 --T 0 0`): the solutions are F in {-3, -2, 1, 2}. So +1 reproduces p9's exact order, as do the other F with `n mod 4` in {0, 1} at the IV.
- **Null control:** p8 logged twice, once all-functions at depth 4 and once with `--func --syms`: 34291 v 34291 sites aligned, 0 unaligned, 0 flips, every delta 0.

## Integrity of the readout
- Every traced object is the graded one: tools/objcheck.py shows UpdateShadowSpace at 7162 B, with the disassembly text (addresses
  and full operands, unmasked) equal line for line to AUTH20's q.py listing. Checked for p8 (both runs) and p9.
- Logs complete: gdb saw the exit, cl rc 0, the target PIPE was seen, and there were no hook errors.
- `lk.py check`:
  - p8: 73670 of 73670 comparator operands recompute from structure (p9: 73676), 0 wrong.
  - Every sort's result equals the stable descending key order.
  - fsolve's replay at zero shift reproduces all 34291 sorts.
- Unaligned (not comparable) runs: 6, all of them the edited statement's own product `Y * (1/|X|) * max` (operands `s430`,
  `max_projected_range_x`, the fabs temp t1295). 2 runs carry line word 141 (line 185); 4 runs carry word 229 (line 273, so c2 relabels
  that copy of the tree with the last line). No x87 tree of the regions below is unaligned.

## Readout of p9's remaining x87 sites (data only, `readout/p9_x87_final_sums.txt`, `readout/p9_L*.txt`)
The last sort of each tree is shown, operands in c2's output order (the order the code generator receives). `n` is the record number.

- **else GetSquaredMagnitude, 0x00B4E5E9 (line 212)** (squares sorted X first; retail squares Y first). INFERRED: s1313 / s1327 = the
  X fields and s2065 / s2074 = the Y fields (unnamed field symbols; X = offset 0, the field whose record is made first):
  - Sum 32188: `X*X` 0x0103EC90 >hash> `Y*Y` 0x01030693.
  - Sum 32191: `X*X` 0x0103F1D0 >hash> `Y*Y` 0x010309F3.
  - X-field numbers 1313 / 1327 give hash 32n = 0xA420 / 0xA5E0. Y-field numbers 2065 / 2074 are >= 2048 and wrap to 0x221 / 0x341.
  - The product hash is 3h + 0x30 mod 65536.
- **diag GetMagnitude, 0x00B4E74A (line 225)** (X first; retail Y*Y + X*X; X / Y labels inferred as above):
  - Sum 32302: `X*X` (n 1335) 0x0103F4D0 >hash> `Y*Y` (n 2086, wrapped) 0x01030E73.
- **persp/else column association, 0x00B4E509..0x00B4E55E (line 203).** Each column is one flat 4-operand sum: `D*frf` or `D3*ps` first (decided by need), then the CSE temps by hash:
  - 32009: t9672 > t9666 > t9572;
  - 32035: t9683 > t9666 > t9572;
  - 32060: t9683 > t9672 > t9666;
  - 32094: t9683 > t9572 > IND(t9656).
  - Retail's shared `(D3*0 + D2*0)` partial sum is a CSE structure, which a reordering of these flat lists does not express.
- **26-point PerspectiveTransformPoint loop, 0x00B4E97E..0x00B4EAAF (line 254).** Sums 22299 / 22344 (and 32681 / 32726 / 32778 for the second expansion) are `p.? * E` products ordered by hash: `s1998*s1422 > s1990*s1413 > s2006*s2096`, then the translation term (decided by need).
- **end, last two PostMultiply expansions, 0x00B4EDC6..0x00B4EECB (lines 269 / 270).** Each column is 4 products `Data[i][k] * temp`, fully hash-ordered:
  - 33755: `s1938*t10249 > s1937*t10253 > s1936*t10165 > s1069*t10257`;
  - 34067: `s2012*t10609 > s2004*t10525 > s1996*t10613 > s1166*t10617`;
  - the remaining columns are in the file.
  - Retail adds the `Data[i][0]*m[0][j]` product last.
- **setinv, 0x00B4D464..0x00B4D476 (line 55, DONOR_PASS26 SetInverse).** 54 final 2-operand cofactor products (sites 14010..), all hash-decided between plain symbols. For example: `s1688 > s1687`, `s1690 > s1689`, `s1693 > s1689`, `s1694 > s1688`.
- **Caution for whoever reads this:** for x87 products with a memory or stack operand the emitted order need not follow these keys; the twelve code-neutral flips above prove it on this row. A key flip is a candidate lever only where the tree is a multi-operand SUM (as at 0x00B4E556), and only once a listing confirms it.
