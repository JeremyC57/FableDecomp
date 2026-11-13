# agentC18 (acctC lane) -- NOTES (continues agentC15: prev_C15/)

## Row 1 0x00603000 CTCInventoryMap::CreateMiniMapMarkers -- IN PROGRESS
Base: probe/r1/b0.cpp = C15 differ (ELSE_NULL fitted text) DIFFER(9538v9571) D113 63 hunks; d0 = b0 with the DEFAULT Reset text.
Tools: tools/vq.sh <tags> = inline trace only (Reset f/r first 5 sites, CCountedPointer dtor f/r first 6, # r=0 decisions).
Constraint (default text c1 95; dtor c1 47): interesting site 0x00603838 called iff f <= ~2.97; the IsMapLoaded-return
marker dtor expanded iff f >= ~0.2 (sc 67 at 0.25, 94 at 0.125). d0: base after IsMapLoaded loop 1.25, dtor 0.625,
interesting = base x5 (maps for) x5 (list for) /2 (materialised bool of the || chain with CCharString temps) = 15.6.
- w1 outer creatures maps loop as while: no change (while containing a loop = for). w2 inner list loop as while: 28.1 (9x).
- Window (measured, c2 trace): interesting Reset called iff f <= 2.97 (sc 73 at 3.125 = expanded); the creature-site
  CVectorMap::LowerBound (same block frequency B/2; retail EXPANDS it at 0x0060363F) expanded iff f > ~1.85 (sc 77 at 1.5625,
  71 at 2.34) -> creatures loop body B in (3.7, 5.94]; IsMapLoaded-return marker dtor expanded iff f > ~0.20; inner list loop
  must stay for-type (while-type: the creature list's _List_base dtor after it drops to f 1 and is called; retail expands it).
- Loop forms measured (dtor@IsMapLoaded-return / base after / interesting f): d0 for+return 0.625/1.25/15.6; for+flag+break (L1)
  and for(;i<n&&loaded;) (X4) 0.94/0.94/11.7; while+return (j1, X6, F2, F7) and search-while+if (L2) 0.125/0.125/1.56 (LB called);
  guard+do-while (F3) 0.0625/0.1875/2.34 (creatures = retail, only the dtor called); if/else empty check (L4) = d0;
  do{if(!x)return;}while(0) inside for (M1) = d0; inside while (M2) 1.125/0.25/3.125 (sc 73, 1 short); guard+do+macro (M3)
  0.625/0.25/3.125. Inner list loop as while (w2) 9x; outer maps loop as while (w1) = for; guard on inner list (G1) /2.
- reset() (FableWin name, header `{ Reset(data); }`) at the interesting site: the nested Reset is re-decided in the caller at the
  same f (expanded) -> no. RESET_CALLS_DTOR text (existing opt-in, from out-of-line 0x00429C6F): c1 95 = default.
- Reset text c1 must stay 95 (only the default text expands like retail; T6 `(void*)data` 96 changes the expansion).
- Research doc rebuild/candidates/manual/batches/2026-11-03_inline_split_research/FINDINGS.md: score = (c*1e8/f + (k-1)c)/k
  integer math; sc 73 at f 3.125 (c 95) -> inline; need f <= 2.968. LowerBound effective c ~87.5 (loop callee) -> expanded iff
  f > ~1.86. Per-site sav[5]=2 = `this` is &local (worth 3 cost units).
- More loop forms: do{while+return}while(0) (N1) 1.25/0.25/3.125; do{for+return}while(0) (N4) leaks x5 (base 6.25).
- Graded (default text): d0 DIFFER(9666v9571) D701 304 calls (interesting expanded); F3 DIFFER(9522v9571) D473 303 calls
  (interesting called, IsMapLoaded-return dtor called instead of expanded).
- Nested `if (allLoaded) { rest }` (NFa/NFb): the not-loaded path then jumps straight to the end cleanup (no
  `test edi,edi; je; push edi; jmp 0x60552c` copy as retail 0x0060347A) -> retail has a real `return` there. Ruled out.
- RESET COST evidence (key): row 2 (0x005FF190) inline trace with the default text: 9 Reset sites f = 0.25, 6.25, 3.125,
  1.5625, 31.25, 15.6, 7.8, 7.8, 78.1; retail calls 7 (all but 31.25 / 78.1). With c=95 that needs a uniform factor
  x in (0.095, 0.19] on those frequencies; row 1 needs x in (0.119, 0.19] on the creatures section. Equivalently ONE cost
  c in [107.5, 109.2) fits all 13 Reset decisions of both rows with today's structure (ELSE_NULL c=108 is why it "works").
- Retail's OUT-OF-LINE Reset<CComponent> 0x00429C6F (xdis): call ReleaseData; Data=data; if (!data) ret; new; ctor reads
  Data back; Info=result (xor eax on new-null). = DEFAULT text. It excludes ELSE_NULL / T7 (else Info=0 store would show:
  the call to ReleaseData is opaque there), R8/R13 `if (Info) ReleaseData();`, R15 `if (!IsNull()) ReleaseData();`.
- Mechanism class found (DIAG tmp/hdrR): a redundant test that c2 folds after inlining into the caller but NOT in the
  callee's own cost: R8 `if (Info) ReleaseData();` c 101 (+6) with an expansion identical to the default (one cmp moved);
  R15 +11. Texts with c=95: `if (!data) return;` (R1), `Info = data ? new..: 0` (R3), `if (Data)` (R5), temp info (R7),
  `if ((Data = data) != 0)` (R9), `if (!IsNull())` (R10), Reset out-of-class non-inline (OOC), RESET_CALLS_DTOR.
- Row 2 "80 missing bytes": NOT a missing block. Same frame (sub esp,7Ch), same jump table (switch on ++second, retail
  table at 0x006001D0 is data that disassembles as `pop edi`), same 171 calls; retail has 16 more `mov` and 22 more
  [esp+..] references (values kept in frame words where ours keeps registers: C15's "allocation from the head") plus the
  Reset expansion shape. The bytes are spread, not one absent construct.
- fwsize (FableWin /Od, a much later compiler): CCPPointerInfo ctor SAME; Reset +36i / ReleaseData +55i / Get +12i = debug
  checks; not a release-cost clue.

### Conclusion for the coordinator (row 1 and row 2 Reset question)
1. Natural frequency spellings cannot do it under c2's measured model: the creatures site needs B in (3.7, 5.94] AND the
   IsMapLoaded-return dtor needs f > 0.2 while the flow entering the loop is 0.25; returns that take flow are cold, hot
   (do-while(0)-wrapped) returns take none (base 0.25 -> f 3.125, sc 73, one short), for-type loops leak x5. Measured
   ~35 spellings (probe/r1: L*, X*, F*, M*, N*, Q*, V*, w*, G1, D1, NF*). Closest: F3 (guard+do-while, creatures = retail,
   only that dtor called; DIFFER(9522v9571) D473) and M2 (do{if(!x)return;}while(0) in a while: all right but sc 73 v 74).
2. A frequency fix would have to be different in each row, but ONE parameter fits all 13 Reset decisions of both rows:
   Reset cost c in [107.5, 109.2) (ELSE_NULL = 108). So retail's Reset costs ~13 more at inline-decision time than the
   default text while its final code (expansions here, out-of-line COMDAT 0x00429C6F) is the default text.
3. Such texts exist in principle (R8 `if (Info) ReleaseData();` +6 with an identical expansion: c2 costs the callee before
   jump threading), but every one found so far is visible in the size-TU out-of-line Reset 0x00429C6F (which calls
   ReleaseData opaquely). The +13 must be pure Reset-local redundancy that full /O1 optimisation removes. Not found.
4. Decision needed: keep the fitted ELSE_NULL text (differ D113, call targets = retail), or allow an inline control
   (owner 2026-11-08 rule) -- no per-site control reproduces a direct `call Reset` except a per-row /d2inlT (T65 flips 17
   other callees in row 1: rejected) -- or keep searching the Reset text (cost-only redundancy class).
- TU consistency check: the matched siblings 0x0060C900 / 0x0060EEB0 have NO Reset<CComponent> site of their own (their
  files carry the CreateMapMarkers same-TU body, ungraded); with FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_NULL added
  (probe/sib/c9E.cpp, eeE.cpp) both still grade RELOCATION_MATCH -> the whole TU can carry one Reset text either way.
- More cost texts (c 95, folded at cost time): `if (IsNull()) return;` (R33), `if (data != 0 && !IsNull())` (R36). Only
  redundancy that runs through ReleaseData's inlined body costs extra (R8 +6, U1 +6, R15 +11) -- and each of those shows
  in the opaque-ReleaseData out-of-line COMDAT 0x00429C6F, which is plain default. Open lead: none left in this class.
- Files: probe/r1/*.cpp (all variants), tmp/vq_*.txt / tmp/cq*.txt (one line per variant), tmp/hdrR + tmp/hdrX = DIAG-only
  overlays (FABLE_DIAG_*; never hand back). No change to hdr/ (agentC18 added no opt-in).
