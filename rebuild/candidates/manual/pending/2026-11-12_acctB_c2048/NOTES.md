# Hand chunk 2048 (CEngineSkyRenderer): 1 of 2 byte-exact -- IN-PLACE HAND-OVER (session 3, context limit); owner: continue until BOTH match

Resume here: 0x00B64550 RenderSunFlare MATCHED (RELOCATION_MATCH, src/00b64550.cpp + test/00b64550.cpp, PASS g++ / VC7.1).
0x00B64FA0 RenderClouds best = differ/00b64fa0.cpp (= cand/c43.cpp, 2688v2688, 731/734: ONE SetRenderState block left, SetTextureUVModeV(2),
or-pattern ours BBAAAABBBBBB v retail BBAAABBBBBBB) + differ/00b64fa0.test.cpp (PASS). Grade with bin/gr.sh <addr> <src> "<note>".
Free size oracle: python3 /opt/fable_lane/tools/fwsize.py <src> [regex] (bin/fwsize_x.py with FW_EXTRA=/EHsc for rows: FableWin is /EHsc).
Session 3 section at the END of this file (grades 33..115).

## What matched (1)
- 0x00B64550 CEngineSkyRenderer::RenderSunFlare RELOCATION_MATCH (session 3, grade 46; src grade 114). Shape: see session 3.
Before session 3: none. 0x00B64FA0 RenderClouds reached the right size and 721/734 equal instructions at its FIRST grade; the remaining 13 are one
c2 tie-break (below), proven to be the function's c2 IL / record amount, not a statement shape.

## Parked for Fable 5.1 (1)
- 00b64fa0 CEngineSkyRenderer::RenderClouds DIFFER(2688v2688): first two SetRenderState expansions only -- U0 0x00B652FF..0x00B6533B retail
  `cmp [eax+2EE8h],ecx ; ... mov edi,ecx` / `mov ecx,[edx+8] ; test eax,ecx ; or ecx,eax ; mov [edx+8],ecx` / `mov eax,[edx+4] ; mov ecx,[esi+2808h]`
  v ours `mov edi,[eax+2EE8h] ; cmp edi,ecx` / `mov edi,[edx+8] ; test edi,eax ; or eax,edi ; mov [edx+8],eax` / `mov eax,[esi+2808h] ; mov edi,[edx+4]`;
  V0 0x00B6538B `test eax,ecx ; or ecx,eax` v `test ecx,eax ; or eax,ecx` | tried: see "Measurements" | next: a natural source with ~3-4 more c2 record
  chunks of IL (ranked ideas below).

### Row facts (all proven)
- Identity: FableWin.pdb ?RenderClouds@CEngineSkyRenderer@@AAEXM@Z (private, float, `ret 4`) + Ego_r.pdb 0x00A15F60 immediately before
  RenderOuterSky 0x00A169E0 (retail 0x00B64FA0 -> 0x00B65A20, same order). Extent: 0x00B64FA0..0x00B65A1F (cold tails 0x00B659BA..0x00B65A1E
  belong to it; RenderOuterSky starts at 0x00B65A20). Catalog length right; no SPLITS.
- Shape (source in differ/00b64fa0.cpp): capture block; params = world->DrawGetEnvironment()->PeekInterpolatedParams(); ids = sets 2 / 1,
  textures 0 / 1; loop i<4 { id; bank = GPResourceManager->PeekGraphicBank(0); CGraphicFrame frame; if (id && bank->PeekGraphicInfoFrame(id,0)
  ->GetFrame(frame,0,0)) textures[i] = frame.GetTexture(); else { textures[i] = GetZeroRGBATexture(); ++num_missing; } }; if (num_missing == 4)
  return; ApplyPixelShader(CloudPixelShader); blends (1 - set2 blend 0, 1 - set1 blend 0); colour 7 copied to a named local; pixel constants
  5 / 6 / 7 (colour.R, G, B, 1); UV U/V wrap on stages 0..3 (8 calls; a loop is not unrolled by VC7.1, probe); src ONE, dest INV_SRC_ALPHA,
  blend on, alpha test off; ApplyVertexShader(CloudVertexShader, false); CloudVector[k] = (History[k][0] - History[k][1]) * interpolate +
  History[k][1] (C2DVector operators, k = 0, 1 written out); SetUserVertexShaderConstant(0, CV[0].X, CV[0].Y, CV[1].X, CV[1].Y);
  PrepareForRender(0); DrawCurrentBuffersIndexed(textures, 4, 0x49, 0x14D, 0x144, 0x240).
