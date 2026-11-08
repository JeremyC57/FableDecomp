# VC7.1 codegen research (research861, 2026-11-08) -- how VC7.1 13.10.3077 (c2.dll) decides the residues that park our near misses

> Committed copy. Tools, probes (micro-TUs) and the per-row STATUS live in
> `rebuild/candidates/manual/pending/2026-11-08_research861/` (`tools/`, `probe/`). Not committed: the agent's `web/` copies of
> third-party pages (follow the URLs) and `probe/c2_strings_all.txt` (a string dump of Microsoft's c2.dll; regenerate it from the
> lane toolchain's c2.dll with `strings`).

Scope: the 26 parked rows of chunks 856 / 859 (oracle `$S/batch/chunk861_oracle.tsv`) plus chunk 860's three
(`chunk860_oracle.tsv`). All 29 baselines reproduce the chunks' md numbers (`STATUS.tsv`). No row was matched or moved:
this was a research chunk and the time went into lane experiments (probe/, every claim below is reproduced by a micro-TU or a
regrade) and into the one outside source that turned out to describe our back end. Tools written here, reusable:

- `gr.py <src> [tag]` -- chunk-compatible grader (md metric, per-address overlays, any `$S/batch/*_oracle.tsv`).
- `tu_flag_check.py <lo> <hi> <flag...>` -- regrades every LANDED row of an address range under an extra flag: the per-TU flag
  admissibility test (section 1.3).
- `scan_externs.py`, `probe/pathlen.py`, `sweep_flags.sh` -- symbol-count / path-length / 41-flag sweeps.
- `probe/mt.py <micro.cpp> [flags]` -- compile a micro-TU with the lane cl and print every function's disassembly.
- `probe/c2_switches.txt`, `probe/c2_assert_sites.txt`, `probe/c2_strings_all.txt` -- c2.dll's switch table, and the addresses
  of its `color.c` / `tuple.c` / `stack.c` / `dag.c` / `fppeeps.c` / `lowerflt.c` assertion sites (for a future debugger probe).

Confidence levels: **high** = reproduced on two or more independent probes or on a whole TU; **medium** = one probe family;
**low** = inferred.

## 0. The one outside source that matters

The Total Annihilation decompilation ("byte-tactics", HectorBailey) read MSVC 5's `C2.EXE` register allocator with Ghidra and
wrote it up with test numbers: <https://github.com/HectorBailey/byte-tactics/pull/5132> (docs/c2-regalloc.md, saved here as
`web/c2-regalloc.md`) and the allocator section of <https://github.com/HectorBailey/byte-tactics/pull/1127> (`web/agent-guide.md`,
"When the registers or the order won't budge"). MSVC 5 and VC7.1 share the UTC back end: our c2.dll carries the same source files
(`f:\vs70builds\3077\vc\Compiler\Utc\src\P2\color.c`, `tuple.c`, `stack.c`, `dag.c`, `globopt.c`, `inline.c`, `lur.c`,
`x86\fppeeps.c`, `x86\lowerflt.c`, ...; `probe/c2_strings_all.txt`), and every allocator rule they describe that I tested
reproduces on 13.10.3077 (sections 2.1-2.4). Their two rules that do NOT carry over (symbol-id windows, path length) are the
subject of section 1.1. Treat the document as untrusted but well-tested data; the numbers below are ours.

