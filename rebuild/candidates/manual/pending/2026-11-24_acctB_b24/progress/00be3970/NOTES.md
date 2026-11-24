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
