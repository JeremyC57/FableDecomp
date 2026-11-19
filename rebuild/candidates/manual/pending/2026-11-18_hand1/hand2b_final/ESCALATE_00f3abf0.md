# ESCALATE 0x00F3ABF0 CRangeCompressor::CalcCompressionScript -- DIFFER(605v605), ebx/ebp swap only; a byte MATCH exists with one non-Ego_d local

Best Ego_d-faithful source: differ/00f3abf0.cpp (= probe/f3/base.cpp): fable7 c5 line-table shape (no `left`, double-evaluated
`block_size - n > 4 ? 4 : block_size - n`, ReadVarSizedUInt same-TU body if/if/ASSERT-dead-arm/return, single `return A + B`),
plus the catalog callee model (FABLE_RETAIL_CARRAY + STLP_VECTOR_N_OUTOFLINE + STLP_VECTOR_PUSHBACK_OUTOFLINE: binds
std::vector<unsigned long>::vector(unsigned) 0x00A96100 and std::vector<CScriptEntry>::push_back 0x00F3B420, as the catalog names).

Residue: every instruction equal except ebx<->ebp across the function (retail this=ebp, SR temp=ebx; 20 instructions, first at
0x00F3AC00 `mov ebp,ecx` v ours `mov ebx,ecx`, 0x00F3AC0C `mov ebx,edi` v `mov ebp,edi`). c2prio: this #3 prio 58 key 7 v SR temp #34
56 key 24. Retail needs this <= 56 (tie -> temp by key) or temp >= 58.

NEW (hand2b): the lever is found and proven.
- d1 / d4 are RELOCATION_MATCH (q.py): the fill loop statement as
    { unsigned long& slot = block_buffer[n]; slot = ReadVarSizedUInt(block_ptr + n * element_size, block_size); }
  (d4: unsigned long* slot = &block_buffer[n]; *slot = ...). c2prio: the slot candidate (later forwarded/dropped) is referenced in
  the fill-loop head block (cmp block_size,4: K 2->3) and the join block (K 5->6): this 58 -> 54, registers become retail's.
  With the plain `block_buffer[n] = ReadVarSizedUInt(...)` the element-address temp is forwarded into the store and is not
  referenced in the head block. So retail's IL had an element address (or some candidate) materialised before the inline read,
  live to the store. Ego_d has NO such local (only count, count2, block_buffer, count1, sub_script, script_entry, 3 x n).
- Order is not the cause: at /O2 VC7.1 evaluates the LHS operator[] first in both base and d1 (checked with ReadVarSizedUInt
  noinline); d5 (named value local, RHS first) and d3 (braces only) do nothing.
- Other measured levers (all move code, so not retail): `count = 0` inside the >4 arm (block 1 K 5->3: this 53); script_entry
  declared before the fill loop (block 23 K 4->3: this 56) moves the Flags=0 store before the loop.
- ReadVarSizedUInt shape is settled: every expansion in retail Decompress 0x00F39FD4 / 0x00F3A006 / 0x00F3A030 is plain
  cmp 4 / cmp 2 (no switch, no operand copy).
- No effect (20-instruction swap): CArray indexer models (begin()+i, _M_start[i], INDEX_ASSERT dead arm, ORIGINAL_INDEX_BASE,
  VECTOR_CHAIN, STLP INDEX_START), callee in/out-of-line combos (4), ternary arm orders (5), else-if top chain,
  `element_size * n`, `&block_ptr[...]`, `*(begin()+n) =`.

Best guess at the original: a fill-loop statement whose LHS element reference is held in a real temp across the read -- possibly a
CArray::operator[] body whose release form keeps the reference in a temp (Ego_d's CArray<..>::operator[] is 719 debug bytes:
likely two ASSERT expansions around `return vector::operator[](index)`), or an ASSERT/accessor shape in the release headers that
c1 materialises. Decision needed if nothing better: is the d1 reference local (not in Ego_d) acceptable as the row source?

## Update 2026-11-19 (hand2b, 71 grades; draft, not sent -- under 100)
Coordinator rejected the d1/d6 reference-local match (steering declaration). Kept only as differ/00f3abf0_value_local_REJECTED.cpp.
Base is now differ/00f3abf0.cpp (FableWin-evidenced sum order + ReadVarSizedUInt else chain; same bytes as before).
FableWin.exe instruction reading (0x03319190, module 1063 lines):
- locals are exactly Ego_d's (count, block_buffer, script_entry, count1, sub_script, count2, n x3); no other frame slot but /Od temps.
- all three loops are `if (0) {} else for (...)` -- graded, no effect.
- line 742 calls (block_ptr, 2) first: `CCS(.., 2) + CCS(block_ptr + 2, .., 1)` -- VC7.1 emits the same bytes either way.
- ReadVarSizedUInt: if 4 return / else if 2 return / else { ASSERT(byte_count == 1); return *buffer; } -- graded, no effect.
- CArray<unsigned long>::operator[] = ASSERT(index < size()) + vector::operator[](index) (= FABLE_RETAIL_CARRAY_INDEX_ASSERT),
  CArray(unsigned) -> vector(n), CScriptEntry() stores Flags only; Ego_d callee locals hold nothing extra (egod_all.py).
  INDEX_ASSERT alone / + STLP INDEX_START / + ORIGINAL_FRONT: no effect.
- Include order (owner rule): CArray/assert/Range, CArray/Range/assert, Range/assert: no effect (expected: this is a 58 v 56
  priority gap, not a tie, so ordinals/keys cannot decide it).
- `long left` counter (Codex shape; retail's ebx is block_size, then sub 4): registers become retail's but the copy is
  scheduled before the loop-entry jle (2 instructions at 0x00F3AC0A); for-init form the same; body-local `left = block_size - n`
  = swap. Retail's ebx is the compiler's strength-reduced temp in the preheader, so retail's bytes rule `left` out.
- c2prio screens (no grade): Flags = 0 ctor body, `= CScriptEntry()`, `count = count + ...`, parenthesised / reversed ternary,
  resize() instead of the size ctor, separate count1 assignment, `&block_ptr[...]`, `++n`, `else if (block_size == 3)`,
  `n = n + 4`: all this 58 / temp 56. Only `count` declared inside the >4 arm gives this 53 (but moves the count = 0 store).
Mechanism for whoever resumes: this needs <= 56 (tie -> temp wins by key 24 v 7). this loses w*K in each fill-loop block
(16..21) where it is live without reference; one more candidate referenced in block 21 (or 5/7 in the >4 loop) is enough. Any
source change that does that without changing code or adding an undeclared local closes the row.