- Header overlay: r3014's hdr/ (c2031) + two new opt-ins (hdr/README.md). 0x00B65A20 regraded under it: RELOCATION_MATCH.

### Names (two sources each)
- RenderClouds: FableWin + Ego_r (above). GetZeroRGBATexture: FableWin COMDAT right after RenderClouds + Ego_r member ZeroRGBATexture
  (+0x2AD4 landed dtor; `add edx,2AD4h`). SetUserVertexShaderConstant(KMMMM) 0x00989B00: Ego_r 0x00AE6350 overload order (C3DVector+M, C4DVector,
  MMMM, CRGBFloatColour = 0x00989B50 per c2031) + engine/CShaderRenderManager.h; the landed 0x00B52160 binds the wrong
  `?SetVertexShaderConstant@CShaderRenderManager@@QAEXJMMMM@Z` (no such name in either PDB) -- already in an earlier NAME_FIXES, repeated here.
  SetTextureUVModeU/V, Set{Source,Dest}AlphaBlendMode, SetAlphaBlendEnable/TestEnable: both PDBs (CRenderStateManager). Members CloudPixelShader
  +0x120, CloudVertexShader +0xE8, CloudVector +0x15C, CloudVectorHistory +0x16C (Ego_r layout, no containers -> retail). D3D tables: header names.
- Descriptive: locals only (texture_ids, textures, num_missing, blend0/1, colour).

### Measurements (grades: 3 -- c1 first draft, the se40 lever diagnostic, the final differ source; all other compiles are free probes)
- Grade 1 (probe/c1.cpp): DIFFER 2688v2688, 721/734 (md.py: only the two blocks above).
- c2prio (bin/c2p, free): block 1's value copy (#54, inline `value` of SetRenderState) has priority 30 (K of the long straight block 46 = 16)
  and is coloured before the bookmark temp #134 (24), takes ecx; retail needs the bookmark temp first. #134's priority follows from which
  operand the `or` result is coalesced with (5 refs when it is the destination): the real lever is the or-operand order.
- Per-block or-operand pattern (bin/orsweep.sh, free; B = `or bookmark,mask`): retail BBAAABBBBBBB, ours AAAAABBBBBBB (12 SetRenderState
  blocks: 8 UV, 4 blend). Unchanged by: line shifts, unused locals, TU context (RenderOuterSky before/after), SetRenderState spellings (3),
  FABLE_RETAIL_CARRAY off, CArray::At in SetPixelShaderConstant, TouchFrame/IsLoading/GetBank helper layering in GetFrame, named locals of every
  kind (texture-set refs, deltas, info frame, vectors), aggregate init, int index, if/else arm order, `if (num_missing != 4) {}`.
- Dead `long deadN = texture_ids[k] + N;` locals, inserted anywhere (start, before UV, end -- identical): k 0-8 AAAAA, 10-18 AAABA, 20-38 ABBAA,
  39-41 BBAAA (= retail), 42-56 BAAAA, 60-140 AAAAA. Unused `const long&` refs: 7 -> AAABA, ~40 -> ABBAA. Grade 2 (probe/v/se40.cpp):
  RELOCATION_MATCH 734/734. So the residue is purely c2's record allocation (r3005/r3010 mechanism); dead locals are a diagnostic only.
- c2 record chunks for RenderClouds (bin/ch.sh, free): base 105 chunks; se20 +2, se40 +4, refs12 +1. Removing one UV setter -4, one pixel
  constant -2, one interpolation -3, PrepareForRender -6. Retail therefore has ~3-4 chunks (~100-130 records) more IL in this function.
- Code-neutral IL spellings found (identical code except the pattern): f1 `C4DVector(C3DVector(colour.R, colour.G, colour.B), 1.0f)` ->
  AAAAB; f2 C3DVector-based blend vectors -> AABBA; f1+f2 -> AABBAA; q1/q2 (named C3DVector colour) -> AAAAB. e1 (each blend vector built from
  four repeated full `1.0f - params...Blends[0]` expressions, +3 chunks) gives retail's BBAAA but computes blend 1 after constant 5 (715 bytes).

- After the owner's "continue until both match" note: r1 (named blend vectors built up front from four repeated full `1.0f - params...Blends[0]`
  expressions, probe/v/r1.cpp) also gives BBAAA (+3 chunks: 108) but 713 bytes (blend vectors get their own storage). Chunk counts of the
  code-neutral levers: f2 106, f1+f2 106 (+1 each) -- far from the +3 the window needs.
- Free-probe count this session: ~75 compiles (orsweep / c2prio / chunk hooks), none graded against retail except grades 1-3.

