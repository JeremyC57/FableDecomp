# chunk 4006 lessons (CTCInventoryStats::ConstructPersonalityListPC 0x005D1F0C, parked at DIFFER 17880v17873)

- (frame) This TU's biased frame (`push ebp ; lea ebp,[esp-74h] ; sub esp,N`) needs `#pragma optimize("s", on)` ALONE; the landed twin
  0x005D7081 carries `optimize("y", off)` as well, which gives `mov ebp,esp` here (18162 bytes).
- (dead lookup) An unused `T* p; ParentThing->GetTC(TCI_x, &p);` compiles to just the bitset test with `mov eax,[this+4] ; lea ecx,[eax+20h]`
  (0x005D1F66); `HasTC()` gives `mov ecx ; add ecx,20h`.
- (statement shape) Sums printed by a line are named locals only where retail keeps them in a frame word (EXPERTOTAL, renown level +
  percentage); elsewhere the getter calls sit inside the expression. `(float)level + percentage` with two named locals avoids `fiadd`.
- (allocator) After the callee-saved registers are taken, c2 splits every candidate live across calls in one pass; split pieces get
  priority K * cost with K counted AFTER the split, while a block-local CSE temporary keeps its pre-split K. A block that also holds
  single-reference split candidates (here the title statement's `bank`, `container`, `true`) therefore ranks the temporary above the
  piece. Any tuple-block boundary between the two equalises K and the piece wins the tie by key (0x005D2F77; tools/pj.py prints the
  per-pass block shares from a c2prio JSON trace).
- (allocator) Zero-code source constructs (empty inline call, `do {} while (0)`, constant `if`, zero-trip loop, `switch (0)`, unused
  label, foreach-macro idioms) leave no block boundary: they are folded before allocation. A forced-inline helper around a loop
  leaves none either.
- (tool) pd.py-style diffs mask call targets; a frame-masked-equal function can still call the wrong callee. Pair relocations
  (tools/rel.py). rel.py's text matcher slides where retail and ours use frame offsets of different sign (`[ebp-20h]` v `[ebp+1Ch]`)
  and then reports "several targets" that are not real -- check the listed retail addresses by hand before believing them.
- (test) For a long straight-line row, cut the retail disassembly into statements at the calls that end them (here the CWideString
  assign / append and the title call) and compare, per statement, the sorted literals, semantic calls, virtual offsets and member
  reads with what the row's own statements do over a logging mock (tools/rtok.py, tools/gentest.py). Sorting inside a statement
  makes the check independent of g++ / VC7.1 evaluation order; one run must take every arm and every loop once. 16 of 16 single-edit
  mutations fail.
- (names) reloc_bindings.tsv binds three callees of this row to stand-in spellings of other landed callers (NAME_FIXES.tsv); the
  PDB manglings are unbound until a caller lands with them.

## Continuation agent (row 0x005D1F0C matched: RELOCATION_MATCH 17873 v 17873)
- (same-TU body, 0x005D2F77) The allocator residue the first agent traced to "a tuple-block boundary after the title statement" was a
  missing same-TU callee body: with `CTCInventoryStats::ConvertMap` (0x005CD6B7, compiled before the row in retail's TU) DEFINED ahead of
  the row, the list iterator's entry piece takes eax and the loop entry merges into the loop bottom as in retail. 40 loop / zero-code
  spellings had not moved it. Try the same-TU callee that receives the local's address FIRST. The body over the shared headers grades
  RELOCATION_MATCH at its own address with the existing FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ opt-in (free model check: `q.py 005cd6b7
  <row source> '?ConvertMap@CTCInventoryStats@@'`).
- (frame packer, inline-asm weight) Every escaped temporary of a statement that contains an inline-asm helper (GFFloatToLong*) gets +2
  frame-packer weight (two increments from c2.dll 0x107E25C9, one per asm instruction). A statement whose temporaries sit in the words
  weight-2 temporaries get (retail 0x005D4BA5..0x005D4C19: FISHING) therefore does NOT contain the helper: the rounded value is a named
  float in its own statement (`float biggestFish = GFRoundFloatNear(..., 2);`), which also costs no code (the value stays on the x87
  stack). 342 displaced frame references closed with that one statement. Read "which slot each temporary of the odd statement joins" off
  the retail frame words first (tools/fw.py, tools/pk/show.py, tools/pk/wl.py).
- (hand-expanded helper, 0x005D50B0) `(float)GFFloatToLongNear(x * 10.0f) * 0.1f` and `GFRoundFloatNear(x, 1)` compile to the same bytes
  (c2 folds the one-trip loop and turns the constant divide into `fmul 0.1f`) but leave different no-code tuples, so the scheduler's
  81-tuple region cuts fall elsewhere for the next 1,100 tuples. When a "load two instructions late" flake wanders with unrelated edits,
  list the region cuts (tools/sched.sh + tools/sched/cuts.py, sdiff.py): a retail cut inside a lookup (0x005D4EB3 `push eax ; mov ecx,
  [bank] ; lea ; push ; call`) pins the tuple count, and the difference is a missing or extra no-code tuple upstream -- look for a
  hand-expanded inline first.
- (shared inline body, parenthesised return) `return (GFFloatToLongNear(value * scale) / scale);` in GFRoundFloatNear leaves one PAREN
  tuple after the divide; the unparenthesised landed body is 5611 / 5617 here, the parenthesised one 5616 / 5617 (and 0x00AFBCE0 still
  RELOCATION_MATCH; the out-of-line GFPackFloat copy still RELOCATION_MATCH at 0x00AF8D50). `return (expr);` is a common house style:
  try it on shared float inlines when only scheduling differs.
- (SIB order = record ids, 0x005D4698) c2prio with the record id added to the leaf info (tools/c2p/c2prio.py `sid`) shows which operand
  record is smaller; the smaller id is the SIB index. Two inline getter results added directly get ascending ids (left smaller); one more
  inline level on the left operand frees a record the right operand then reuses, which swaps base and index with no other byte change.
  Row-level spellings (25 tried) cannot do that. Both personality rows show the same retail order, so it is not an id wrap.
- (tools) Tags starting with `h` are row 2 in cc.py / pd.py (a probe tagged `hb` was diffed against the wrong row). A nid in a
  `--tracew` packer trace is first-seen order, in a plain trace list order: use tools/pk/wl.py for list positions.
- Review at landing: (1) the FITTED inline `CTCHeroStats::GetNumberOfBuildingsOwned` (descriptive name, in no PDB; FableWin's later
  revision lists no such helper) -- the bytes prove an extra inline level on the houses side, not which form; (2) the changed SHARED body
  in engine/GFPackFloat.h (parenthesised return); (3) the same-TU ConvertMap body in the row file (pair-landing with 0x005CD6B7: the
  landed row uses local stand-in classes); (4) the predecessor's GFRoundFloatNear.h was a duplicate of the shared helper and is gone.
