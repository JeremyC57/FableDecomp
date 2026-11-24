# C2T1 LOG (2026-11-22)

## Inputs (copied, never edited)
- `in/p8.cpp` sha256 235379f4848f05b5f0f60897ff6dffb0ca1936c71c3a00c78d87539d4994be08 (= $S/w22/auth20/00b4d370/x/p8.cpp)
- `in/p9.cpp` sha256 5a726b1933f37b1a1e252f3c424a420ce36ad72a16047afc4db7cb9c09963389 (= x/p9.cpp)
- `in/ov/` = auth20 `ov/` (engine/CEngineLightingManager.h f36df92d..., engine/CEngineShadowRenderer.h 02ce56bf..., engine/GFLimit.h e1b6a582...)
- flags `/d2inlT314`; c2.dll 13.10.3077 sha256 bcd28f39b1798b07989c30a1df471cf87fc20e3802d130901a08f83800e6b81e.
- p8 v p9 source diff: one line (185), `Y / fabs(X) * max` -> `Y * max / fabs(X)`.

## c2.dll facts (read from the disassembly, work/c2.dis)
- Comparator 0x1076A394: `mov eax,[ecx+10h]; mov ecx,[edx+10h]; cmp; jb -> 1; equal -> 0; else -1`: descending unsigned sort on the
  node key at +0x10. Pushed as the comparator by exactly two sort calls: 0x1075CD07 (in 0x1075CC68, the recursive canonicaliser:
  child keys via 0x1075D388, sort [edi+28h], then own key) and 0x1075EEA0 (in 0x1075D6AD, the CSE pass; it makes temps via 0x1075C327).
- Key 0x1075D388: leaves (kind mask 0x1FFE) = hash16 | (kind not in 7..9) << 16; interior = need<<24 | count<<16 | hash16 with
  count = 1 + sum child counts, need = Sethi-Ullman-like (first child's need; a later child with need > cur gives need+1, equal gives cur+1;
  kind 0xF +7, kind 0x13 +5).
- Hash 0x1075D54E: interior = sum(child16 << (i&7)) + op - 0x24A; leaf kinds 0..3: n = [sym+1Ch]; kind 1/2 with [sym+14h] != 0 ->
  hash of that value node; kind 1/2 with sym kind 3 (CSE temp) -> 64n mod 65536; else h = fold(n), kind 3 -> h, else
  (h>>11) ^ ((h&0x7FF)<<5). Kind 4 fold([sym+28h]); 5/6 fold([+30h]) + op - 0x24A + hash([+34h])<<8; 7 fold(lo^hi); 8 fold([[sym+24h]]);
  9 fold([+16h]); 10..12 0.