### Ranked untried ideas
1. A natural source with ~3-4 chunks more IL around the pixel-constant / blend code that c2 folds to retail's code (e1 shows the region and
   amount work; needs a form that keeps both blends computed before the first constant store).
2. Real Lionhead helper layering elsewhere that adds IL without code (FableWin lists for GetFrame: operator=, AddReference, IsInitialised,
   CFrameData::IsLoading, TouchFrame, TouchTexture, CGraphicInfoFrame::GetBank; PeekGraphic -> CGraphicInfo::IsValid). Tested layers added no
   chunks inside RenderClouds; IsValid (body unknown) untested.
3. c2-side: hook the record allocator to read retail's needed record count exactly (bin/c2h7/chunks.py counts chunks only) and the coalescing
   rule for the or result (operand node keys in tmp/k3.json: bookmark loads ids 766..926, mask loads 1401..1560).

## Not attempted (1)
- 00b64550 CEngineSkyRenderer::RenderSunFlare (2631 bytes, FableWin ?RenderSunFlare@CEngineSkyRenderer@@AAEXXZ, Ego_r 0x00A17F00, `ret`):
  not started (first row not matched). Outline from the disassembly: 0x7C frame; capture block; a sun-visibility query value (call 0x00B8D3B0
  area via [0x01436E5C]) clamped 0..1; the same UV/alpha state blocks (12 SetRenderState expansions); camera transforms (FableWin
  TransformViewToScreen / TransformWorldToView listed after it); a loop over flare elements (stride 0x4C) with GetFrame-style texture fetches and
  RenderScreenSpaceSprite 0x00B63530 calls; a final sprite; frame-fetch tails like RenderClouds. Expect the same or-pattern lever.

## Skipped
None.

## Label corrections
- 0x00989B00 bound as `?SetVertexShaderConstant@CShaderRenderManager@@QAEXJMMMM@Z` by the landed 0x00B52160: PDB name is
  `?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z` (NAME_FIXES.tsv).

## Header debt
- None new. GetZeroRGBATexture returns a cast of the opaque `unsigned char ZeroRGBATexture[8]` of the retail branch (the member is a CTexture).

## Continuation session 2 (agent2048-b, 2026-11-12) -- RenderClouds grades 4..21 (log: grades.log, one line each; bin/gr.sh = grade + log)
- Coordinator note mid-session: grade more, trace less (no dead-local sweeps). Grade tool bin/gr.sh <addr> <src> <note> logs result,
  equal count, or-pattern and first difference.
- FableWin debug sizes (signatures.tsv col 5) v VC7.1 /Od of ours (bin/dbg.py; VC7.1 adds 4 per function): RenderOuterSky FW 219 = ours
  223-4 (same text); RenderClouds FW 366 v ours 356-4 = 352 (+14 in FW; 6 by-value C2DVector operator returns are a likely VC10 artefact);
  setters (UV, blend) equal; PrepareForRender equal; SetRenderState FW 213 / AddStateToUpdateList 108 / scoped capture ctor 93, dtor 92 /
  PeekGraphic 170 / PeekGraphicInfoFrame 108 are far larger than any release body (FableWin debug checks; no game assert strings in retail).
  FableWin ??0CRenderStateCaptureBlock 10 instr (= Open = false), ??1 13 instr (= if (Open) Restore();).
- New overlay opt-ins tried (all code-neutral; default text unchanged): FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE (scoped block derives
  from CRenderStateCaptureBlock, inline Open bodies) -> pattern AAAABBBBBBBB (block 5 flips; 718 eq); RenderOuterSky stays
  RELOCATION_MATCH under it (grade 15). FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL (GetR/G/B/A), FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
  (GetFrame -> TouchFrame + private GetBank), FABLE_RETAIL_CFRAMEDATA_ISLOADING: no pattern change alone; TouchFrame cancels the capture
  block's flip.
- Source candidates graded, no change (721, AAAAABBBBBBB): const 7 via C4DVector(colour)+W=1 (g5), GetR accessors (g6), CloudVector
  b + (a-b)*t (g7), `bool found` (g14), PeekGraphic if-return (g22). Code changes: CloudVector loop (g8), nested ifs (g13), texture local (g15).

### RenderClouds: ranked untried ideas after session 2
1. Combinations of the capture-block opt-in (moves block 5 only) with other code-neutral layers that move blocks 1-2 (none found yet): the
   pattern is a record-id tie and responds non-additively; candidates still ungraded: C4DVector(colour) for constant 7 together with the
   capture block, `const CIDrawEnvironmentParams::CThemeChannel&` reads combined with the capture block, PeekGraphicInfoFrame via a named
   `const CGraphicInfo* info` (code change risk: retail evaluates PeekGraphic twice).
