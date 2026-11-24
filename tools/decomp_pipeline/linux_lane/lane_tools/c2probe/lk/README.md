# c2probe/lk: VC7.1 c2 operand-sort comparator readout (lk / lka / fsolve)

**Scope (advisor ruling 2026-11-22, read first).** Accepted as an instrument, not as a predictor of emitted code:
- Proven: every comparator key recomputes from its logged tree, the zero-shift replay reproduces every sort (34,291 on 0x00B4D370), a
  repeat compile gives 0 flips, and where a tree's final order was checked (0x00B4E556) it equals the emitted operand order.
- NOT proven: that a flipped bit changes the code. On 0x00B4D370, 12 of 12 flipped binary products `IND(temp) * symbol` emitted
  identical code (the wfA6g pattern). The readout predicts emitted order only at sites where tool order = emitted order was checked.
- The rule: for two plain CSE temps B = [2X mod 2048 >= 2Y mod 2048] (297 / 297 hash-decided temp-v-temp compares agree). For an IND
  over a pointer temp the hash keeps only the low byte of 64n: B = [0x4000*(X mod 4) + 8 >= 64Y mod 65536].
- fsolve over-predicts (the +1 shift replay on 0x00B4D370 gave 10 flips that did not happen): a screen only; confirm with a real lk run.
- Candidates still come from PDB evidence, pre-registered and graded once; never fit text to hit a record count.
Rebuilt by tools agent C2T1 (2026-11-22) from the acctC wfE6 notes (`rebuild/candidates/manual/pending/2026-11-15_acctC_r6_wfE6/`).
Acceptance run: `ACCEPTANCE_00b4d370.md`; readouts in `readout/`. Paths: `source env.sh` with S set (runs default to $S/c2lk_runs).

# c2 operand-sort comparator readout (lk / lka / fsolve) -- C2T1, 2026-11-22

Rebuilds the acctC wfB3/wfE6 comparator tools (lkb2.py, lka.sh, fsolve.py; never committed) on the lane's c2probe kit.
Diagnostic only: nothing here goes into a row source. Destination when committed:
`tools/decomp_pipeline/linux_lane/lane_tools/c2probe/` (the tools import `c2dbg` from there; `LK_C2PROBE` overrides the path).

```
source tools/env.sh                     # lane env + S + PYTHONDONTWRITEBYTECODE=1 + C2PRIO_RUNS=$S/w22/c2t1/runs
python3 tools/lk.py run   <src.cpp> <log.json> --func <name substring> [--ov <overlay>] [--syms] [--obj o.obj] --flags <cl flags>
python3 tools/lk.py check <log.json>                      # decode self-check (must say wrong 0, permutation exceptions 0)
python3 tools/lk.py show  <log.json> [--line LO HI] [--pass a|b]
python3 tools/lka.py      <base.json> <other.json> [--shifts] [--quiet]
python3 tools/fsolve.py   <log.json> <first> <last> <flips> [--min N] [--F LO HI] [--T LO HI --late-min N|--late-after SITE|--late-temps]
python3 tools/objcheck.py <o.obj> <leaf> [q.py -v log]    # the traced object is the graded one
```
Cost: one `lk run` of 0x00B4D370 (7 KB, /d2inlT314) = 15 min alone, ~38 min with `--syms` and two runs in parallel
(34k sorts, 43k comparator calls in the row's window), a log of ~12 MB with `--func`. Run at most two at once; every log is checked for completeness (gdb saw the exit, cl rc 0,
target PIPE seen, object produced, no hook error) and `lkrun` exits 2 on an incomplete one.

## What c2 does (c2.dll 13.10.3077, read from the disassembly; every number below is checked by `lk.py check`)
- The comparator 0x1076A394 (`mov eax,[ecx+10h]; mov ecx,[edx+10h]; cmp; jb -> 1; = -> 0; else -1`) is pushed by exactly two merge
  sorts (0x1076A2F3) of a commutative node's operand list `[node+28h]`: **pass a** = 0x1075CD07 in the recursive canonicaliser
  0x1075CC68 (child keys, sort, own key), run many times per tree; **pass b** = 0x1075EEA0 in the CSE pass 0x1075D6AD (it creates the
  CSE temps through 0x1075C327). The list ends in stable DESCENDING order of the unsigned key at +0x10.
- Key (0x1075D388) = `need:8 | count:8 | hash:16`; leaves `hash | 1<<16` (constants: top 0). Interior: count = 1 + sum, need =
  first child's need, then per child `>` gives child+1, `==` gives +1 (kind 0xF +7, kind 0x13 +5).
- Hash (0x1075D54E): interior `sum(child16 << (i & 7)) + op - 0x24A`. Leaf, n = symbol number `[sym+1Ch]`:
  CSE temp (leaf kind 1/2, symbol kind 3) `64n mod 65536` = 32 * (2n mod 2048); other symbol `(h >> 11) ^ ((h & 0x7FF) << 5)`,
  h = fold(n); leaf kind 3 h; IND (kind 5/6) `fold(+30h) + op - 0x24A + (hash(+34h) << 8)` -- **only the low byte of the
  inner leaf's hash survives, i.e. n mod 4 for a temp**; constant fold(lo ^ hi). Copy rules (measured): a CSE-temp leaf with a value
  node (`[sym+14h]`) carries that node's full key; an IND (+30h == 0) over such a leaf carries the same key.
