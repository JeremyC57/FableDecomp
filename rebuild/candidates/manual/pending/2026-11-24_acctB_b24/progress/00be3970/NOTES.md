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
