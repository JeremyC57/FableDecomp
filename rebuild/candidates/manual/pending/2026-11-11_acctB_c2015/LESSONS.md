# Chunk 2015 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2015, 0x0097FEA0) A materialised HasTC bit (`shr ecx,3 ; and cl,1 ; mov [esp+N],cl ; je`) before GetPVillage is the DEFAULT
  CThing::GetTC body with its result unused (`GetCreature().GetTC(&member);`), as the landed 0x0090C540; GETTC_EXPANDED gives `mov 1 /
  xor` constant paths instead. The byte is the frame slot a later address-taken bool reuses (29 such village-member sites in Fable.exe).
- (chunk 2015, 0x0097FEA0) A retry loop `for (;;) { r = Call(); if (r != 2) break; Yield(); if (IsStopping()) {stop; return;} }` nested in
  another loop is rotated by VC7.1 (the call body duplicated at the bottom); retail's single copy is `do { r = Call(); if (r != 2) break;
  Yield(); } while (!IsStopping()); if (r == 2) {stop; return;}` (c2 threads the retest). At top level both shapes give one copy.
- (chunk 2015, 0x0097FEA0) A loop entered with `jmp` over `lea ebx,[ebx]` padding (aligned head) was a `do { } while (result == RUNNING)`;
  the same body as `for (;;) { ...; if (result != RUNNING) break; }` lost the alignment (same size, 8 bytes moved).
- (chunk 2015, 0x0097FEA0) A local initialised with a constant whose store retail schedules between `fild` and `fmul` of a later
  computation is declared BEFORE that computation (`long last = 0x80000000; long n = (long)(def->Lifespan * 0.8f);`).
- (chunk 2015, 0x007A0D90) The CIntelligentPointer member assignment `beam->m_tcpTarget = thing;` needs FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
  (GetPItem then SetPItem); without it VC7.1 copies PData directly.
- (chunk 2015, 0x007A0D90) c2's static frequency at code after an inner list search took only two values over 16 plain spellings: a `for`
  search keeps the code after it at the loop's frequency (new-beam arm f 0.536), a `while` / do-while / for(;;) search drops it ~7.5x
  (0.0715); condition spellings (`||`, nested `&&`, continue vs if-body) did not move it. When retail's split needs a frequency in
  between and no /d2inlT fits, `#pragma inline_depth(0)` around the called sites (sum formed inside the region so the add keeps
  retail's `add eax,[mem]`) closes it.
- (chunk 2015) Grading a source that includes an overlay-only header WITHOUT `FABLE_HDR_OVERLAY` hangs cl.exe until the 200 s timeout
  (g.sh does not set it): export the overlay in every grading shell (tmp/pq.sh does).

Review at landing:
- 0x007A0D90 carries an `// Inline-budget:` device (`#pragma inline_depth(0)` around two statements of the new-beam arm); the trace numbers
  and the 16 plain-C++ variants are on lines 3-11 of the source.
- Overlay headers (hdr/README.md): 6 new headers + opt-ins in 8 copied repo headers; re-copy HEAD and re-apply before merging.
  NEntityEvents::CEventBase is defined in the new NEntityEvents_CEventExpressionPerformedTo.h (slots 0..4) and, smaller, in
  NEntityEvents_CEventHitBase.h: merge before one TU needs both.
- NAME_FIXES.tsv: 12 entries (see NOTES.md); the vland RELOC_NAME warnings for 0x007A3CA0 / 0x007A3200 / 0x006691C0 are those fixes.
- The landed stand-in rows of this TU (0x007A3CA0 CThingSearchTools_Calls, 0x007A3200 CUnprovenPointee, 0x007A4210 CCountedPointer_007a4120,
  0x006691C0 CAIStateGroupWithBuilding_006691c0) can now be respelled with the proven types.
