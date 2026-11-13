# Chunk 2004 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2004, 0x009609B0) A local whose address goes to a same-TU template search (`GetPBestThingInRoughArea(..., &thing)`) stays in a
  callee-saved register with a frame home only when the search body is visible (FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY);
  declaration-only keeps it in memory (1347 v 1352). A trivial stand-in body proves the lever but shrinks the frame: use the real body.
- (chunk 2004, 0x009609B0) `cmp eax,edi` with the comparand loaded BEFORE the GetPItem call is the free FableWin comparison
  `pointer == item` (FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW): the item is an argument, evaluated first; `&x == p.Get()` gives `cmp edi,eax`.
- (chunk 2004, 0x009609B0) Two arms ending in the same `ThingReactingTo = X;` show up as `push edi ; jmp` into the other arm's copy
  (cross-jumped tails): write the statement in both arms, not through a temporary variable.
- (chunk 2004, 0x00912BA0) A visible noinline STLport `_M_push_back_aux_v` (the retail TU holds it) changes the inlined push_back's
  12-dword copy of a local from load/store alternation to load-ahead; overlay opt-in FABLE_RETAIL_STLP_DEQUE_PUSHBACK_AUX_BODY.
- (chunk 2004, 0x009FED30) `X tmp = f().g(); if (tmp == a && b)` destroys the f() temporary before the test (retail); writing the call
  inside the condition makes VC7.1 materialise a bool across the temporary's destructor (+27 bytes).
- (chunk 2004, 0x009FED30) A mip-reduction loop with no aligned head (`test eax,eax ; jbe ; L: ... dec eax ; jne L`) is a down-counting
  `for (n = level; n > 0; --n)`; the up-counting `for (i = 0; i < level; ++i)` adds `jmp` + `lea ecx,[ecx]`.
- (chunk 2004, 0x00725500) Which components of a copied C3DVector go through memory (`mov eax,[esi+8Ch] ; mov [esp+..],eax`) and which
  stay on the x87 stack across `__ftol2` depends on where the copy is declared: after the two magnitude computations and right before
  the `(long)(speed * 14.0f)` statement gives retail's Y-only copy (fragment probes v1..v9, no grade needed).
- (chunk 2004, 0x00725500) A byte-register flag losing its register to a reference parameter: c2prio showed collided 16 (memory) v pos 35
  (ebx); assigning `collided = true` in each of the two non-cylinder arms (c2 hoists the two stores into retail's single `mov bl,1`)
  raised it to 60 and gave retail's allocation (pos and the vector split into pieces).
- (chunk 2004, 0x00725500) `if (!collided) {...} else if (t) f = false;` followed by `if (!collided && special) ...` moves the else arm
  out of line behind the loop's later code (retail `jne` to 0x007259C7, `jmp` back to the counter); with the special test inside the
  then-arm the else arm stays in place (layout probes l1..l10).
- (chunk 2004, 0x00725500) `fld Y ; fadd X ; fmul 0.0 ; fsub Z` is `0.0f - n.Dot(C3DVector(0,0,1))`; `n.Dot(C3DVector(0,0,-1))` keeps a
  `fmul 1.0` on Z (VC7.1 rewrites `+ Z * -1` as `- Z * 1` after its x*1 folding).
- (chunk 2004, 0x00725500) A loop-local `C3DVector old = pos; pos = n; moved += pos - old;` is scalarised (old.X / old.Y kept on the x87
  stack); retail's full int copy to memory is a function-scope `C3DVector old_pos;` assigned in the loop.
- (chunk 2004) md.py (relocation-masked instruction diff with optional register / frame masking) and rel.py (each relocation of the
  built leaf against the retail target and the bound spellings) in this directory are quick checks before vland.

Review at landing:
- 0x00725500 used 10 graded variants in pass 1 plus one comment-only confirmation grade of the final src file (identical code).
- 0x009609B0 grades retail_asm (shared GFFloatToLongCeil helper; the row carries the `// Retail-asm:` evidence line purity.py needs).
- Overlay headers: 7 new headers + opt-in blocks in 16 copied repo headers (hdr/README.md); re-copy HEAD headers and re-apply before merging.
- Landed spellings to respell (NAME_FIXES.tsv), notably 0x00864620's GetExtendedSightRadius binding to 0x007448F0 (GetSightRadius) and
  the landed self rows 0x00912700 (FlyingMethods::GetRandomFlightPos, pointer out-parameter) and 0x00960FC0 (operator()(CThing*)).
- vland RELOC_NAME: 0x0129BA3C is labelled 'EPSILON' somewhere while the shared header (and this row) use DELTA_NOTIONAL_ZERO.