2. Write RenderSunFlare to a match first: it shares the capture block, the 4 blend SetRenderState blocks (its retail pattern BBBB is already
   reproduced by the draft) and the texture fetch helpers; whatever capture-block / helper body it needs (one body for all rows) must also be
   used by RenderClouds and may shift RenderClouds' records.
3. The r3010 c2 model (agent3010/tools/c2h7) for the OR operand keys: the predecessor's key.py hook (0x1075CD1D) gave NO events for
   RenderClouds' or-blocks (the canonicalisation runs before the function's pipe event; with target PrepareForRender 280 events, none naming
   BookmarkMask) -- the deciding hook point is still unknown.

## RenderSunFlare (0x00B64550): draft state after session 2 (grades 22-27, 30)
Statement list (proven by retail control flow): capture block; params; visibility = g_pOcclusionManager->GetPrevFrameFlareTestResultFromRetrivalID
(0x1000000); `if (v < 0) return; if (v > 1) v = 1; else if (v < 1/255) return;`; `C3DVector sun = PeekSunVector();` (copy: Y/Z to frame);
Source ONE, Dest INV_SRC_COLOR (table index 3, 0x01396F74), AlphaBlendEnable(true), ZBufferEnable(false) (+0x2868 = DeviceState +0x50);
screen = sun * 2000 + PeekCameraPos(); TransformWorldToView; `if (!(screen.Z > 0.01)) return;` (double literal); TransformViewToScreen;
centre = GFVXToAX(32768), GFVYToAY(32768); for i < FlareElements.size() { texture id 0 -> break; not loaded -> BeginLoading(0,0) and next;
CGraphicFrame frame; PeekGraphic(id)->GetFrame(frame,0,0,1) -> size = (long)GFVXToAX(Radius); delta = screen - centre; ratio = |delta| / |centre|
clamped 0..1 (ternary form); colour = CRGBFloatColour(element.Colour) * Colours[11] * visibility * ratio; pos = delta * Position + centre;
RenderScreenSpaceSprite(texture, pos - size, size*2, size*2, colour.GetRGBColour()) }; then the sun-flare sprite (SunFlareTexture +0x50,
SunFlareRadius +0x4C as float, Colours[10] * visibility, flags 0; not loaded -> BeginLoading(0,0) and return).
Remaining diff of differ/00b64550.cpp (grade 32; grade 31 = wrapping everything after the Z test in `if (screen.Z > 0.01) { }`: 597 -> 608):
- frame 0x80 v 0x7C (one extra word);
- `if (!(screen.Z > 0.01)) return;` emits its own `je ; fstp st(0) ; jmp exit`; retail `jne` to a shared `fstp st(0)` at 0x00B64F82 before the
  common exit -> try wrapping the rest of the function in `if (screen.Z > 0.01) { ... }`;
- loop head: ours aligned (`jmp ; lea esp,[esp]`), retail falls into the body with the counter in [esp+24h] and count in [esp+3Ch]; tried:
  if+do-while (worse, grade 25), texture test in the for condition (grade 26), if/else instead of continue (grade 27);
- ratio: retail `fst [dx] ; fst [dy]` then |delta| from st copies (`fld st0 ; fmul st1 ; fld st2 ; fmul st3 ; faddp ; fsqrt ; fstp st2 ;
  fstp st0`) and |centre| from memory -> try a C2DVector delta / centre with GetLength (check C2DVector header for the PDB accessor);
- final sprite / exits: retail has three RestoreCaptureBlock call sites (two early exits after BeginLoading + the end).
Ranked next steps: (1) DONE (grade 31; the inner early returns of the final sprite may also want the nested form), (2) C2DVector delta/centre with GetLength, (3) loop shape (index loop reading PSkyDef each time
is right; look at the alignment rule in GOTCHAS: `for` + break v `while`), (4) frame word, (5) write the behaviour test (none yet).
Names: all from FableWin / Ego_r except IsFrameDataLoaded (descriptive inline) and the three engine globals (bound spellings, no PDB data
names); NAME_FIXES.tsv lists BeginLoading (const), CGraphicInfo::GetFrame and RenderScreenSpaceSprite spellings.
FITTED v proven: CSkyDef layout (proven by offsets), TransformViewToScreen body (fitted to retail x87; FableWin name), IsFrameDataLoaded body
(fitted), CFrameData::IsLoading body (fitted, unused), capture-block inline bodies (FableWin sizes agree; FITTED, unused).

