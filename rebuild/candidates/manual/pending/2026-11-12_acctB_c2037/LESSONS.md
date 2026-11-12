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

Review at landing:
- engine/CEngineMap.h: two bodies of CEngineMap::MapHasDataAtPoint now exist -- the c2021 && chain (default; their rows 0x00B3F5E0 /
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
- RenderRain / RenderSnow sources contain this chunk's unmatched ClipBoxToLandscape body (see NOTES.md); if a later session changes that
  body, regrade both.
- purity.py grades the whole row TEXT: a row that is plain C++ itself but holds a same-TU callee body calling a shared asm helper
  (RenderRain / RenderSnow with ClipBoxToLandscape) is `naked_asm` / NOT_GENUINE in vland until it carries a `// Retail-asm:` line whose
  first line already names an address and a helper mnemonic (continuation lines without the tag are not read).
