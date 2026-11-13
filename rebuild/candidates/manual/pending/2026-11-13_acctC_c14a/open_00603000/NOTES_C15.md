# agentC15 (acctC lane) -- NOTES (continues agentC14: prev_C14/)

## Row 1 0x00603000 CTCInventoryMap::CreateMiniMapMarkers -- IN PROGRESS
Base chain (probe/r1/): b0 = C14 differ DIFFER(9682v9571) D1760 (mx) / 223+ hunks
- c1: every GetRelativePosOnMiniMap result copied into a second C2DVector -> D1116: proved retail's position values are
  NOT address-exposed (retail loads X,Y together after GetStates()[0] and keeps them in esi/edi across AddChild etc. for the
  CUIMiniMapMarker copy; ours reloaded them field by field).
- d1: the real fact = callee body visibility: the landed same-TU body of GetRelativePosOnMiniMap (0x005FBAA0, retail order before
  this row; rebuild/src/compiled/00/5f/CTCInventoryMap_GetRelativePosOnMiniMap_005fbaa0.cpp) pasted before the row (tools/addbody.py,
  minus fable_game.h) -> VC7.1 knows the out-param does not escape. DIFFER(9667v9571) D905, frame 0xB4 = retail.
  (IsThingAsPosBeingDrawn 0x005FABA0 body: no effect.)
- e1 (= v/g1): `const CPlayerGuiDef& guiDef = CPlayerGui::PeekGuiDef();` + `guiDef.MiniMapGraphics.find/end` instead of a map
  reference local -> retail's `lea esi,[eax+94Ch]` (0x00603034).
- s1: `const long length = miniMapMarker.UIDefName.GetLength(); miniMapMarker.DefNameLength = length;` (7 sites) -> retail's
  `test eax,eax; je; mov eax,[eax+4]` + single store after the join (0x00603B97 etc.; with the direct assignment VC7.1 stores in
  both arms). hunks 223 -> 111; 9572 bytes. FITTED spelling: look for a more natural source of the temp.
- n1 (BEST, differ/00603000.cpp): named `CThingFilter_HasInformation hasInformation;` in the creatures loop (the value-initialised
  `CThingFilter_HasInformation()` temp stores a zero byte, retail 0x00603576 has none) -> DIFFER(9511v9571) D734 DR696, 106 hunks.
Residue (n1): head: retail region=edi (0x00603031), courtyard1/2 in memory; ours courtyard1=edi, region from [ebp+0Ch].
  what-if (tools/wq.sh): courtyard1 prio <=25 (natural 28 = K7*4 of block 2) fixes the head (split pass after heroMarker #90
  (26) splits region -> piece prio 46 takes edi) but the rest gets worse (n1: D 801). Hero block: retail hero=esi heroMarker=edi;
  ours swapped.
- p1 (BEST now, differ/00603000.cpp) = n1 + `#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY` (existing repo opt-in: __find body
  visible but out of line): retail keeps `creature` in edi across __find / GetDefGlobalIndexFromName (0x006036C9..) although
  &creature is passed -> c2 must know __find keeps no reference. DIFFER(9538v9571) D429 DR399, 63 hunks; head now = retail; call
  targets = retail (303 v 303).
- Residue classes in p1: (a) Reset<CComponent> inline text (~30 hunks): retail's expanded sites read Data back after `new`
  (`mov ecx,[esp+10h]` 0x00603A7F) and the data==0 path skips the Info store (`je 0x603A9D`); our FITTED ELSE_NULL text uses
  the argument and merges `else Info = 0` with the new-failed store (and the merged store gets cross-jumped into GetLength sites,
  0x006049F9 / 0x006053F3). Inline trace (tools/inl.sh, FUNC='?CreateMiniMapMarkers@'): sites sc = c1*(1/f+2)/3 vs 74; retail
  calls the first three sites (cached f1.25, hero f0.31, interesting f15.6) and expands the other ten -> needs c1 in [107.6,110.3].
  Measured c1: ELSE_NULL(data) 108 (OK), ELSE+Data 106, default (no else, Data) 95-96, explicit release (U3) 95, temp info 106 (U2),
  Get() spellings = default; /d2inlT65 + default text: 320 calls (no). Row 2 retail also reloads Data at its expanded sites
  (0x005FF906, 0x005FFC37). Open: a Reset text with c1 108-110 that expands like the default text.
  (b) alignment padding (nop / lea ebx,[ebx] / mov edi,edi): follows sizes. (c) expression-temp rotation 0x006031D7 (movsx ecx v
  edx). (d) traders loop: retail keeps `trader` in a frame word ([esp+2Ch]) and `it` in esi; ours reloads [it+8].
- Row 2 port check: row 2 has none of the row-1 constructs (no find/GetLength/GetRelativePos); the same-TU GetRegionTeleporter body
  (0x005FBEA0, landed) is inert there (m1 = m0, DIFFER(4096v4176)).
