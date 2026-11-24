# 0x00BE3970 CLandscapePatchTesselator::TesselateEdge -- B24C4 (2026-11-24): ported, baseline only
- The hand873 draft used an overlay made from 2026-11-08 headers (pending/2026-11-08_linux_hand873/hdr_round2). Re-applied onto the
  CURRENT headers (hdr/): CPatchTesselationEdgeStrip.h FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDVERT (FindVert + PeekVertexCount,
  FableWin names; FindVert body kept __forceinline as hand873 had it -- to be justified or removed), CLandscapePatchTesselator.h
  FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR_EDGE_HELPERS (the three FableWin helper declarations), and the water rows' (B24C2)
  _quarantine/CEngineLandscapeMap.h FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP. The vertex layout and SetTargetVertexNormal now
  come from the repo's FABLE_RETAIL_LANDSCAPE_BUILD_DEFAULT_SECTION_PASS28 (replaces hand873's _HELPERS vertex class and
  _VERTEX_NORMAL). Source: v1.cpp = differ/00be3970.cpp (defines + include path changed only).
- Baseline grade (ATTEMPTS.log): DIFFER(2399v2390) hunks=51 -- the same 2399 bytes hand873 recorded (its "13 opcode hunks"; fg also
  counts register-only hunks). Residue as hand873: retail keeps the loop-top vertex pointer (`mov ecx,[esi+8] ; lea eax,[edi+edi*4] ;
  ... add ecx,eax ; mov [esp+38h],ecx`), ours keeps Verts.Data in edi and indexes it; everything downstream is register drift.
- Lessons to read first (hand873 NOTES / LESSONS in prior/): TE_AXIS variants, GetMainAxisVertexWorldPos spellings tried.
- Next: hand-author the loop top: a `CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(i)` reference taken before the
  start/end tests (retail computes &Verts[i] once, stores it, then reads +8/+0xA through it), then FindVert's loop form.
## B24C5 (2026-11-24)
- Tools: c5/g.sh <src> [note] (fg grade + our listing c5/<name>.dis + instruction diff c5/<name>.dd v c5/retail.dis; OV= overlay).
- FindVert rewritten to the FableWin-exact while-loop text (fwsize 48i/140b SAME; the for/break text was the same release code):
  c5/hdr/engine/CPatchTesselationEdgeStrip.h (probe macro FABLE_PROBE_FINDVERT_INLINE). FABLE_FORCEINLINE is REQUIRED: plain `inline`
  leaves one FindVert call (f2: 2364 bytes, 21 calls v retail 20) -- this is the evidence for the force-inline (with FableWin's out-of-line
  debug copy 0x02CBD3D0 and no Ego_r proc record).
- Helpers to FableWin sizes: AddFanBaseVertex with a named `CVertex& vert = strip->GetVert(index)` = FableWin 56i/172b SAME (a1, no
  release change); AddCompositeVertex with `CVertex& vert = strip->GetVert(index); CVertex& next = strip->GetVert(index + 1);` (vert. /
  next. instead of vert[0] / vert[1]): 129i/395b v FableWin 133i/407b (old 123i/377b) -> 51 -> 35 dd blocks (a2). x87 order unchanged.
- Not-found arm `previous = -1;` BEFORE the index store (retail 0x00BE4294 then 0x00BE429C): b0, store order now retail's (35 blocks).
- No effect / worse: vert as pointer (b1 35 = same), vert after start/end (b2 40).
- Residue mechanism: retail computes vert = Verts.Data + i*20 as its own value, homes it at [esp+38h] and reads it at the end
  (0x00BE4252 / 0x00BE4281: `mov eax,[esp+38h]`), reloading Verts.Data (esi) for start. Ours folds vert into the end uses as
  [Data + i*20 + off], so Data is homed at [esp+38h] and i*20 at [esp+20h]. Either retail's IL has two Data loads (not CSE'd) or c2
  did not fold vert; source cause not yet found.