## Target (no compile; AUTH20's graded listings x/p8_t314.log v x/p9.log, built side)
- The only built difference: 0x00B4E556..0x00B4E55F (last column of the PostMultiply(perspective_transform) expansion):
  p8 `fmul [eax-0Ch] ; fadd [eax-14h] ; fadd [esp+14h] ; fadd st(1)`, p9 `fmul [eax-0Ch] ; fadd [esp+14h] ; fadd st(1) ; fadd [eax-14h]`
  (retail = p9's order). So the site is a 4-operand commutative sum; the operand `[eax-14h]` moves from 2nd to last.

## Runs
- r1 p8, all functions, depth 4 (`logs/p8_all.json`, 910 s, 48213 sorts / 60416 comparator calls in 40 function windows; complete: exit
  seen, cl rc 0, obj produced). The traced object's UpdateShadowSpace is 7162 B and its disassembly equals AUTH20's graded p8 listing
  (tools/objcheck.py: 1754 instructions SAME). UpdateShadowSpace's window (prev PIPE .. its PIPE) holds 34291 sorts: the inline bodies
  (PostMultiply, PerspectiveTransformPoint, ...) are sorted inside the caller's window, as expected.
- Decode self-check (`lk.py check`): every operand key the comparator reads recomputes from its logged structure: 72990 ok, 0 wrong,
  680 not decodable (kind 9 leaves; r1's gdb script did not log their field yet, added for later runs); every sort's result equals the
  stable descending key order (0 exceptions). Two measured copy rules were needed (documented in lk.key_model): a forwarded CSE-temp
  leaf carries its value node's full key, and a kind-6 IND (+0x30 == 0) over such a leaf carries the same key.
- Node +0x14 low 16 = source line RELATIVE to the function's first line (p8: UpdateShadowSpace at line 44; word 141 = line 185, the
  edited statement; 159 = line 203 PostMultiply(perspective_transform); inline bodies carry the call-site line). Verified on 10 words.
- Pass "b" (CSE pass) sorts of 4-operand sums all come in already ordered (perm identity); the orders are made by pass "a".

## Pre-registered acceptance criterion (written before the p9 run)
- Runs: p8 again (null control) and p9, same flags / overlay / tool config (`--func UpdateShadowSpace --syms`).
- Alignment: lka.py, id-free signature = pass, parent op, relative line, per operand op / kind / need+count shape (depth 3).
- Null control: p8 v p8 must give 0 unaligned sites and 0 flips; otherwise the tool is not deterministic and no p8/p9 reading is made.
- p8 v p9: unaligned runs whose lines are 141 (line 185) are "not comparable" (the edited statement's own trees). Every flip in aligned
  sites is listed, grouped by line. PASS = all flips sit in sorts of line 159 (the PostMultiply(perspective_transform) expansion) on
  the same tree (the last column's 4-term sum, operands `D*m`, `[eax-14h]`, the two CSE'd products), and the flipped compare is
  hash-decided. FAIL (b) = no flip on that tree. Extra flips elsewhere = reported as data (outcome c). No filter is added after p9.
- r2 p8 (`logs/p8b.json`) and r3 p9 (`logs/p9.json`), `--func UpdateShadowSpace --syms`, run in parallel: 2307 s / 2293 s (the symbol
  hooks and another agent's compiles roughly double the time), complete, objects = AUTH20's graded listings (objcheck SAME, 7162 B),
  `lk.py check` 73670 / 73676 operands ok, 0 wrong, 0 undecodable.
- lka signature change (CORRECTED record): the null control passed with the ORIGINAL signature (incoming operand order in the
  signature) too; what forced the change was the first p8 v p9 pass: it reported 11 flips and left the TARGET tree (the line-159
  4-term sums) "not comparable", because a re-sort's incoming order is itself the result of earlier, flipped sorts. Changes: (1) the
  signature uses the sorted multiset of operand shapes, shapes sort their own children; (2) a flip = the OUTPUT order of operand
  labels differs; (3) flips grouped per tree. Effect: the target tree became comparable and the flip count grew (11 -> 132 sorts,
  13 trees); no flip of the first pass disappeared (its 11 were all products on lines 149 / 159 / 189 / 224, all still listed).
  Null control re-run after the change: 0 unaligned, 0 flips.
- p8 v p9 (`logs/lka_p8b_p9.txt`): 34243 aligned sites, 6 unaligned runs = the edited statement's own trees (words 141 and 229),
  132 flipped sorts in 13 trees: the target sum @1287a44 (rel 159) + 12 binary `IND(temp) * symbol` products (rel 149 x4, 159 x4,
  189 x2, 224 x2). Target: IND(t9655) 0x1C008 -> IND(t9656) 0x10008 (n mod 4: 3 -> 0) moves from 2nd to last, matching the listing.
  The 12 products flip without any code change (listings identical outside 0x00B4E556). Verdict in ACCEPTANCE.md.
- fsolve validation: the first version mis-modelled two cases found by the zero-shift replay (stale stored keys read by the comparator
  / an interior parent, and IND / forwarded values that c2 RECOMPUTES); fixed with a zero-shift reference row (a stored key the model
  does not reproduce at shift 0 is held constant). Zero-shift replay: 34291 / 34291 sorts reproduced. Uniform +1 for numbers >= 1174
  (the first allocation where p8 / p9 diverge): predicts all 132 actual flips + 10 extra (rel 151 / 128), 0 missed.
- Data readout for p9's remaining x87 sites: `readout/p9_x87_final_sums.txt` (last sort of every tree per region), `readout/p9_L*.txt`
  (every final comparator call per region line, lk.py show --final).
- Review fixes (after the advisor's check): objcheck compares the full operand text (no 0x masking): SAME for p8_all / p8b v
  p8_t314.log and p9 v p9.log; unmasked diff of the two traced objects = exactly the 3 lines at 0x00B4E556. lk `rule` column now only
  for two plain CSE temps; counted: 297 / 297 hash-decided temp-v-temp compares agree in p8b and in p9 (0 "!"). fsolve: T defaults
  to 0 and needs an explicit late split (--late-min / --late-after SITE from the --syms allocation events / --late-temps); T-only
  check on site 32091 with --late-after 28700 (the line-159 temps t9571..t9682 are allocated at sorts 28799..28863):
  T in {-7..-6, -3..-2, 1..2, 5..6} flips it (period 4, as the IND `n mod 4` term predicts); joint F x T table for p9's exact order
  `32091=0231` has 23 points in F -2..2 x T -4..4, with gaps.
- Cleanup: work/c2.dis (24 MB) and the run dirs deleted (regenerate: `objdump -d -M intel $FABLE_VC71/bin/c2.dll > work/c2.dis`);
  logs kept (p8_all 16 MB, p8b / p9 12 MB each).
