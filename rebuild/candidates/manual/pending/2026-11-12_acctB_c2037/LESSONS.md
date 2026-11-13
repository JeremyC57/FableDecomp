# Chunk 2037 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- c2 never plain-inlines a callee whose cost (callee code bytes minus the call savings) is over 450: c2.dll 0x1078D248 `cmp esi,1C2h ;
  jg` rejects it before the frequency score, whatever `/d2inlT` says. A retail expansion of such a callee is a forced inline (0x00B52DD0:
  CEngineWorldMap::MapHasDataAtPoint, 540 bytes, cost 533, expanded four times; tools/inl.sh prints `c1=-` for these sites).
- An uncalled retail COMDAT of a header inline is a free answer key for its body even when the row expands it differently: taking the
  member's address at file scope emits our copy (0x00B56880 CEngineWorldMap::MapHasDataAtPoint MATCH, 0x00B566B0 PeekLandscapeHeight
  RELOCATION_MATCH; tools/qx.py grades raw exe bytes by address and length with QLEN).
- `return a && b && c && d && e;` inlined into a returning caller is materialised as int 0 / 1 (`xor eax,eax` / `mov eax,1`) and the
  copies merge; retail's `test ; setne al` with the false paths on the caller's `xor al,al` is `if (a && b && c && d) return e; return
  false;` (0x00B56880). The same if-return body expanded into an `if` condition leaves `setne al ; test al,al` (c2 does not thread a
  setcc), so a retail condition that is pure branches does not call that function (0x00B567AF, 0x00B439C0).
- Named locals in a forced wrapper decide the evaluation order of its call arguments: `long mapX = x - MapOffsetX; long mapY = ...;
  GetMap(mapX, mapY)` gives retail's x-first subtraction (0x00B56880 / 0x00B566B0); `GetMap(x - MapOffsetX, y - MapOffsetY)` evaluates y
  first. Conversely a y-first retail order next to an x-first test (0x00B567AF) is an argument list evaluated right to left.
- A C3DVector local whose Z is never stored still takes a 12-byte frame node (frame 0x38 v retail 0x34 in 0x00B52DD0); retail's two
  floats in reversed slot order are two scalar locals.
- Pair-landing confirmed: RenderRain 0x00B54890 / RenderSnow 0x00B54E30 grade RELOCATION_MATCH once the three same-TU clip bodies
  (ClipBoxToCamera, ClipBoxToWeatherMaskSphere, ClipBoxToLandscape) are visible and noinline; the landscape body did not have to be
  byte-exact itself (it is compiled at the caller's default inline budget there).
- In a function made of N identical inline-expanded blocks, the LAST block can be allocated differently from the others although its IL is
  the same (0x00B52DD0 corner 4: identical sums and reloads, other register / spill choices, in retail as in ours, but not the same ones);
  outer named-symbol counts k = 1..7 (r3004) do not move it.

- (continuation) When only the LAST of N identical inline-expanded copies is allocated differently, read c2prio `--blocks` for that
  copy's candidates first: in 0x00B52DD0 corner 4's neighbour cells had 8 references (priority 74 / 66) against 4 (32 / 24) in corners 1..3,
  because the header's written-out test passed the named cells to the recursive call; writing the call's own argument expressions (c2 CSEs
  them with the cells, which keep only the test's references) gave retail's corner-4 spills and the match. The equal-priority copies are
  coloured last-copy-first (larger key), so a priority shift there shows only in the last copy.
- A residue in a row that expands shared header inlines can be the header's fault even when the row's other copies match: grade the inline
  bodies against every uncalled retail COMDAT (tools/qx.py with QLEN) under each candidate header before spending row grades.
- An inline bool predicate's `if (range) return p != 0; return false;` keeps `test ; setne` when its value is returned or merged (0x00B56880,
  0x00B52DD0 corners), and leaves `setne al ; test al,al` when the inline is a lone `if` condition (0x00B566B0); `? true : false`,
  `(bool)(p != 0)` or a bool local written out in the caller give the same 0x00B56880 bytes but are jump-threaded in an expanded row.
- A bare `fistp dword` has no VC7.1 C++ or intrinsic spelling (93 C++ spellings over Near / Floor / Ceil shapes x /O2, /O2 /QIfist: __ftol2
  or `fistp qword`; 12 SSE / SSE2 / MMX intrinsics: cvt*): probe/asm/ in chunk 2037 is a reusable one-TU table generator.

Review at landing:
- engine/CEngineMap.h (still open after the continuation session, see hdr/README.md): two bodies of CEngineMap::MapHasDataAtPoint exist -- the c2021 && chain (default; their rows 0x00B3F5E0 /
  0x00B40BD0 match with it and with PeekLandscapeHeight calling it) and this chunk's if-return opt-in
  `FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN` (with PeekLandscapeHeight's neighbour test written out). One retail function has one body:
  the coordinator must decide (evidence for if-return: 0x00B56880 MATCH, 0x00B52DD0 corners 1..3 exact with the setne value; evidence for
  the && chain + call in PeekLandscapeHeight: the c2021 rows, and retail's out-of-line 0x00B439C0 compiled from it scores 119 / 132
  instructions against 84 / 132 for this chunk's written-out neighbour test). Under the opt-in 0x00B3F5E0 DIFFERs (1588v1624 at T444).
  The "explicit world wrapper" alternative (hdr/README.md, tmp/CEngineMap.unified.h) satisfies both COMDATs and the c2021 header but not
  0x00B52DD0's corners 1..3 (1139 / 1528).
- The landed 0x00B51F60 (CWeatherRendererMethods::ClipBoxToCamera) and 0x00B52090 (CEngineWeatherRenderer::IsPointInPrecipitation) stand-in
  sources: their PDB names are ClipBoxToCamera / ClipBoxToWeatherMaskSphere (NAME_FIXES.tsv); this chunk's shared-header ports grade
  RELOCATION_MATCH (probe/pc.cpp) and could replace them.
- RenderRain / RenderSnow sources contain the matched ClipBoxToLandscape body (same text as src/00b52dd0.cpp); if a later session changes
  that body or engine/CEngineMap.h, regrade all three.
- Under the IFRETURN opt-in, c2021 row 0x00B40BD0 (PeekWaterDepth) DIFFERs (1780v1736) while 0x00B3F5E0 still matches: the header needs one
  text that also serves 0x00B40BD0 (NOTES.md continuation section ranks the next ideas).
- purity.py grades the whole row TEXT: a row that is plain C++ itself but holds a same-TU callee body calling a shared asm helper
  (RenderRain / RenderSnow with ClipBoxToLandscape) is `naked_asm` / NOT_GENUINE in vland until it carries a `// Retail-asm:` line whose
  first line already names an address and a helper mnemonic (continuation lines without the tag are not read).