- So between two plain CSE-temp operands with equal need/count, the bit is the wfE6 rule **B = [2X mod 2048 >= 2Y mod 2048]**
  (X the operand first in the incoming list, Y the second; a tie keeps the order). `show` prints this rule next to the real B for
  every compare of two plain CSE temps ("!" = disagreement on a hash-decided compare): 0x00B4D370 p8 / p9: 297 / 297 agree each. Other operand shapes are decided by their real
  keys (need / count / hash column says which part decided).
- Node `+14h` low 16 = source line RELATIVE to the function's first line (inline bodies carry the call-site line).

## Tools
- `lk_gdb.py` (gdb side): hooks PIPE 0x1077C08E (function boundary; events since the previous PIPE belong to that function),
  both sort calls and returns (operand list before / after, each operand's tree to depth 4 with symbol numbers and names), the
  comparator entry (a, b), and with `--syms` the symbol allocator 0x1075A0C5 (kind, number, allocator return site).
- `lkrun.py` / `lk.py run`: compile + log + completeness checks. `lk.py check`: recomputes every operand key the comparator reads
  from its logged structure and replays every sort (0x00B4D370 p8: 72990 ok, 0 wrong; 0 permutation exceptions).
  `lk.py show`: one line per comparator call (site#, pass, rel. line, parent op, operand indices, B, deciding part, both keys, X / Y,
  rule, operand leaves: `t<n>` temp, `s<n>:name` symbol, `*n` IND over n, `c<v>` constant, `=>` forwarded temp).
- `lka.py`: aligns two logs by site on an id-free, ORDER-free signature (pass, parent op, rel. line, sorted multiset of operand
  op/kind/need+count shapes, children sorted too: the incoming order of a re-sort is itself a sort result) with difflib, labels
  operands by shape (+ rank by leaf numbers among equal shapes) and reports a FLIP when a sort's OUTPUT order of labels differs,
  grouped per tree (c2 re-sorts one tree 4..15 times), with the swapped operand pairs and both sides' compares; the unaligned runs
  ("not comparable": the edited statement's own trees) and, with `--shifts`, the histogram of leaf-number deltas.
- `fsolve.py`: uniform-shift solver. Every number >= `--min` moves by F, every LATE number by F + T. T defaults to 0; a T range needs
  an explicit split: `--late-min N`, `--late-after SITE` (numbers the `--syms` log saw allocated after that sort: the wfE6 X first-pass
  / Y late split) or `--late-temps` (all CSE temps; in the bookmark case X is a temp too, so T would cancel -- use --late-after);
  all operand keys of sites first..last are RECOMPUTED through their trees with c2's hash, the sorts replayed, and the exact (F, T) set
  printed per F as runs (no first..last ranges that hide gaps). Where c2 READS a stored key (the comparator's operands, an interior
  node's children) a key the model does not reproduce at zero shift (stale, e.g. 0) is held constant; where c2 RECOMPUTES (the leaf
  under an IND, a forwarded temp's value) the tree is re-hashed. Validated on 0x00B4D370: zero-shift replay 34291 / 34291 sorts;
  F = +1 for n >= 1174 (p9's one extra record) predicts all 132 sorts that really flip in p9 + 10 extra, 0 missed. `<flips>`: `N` = site N must change its permutation, `N=0312` = must give
  that permutation, `-` = keep everything.
- `objcheck.py`: size of the traced object's function and equality with a graded q.py listing (needs tools/env.sh: FABLE_OBJDUMP).

## Paths to adjust when committing
- `tools/env.sh` hard-codes S and `C2PRIO_RUNS=$S/w22/c2t1/runs`; `lkrun.py` defaults `C2PRIO_RUNS` to `$S/w22/c2t1/runs` (keep it
  OUT of the shared `$S/w22/c2probe/runs`: c2dbg deletes run dirs older than 6 h there). `lk_gdb.py` / `lkrun.py` import c2dbg from
  the repo's c2probe dir (`LK_C2PROBE` overrides); objcheck imports verify_and_land from the repo.

## Known limits
- Sites are sort calls, not emitted instructions: pass a re-sorts the same tree many times; the LAST pass-a sort of a tree before its
  pass-b sort (and later re-canonicalisations) is what the code generator sees. lka reports every repeat.
- wfA6g, confirmed on 0x00B4D370: for x87 products with a memory / stack operand the emitted order need not follow the key order
  (12 `IND(temp) * symbol` products flipped keys between p8 and p9 with identical code); multi-operand SUMS did follow it there.
  Check the listing before calling a key flip a lever.
- An IND over a temp hashes as `fold(+30h) + op - 0x24A + (64n << 8)`: only n mod 4 survives, so a +1 record shift can flip many
  of them at once (ACCEPTANCE.md).
- Depth-4 trees: fsolve marks a result "APPROX" if a truncated subtree had to keep its logged hash.