- (B24C5) BIG MOVE: CEdgeStipVertexArray<T>::operator[] with a release ASSERT dead arm (FableWin.pdb: every
  ??A?$CEdgeStipVertexArray@..@@QAE / QBE operator[] is 73i/245b = the plain body + ONE assert; B24C3's /Od probe ../00be4660/c3/dbg:
  `index < Count` = 73i/249b, the closest; `index < Size()` 73i/251b, `index >= 0 && ...` 75i) -> 35 -> 25 dd blocks (c5/e1.cpp,
  probe overlay c5/hdr/engine/CEdgeStipVertexArray.h, FABLE_RETAIL_ASSERT_DEAD_ARM + FABLE_PROBE_EDGESTIP_*). Only the NON-const
  operator[] matters (e5 = 25, e6 const-only = 35); both kept (FableWin has both). FABLE_RETAIL_ASSERT v _ARM: same; `index < Size()`
  / `index >= 0 && index < Count`: 27. Everything from 0x00BE3AC9 to the epilogue now matches (body, helpers, x87, tails).
- e1 residue (25 blocks, 2392 v 2390), all in the head: (1) the hoisted -1 constant is ebp (ours) v edx (retail), so ours pushes ebp
  in the prologue, retail at 0x00BE3A50 just before the loop (and pops it first); (2) FindVert: ours shares the zero (`xor eax,eax`
  before the adjacent_strip test, adjacent_index = eax) where retail stores `mov [esp+1Ch],0` and zeroes eax inside FindVert after
  `mov edx,[ebx+0Ch]`; (3) loop top: ours now materialises vert (`lea esi,[eax+edi]`) but computes start through it; retail reloads
  Verts.Data (esi) for start / end.
- No effect / worse under e1: CArray INDEX_ASSERT dead arm (p3 27), assert after vert in TesselateEdge (p1 25 = same; without the
  operator[] arm p2 35), loop-top orders q1 (71) / q2 (39), vert pointer q3 (25 = same).