## Session 3 (agent2048-c, 2026-11-12): grades 33..115 (grades.log, one line each)
### RenderSunFlare 0x00B64550 -- RELOCATION_MATCH (grades 33..48, 94, 96, 98, 114; 14 row grades)
Closing steps (each graded): s8 final sun sprite inside `if (sun_texture != 0) { }` (its frame shares the loop frame's slot: frame 0x7C, the three
RestoreCaptureBlock epilogues appear; 608 -> 687); s15/s16 flare ratio on unnamed temporaries `C2DVector(screen.X - centre_x, screen.Y -
centre_y).GetMagnitude() / C2DVector(centre_x, centre_y).GetMagnitude()` with the position repeating `(screen.X - centre_x) * position +
centre_x` (the difference is a CSE kept on the x87 stack; 722 / 727); s17 FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS (existing repo opt-in:
X row E21*y+E31*z first, Y/Z rows left to right; 737); s18 the unloaded-texture test as if/else around the frame instead of `continue`
(the zero store of the frame's dtor dies; RELOCATION_MATCH). Tried and worse: named C2DVector centre/delta (frame 0x88..0x94: by-value
C2DVector temporaries are not scalar-replaced; pack(1) is not the cause, diagnostic grade 42), sqrtf on named floats, separate length statement.
FITTED v PROVEN: final-sprite scope, if/else, ratio/position spellings = FITTED to retail bytes (natural Lionhead forms; FableWin RenderSunFlare
473 /Od instr v ours 442 under /EHsc). GetMagnitude / C2DVector(float,float) = PDB names (Ego_r + FableWin). TRANSFORMPOINT_SUMS = existing proven opt-in.
src/ defines the session-3 helper opt-ins too (one body per helper for the three rows); it matches with and without them.
Possible spelling (not needed): FableWin lists ?GFMakeOccluderRetrivalID@@YIKW4OCCLUDER_RETRIVAL_SUPER_ID@@K@Z near Render; the literal
0x1000000 passed to GetPrevFrameFlareTestResultFromRetrivalID may be GFMakeOccluderRetrivalID(<super id 1>, 0) (enumerator name unknown).
### RenderClouds 0x00B64FA0 -- DIFFER 731/734 (session 3 grades 49..115 minus the sibling ones, ~55 row grades)
- Coordinator lead: FableWin debug sizes (fwsize.py) are a per-helper text oracle. Fitted (hdr/README.md, session 3 table): named-result
  C2DVector -/* (AABBA), ReleaseData layer, GetBank / GetCurrentFrame direct, IsLoading !IsNull, TouchTexture via Get, PSConstTable[index].
- Grade map (pattern; retail BBAAABBBBBBB): base AAAAAB..; +named -/* (c5) AABBA; + all fits (c10) AAABA; c10 + f1 + f2 (C4DVector(C3DVector(..),w)
  for constants 5/6/7) AABBAA; + capture INLINE (c28) BBBAAA (728); + GetR/G/B for constant 7 (c43) BBAAAA (731) = block 6 only.
- Sensitivity around c33/c43 (one change each): ReleaseData, PSCONST_INDEX, TOUCHTEXTURE_GET (on c33), CURRENTFRAME_DIRECT, GETBANK_DIRECT,
  RELEASE_DELETES/MEMBER (on c28), CloudVector b+(a-b)*t, `texture_id &&`, num_missing++, const colour, frame before bank, RELEASE_INNER:
  no change. Changes: IsLoading body (any other spelling -> AABBAA), aggregate-init ids (-> AABBAA), f2 off (-> AABBA),
  RELEASE_DELETES+MEMBER on c43 (-> BBBAAA), TOUCHTEXTURE_GET off on c43 (-> BBBAAA).
- Next ideas (ranked): (1) TouchFrame text (FableWin 69/228 v ours 64/209: +5 instr, still unfitted) and CIVCountedPointer::Release (17/50) /
  pointee Release (24/58) texts -- each is natural IL in the texture loop; grade each on c43. (2) Remaining row /Od gap under /EHsc
  (RenderClouds 348 v 366, RenderOuterSky 208 v 219, RenderSunFlare 442 v 473; all ~2.7 bytes/instr: small ops) -- a construct shared by the
  three rows; may be VC10 codegen. (3) Other FableWin accessors on the constants path (blend reads, C4DVector / C3DVector forms).
- Names: GetR/GetG/GetB (FableWin), ReleaseData (FableWin private, labelled `Clear` in FableWin.exe.signatures.tsv), CIVCountedPointer::Get (FableWin).