Other web material checked and found empty for our purpose: Microsoft's /G7 documentation
(<https://learn.microsoft.com/en-us/archive/msdn-magazine/2003/march/new-and-upgraded-features-in-visual-studio-net-2003>,
<https://www.devx.com/code-magazine/a-preview-of-visual-c-net-2003/>) says only "optimize for Pentium 4 / Athlon"; the MSVC
hidden-flag list (<https://lectem.github.io/msvc/reverse-engineering/build/2019/01/21/MSVC-hidden-flags.html>, `web/`) is for
19.x and gives no semantics; the swkotor MSVC 7.0 reconstruction
(<https://github.com/mattlgroff/swkotor1-reconstruction/pull/2>) has no codegen notes; the UTC "Universal Tuple Compiler" name
is confirmed by <https://www.infoworld.com/article/4227839/microsoft-doubles-down-on-rust.html>. Agner Fog's manuals and the
comp.compilers archive gave nothing specific to VC7.1 register choice.

## 1. Toolchain identity per row (direction 1)

### 1.1 VC7.1 c2 is function-local and deterministic -- **high**
MSVC 5 decided some ties from the front end's symbol counter (ids modulo 65536, bit 14), which the compile path moved by 4 ids per
character (`web/c2-regalloc.md` "Symbol ids"). I tested the same levers on two register-tie rows:

| lever | rows | result |
|---|---|---|
| 0..70,000 `extern int` declarations before the row (`scan_externs.py`, also 1..5,000 one-member structs = 7 ids each, 1..5,000 prototypes) | 0x0085C7D0, 0x00C52BB0 | every build byte-identical (builtmd5 ab2312eee6 / de54bb91c4) |
| source path length 122..221 characters (`probe/pathlen.py`) | 0x0085C7D0 | byte-identical (md5 b4e8ec893710) |
| 1..20 dummy functions with temporaries and loops before or after the row (PADMODE=prefn/prefp/postfn) | both | byte-identical |

So 13.10.3077 does not carry MSVC 5's symbol-id tie-break, the lane's compile paths cannot cause ties, and a parked row's bytes
do not depend on what else is in the TU unless it is inlined into or called by the row. "Retail's neighbouring functions"
(direction 3) can only matter through inlining / interprocedural information (section 3), never through c2 state.

### 1.2 Processor target switches: /G5 = /G6 = /GB = default; /G7 is different and retail is not /G7 -- **high**
`tu_flag_check.py 0x006AF000 0x006B2000 <flag>` regrades the 50 landed rows of the CTCPhysicsBase / CAtmosProcess TU:
- /G5, /G6, /GB: every landed row still MATCH / RELOCATION_MATCH (byte-identical to the default).
- /G7: 23 landed rows DIFFER (0x006AF1A0 19v17, 0x006B0460 335v310, 0x006B1F30 879v811 ...): /G7 changes inc/dec vs add/sub,
  movzx forms, alignment -- real codegen differences, so retail cannot be /G7 there. The 26 parked rows under /G7: none matches,
  22 change (`tmp/attempts.log`). The 2026-10 reports that "/G5-/G7 change nothing" were row-specific (3 rows here also do not
  change); as a lane rule the /G switch space is {default, /G7} and retail is default. Scheduling residues cannot be a /G target.

### 1.3 /Oa and /Ow: a per-TU flag must be checked against the TU's landed rows -- **high**
0x006AFE60 RespondToCollision is RELOCATION_MATCH under /Oa and (new) under /Ow. But `tu_flag_check.py` shows the same TU's
landed rows break: under /Ow 0x006B1450 GetRHSet and 0x006B1960 ~CAtmosProcess DIFFER; under /Oa 12 rows DIFFER (0x006AF290,
0x006AF2B0, 0x006AF570, 0x006AF640, 0x006AF700, 0x006AFBF0, 0x006B01D0, 0x006B0C10, 0x006B0E00, 0x006B1450, 0x006B1960,
0x006B1F30). Retail's TU was therefore neither /Oa nor /Ow, and 0x006AFE60's flag match is a false lead: the residue is a
may-alias ordering that retail's SOURCE removed (not its flags). The 67 landed rows carrying `/Oa` in the catalog
(CRetailTree copies, CTC* ctors, CAFile parsers, ...) deserve the same check per TU; several sit in TUs whose other rows match
without it, which makes them suspect "flag-fitted" matches rather than evidence of a /Oa TU.

Recommendation: before landing any row on a non-base flag, run `tu_flag_check.py` over the TU's address range; land the flag
only if every landed neighbour survives it.

### 1.4 Other flags
41-flag sweep on 0x0047ED76, 0x006AFE60, 0x0085C7D0 (`sweep_flags.sh`): /Ot, /Oi, /Oi-, /Ob1, /Gy, /GF, /Gf, /GR-, /GX-, /Zp1-8,
/GS, /QIfist, /QIfdiv, /Zi, /Z7, /Gz, /Gd, /Gr (on non-member rows) are byte-identical to the default; /Os, /O1, /Op, /Oa, /Ow,
/EHsc, /Gs0, /Ge, /Gh, /Og- change bytes and none matches. LTCG was already excluded by REGISTER_SWAP_SUMMARY rounds 17-18
(custom conventions) and by chunk 835; the Rich header says 1,369 RTM C++ objects, 5 QFE 4035 objects (GOTCHAS, FINDINGS_LOG
2026-10-20/30). Nothing in the Rich header distinguishes per-object flags; the TU-consistency check above is the only
instrument we have, and it works.

## 2. How c2 decides registers, scratch rotation, constants and frame slots (direction 2)

### 2.1 Callee-saved register roles = priority colouring in table order -- **high**
`probe/prio.cpp`: locals live across calls get esi, edi, ebx, ebp in order of weighted reference count (p1: a 4 refs -> esi,
b 2 -> edi; p2 reversed -> reversed); equal counts go to the value DEFINED FIRST in code (p3/p4); with four values (p6) the order
is esi, edi, ebx, ebp by count with the tie by first definition; one reference inside a loop outranks seven straight-line ones
(p7: the loop value gets ebx ahead of a 7-reference value that gets ebp). This is exactly MSVC 5's `color.c` rule (candidates
sorted by priority, each takes the first allowed register in the fixed table eax, ecx, edx, esi, edi, ebx, ebp; call-crossing
values lose eax/ecx/edx). It explains the lane's observations in REGISTER_SWAP_SUMMARY section 2 ("retail keeps X in esi, ours
in edi"): the two values' weighted counts are in the other order, and a tie is broken by which is written first.

Levers that follow (all plain C++): change the number of references the compiler sees for one value (an extra read in a hot
block, or removing one), move a value's first definition earlier or later (a tie), or move a reference into or out of a loop.
Spelling changes that keep the same reference multiset (aliases, casts, accessors) cannot move it, which is what rounds 11-16
found on 0x008508B0.

### 2.2 Scratch rotation (eax/ecx/edx) -- **high**
`probe/rotation.cpp`: expression temporaries take eax, ecx, edx round-robin from a pointer reset once per function, skipping
registers that hold live values (r1/r3: eax, ecx, edx, eax, ...; r2 with the base in eax: ecx, edx, ecx, edx). MSVC 5's
`regasg.c` rule holds. Consequence: one temporary more or fewer anywhere EARLIER in the function (an inlined accessor, a
folded cast, a hoisted address) shifts every later scratch register -- the "rotation persists for a few instructions then
converges" pattern. Lever: add or remove exactly one temporary before the tie (a named value local, an inline accessor at one
site, `*(begin()+i)`), as REGISTER_SWAP_SUMMARY section 1 already lists; now we know why those work and why they must be placed
BEFORE the site.

### 2.3 Constants in registers -- **high**
`probe/consts.cpp`: `p->f[i] = 0` stores: 1-2 -> immediates; 3+ without calls -> `xor ecx,ecx`; with a call after each store,
1-4 -> immediates, 5+ -> `xor edi,edi` (callee-saved). Identical to MSVC 5's table. "Ours keeps 0 in ebx, retail uses
immediates" (or the reverse) is a weighted-count threshold: the count includes stores the compiler later merges or splits
(cross-jumped tails are counted per arm before merging), so the lever is the number of zero stores visible before tail merging,
not the zero's spelling (typed zeros were already shown to be byte-identical).

### 2.4 Frame slots -- **high** for the mod/ref condition, **medium** for the ordering
`probe/frame.cpp`, `frame2.cpp`, `frame3.cpp`:
- Declaration order of address-taken locals does not move slots (fr1 = fr2 byte-identical; agrees with GOTCHAS chunk 831).
- Address-taken block locals with disjoint lifetimes do NOT share a slot when the address reaches a declared-only callee (v3:
  3 slots) or a visible callee that WRITES through the pointer (v2: 3 slots, even under `#pragma auto_inline(off)`).
- They DO share when the visible `auto_inline(off)` callee only READS through the pointer (v1: one slot for three locals, and
  the values are constant-folded; v4 with `C* const&`: one object slot + one reference slot).
So the chunk 859/860 lever ("give every callee that receives a local's address a same-TU body") works through VC7.1's
interprocedural mod/ref summary of visible bodies: the body must not store through the pointer. STLport `LowerBound(const K&)`,
`find`, `GetDef(const T&)` qualify; `Read(&field)` / output parameters do not -- that is why 0x00AAD650's `Read(&field)` sites
stayed aliased. Expect: dead-store elimination, store->load forwarding (chunk 859 round 2) and slot sharing all switch together.

### 2.5 "Scheduling" is emission order, not a latency scheduler -- **medium**
c2.dll 13.10.3077 has no scheduler diagnostics strings and no `sched` source file; its reordering shows as load hoisting across
statements bounded by the three scratch registers (r1/r3: 3 loads, 3 adds, 3 stores) and as `PEEP: Reordered
conditional/unconditional branch pair`. Prologue pushes of callee-saved registers are interleaved right before the register's
first definition, and the first definitions follow the first uses in emission order (`probe/sched.cpp`: `this` copied after a
parameter load exactly when `this` is first needed later, which is retail 0x00B92300's shape). Therefore the (b)-class residues
("a load one slot early", "push before/after fsubp", "mov ebp,ecx before push edi") are decided by the IR tuple order, i.e. by
which operand/address node was created first and which value is first needed; a /G target cannot move them (1.2), and the
levers are the ones that create or delete a node before the site (2.2) or change what is first needed (2.1).

## 3. Neighbouring functions (direction 3)
Section 1.1 rules out any effect of non-inlined neighbours. The only neighbour effects are interprocedural: a visible body
changes (a) inlining (the lane's frequency/size model), (b) mod/ref summaries (2.4), (c) address exposure (chunk 859). For a
parked row the question is therefore "which of its callees were defined in retail's TU (headers or earlier in the .cpp)", and
the body must be modelled faithfully in one respect: whether it writes through the pointer it receives.

## 4. x87 operand order (direction 4) -- **high** for parameter/member bases, **medium** for call-result bases
`probe/x87order.cpp` ... `x87order4.cpp`, `cross.cpp` (listings in the probe output; summary):

R1. For a commutative op (`*`, `+`) on two register-indirect loads whose bases are parameters or members, the operand with
    the LARGER displacement is `fld`ed first and the smaller becomes the `fmul/fadd` memory operand; textual order is
    irrelevant (f1=f2, f4=f5, h4=h5, h9=h10, h13=h14). Equal displacements: the earlier-declared parameter's operand first
    (h1-h3), a pointer-member / `this`-based indirection ahead of a parameter (e4/e5, g2).
R2. Direct operands (globals, by-value parameters, frame locals) are not canonicalised: the RIGHT operand is `fld`ed first
    (f10 vs f11, h19 vs h20, h21 vs h22), and a direct operand beats an indirect one whatever the text (f8/f9, k5-k8).
R3. A computed (register) operand is evaluated first; with two computed operands the right subtree is evaluated first
    (k9-k12).
R4. Chains and sums of products are flattened and re-sorted by the same key (h28/h29; k20-k22; m1, m5-m7, m11-m13):
    parentheses, a named float local, `(float)(double)`, `*1.0f`, `const float&` do not pin the order; a store to memory
    (global, `*out`, by-reference parameter) or a call between the terms does (m2-m4, m14). `-` and `/` keep textual order
    (m8/m9, k13-k15); `-(a*b)+c*d` is re-sorted as an addition (m7).
R5. Pre-offset pointers `pb = &b.Y; pb[0]` / `pb[-1]` are folded to `[reg+disp]` before the sort (k17-k19): they move nothing
    unless the pointer survives as a variable (the lane's successful `*(&b.Y - 1)` cases must be of that kind).
R6. Off a register base that is a CALL RESULT (0x00986ED0's orientation), the per-product order is not R1: seven spellings of
    the same cross product give three different load orders (`probe/cross.cpp`), none retail's; the order follows the IR
    creation / CSE history of earlier products in the same expression (the lane's "an element-address operand order can come
    from an earlier loop's IR numbering"). This class is the least predictable; the practical lever is to change which
    operand node is created first (reorder the TERMS, not the factors; or bind one vector to a named reference whose first use
    precedes the other's).

This explains chunk 857's "VC7.1 reorders X*X+Y*Y: split it into two statements" (only a store or call pins it, per R4) and
chunk 847's "textual fmul operand order has no effect".

## 5. Block layout: loop-head alignment -- **high**
`probe/align.cpp`, `align2.cpp`: VC7.1 /O2 pads the head of a counted `for` loop to 16 bytes only when the pad needed is small
(observed 4- and 6/7-byte pads; heads needing 10, 12 or 14 bytes stay unaligned), never pads `while (!call())` / do-while-with-call
loops, and never pads under /Os. The decision depends on the head's address modulo 16, i.e. on every byte before it. So in
0x00E72630 retail's two 16-byte pads are consequences of the single missing 4-byte dead store at entry (a conditional-temporary
flag home, GOTCHAS chunk 845); fix the earlier byte and the pads follow. General rule for (d)-class parks: a pad difference is
never the residue, the first byte difference before it is.

## 6. Ranked remedies for an authoring agent (plain C++ only; each backed by the probes above)

1. **Check the flag, not the row**: before trying a flag match, `tu_flag_check.py <TU lo> <hi> <flag>`; a flag that breaks a
   landed neighbour is not retail's (1.3). Never spend grades on /G5-/G7 (1.2).
2. **Callee-saved role swap (esi/edi/ebx/ebp)**: compare the two values' reference counts in retail vs ours (loop references
   weigh ~4-8x); add/remove one real reference, or change which value is defined first (2.1). Aliases, casts and accessors are
   dead ends (rounds 11-16).
3. **Scratch rotation (eax/ecx/edx)**: count temporaries from function entry to the site; add or remove exactly one BEFORE it
   (named value local, inline accessor at one site, folded cast) (2.2).
4. **Zero in a register**: count the zero stores the compiler sees before tail merging; cross the 3 (scratch) / 5 (callee-saved
   with calls) thresholds by writing the stores out per arm or merging them (2.3).
5. **Frame-slot sharing / dead stores / reloads**: give every callee receiving a local's address a same-TU
   `#pragma auto_inline(off)` body that does not write through the pointer (STLport find/LowerBound/GetDef shapes); a writing
   callee or a declared-only one keeps the slot and the stores (2.4). The reverse lever reproduces a retail dead store: make the
   callee declared-only / writing.
6. **x87 pair order with parameter/member bases**: compute the retail order from R1/R2; to flip a pair, change the displacement
   relation (access through a real pointer variable, a by-value copy = direct operand -> textual order applies), or pin a sum
   with a store/call (R4). Do not try textual swaps, parentheses or identity casts.
7. **x87 pair order off a call-result base** (R6): reorder TERMS or name one operand vector before the other; budget it as a
   low-probability park.
8. **Scheduling slot of one instruction**: treat as IR-order; the levers are 3 and "what is first needed" (prologue order
   follows first use), not flags.
9. **Alignment pads / block layout**: fix the first byte difference before the pad; a pad never needs its own spelling (5).

## 7. Residue classes probably unreachable from C++, and why
- **R6 orders in large expressions** whose operands come from call results: the order depends on CSE history of nodes we cannot
  see; a few rows (0x00986ED0, 0x009FA780 / the SetRenderState bookmark flips) may stay parked unless the full retail expression
  shape is guessed.
- **Inline-budget splits** (0x00D18DF0, 0x00ED4B40, 0x00EC4A00, 0x00E799F0): already documented as budget/threshold effects
  (`/d2inlS`/`/d2inlT` reproduce some); not a register/scheduling question and outside this chunk's experiments.
- **Ties at equal priority where the tie-break is "last write tuple number"**: reachable in principle (move the definition), but
  only by changing statement order, which the row's semantics may not allow.
- Everything else in classes (a)-(d) is reachable once the deciding count is known; what is missing is INSTRUMENTATION, not
  levers: byte-tactics reads the allocator's own priorities from C2 under a debugger (`c2prio.py`). The same is feasible here:
  c2.dll's `color.c` assertion sites are at 0x107C1C73..0x107EE698 (28), `stack.c` 0x107E2C04.., `tuple.c` 0x107E3D82..
  (`probe/c2_assert_sites.txt`); `/B2` points cl at a copy of c2.dll and winedbg --gdb can break there. That tool would turn
  register-tie parks from guesswork into arithmetic and is the single highest-value follow-up.

## 8. Recommendations on flags / LTCG / neighbours
- Flags: default /O2 /Oy (plus the TU's proven /Gr, /GS, /EHsc, /Os where the TU shows it). Do not use /Oa, /Ow, /G5-/G7, /Op as
  landing flags; /Oa rows already landed should be re-checked with `tu_flag_check.py` per TU.
- LTCG: no (custom conventions; rounds 17-18, chunk 835).
- Neighbours: only inlined / visible-body neighbours matter; model their mod/ref behaviour (does the body write through the
  pointer it receives?) rather than adding them wholesale.

## 9. Per-row notes (no row moved; see STATUS.tsv)
- 0x006AFE60: /Oa and /Ow match but are refuted for the TU (1.3). The alias constraint retail did not have is between the
  `*velocity` loads and the def loads; a source shape that makes the velocity reads provably distinct (e.g. retail's CDefPointer
  Get() through a different base type, or velocity copied by value first) is the remaining direction.
- 0x00E72630: dead entry store = conditional-temporary flag home; both pads derive from it (5).
- 0x0085C7D0 / 0x00C52BB0: insensitive to every environmental lever (1.1), so the tie is inside the IR (2.2/2.5).
- 0x00986ED0: R6 class; `probe/cross.cpp` enumerates seven spellings and their orders.
- 0x0047ED76: fsubp/push slot; IR order (2.5); not a flag.
- Chunk 860's 0x005AECC0 (4-byte slot gap): check that every same-TU body given to the address-taking callees is read-only
  through the pointer (2.4); a writing body is as bad as none.