- (B24C5) c2prio traces (c5/prio_e1.txt v prio_b0.txt): facts (1) and (2) are colouring consequences. e1: const 0 gets a split piece
  in eax (#113, the zero sharing); the main-loop strip piece takes edx (prio 65, allowed ..d...p, first in table order) and i's piece
  ecx, so the const -1 piece is left only ebp (-> push ebp in the prologue). b0: i piece esi, strip piece ecx, -1 piece edx. Retail:
  strip esi, i edi, -1 edx. The interference set at the loop head comes from fact (3) (which values are live there: ours keeps Data
  (edi) for `end` and vert (esi) for `start`; retail keeps only the reloaded Data), so (3) is the root to solve first.
- (B24C5) More no-effect / worse under e1: dead-arm asserts placed in TesselateEdge at the start (h1 25 = same), after FindVert (h2 27),
  before the loop (h3 27); vert taken inside the span else-arm (r2 72).
## Exact state at hand-back (B24C5)
- Best: differ/00be3970.cpp = c5/e1.cpp, DIFFER(2392v2390), 25 dd blocks, all in the head (0x00BE3991..0x00BE3AC5 + the ebp pop);
  grade with FABLE_HDR_OVERLAY=c5/hdr (repo-current headers + FINDVERT (now the FableWin-exact while loop) + EDGE_HELPERS +
  GETEDGESTRIP + the CEdgeStipVertexArray.h probe opt-in). Helpers: AddFanBaseVertex named vert (FableWin-exact), AddCompositeVertex
  vert/next refs. AddPoly / helpers still carry bare `__forceinline` in the row: before hand-back use the shared AddPoly
  (engine/CLandscapeTesselatorBase.h FABLE_RETAIL_CLANDSCAPETESSELATORBASE_ADDPOLY, the water rows' overlay) and FABLE_FORCEINLINE with
  the evidence line (plain inline leaves calls; FableWin has out-of-line debug copies; Ego_r has no proc record).
- Before hand-back the probe macros must become real opt-ins: FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT (CEdgeStipVertexArray.h,
  evidence: FableWin operator[] 73i/245b, `index < Count` 73i/249b) and FINDVERT with FABLE_FORCEINLINE (c5/hdr is probe-only).
- Next: fact (3), the loop top. Retail: `ecx = Verts.Data; eax = i*20; ecx += eax; [esp+38h] = ecx (vert)`, then `esi = [strip+8]`
  again for start / end with eax = i*20 shared. Find what makes VC7.1 not reuse vert's address for start (start/end spelt through a
  path that is not the same CSE expression as GetVert(i), e.g. a different accessor body text in GetMainAxisVertexWorldPos /
  GetVert that is still FableWin-exact at /Od), then re-check (1) / (2), which should follow.
- (B24C5, late) KEY PROBE (not hand-back): with `vert` made memory-resident by an escaping address (c5/m2.cpp: pointer vert +
  `probe_addr = &vert; *(volatile int*)&probe_addr;`) our loop top becomes retail's EXACTLY: `mov esi,[strip]; mov edi,[i]; or edx,-1;
  mov ecx,[esi+8]; lea eax,[edi+edi*4]; shl eax,2; add ecx,eax; mov [vert],ecx; mov cl,[esi+29h]; test cl,cl; mov esi,[esi+8];
  mov [i*20],eax; ...` (esi = strip, edi = i, edx = -1, Data reloaded for start/end). So retail's IL has a memory barrier / aliasing
  store between vert's Data load and start's. The escape junk itself costs other blocks (m2 122). Closed: unused address-of (m1 = e1),
  dead-arm escape `if (0) ProbeEscape(&vert)` (m3 43), `FABLE_RETAIL_ASSERT(&vert)` (m4 = e1), `CVertex* volatile vert` (m5 36),
  other dead-arm conditions (e3 / e4: no reload). Retail has no `lea r,[esp+38h]` anywhere (vert's address never escapes in code).
  Next: find the natural statement that puts a store / barrier there (a store through a pointer the compiler cannot disambiguate from
  strip->Verts.Data, written between `vert` and `start`; or vert living in a struct / iterator object) -- read the Ego_d / FableWin
  line records on the owner's box if possible (TesselateEdge 0x02CBCC00 locals: is vert a CVertex*, CVertex&, or something else?).
## B24C6 (2026-11-24)
- Baseline regrade: c5/e1.cpp under c5/hdr = DIFFER(2392v2390), 25 dd blocks (unchanged). Tools: c6/g.sh (= c5/g.sh writing to c6/).
- /Od oracle (fwsize, FW_EXTRA=/EHsc, c5/hdr): TesselateEdge ours 283i/854b v FableWin 458i/1600b (+175i, unaccounted: asserts + EH
  frame?); AddCompositeVertex 129i v 133i; all small accessors SAME. FableWin address order after TesselateEdge 0x02CBCC00: FindVert,
  GetVert, GetFanBaseVert, GetMainAxisVertexWorldPos, PeekVertexCount, PeekIsVertical, PeekEngineMap, AddPoly, AddFanBaseVertex,
  AddDuplicateEdgeVertex, AddCompositeVertex (ours emits in definition order; PeekIsVertical only after AddCompositeVertex).
- Real opt-ins built (hdr/, the B24C4 overlay kept as hdr_b24c4/): CPatchTesselationEdgeStrip.h FINDVERT (FableWin while loop,
  FABLE_FORCEINLINE + evidence comment) on top of the twins' FINDWATERVERT_EARLYOUT header; CEdgeStipVertexArray.h
  FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT (Inferred, evidence in the header); CLandscapeTesselatorBase.h ADDPOLY (twins');
  CEngineLandscapeMap.h GETEDGESTRIP; CLandscapePatchTesselator.h EDGE_HELPERS; CWaterPatchTesselatorBase.h (twins'). c6/s1.cpp
  (no local AddPoly, helpers FABLE_FORCEINLINE) = byte-identical to e1: DIFFER(2392v2390) 25 blocks. Twins on it: water 16 / sea RM.
- Clean escape probe x1 (pointer vert declared before the loop, &vert passed to a declared-only function AFTER the loop): loop top
  UNCHANGED (still CSEs through vert; 47 blocks from the extra call). So address-taken-ness alone does not make the reload: m2's
  effect came from its volatile access between vert and start (a barrier), which emits a load retail does not have.
- x2 (vert after start/end, under s1): 71 blocks, Data loaded once and vert = Data + i*20 CSE'd -- no.
- /Od decomposition of the +175i gap (c6/od, probe DBG_ASSERT = ../00be4660/c3/dbg macro): 1 assert +60i/+236b (EH frame), each
  further +53i/+244b; 3 x ASSERT(strip) = 449i/1575b (gap +9i/+25b, condition text). So FableWin TesselateEdge = our statements +
  exactly 3 asserts (a 4th overshoots by ~44i); no live statement is missing. FableWin emits PeekIsVertical in the FIRST group (right
  after PeekVertexCount): our fwsize obj does the same only when TesselateEdge references it directly (b1), so one of the 3 asserts
  is likely `ASSERT(strip->PeekIsVertical() == vertical)` (b1: strip / that / indices = 453i/1588b, gap +5i/+12b).
- Release dead arm of that assert (y1 top / y2 loop top before vert / y3 after vert): 27 blocks each (only an operand-order flip at
  0x00BE3FA3/0x00BE3FAD), head unchanged.
- Compile order (one TU with the water / sea S-form bodies, 3 orders, c6/ord): 0x00BE3970 byte-identical to its one-row compile.
## Exact state at hand-back (B24C6)
- Best: differ/00be3970.cpp (= c6/s1.cpp) DIFFER(2392v2390), 25 dd blocks, on REAL opt-ins (hdr/, README.md); identical bytes to e1.
- Lesson (B24C6): a loop-top value read through a CSE'd pointer where retail reloads the base is NOT moved by: escaping the
  pointer's address after the loop (x1), dead asserts anywhere (y*, z*: only flip unrelated operand orders), compile order with the
  sibling bodies (c6/ord). m2's match came from its volatile access. The head residue stays open; the operator[] arm stays Inferred.
- Next ideas not tried: the prio trace of s1 vs a probe where vert has no register (which candidate takes esi in retail's head);
  c2 IL dump of the loop top (does retail's IL have one Data value? needs a c2 tuple-dump hook, c2probe/c2dbg.py).
- (B24C6, corrections after review) (a) The /Od assert decomposition is NOT calibrated for this function: the same DBG_ASSERT costs
  42i in operator[] (B24C3: 73i v plain 31i, = FableWin) but 53..60i here, so "exactly 3 asserts, no live statement missing" is only
  "3 or 4 asserts, leftover unknown". (b) x1 put the escape after the loop, m2 read the escaped slot inside the loop, so "m2 worked
  because of its volatile access" is one of two readings (inference). Separating probe: m2 with the volatile read moved BEFORE the
  `vert` assignment (if the loop top stays retail's, it is the escape, not the barrier). (c) Also next: AddCompositeVertex is still
  129i/395b v FableWin 133i/407b; moving it toward FableWin was worth 16 blocks before (51 -> 35).
- (B24C6) Separating probe run: c6/m2b.cpp = m2 with the escape (probe_addr = &vert; volatile read) BEFORE `vert = &strip->GetVert(i)`
  (vert still address-taken inside the loop): loop top CSEs through vert again (ours shape; 117 blocks from the junk). So m2's retail
  loop top needs something BETWEEN vert's definition and start's Data load that VC7.1 treats as a memory barrier (here a volatile
  access); address-taken-ness alone does not do it. Retail emits nothing there, so the natural statement (a store / call that the
  optimiser later removes, or a dead arm that survives CSE) is still unknown.

## B24C7 (2026-11-24): not reached (context spent on 0x0070D940). State unchanged from B24C6.
## B24C8 (2026-11-24)
- Baseline regrade (c8/g.sh = c6/g.sh writing to c8/): differ/00be3970.cpp DIFFER(2392v2390) 25 dd blocks (unchanged).
- Coordinator lead "callee body visible" checked: retail's object has BuildBridgeVertexAndIndexBuffers 0x00BE3660, TesselateEdgeGeneric
  0x00BE37E0 and TesselatePatch 0x00BE3910 ahead of the row (manifest), but TesselateEdge calls none of them. Its out-of-line callees are
  in other objects (GetEdgeStrip 0x00BDC980, InsertGrow/_M_insert_overflow 0x005135D0, _M_insert_overflow 0x00B6D0B0) or after the row
  (push_back 0x00BE4380, SetTargetVertexNormal 0x00BE66F0). No landed callee body can be made visible ahead of it: the lever that moved
  0x0070D940 does not apply here (consistent with the brief's Gate-0 note and c6/ord).
- Reading of the head residue (unchanged): retail's loop top loads strip->Verts.Data twice (0x00BE3A61 for vert, 0x00BE3A75 for start /
  end, i*20 shared) -- redundant-load elimination did not merge them; ours merges (start through vert, end through Data #1). B24C5's m2
  probe shows any barrier between the two loads gives retail's loop top. (A join / block-local-CSE reading is refuted by retail itself:
  [esi+29h] and the second [esi+8] are loaded once and reused for `end` after the ?: join at 0x00BE3A8A, so the barrier sits only
  between GetVert and the first GetMainAxisVertexWorldPos.) Open: B24C6's c2 tuple-dump idea; whether retail's TU saw template / inline
  bodies ours treats differently (InsertGrow, _M_insert_overflow, SetTargetVertexNormal) is unanswered -- only "no same-object callee
  ahead" is shown.
- (B24C8) j1: GetMainAxisVertexWorldPos as if/return (c8/hdr_j): DIFFER(2372v2390) 70 blocks -- worse; ternary kept.
## B24C9 (2026-11-24) -- short pass (context spent on 0x0070D940, which reached RELOCATION_MATCH under /d2inlT67)
- Baseline regrade (c9/g.sh = c8/g.sh writing to c9/): differ/00be3970.cpp DIFFER(2392v2390) 25 dd blocks (unchanged).
- p2 (GetVert through a named `unsigned long vert_index = i`): identical (25). p1 (function-scope `CVertex* vert`) did not compile as
  written (other `vert.` uses); B24C5 already tried pointer forms.
- Lessons from 0x0070D940 worth trying here (not yet done): (1) c2prio.py --rotation / --trace on a LINE-SPLIT copy of the row (one
  sub-expression per line; code is unchanged by line breaks) names which IL temp / candidate differs at the loop top -- use it before any
  more spelling probes; (2) VC7.1 evaluates a member call's object expression BEFORE the arguments when the object is itself a call
  (`GetX().F(arg)`), but the argument first when the object is a plain pointer or implicit `this`; an accessor chain written through a
  call can therefore reorder IL (and CSE opportunities) with no other code change; (3) `mov ecx,this` before `push arg` at a join =
  two calls tail-merged from an if/else.
## B24C10 (2026-11-24)
- Lessons applied: B24C9's call-object-first lesson (no call-object chain at the loop top: strip is a plain parameter pointer; the head's
  `map->GetLandscapeMap()->GetEdgeStrip(..)` already evaluates the object first, = retail 0x00BE39BC); the sea twin's match (start through
  a second accessor call instead of the reference); B24A's budget-path mechanism (callee compiled after the caller is expanded from its
  unoptimised IL) as a candidate for the uncombined Data loads.
- Baseline regrade c10/k0.cpp (= differ/) DIFFER(2392v2390) 25 dd blocks. No change: vert = strip->Verts[i] (k1, byte-identical),
  `const CVertex& vert` (e2, 25); vert after start/end (e3, 71 -- as B24C5 b2).
- Both head facts are "retail did NOT combine what ours combines": the loop-top Data load (fact 3) and the adjacent_index zero (fact 2:
  retail `mov [esp+1Ch],0` + a separate `xor eax,eax` in FindVert; ours one zero register).
- Compile-order / budget-path test (c10/runf.sh = B24A5 inlf.py on this row; FORCE regex, FORCE_AFTER=0): forcing
  GetMainAxisVertexWorldPos down the budget path (all 5 sites expanded there, c10/fgm.txt) gives code BYTE-IDENTICAL to k0 -> the
  inliner path / compile order of GetMainAxisVertexWorldPos does not make retail's second Data load. Forcing GetVert + both
  CEdgeStipVertexArray::operator[] too (c10/fgv): operator[] is REJECTED on the budget path (called; 105 blocks) -> not retail either.
  So the uncombined loop-top Data load is not an inliner-order fact; B24C5's m2 (a barrier between vert and start) stays the only
  reproduction. Not escalated for the build mode.
- Row state unchanged: differ/00be3970.cpp DIFFER(2392v2390) 25 dd blocks. Next idea not tried: a c2 tuple dump (c2probe/c2dbg.py hook
  on the CSE pass) of k0 v m2 at the loop top, to see which IL node keeps the two [strip+8] loads apart in m2 and look for a source
  construct that creates the same node without a volatile access.
- (B24C10, KEY PROBE -- read first) Barrier class settled. c10/b1.cpp = k0 + `indices[0] = 0;` (a plain store through the
  unsigned short* parameter) right after `vert = strip->GetVert(i)`: DIFFER(2403v2390) with only 4 dd blocks -- the WHOLE head becomes
  retail's (prologue: no early push ebp; adjacent_index immediate store + separate FindVert zero; loop top: Data reloaded for start/end,
  edx = -1, esi = strip, edi = i). What is left: the probe store itself (2 instructions), the SIB operand order of start's two loads
  ([eax+esi+0Ah] v retail [esi+eax+0Ah]) and one alignment nop. c10/b2.cpp (a declared-only call there instead): 29 blocks (the call
  clobbers registers) -- so a MEMORY STORE that c2 cannot disambiguate from strip->Verts.Data, between vert and start, is the missing
  IL. Retail emits no such store, so it is a store c2 deletes after CSE (dead / redundant store) or one on a path we do not model.
  Next: find the natural statement -- e.g. a store through `indices` or a vertex reference that is overwritten later on every path
  (dead-store elimination after CSE), or a debug-only store whose target is later proven dead; grade each with c10/g.sh and look for
  2390 bytes with the b1 head. (b1 / b2 are diagnostics only, not candidates.)
## B24C11 (2026-11-24)
- Tools: c11/g.sh (= c10/g.sh, writing to c11/, grader output to c11/<n>.g -- no pipe into head). Baseline k0 = 25 blocks.
- Lessons applied: B24C10 b1 (indirect store = barrier); B24C9 call-object-first (n/a at the loop top).
- The "natural dead store" lead is CLOSED by probes (all diagnostics, c11/):
  * local / aliased-local / reference-out / self-assign stores are NOT barriers: e1/e2 (dead local CVertex copy), e3 (*&local), a1
    (local whose address escapes to a call after the loop, stored between vert and start), r1/r2 (inline ref/pointer out-param into a
    dead local), d2/d3/d4 (vert.F = vert.F, vert = vert), q2 (indices[k] = indices[k]): all byte-identical to k0 (25).
  * a branch (join) between vert and start is not a barrier either (j1: start still read through vert) -> CSE is global, not EBB.
  * any store to NON-local memory is the barrier: b1 (indices[0]=0), s1 (vert.U = 0), s2 (strip->Tesselated = true): 4 blocks each,
    the same head; q1 (store to parameter x) 126 (not a barrier, colouring junk).
  * c2 DOES delete an indirect store, but only when the next thing overwrites the same address (q3: indices[0]=0; indices[0]=1 -> one
    store emitted, barrier kept, 4 blocks). Across the [strip+29h]/[strip+8] loads it does not (q4: both kept, 32). Retail has no
    pointer store before those loads and the short-span path writes nothing through a pointer, so no overwriter can exist -> retail's
    construct is not a deleted dead store.
  * Every store barrier gives start's load as [eax+esi+0Ah] (t + Data2) where retail has [esi+eax+0Ah] (Data2 + t): treat that SIB
    order as the fingerprint of the real construct (retail builds start's address differently), not as a leftover.
- Caveat: FableWin's Engine_landscape_tesselator.obj has std::move / std::forward / _Cons_val / _Debug_pointer (VC10-era STL), so
  FableWin was NOT built with VC7.1: the "FableWin-exact /Od size" claims in the headers are approximate; do not chase sizes.
- (B24C11) Fingerprint scan of every old listing (c11/fp.py: Data reloaded after vert? start load operand order) + new probes:
  * Data-first SIB ([Data2+t], retail's) comes ONLY from volatile reads (c5/m2, c11/v1 `*(volatile unsigned short*)indices;`,
    c11/v2 volatile global read) and `__assume` (u1 `__assume(strip != 0)` / u2 between vert and start: the loop top
    0x00BE3A61..0x00BE3A9E is retail's EXACTLY, incl. the late t spill `mov [esp+28h],eax`). Store (b1/s1/s2/q3), call (c10/b2) and
    `_ReadWriteBarrier()` (u3: 2390 bytes, 4 blocks = SIB pair + t spill placed before the barrier) give t-first.
  * __assume is out (owner rule; and it is not local: u1/u2 also change FindVert before the loop and the push_back expansions in the body,
    33 blocks; u6 at the function top = no effect at all; u8 after `end` = other code; u4 `__assume(index < Count)` in operator[] = 138).
    GOTCHAS 359/392: __assume stops VC7.1 forwarding inlined reference args / return values -> reading: retail's start computes its own
    i*20 (inlinee index not forwarded), merged with vert's late; so the missing ingredient is "no forwarding of start's inlined index"
    + a memory barrier, or one construct doing both.
  * No effect / not barriers: scoped object with an inline dtor (o1), address-taken loop index + barrier (h1, 137); spelling sweep on
    the rwb / indices scaffolds (c11/sw: start via (unsigned long)i, `next` reference, end before start, vert via Data+i / (&GetVert(0))[i],
    start via PeekIsVertical()?GetVert(i).WorldY:..): SIB never flips.
  * More no-flip probes: accessor-body overlays c11/ov_{oa,ob,oc} (operator[] via named T& r / *(Data+index) / T* p) and ov_ga
    (GetMainAxisVertexWorldPos via named CVertex& v: 62), ov_gb (via GetVert(index)), each with and without the q3 store scaffold:
    same t-first order; c8/hdr_j if/return accessor with scaffolds (q_j 52, u_j 77): t-first. u7 (__assume after the loop) = 140,
    i.e. __assume changes the loop top from elsewhere: it is a mode, not a statement.
  * Volatile reads (v1/v2) bring the SAME body diffs as __assume (0x00BE39F2 FindVert, 0x00BE3AD3.., push_back at 0x00BE3B24):
    retail's start order and retail's body have never co-occurred in a probe.
- (B24C11) DEBUG BUILD READ (owner's FableWin.exe, /opt/fable_lane/dl/debug_owner; c11/fw/res.py resolves ILT thunks to PDB names,
  c11/fw/str.py reads assert strings; listing c11/fw/te.r, lane only). FableWin TesselateEdge 0x02CBCC00, statement by statement:
  * head = ours; inside `if (adj_strip)`: ASSERT "adj_strip->PeekIsVertical()==is_left_right_edge" then FindVert.
  * loop top: `vert = strip->GetVert(i)` [ebp-40h], `GetVert(i + 1)` stored to [ebp-3Ch] and NEVER read (an unused `next`
    reference), start, end -- nothing else (no store, no call) between vert and start.
  * `if (end-start<=1) previous=-1; else { found=false; fan=-1; if (adj_strip) { while (Main(adj_idx) < end) { ASSERT
    "adj_strip_index<adj_strip->PeekVertexCount()"; ... } if (found) { ASSERT "last_new_edge_vertex>=0"; ... } else {...} } }`:
    the found / not-found tail is INSIDE `if (adj_strip)` (retail 0x00BE3AC9 `je 0x00BE42A0` agrees).
  * operator[] (0x02CBDD10) assert text is "index>=0 && index<Count" (not `index < Count`).
  * Names from the assert strings: adj_strip, adj_strip_index, is_left_right_edge, last_new_edge_vertex (locals; PDB local records
    not read yet: llvm-pdbutil fails on FableWin.pdb "Too many directory blocks").
  * c11/f0 (FableWin structure, no asserts) = 25 (byte-identical to k0); f2 (+ unused next) 27; f3 (+3 dead-arm asserts) 27; f1 (both) 30.
    None reloads Data. __noop (z1/z2) inert. So the debug build shows NO statement that would be retail's barrier: the loop-top
    residue is not a missing statement visible at /Od. Remaining readings: a TLC-era source difference the later debug build lost,
    or a compiler-side cause.
- (B24C11) FableWin helpers checked: AddCompositeVertex 0x02CBD7C0 = ours statement for statement (refs adjacent, vert, next; then
  size / push_back / v; the U/V interpolation identical); no asserts in the landscape helpers. FableWin and Ego_d sizes are equal
  (1600 etc.), and FableWin carries VC10-era STL: treat it as a later build of the TLC tree (path e:\fabletlc\...\fable1_5mainpc).
- (B24C11) The loop-top residue is an operand-order family: with any zero-byte barrier (c11/rf0..rf3 = FableWin forms +
  _ReadWriteBarrier) everything is retail's except the start SIB pair 0x00BE3A7E/85 (+ t spill 0x00BE3A78), and the evidenced unused
  `next` / asserts additionally flip 0x00BE3FA3/AD ([eax+edx] v retail [edx+eax]); B24C6's dead asserts flipped the same pair. So the
  base/index order follows IL temp numbering; retail's numbering differs from every text tried. Volatile / __assume (Data-first at the
  loop top) put VC7.1 into a mode that also changes FindVert and the push_back expansions (gma/gmb: FableWin form + __assume = 35).
- (B24C11) Flags tried on f0: /Ow 141, /Oa 134, /Op 109, /Og- 23 blocks-equivalent hunks (not retail).
## Exact state at hand-back (B24C11)
- differ/00be3970.cpp = c11/f0.cpp: FableWin structure (found / not-found tail inside `if (adjacent_strip)`), byte-identical to k0:
  DIFFER(2392v2390), 25 dd blocks, overlay hdr/. The FableWin-evidenced additions (unused `next`, 3 asserts as dead arms; operator[]
  text "index>=0 && index<Count") are NOT in it because they cost 2-5 blocks (c11/f1/f2/f3, if0..if3) -- they belong in the final text
  once the residue is understood.
- Diagnostics (never candidates): c11/u3.cpp / rf0.cpp (_ReadWriteBarrier after vert: 2390 bytes, 4 blocks), c11/u1.cpp (__assume:
  exact loop top), c11/q3.cpp (overwritten store: 4 blocks).
- Next ideas: (1) c2 temp numbering at the start / 0x00BE3FA3 sites (c2prio.py --trace on f2 v rf2) to see which IL temp order makes
  Data the base; (2) local names from FableWin.pdb / Ego_d.pdb S_REGREL32 records (llvm-pdbutil fails on FableWin.pdb; Ego_d.pdb
  modules dump works: c11/fw/mods_d.txt) -- use the real local names (adj_strip, adj_strip_index, is_left_right_edge,
  last_new_edge_vertex from the assert strings) when the row is handed back.
- (B24C11, correction) "the base/index order follows IL temp numbering" above is a READING, not measured (no c2 trace was run); the
  measured facts are only which texts flip which operand pairs. Constant guard `if (0) return;` before the loop (FableWin's
  `xor ; je ; jmp end`): inert (c11/gd0, 25). Declaring start/end before vert or at function scope on the rwb scaffold (dc1/dc2): no flip.
