# Chunk 821 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 821) Frame-slot residue where a later def pointer should reuse an earlier float's slot: wrap the float (halton), its loop and
  the loop after it in a sibling block before the def's block; VC7.1 then gives the def the float's slot. 0x007D8E20 FrameUpdate:
  [esp+0x14] -> retail [esp+0x18], 22 lines -> RELOCATION_MATCH on the first try after 18 parked grades.
- (chunk 821) Speech-exit MovieBase dtor (the per-site implicit-dtor residue of chunks 797-814) has a plain C++ spelling in
  CEndGameScythe::Main 0x00D2ED20: one `do { } while (0);` round the whole SAY block `if (GetHealth > 0) { Speak; wait; T }`.
  Both speech-wait returns become inner-loop exits (c2 frequency rule) and VC7.1 expands and tail-merges them as retail. Wrapping the
  separate T checks, or Speak/wait inside the if, expands one exit only; also wrapping the inner T checks is too hot.
- (chunk 821) Try /d2inlS15 as a diagnostic meter before parking a long-Main MovieBase row: it matched 0x00D2ED20 and 0x00D2D930
  exactly (S14 = default). If S15 matches, the sites lose to c2's caller-size penalty, not to the dtor spelling. 0x00D2ED20 was then
  fixed by heating the exits; 0x00D2D930's four scope-end sites were not (do-while scopes do nothing there). GOTCHAS chunk 805's
  "no value reproduces" only tried S14 and S16.
- (chunk 821) Do not wrap a single terminate check inside a cut-scene scope in do-while(0) to heat a dtor site: in 0x00D2ED20 any
  such wrap moved the movie from [esp+0x48] to [esp+0x58] and turned its scope-end dtor into a call (2239v2248).
- (chunk 821) A guarded pointer `for (it = begin(); it != end(); ++it) { if (..) { erase(it); break; } }` before the final exit leaks
  its body frequency (~5x) to the code after it, so VC7.1 expands ~CScriptThing at the final natural exit. Written
  `it = begin(); for (;;) { if (it == end()) break; ...; ++it; }` (or `while (it != end())`) it gives the same loop bytes and does not
  leak. COracleTablet::Main 0x00CFE030 with a row-local `inline ~CScriptThing() { }`: 183 -> 8 lines, size exact.
- (chunk 821) A `for (;;) { if (!IsPerforming) { if (T) return; break; } NSF; if (T) return; }` speech wait compiles to the same
  bytes as `while (IsPerforming) { NSF; if (T) return; } if (T) return;` (0x00D0A830): use it only as a shape probe.
- (chunk 821) Removing code AFTER the residue site never freed inline budget (0x00D2ED20 diagnostics x1 / x3); removing code BEFORE
  it did (x2). Inline sites are visited in source order.

Review at landing:
- 0x00D2ED20 src carries a line-2 `// Inline-split:` note (one SAY-level do-while(0); construct unproven), like the landed rows of
  INLINE_DEPTH_REVISIT lever 3.
- 0x00D0A830 differ keeps chunk 800's line-5 `// Inline-split:` note (its movie-scope do-while).
- vland RELOC_NAME warnings on 0x005E9A60 / 0x008AB1E0 / 0x00CDBFB0 come from catalog labels that are already there (see NOTES.md).
- No row needed a /d2 flag; the /d2inlS / /d2inlT runs were diagnostics only and are not in any source.