- Reset deep-dive (grades R1-R4, T1-T11, U1-U3, D1, p2, w1-w4, fl*): DIAG D1 = default Reset text (no ELSE_NULL) + the interesting
  site forced out of line through a noinline wrapper -> every expanded Reset site byte-equal to retail (0x00603A65.. shape), 9570
  bytes. So retail's Reset TEXT is the header DEFAULT; only the inline DECISION at the interesting site (0x00603838, f=15.625)
  differs: default c1=95, sc=95*(1/f+2)/3=65.4 < 74 -> ours expands. Retail calls it => in retail either c1 >= 107.6 (no text with
  the default expansion found: Get(), temp info, explicit release, Data-in-branch all 95-106), or f <= 3.2 at that site (base
  frequency after the early returns / IsMapLoaded / erase loops 5x lower: ours 1.25), or another threshold. Structural spellings of
  the early returns / loops (w1-w4) leave f unchanged. /Ob1, /Os, /Og, /d2inlT65: other inline decisions change (no).
  fwsize: FableWin Get 21i, operator-> 25i, Reset 69i, ReleaseData 93i v ours 9/9/35/38 = debug asserts (not a release-cost clue).
- r1 (BEST, differ/00603000.cpp) = p1 + a separate `unsigned long i` per maps loop (retail keeps three different counter frame
  words: votes -140/-156/-104 in tools/efm.py) -> DIFFER(9538v9571) D113 DF113 DR88 (exact metrics), 63 hunks.
- Reset decision = c2 STATIC FREQUENCY of the interesting site (tools/fq.sh: inline-trace f/r per Reset site + grade + calls):
  the IsMapLoaded loop `for (...) if (!IsMapLoaded(maps[i])) return;` multiplies the post-loop frequency by 5 (diag e1 without the
  loop: hero f 0.0625, interesting 3.125). As `while (i < maps.size()) { if (!..) return; ++i; }` (i3/j1, SAME loop bytes as retail)
  the interesting site gets f 1.5625 and is CALLED as in retail (first 4 Reset decisions = retail) -- but the IsMapLoaded-return
  dtor (??1CCountedPointer c1 47) drops to f 0.125 and is called (retail inlines it and cross-jumps the cleanup into the end).
  flag+break (i2/i5) 11.7, continue form j2 = for. Need: interesting f <= ~2.97 AND that return-path dtor f >= ~0.37.
- More frequency probes (fq.sh; all with the DEFAULT Reset text): erase loop as while (k1) / `count` local (m1) / `!=` (m2) = for
  (interesting f 15.6, called sites 2 of 3); `for (...; ) { ...; ++i; }` (k3), loop-then-check (n1) = while (f 1.56, but the
  IsMapLoaded-return dtor ??1CCountedPointer (c1 47) at f 0.125 is called -> 304 calls, cleanup not cross-jumped). No spelling found
  that gives both. The for-loop's post-loop frequency (1.25 = 2.5 x entry) comes from the `return` inside it (plain for loops keep
  the entry frequency). Hypotheses left: the site's own loop structure; a different early-exit arrangement around maps.empty();
  a TU-level inline setting (rows 0x005FF190 / 0x00605570 hint at it, C7 NOTES).

## Row 2 0x005FF190 CTCInventoryMap::CreateMapMarkers -- DIFFER(4096v4176), not worked in depth this session
- probe/r2/m0.cpp = C10 best differ. Port checks: GetRegionTeleporter same-TU body (m1) inert. efm.py frame votes: `it` maps to two
  retail slots, `i` to two -> separate loop variables: n2 (separate `it` per cards loop) D708 DR452 (m0 713/457), n1/n3 (separate i)
  DR527 (worse). Head (manager memory, const 0 / parent in ebx) unchanged.
- Reset cost probes, last batch: release-ASSERT dead arms (A1-A3, FABLE_RETAIL_ASSERT_DEAD_ARM), __assume (A4/A5), empty / trivial
  Validate-like inline calls (V1/V2): c1 stays 95 -- the inliner costs the callee AFTER its own optimisation, so only IL that
  survives in the callee and vanishes in the caller could reach 108. None found.

## Descriptive local names (no PDB source for locals): guiDef, hasInformation, length (row 1); unchanged from C14 otherwise.

## For the coordinator / successor
- Row 1 best: differ/00603000.cpp DIFFER(9538v9571) D113 / DR88 (was 9682v9571 D1760). Uses only existing opt-ins (+ the existing
  FABLE_RETAIL_STLP_FIND_NOINLINE_BODY) and the landed same-TU GetRelativePosOnMiniMap body pasted before the row (decision already
  taken: same-TU class helper bodies may live in the row file when equal to retail's out-of-line body).
- The ONLY structural blocker left on row 1 is VC7.1's inline decision for Reset<CComponent> at 0x00603838 (see the frequency
  notes). Retail's expansion text is the header DEFAULT (proved by D1); the FITTED RESET_ELSE_NULL text reproduces the decision but
  not the expansion (~30 of the 63 remaining hunks are its shape: `(void*)data` instead of the Data reload, merged `Info = 0` stores
  cross-jumped into GetLength sites). The same decision/shape question governs row 0x005FF190 (C4/C7 notes). Coordinator: a decision
  on a TU-level inline setting (C7: /d2inlT65 + default text reproduces row 2's split; row 1 then changes elsewhere) or on a per-row
  fitted text is needed if no source fact is found.
- Grades this session: ~125 on row 1 (ATTEMPTS.log + vb batches + ~25 what-if / inline-trace runs), ~7 on row 2.
