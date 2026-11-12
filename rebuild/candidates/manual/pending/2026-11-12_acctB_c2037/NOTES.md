# Hand chunk 2037 (CEngineWeatherRenderer): 3 of 3 byte-exact (3 RELOCATION_MATCH)

Every compile used `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2037/hdr` (hdr/README.md: the 2031 overlay + the 2008 camera headers +
the 2021 CEngineMap.h + this chunk's opt-ins). Row extents checked from Fable.exe: all three catalog lengths are right (no SPLITS.tsv).
0x00B52DD0 starts after int3 padding (the previous function ends `ret 4` at 0x00B52DC6), ends `ret 4` at 0x00B53F56, int3 after; its
only callers are RenderRain 0x00B54C65 and RenderSnow 0x00B55395.

## What matched (3)
- 00b52dd0 CEngineWeatherRenderer::ClipBoxToLandscape: RELOCATION_MATCH (4489v4489) at `// Flags: /d2inlT507`, continuation session g8
  (probe under tmp/hX) and g10 (the final src/00b52dd0.cpp under hdr/). The body is g3's, unchanged; what closed corner 4 was the shared
  header: CEngineMap::PeekLandscapeHeight's written-out neighbour test now passes the recursive call its own argument expressions instead of
  the named cells (engine/CEngineMap.h, IFRETURN opt-in; details in the continuation section below and hdr/README.md).
- 00b54890 CEngineWeatherRenderer::RenderRain and 00b54e30 CEngineWeatherRenderer::RenderSnow: RELOCATION_MATCH. The 2008 / 2031 sources
  unchanged except line 2 (the DIFFER grade line replaced by the pair-landing note); the same TU now holds the three clip bodies retail
  calls, each `__declspec(noinline)` and in retail address order: ClipBoxToCamera 0x00B51F60 and ClipBoxToWeatherMaskSphere 0x00B52090
  (ported from the landed stand-in sources onto the shared headers: CEngineCamera::PeekCameraPos / ClipBox, CMatrix3x4 E11..E43,
  C3DBoundingSphere::GetCenter (by value) / Radius, GetBoxBoundingSphereRadiusSquared; each compiled out of line grades RELOCATION_MATCH
  against its retail bytes, probe/pc.cpp), then SetCommonParticleShaderConstants (already there), then ClipBoxToLandscape 0x00B52DD0
  (this chunk's body, see below). With them the two inline push_back C3DVector copies are scheduled load-ahead as retail.
  The ClipBoxToLandscape body in these two TUs is the matched src/00b52dd0.cpp body (same text; regraded after the header change). It is compiled there at the TU's default inline budget (its CEngineMap expansions partly called), so its
  own bytes in those TUs are not retail's; what RenderRain / RenderSnow depend on is only that the body is visible and has unknown side
  effects (asm blocks, the virtual DrawBlockGetSizeZAt, the 0x00B439C0 call) -- the 2031 diagnostic proved that a side-effecting stub works
  as well. So the two matches do not rest on 0x00B52DD0's exact bytes, but their TU text contains an unmatched body.

## Parked for Fable 5.1 (0)
(historical, closed by the continuation session) 00b52dd0 CEngineWeatherRenderer::ClipBoxToLandscape DIFFER(4482v4489): corner 4 only -- 0x00B53B65 retail `mov edi,[ebp+8]` / ours `mov esi,[ebp+8]` (y - MapOffsetY) and 0x00B53BA2 `mov esi,edi` (grid index: retail esi, ours edi), then the neighbour path 0x00B53E04..0x00B53F13 (retail keeps lx / ly / WorldX / WorldY in edx / esi / ebx / edi, spills the map to [ebp-2Ch] and the neighbour cells to [ebp+8] / [ebp-34h], `cmp dword ptr [ecx+8],0` at 0x00B53EC6; ours spills WorldY and the map) | tried: 7 grades (below) and ~45 free probe compiles | next: retail's corner 4 behaves like neighbour cells that are call-argument temporaries evaluated y first (g6 / g7 reproduce corner 4's y register and the memory compare) while corners 1..3 need the if-return world-wrapper value: find one CEngineMap::MapHasDataAtPoint / PeekLandscapeHeight / CEngineWorldMap::MapHasDataAtPoint spelling that gives both (the header conflict in hdr/README.md is the same question).

## 0x00B52DD0 ClipBoxToLandscape
Shape (proven by grading): half box extents X / Y as two floats and the box top `pos.Z + Span.Z * 0.5f` (three scalars: a C3DVector half
costs a 12-byte frame node and a 0x38 frame, grade 2), then four corners in source order (floor X, floor Y), (floor X, ceil Y),
(ceil X, floor Y), (ceil X, ceil Y) through the shared asm helpers GFFloatToLongFloor / GFFloatToLongCeil, each
`if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top) return WEATHER_BOX_VISIBLE;`,
then `return WEATHER_BOX_CLIPPED;`. Retail expands CEngineWorldMap::MapHasDataAtPoint / PeekLandscapeHeight (both FABLE_FORCEINLINE:
c2 never plain-inlines a callee costing over 450, c2.dll 0x1078D248 `cmp esi,1C2h`, and MapHasDataAtPoint costs 533), inside them
CEngineWorldMap::GetMap(x, y) and CEngineMap::PeekLandscapeHeight (plain inline, expanded at `/d2inlT497`: highest site score 496 =
PeekLandscapeHeight c1 338 at f 0.5 inside the forced wrapper; GetMap 146, CEngineMap::MapHasDataAtPoint 141), whose neighbour path calls
the out-of-line copy 0x00B439C0.
Corners 1..3 (0x00B52DD0..0x00B53AEA, 1137 of 1528 instructions) are byte-identical to retail. Corner 4 differs only in register
allocation: the instruction sequence is the same IL (same reassociation of the neighbour cell sums, same reloads), but retail keeps
y - MapOffsetY in edi and the grid index in esi (0x00B53B65 `mov edi,[ebp+8]`, 0x00B53BA2 `mov esi,edi`), ours swaps them, and in the
neighbour path retail keeps lx / ly / WorldX / WorldY in edx / esi / ebx / edi and spills the map (0x00B53E04 `mov [ebp-2Ch],eax`, the
cells to [ebp+8] / [ebp-34h], 0x00B53EC6 `cmp dword ptr [ecx+8],0`), ours spills WorldY and the map instead. 7 bytes longer in retail.
Grades (all pass 1; passes 2 and 3 not used): g1 default flags (703 bytes: helpers called), g2 forced wrappers + T497 with a C3DVector
half (4482v4489, frame 0x38), g3 scalar halves (4482v4489, 1351 of 1528 equal, frame right), g4 function-scope x / y (object identical to g3),
g5 the delivered differ source after the header clean-up (= g3), g6 the "explicit world wrapper" header model (c2021 && chain + call in
PeekLandscapeHeight + CEngineWorldMap::MapHasDataAtPoint written out with `PeekGameMap() ? true : false`: 4523v4489, 1139 / 1528, corner 4's
y register and `cmp [ecx+8],0` right, corners 1..3 wrong), g7 the if-return model with the neighbour cells unnamed (4478v4489, 1204 / 1528,
corner 4's y register right, corner 1's neighbour cells x-first). Why passes 2 and 3 were not spent: every natural variant screened for free
(outer forwarded names k = 1..7, else-if / if-return-false world wrappers, the span reference, a halfZ local, top first, split ifs, a named
height, ternary and if-else tails, FABLE_RETAIL_CARRAY, both wrapper definition orders, three neighbour-test spellings) compiled to an object
byte-identical to g3 (tools/prop.sh compares with OUR g3 object, never with retail), so grading them would re-measure the same bytes; the two
models that moved corner 4 (g6, g7) lose corners 1..3. The context budget of this session ran low after that.
Free work (no retail diff): inline traces (tools/inl.sh), c2prio traces (tools/trc.sh, tools/fit.sh), the corner-4 property probe
(tools/prop.sh: reads only the corner-4 y register and the `cmp [ecx+8],0` count from our object), the COMDAT probes (tools/cp.sh) against
retail's uncalled COMDATs 0x00B56880 / 0x00B566B0 (not rows of this chunk).

## Names (two sources each; LABELS.tsv, NAME_FIXES.tsv)
- 0x00B52DD0 ?ClipBoxToLandscape@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@@Z: Ego_r.pdb 0x009F3A70 + FableWin.pdb
  0x02D3AF40; FableWin's COMDAT run after it (CEngineWorldMap::PeekLandscapeHeight 0x02D3B1F0, MapHasDataAtPoint 0x02D3B260) is retail's
  uncalled COMDAT pair in this TU (0x00B566B0, 0x00B56880), which the two expanded helpers reproduce.
- Callees: CEngineMap::PeekLandscapeHeight 0x00B439C0 (c2021 label), CIDrawMap::DrawBlockGetSizeZAt (slot 5), CEngineCamera::ClipBox 0x00B30610
  (Ego_r ?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z), GetBoxBoundingSphereRadiusSquared 0x00B50E50 (Ego_r / FableWin, private const).
- Globals (established spellings, no PDB data symbols): g_pEngineWorldMap 0x01436E8C, NEngineInternal::GPCamera 0x01436EA0 (the landed
  0x00B51F60 binds ?gWeatherCamera@@3PAUCWeatherCameraMethods@@A there -- review at landing).
- Descriptive: the enumerator names WEATHER_BOX_VISIBLE / WEATHER_BOX_CLIPPED (2031 spelling; values 0 / 1 from the bodies), all locals
  (halfX, halfY, top, x, y, mapX, mapY, cellX / cellY, box_radius).

## Header debt
- engine/CEngineMap.h: two bodies of CEngineMap::MapHasDataAtPoint (c2021 default && chain; this chunk's IFRETURN opt-in) -- see
  hdr/README.md CONFLICT.
- engine/CEngineCamera.h / CEngineCameraDesc.h / CVolumeTexture.h / CEngineComponentRenderParams.h are the c2008 copies (+ CLIPBOX here).

## C++ attempts before an asm helper
0x00B52DD0 reaches GFFloatToLongFloor (4 sites) / GFFloatToLongCeil (4) / GFFloatToLongNear (4) (addresses in its `// Retail-asm:` block).
Per-site attempts (continuation session; one fragment TU probe/asm/spell.cpp compiled by probe/asm/run.py, cl 13.10.3077 /O2 and
/O2 /QIfist; full table probe/asm/result.tsv): Near 32 C++ spellings, Floor 30, Ceil 31 -- every one `call __ftol2` under /O2 and
`fistp qword` under /QIfist, none a bare `fistp dword`; 12 intrinsic spellings -- cvtss2si (4), cvttss2si (2), cvtsd2si, cvttsd2si,
cvtps2pi, cvttps2pi, cvtps2dq, cvttps2dq. Summarised in the row's `// Retail-asm:` block. RenderRain / RenderSnow reach the helpers only through the ClipBoxToLandscape body in their TU
(not their own leaf).

## Verification
- gall.sh: RenderRain / RenderSnow RELOCATION_MATCH (final files); tall.sh + tall.py: both PASS; differ/00b52dd0.test.cpp PASS under g++ and
  VC7.1 (verify_and_land.behaviour_of).
- Read-only vland (payload.json / oracle.tsv from finish.py): both rows RELOCATION_MATCH + behaviour PASS, no RELOC_IDENTITY; only
  WARN RELOC_NAME for the respelled callees (ClipBoxToWeatherMaskSphere 0x00B52090, NEngineInternal::GPCamera 0x01436EA0 labelled
  g_pEngineCamera), which NAME_FIXES.tsv covers (GPCamera: established spelling, data-symbol review).
- purity: both src rows grade `retail_asm` (they reach GFFloatToLongFloor / Ceil / Near through the same-TU ClipBoxToLandscape body; each
  carries a `// Retail-asm:` line saying so). Without that line vland reported NOT_GENUINE: a row whose TU merely contains a helper call
  needs the evidence line too.

## Continuation session (2026-11-12, second agent): g8 RELOCATION_MATCH
- Measured first: c2prio (`--blocks`, tmp/blk_l3t.txt) on g3 shows corner 4's PeekLandscapeHeight neighbour cells (the named
  mapX / mapY of the IFRETURN neighbour test, ids 190 / 189) at priority 74 / 66 with 8 references each, against 32 / 24 and 4
  references in corners 1..3: in corner 4 the recursive call's arguments were the SAME named locals as the range test, so the
  cells carried the call's references and were coloured early (eax / edx), pushing lx / ly to esi / edi and mapY to esi.
  Retail spills them: its call arguments are separate expressions (call-argument temporaries, CSE'd with the cells).
- COMDAT probes (tools/cpx.sh, overlay tmp/hX with switches PX_*; M = 0x00B56880, P = 0x00B566B0, C = 0x00B439C0 out of line):
  unified header (c2021 && chain + call form, world wrapper written out) M MATCH, P RELOCATION_MATCH, C 119/132 (fail-path layout only);
  + if-return MHD: P 469v455 (setne leftover). MHD bodies with the calling world wrapper (PX_WW_CALL) + c2021 call-form PLH:
  && chain M 533v540; `?true:false` on the call 547; `!= false` 643; B1 `if (chain && GameMap) return true; return false;` M 535 /
  P RELOC; B2 `if (out) return false; return GameMap != 0;` M MATCH / P 469; B3 if-return `GameMap ? true : false` M MATCH / P 469;
  B4 range ? (GameMap != 0) : false 560 / 471; B5 result flag 481 / 469; B7 `(chain) ? true : false` 535 / RELOC.
  => M needs the if-return value, P needs the neighbour test NOT to call MapHasDataAtPoint. PLH neighbour spellings under
  if-return MHD + calling world wrapper: PLH1 (predecessor: named mapY / mapX used in the test AND the call) M MATCH, P RELOC, C 84/132;
  PLH3 (named mapY / mapX in the test, the call's arguments written as the expressions again) M MATCH, P RELOC, C 119/132 (= the c2021
  call form's C result).
- PLH3 in the row: inline trace (tools/inl.sh, tools/inl/xL3.txt) puts the four CEngineMap::PeekLandscapeHeight sites at score 506
  -> /d2inlT507; tools/prop.sh: len 4489, corner-4 y register edi, one `cmp dword ptr [ecx+8],0` (all retail's).
- g8 (pass 2, 1st grade): probe/x_L3.cpp under tmp/hX with PX_WW_CALL + PX_MHD_IFRETURN + PX_PLH 3, /d2inlT507: RELOCATION_MATCH.
- g9 was a mis-invocation (g.sh without `--flags`: default budget, 1926 bytes; counted); g10 = final src/00b52dd0.cpp under hdr/ with
  /d2inlT507: RELOCATION_MATCH. gall.sh after the header change: all three rows RELOCATION_MATCH.
- Header after the session (hdr/engine/CEngineMap.h, IFRETURN opt-in): MapHasDataAtPoint if-return; the three Peek bodies' neighbour tests
  written out on named cells (mapY first) with the recursive call's own argument expressions. The CEngineMap TU's out-of-line copies
  (tools/cpt.sh, T444 / T576 / T600): 0x00B43720 114/127, 0x00B43870 114/127, 0x00B439C0 119/132 -- the SAME objects as the c2021 call
  form (only a fail-path reload stub placed differently, both forms).
- c2021 rows (copies in probe/, overlay tmp/h21 = hdr + c2021's CEngineLandscapeRenderer.h; not this chunk's grades): default path
  0x00B3F5E0 and 0x00B40BD0 RELOCATION_MATCH (header default text unchanged); with the IFRETURN opt-in defined 0x00B3F5E0 RELOCATION_MATCH
  (T444; PeekLandscapeHeight c1 345 in both forms), 0x00B40BD0 DIFFER(1780v1736) (448/622, block layout + allocation from the first
  PeekThemeId copy). Screened PeekThemeId / PeekThemeBlend spellings for 0x00B40BD0 (tools/c21.sh): written-out named cells passed to the call
  1804 (both orders), x-first names + expression call 1796, PeekGameMap() test / nested GameMap if = 1780, unnamed cells 1755, the call form
  calling the if-return body 1223 (budget shift + setne). The same written-out form under the && chain body is also 1780: 0x00B40BD0
  needs the CALL form in PeekThemeId / PeekThemeBlend, independent of the MapHasDataAtPoint body.
- Why no single text was found (all measured): 0x00B56880 + the row's corners 1..3 need CEngineWorldMap::MapHasDataAtPoint to return an
  if-return value through an inline bool call (written-out `? true : false`, `(bool)(.. != 0)`, a bool local all MATCH 0x00B56880 but the
  row jump-threads them: 4 setne left of 12; `!= 0` / `!!` give 556v540); 0x00B566B0 / 0x00B439C0 / 0x00B43720 / 0x00B43870 need pure-branch
  neighbour tests (a call to the if-return body leaves `setne al ; test al,al`: 469v455); 0x00B40BD0 needs the call form in PeekThemeId /
  PeekThemeBlend. Only a second, chain-shaped CEngineMap predicate would serve all three, and the PDB name sets have none
  (CEngineMap members listed from Ego_r + FableWin). Ranked untried ideas: (1) a PeekWaterDepth spelling that matches under the opt-in
  (c2021 took 9 grades on the default path); (2) PeekThemeId / PeekThemeBlend defined in CEngineMap.cpp rather than the header (only the
  CEngineMap TU expands them; check every other TU's expansion of them first); (3) a c2prio comparison of 0x00B40BD0 under both forms to
  find the first differing decision.
- Free probe compiles this session: ~30 (COMDAT probes M / P / C / I / B, 3 prop.sh row properties, 2 inline traces, 1 c2prio --blocks,
  1 asm fragment TU x 2 flag sets, 10 c2021-row probes).
- Final verification (continuation): gall.sh 3 x RELOCATION_MATCH; tall.sh + tall.py 3 x PASS; read-only vland (payload.json /
  oracle.tsv, work dir deleted) 3 x RELOCATION_MATCH + behaviour PASS, no RELOC_IDENTITY, only the known RELOC_NAME warnings (GPCamera,
  ClipBoxToWeatherMaskSphere; NAME_FIXES.tsv); crawl/purity.py: all three `retail_asm`, genuine. The predecessor's parked differ files
  (g3 source + test) moved to prior/differ_00b52dd0_g3/.
