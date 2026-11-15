# wfB4 NOTES (author agents, 2026-11-14/15) -- three render rows; 0x00BCC800 MATCHED (second agent); 0x00B8B780 MATCHED (fourth agent, 4 dead asserts); 0x00B89C30 RELOCATION_MATCH with FITTED devices (fifth agent; coordinator decision)

## Shared finding (rows 1 and 2): how VC7.1 picks the bookmark-test operand order, and a 3-minute tool that prints the numbers
- Tool (no new hook machinery: agentC20's sched2/host.py + the repo's c2probe/c2dbg): `k/run.sh k/cmp.py <src> <out.json>` breaks on c2's
  merge-sort comparator 0x1076A394 (operands in edx / ecx; opcode word at +4, key at +0x10), `k/ana.py <out.json>` keeps the row's events,
  `k/ab.py <name>` (row 1) prints the decisive keys per SetRenderState block, `k/m.sh <src...>` runs a batch into k/TABLE.txt. ~3.3 min a
  trace (row 2: ~6 min), a few hundred bytes of output. The trace reproduces the graded block pattern of both rows exactly.
- The order is decided in TWO compares per inlined `state.BookmarkMask & CurrentBookmarkMask`:
  1. FIRST PASS (when the setter is expanded; operands op 0x252, flags 0x2004 0006): keys 32*k + 62 v 32*m + 22 where k / m are record ids
     (rotl16(id,5)); it fixes the incoming order (a, b) of the second pass. Ids are taken from a FREE LIST (step 3 per block) until it is
     empty, then from the high-water mark; an id >= 2048 wraps (rotl) and swaps the incoming order.
  2. SECOND PASS (row's own pass; event 76 of each 199-event block region, regions end on the `(op 0x278, op 0x250)` marker): two temps,
     keys 64*id mod 65536 (id mod 1024). One sequence grows 36-37 per block ("beta"), the other 19-22 ("alpha"). Retail-order S
     (`test S,C`: state mask loaded first) iff a > b positionally.
- Dead FABLE_RETAIL_ASSERTs: position-independent, about +8 each to BOTH second-pass sequences in row 1 (granular: 1 often = no change);
  they never touch row 2 (its residue is in the first pass). One named local / one trivial inline accessor moves alpha by 23..36 at once
  (coarser than row 1's 16-id window), which is why spelling sweeps jump over the retail state.

## 0x00B8B780 CEngineBillboardGenerator::RenderBillboard -- DIFFER(3921v3921), 3 sites; near miss, not closing
- Start (regrade of prev/differ on this overlay): DIFFER(3921v3921), sites 0x00B8BC75, 0x00B8BCE2, 0x00B8C393 (pattern CCCCSSSCCCCCC v retail CCSSSSSSCCCCC;
  one letter per setter block: AlphaTest, AlphaBlend, AlphaRef, AlphaTestFunc, Fog | ZBuffer, TexArg1, TexOp, TexAlphaArg1, TexAlphaOp, AlphaBlend, AlphaTest, LODBias).
- differ/00b8b780.cpp (w1/r4.cpp): every text below is code-identical to the old draft outside the three sites; sites now 0x00B8BC75, 0x00B8BCE2, 0x00B8C401.
  * NEW STRUCTURE (EVIDENCED): `view.SetTranslateOnly(0.0f, 0.0f, 0.0f); world.PostMultiply(view);` replaces the draft's twelve-sum
    "SetMultiplyNoTranslation" of the FABLE_RETAIL_CMATRIX3X4_BILLBOARD opt-in (a body that contradicts retail's own SetMultiplyNoTranslation
    0x00A4B820 = 9 sums + clear, and FableWin's 160i). VC7.1 folds the `+ 0.0f` terms and drops the dead zero stores: bytes 0x00B8B9CB..0x00B8BAF3
    unchanged. It also explains the by-value copy of NoScaleViewTransform (rep movsd 0x00B8B969). fwsize: SetMultiply 205i/575b, PostMultiply 21i/48b,
    SetTranslateOnly(MMM) 16i/40b all SAME. Existing opt-ins only (CMATRIX3X4_SETMULTIPLY + _PREMULTIPLY + _POSTMULTIPLY, _SETTRANSLATEONLY + _XYZ).
  * FableWin COMDATs emitted with this TU (0x02DBF1B0..0x02DBF920, right after RenderBillboard 0x02DBE530) name the accessors the row uses:
    GetMipmapSizeMultiplier, GetBoundingSphere, C3DTextureStages(), C3DPrimitive2::GetAverageTextureStretch / GetVertexCount / GetMaterialIndex /
    GetVertexBuffer, C3DMeshMaterial::GetDiffuseMapBankIndex, C3DMesh2::GetMaterial / GetPrimitiveCount, CShaderRenderManager::SelectVertexBuffer +
    GetD3DDevice, scoped_ptr::get. All but GetVertexCount are now in the text (existing opt-ins C3DMESH2_PRIMITIVES, C3DPRIMITIVE2_RENDER,
    C3DPRIMITIVE2_GETVERTEXBUFFER, C3DTEXTURESTAGES_CTOR; the ctor's two dead -1 stores give retail's store order byte / +0x54 / +0x58).
    GetAppropriateMipmap and GFLimit<long,long,long> are emitted LATER in FableWin (0x02E74CC0, 0x032525F0): this TU does not use them
    (the mip expression and the clamp are hand-written here, as in the draft).
  * GetTranslation plain (FableWin 22i = 19i + 3 later-compiler float pushes; the NAMED opt-in is no longer needed: 0x00B8C325 stays right),
    FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX (28i SAME), globals in PDB spelling (GPCamera, GPEngine, GPPrimitiveRendererSharedData, GTopBitInByteTable).
  * In-row helper bodies of the differ (to become header opt-ins if the row closes): CShaderRenderManager::GetD3DDevice / SelectVertexBuffer,
    CVertexBufferBase::GetD3DVertexBuffer / GetStride (9i each SAME), CGraphicInfoFrame::GetWidth, PeekGraphic / PeekGraphicInfoFrame, FlushRender.
- Measured state (k/TABLE.txt; variant r2 = differ minus GetTranslation/PSConst/SelectVertexBuffer texts, 2 asserts):
  alpha [138,160,177,196,216,325,345,364,383,402,421,441,458], beta [892,929,965,1001,1037,1285,...,1537]. Retail needs
  (a) beta of the pre-loop blocks +59..+94 (the wrap past 1024 between AlphaBlend and AlphaRef) and (b) (beta-1024) between alpha at TexOp and TexAlphaArg1
  (a 16-id window). Every text change tried moves alpha by 23..36: GFLimit +34/+36, SelectVertexBuffer accessors +32/+25, plain GetTranslation + PSConst -36.
- Diagnostics (rulers, NOT in the hand-back): differ + 7 or 8 asserts = ONE site (0x00B8C401; start fixed, end one block late); the same text without the
  SelectVertexBuffer accessors (w1/r3.cpp) + 8 asserts = ONE site (0x00B8C393; end one block early). 48 combinations of {GetPrimitive, SelectVertexBuffer
  none/GetD3DDevice/+GetD3DVertexBuffer/full, GFLimit, N=6..8} and 20 named-local variants (material / frame / vertex_buffer) x N=7,8: end is 6 or 8, never 7.
- Not text (measured): operator*(float) FableWin 31i and operator+/- 34i = the default one-expression bodies under the later compiler's float rounding
  temporaries (+2i per binary float argument); GFFloatToLongFloor 21i = the two constant stores as fld/fstp. sphere.GetRadius()/GetCenter() change code (0x00B8BB4A).
- Next: find the alpha step that lands in the window (needs alpha +7..+22 against w1/r3 once the start is right): place GetVertexCount (FableWin-evidenced, use
  unknown), check the loop-2 vertex set-up for helper texts (CTVertexRHWColTex1 setters), then re-measure with k/m.sh before any assert count is fitted.

### 0x00B8B780 -- successor measurements (2026-11-14, second agent); still DIFFER(3921v3921), the same 3 sites
- differ/00b8b780.cpp now: (1) CRenderTarget from the SHARED engine/CRenderTarget.h (the local class model is gone: the strict-header check would have
  rejected it; same bytes); (2) ONE dead assert, in the mesh loop, `FABLE_RETAIL_ASSERT(primitive.GetVertexCount() > 0)` (needs the existing overlay
  opt-in FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS for the accessor) instead of the two inherited top-of-function asserts. Evidence for ONE: the row at
  /Od /EHsc /GS is 524i / 2287b v FableWin 645i / 2558b; 27 constant float arguments explain +54i / ~+35b, the rest is +67i / +236b = one assert
  (+63i / +223b) plus ~4 instructions = an accessor call in its condition; C3DPrimitive2::GetVertexCount is first emitted with this TU and no retail
  byte reads VertexCount. The condition itself is inferred. Block letters are the same with 1, 2 or 3 asserts wherever they stand (0 asserts: CCCCCSSSSCCCC).
- Row-level rulers (diagnostic only): unused named locals change nothing up to 8 (9..12: CCCSSSSCCCCCC); every dead assert adds 8 to both second-pass
  sequences, wherever it stands and whatever its condition.
- The landed FITTED SetRenderState opt-ins of the shared header (never tried on this row before): SETRENDERSTATE_DESIRED or ADDSTATE_DIRTY alone
  -> CCCSSSSCCCCCC; + 4..6 asserts -> CCSSSSSCCCCCC (start right, end ONE BLOCK EARLY, 0x00B8C393 side); DESIRED + BOOKMARK + ADDSTATE_DIRTY ->
  CCCSSSSSSCCCC (retail shifted by one block at both ends); + 4 asserts -> CCSSSSSSSCSCS. No opt-in + 7..8 asserts -> CCSSSSSSSCCCC (end one block
  late). Unused locals on top of the near hits do not move the end. So the end is quantised at 7 (one more symbol per setter expansion) or 9 (none):
  retail's 8 needs the loop-2 `alpha` 7..22 above / 3..18 below those states, i.e. a change BETWEEN the pre-loop setters and loop 2 (one named local more
  or less in the mesh loop / loop-2 head), on top of a natural source for the pre-loop +59..+94 of `beta` that is still not found (only asserts move beta).
- In-helper asserts are NOT the missing source: a dead assert inside SetRenderState / AddStateToUpdateList (FableWin: 2 + 1 there) wrecks row 2's
  first-section letters (all S / mixed) and with three per expansion changes the code size: retail's helpers expand without assert residue.
- FableWin assert sizes come in two classes: ~+63i / +223b (operator->, FindID, PeekGraphicBank) and ~+82i / +318b (capture block ctor / dtor, GetWidth,
  GetD3DDevice, GetMaterial, PeekInfoFrame, AddStateToUpdateList); SetRenderState and StartCaptureBlock = two of the large ones each.

### 0x00B8B780 -- third agent (2026-11-15, acceptance-test devices): still DIFFER(3921v3921); S-run (start,end) map over the allowed devices
- Tool: `w4/sw1.py <base.cpp> name:A:n[:defs[:old=>new]]` (A = total dead asserts: the in-loop GetVertexCount one + A-1 at the top; n = INITIALISED unused locals;
  defs = D,B,A for the landed SetRenderState opt-ins or raw macro names) and `w4/lets.sh <prefix>` (letters of everything already graded). ~6 s a grade.
- Retail S-run = blocks 3..8 (CCSSSSSSCCCCC). Measured (start,end) on the differ text, no opt-in:
    n = 0..7 : A=0 (6,9)  A=1..3 (5,9)  A=4,5 (4,9)  A=6..8 (3,9)  A=9..12 (2,9)      -> the end never leaves 9 (asserts move both id sequences together)
    n = 8..12: A=0 (5,7)  A=1..3 (4,7)  A=4,5 (3,7)  A=6..8 (2,7)                      -> one jump at n = 8: start -1, end -2 (the `beta` side takes a new
               32-id chunk; blocks 8 and 9 flip together). The earlier "unused locals change nothing up to 8" was measured with UNINITIALISED locals.
  SETRENDERSTATE_DESIRED: A=0 (5,7), A=1..3 (4,7), A=4 (3,7) for every n = 0..12 (end 7; a stray S at block 13 for n = 8/10).
  => asserts x initialised unused locals x landed opt-ins never give end 8: (beta - alpha) moves 0 or 32, retail needs +3..+18 (block 9 is S by a margin of
     about 2 ids at A = 6, block 8 by about 19).
- Code-identical EVIDENCED respellings found (same bytes, same letters at every grid point tried; candidates for the final text, in w4/b_peek.cpp / b_g1.cpp):
  * `CMatrix3x4 view = NEngineInternal::GPCamera->PeekNonHomogenousViewTransform();` (existing opt-in FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM;
    FableWin.pdb ?PeekNonHomogenousViewTransform@CEngineCamera@@QBEABVCMatrix3x4@@XZ) instead of the direct member read;
  * `GFLimit(level, 1, 255L);` for the 1..255 clamp: FableWin has ??$GFLimit@JHJ@@YIXAAJABHABJ@Z at 0x01A93F80 (an EARLIER TU, so it is not in this TU's
    first-emitted list; the JJJ instance the earlier agents looked for is the one emitted later) and retail's clamp 0x00B8BF01..0x00B8BF19 is exactly the
    template's `if (value < lower) value = lower; else if (value > upper) value = upper;` shape (jge / mov 1 / jmp / cmp 0xFF / jle / mov 0xFF). Worth +11 `alpha`.
  * CEngine::GetScratchZBufferD24S8 returns CSurface& in FableWin.pdb (?GetScratchZBufferD24S8@CEngine@@QAEAAVCSurface@@J@Z): the differ's
    `*(const CSurface*)` cast hides an overlay declaration with the wrong return type (not changed: needs an opt-in edit).
  * the const CArray<CVertexShaderConstant>::GetAsCArray chain of PrepareForRender (see row 0x00B89C30, probe/hdr_const): letter-neutral here too.
- Single levers measured by letters on the two flanking bases (A=6,n=0 -> (3,9); A=5,n=8 -> (3,7)): direct table read instead of GFGetTopBitInByte, no GetStride
  accessor, a named `material` local, GFLimit: no letter moves; no GetD3DDevice accessor: (3,9) stays, (3,7) becomes (4,9).

- Second-pass ids at the two flanking states (k/TABLE.txt, traces pk_A6_n0 / pk_A5_n8 / xL4b_A6; `alpha` = loop-2 blocks 6..10, `beta` mod 1024):
    A=6,n=0: alpha 378 398 417 436 455, beta 326 362 398 434 470 -> block 8 is S by 19, block 9 is S by 2 (retail: C): needs alpha -3..-18 (or beta +3..+18)
    A=5,n=8: alpha 346 366 385 404 423, same beta                 -> needs alpha +14..+29
  Every acceptance-list device is quantised at 32: the 8th initialised unused local adds 32 to `beta` only; asserts move BOTH sequences together, in block-sized steps at A = 1, 4, 6, 9
  (inferred from the letter map: the start moves, the end never does; one traced pair, A=5/n=8 v A=6/n=0, differs by exactly 32 in alpha); so (beta - alpha) moves 0 or 32 and never lands in the 16-id window. Only TEXT moves `alpha` finely and positionally (text between the
  pre-loop setters and loop 2 moves loop-2 `alpha` only): GFLimit clamp +11 (pre-loop +2); the GetD3DDevice() accessor inside SelectVertexBuffer -7 (without
  it: alpha 385 at A=6, k trace xL4b_A6); the camera Peek accessor, the const PrepareForRender chain, GFGetTopBitInByte, GetStride / GetD3DVertexBuffer,
  a named `material` local: no letter change at either flank. Fine effects do not add linearly with the 32-id jumps (GFLimit + no GetD3DDevice on A=5 with
  n = 8, 9, 10: still (3,7)): each candidate needs its own 3-minute trace (`k/m.sh <ABSOLUTE path>`; a relative path fails silently).
- What would close it: one more by-value inline accessor layer of the GetD3DDevice class (about -7) anywhere before loop 2 on the A=6..8, n<8 text, i.e. an
  accessor the original used where the differ reads a member directly. Unverifiable helper texts left (FableWin bodies carry asserts): CRenderManagerCore::FlushRender
  (48i v 252i), SelectVertexBuffer (both classes), CGraphicInfoFrame::GetWidth, PeekGraphic / PeekGraphicInfoFrame, CEngineBillboard member access (no header,
  no accessor names in either PDB). FableWin sizes of the remaining helpers are SAME or later-compiler float differences (SetIdentity 31i v 43i = 12 fld/fstp stores).
- The assert count is therefore NOT fitted by this agent: the differ keeps the one in-loop assert (FableWin-sized); 6..8 asserts are only the diagnostic that
  puts the start right. Hand-back candidates if a later owner accepts a ruler: none found that matches.

### 0x00B8B780 -- fourth agent (2026-11-15): RELOCATION_MATCH, test PASS, addmatch done (src/00b8b780.cpp, test/00b8b780.cpp, flags /GS)
- What closed it: (1) the `const float radius = sphere.Radius;` local of the differ removed -- `billboard->Scale = sphere.Radius * 2.0f;` and
  `world.GetTranslation() * sphere.Radius` (code-identical; that named local was the source of the loop-2 `alpha` offset: without it the S-run END
  moves from block 9 to block 8 = retail at every assert count 0..9); (2) the hand-written 1..255 clamp of the draft instead of the third agent's
  GFLimit(level, 1, 255L) respelling (GFLimit puts the end back at 9 under (1)); (3) dead asserts for the START (only they move `beta`):
  four FABLE_RETAIL_ASSERT statements = mesh, billboard, distance > 0.0f at the top + the in-loop primitive.GetVertexCount() > 0.
- Measured (w5/tg.py single toggles on the A=6 no-GFLimit base: only `radin` matched; w5/as_*.cpp condition sets): the start depends on the number
  of assert statements AND their IL weight: 3 statements in every tried form (2 top + loop, 3 top w/o loop, heavy && conditions) start at 4..5;
  4 statements match with (mesh, billboard, alphaRef >= 0 | distance > 0.0f | billboard->Texture), not with (mesh, mesh, mesh) (start 4);
  5 with two float compares overshoot (start 2); 6..8 x `mesh` match. Chosen: the smallest count (4), parameter checks a Lionhead author writes.
- FITTED / for the coordinator: the assert COUNT is a device (d) ruler (`// Inferred:` in the source). The FableWin /Od size suggests one assert;
  three of the four are fitted. Every other text is the third agent's differ (camera Peek accessor, CRenderTarget shared header, etc.).
- Toggles that did NOT move the letters (A=1 / A=6 bases): `!graphic`, mip expression inlined into GFFloatToLongFloor, const halfTexel / width /
  size / graphic / topBit, verts declared earlier, parenthesised GetPrimitive, const scale / const sphere / view(...) direct-init. By-vertex order
  of the quad stores changes the code (12 hunks): the by-field order of the draft is retail's.

## 0x00B89C30 CEngineScreenEffectOutlineGlow::ApplyOutline -- DIFFER(6431v6431), 1 site 0x00B8AF59 (SetAlphaRef(1)); parked, cause located
- differ/00b89c30.cpp = prev source unchanged (regraded: same single hunk). Block pattern ours ...CCCCSSCSSCCCCCCC v retail ...CCCCSCCSSCCCCCCC (final section).
- Cause (k/row2_base trace): the FIRST-pass key ids of the final-section blocks are 1681, 1684, 1686, 1689, 1692, 1695 (SetAlphaRef) then 2049, 2052, 2055:
  the record free list runs dry after SetAlphaRef, the next ids come from the high-water mark (>= 2048 wraps) and the incoming operand order swaps from
  SetSourceAlphaBlendMode on. Retail swaps one block earlier: 1..11 more records taken from that list anywhere before the SetAlphaRef expansion close the
  row (12 or more would also flip SetAlphaTestFunc). Second-pass ids at the three blocks: (946,943) (982,959) (978,1020): no uniform shift can fix it there.
- This is why 1..8 dead asserts never moved it (they allocate nothing from that list at that time) and why acctB's invented one-copy wrapper did.
- The draft's `{ CRenderTarget current = GetRenderTarget(); OldRenderTarget = current; }` is itself FITTED: the natural `OldRenderTarget =
  NEngineInternal::GPDisplayManager->GetRenderTarget();` (w2/v1.cpp) is code-identical elsewhere and leaves TWO sites (0x00B8AF59, 0x00B8AFCA): the real text
  has about one block's worth more records than v1, from a source not yet found.
- FableWin: the only COMDAT first emitted with this TU is ?GetPRenderToSurface@CRenderTarget@@QAEPAVCSurface@@XZ (0x02DBE120, 9i): some function of the TU
  (BeginOutlineRendering / FinishOutlineRendering / ApplyOutline / DrawDebugTextures) uses it; an inline accessor is exactly a 1-record source. Not placed:
  no retail byte in this row shows where. ApplyDebugParameterOverriddes(float&, float&, CRGBColour&, long&, long&) (0x02DBDFC0, 31i) copies the five
  Overridden* members and belongs to the outline primitive path, not to this row. The row is 523i v FableWin 619i: 38 constant float arguments
  (later compiler: 3 instructions each instead of 1) explain 76 of the 96, so at most one small call is missing, not an ASSERT.
- Volatile statics left as ruled. (Untested alternative for a later owner question: `1.0f * offset` / `-1.0f * offset` argument expressions.)

### 0x00B89C30 -- successor measurements (2026-11-14, second agent): the first-pass id map, and why no uniform count closes it
- Landed FITTED SetRenderState opt-ins on this row (SETRENDERSTATE_DESIRED / _BOOKMARK / ADDSTATE_DIRTY, all 5 combinations, on v1 and s0): none gives retail; the closest (DESIRED+BOOKMARK or DESIRED+DIRTY) is `CCCC S x18 | CCCC S C SS C x8` (first section one block off, final section one block off). FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN alone does not compile (cl spins: kill by PID).
- Tool: `k/t2.sh <w2 name...>` (5 min a trace) + `k/ids.py <name...>` prints, per setter block, the two first-pass record ids (k from key 32k+62,
  m from 32m+22; `?` = id >= 2048, the wrap that swaps the operand order). Grade-only proxy: `tools/p2.sh <src>` prints the block letters (2 s).
- Decomposition (final 16 setter blocks; retail CCCC S CC SS CCCCCCC): letters = fixed second-pass base `CCCC SSS CC SSSSSSS` XOR "swap from block p on"
  (p = first block whose k id >= 2048). Retail p = 6 (SetAlphaRef); natural text v1 (`OldRenderTarget = GetRenderTarget();`) p = 8; the fitted block (s0) p = 7.
- Ids (v1): k = 1679 1682 1684 1687 1690 1693 1695 then 2050 2053 ... : the final section first drains a FREE RUN that ends at 1696/1697, then takes new
  ids from the high-water mark H ~ 2049. Earlier free runs feed the first section (369..383, 704..737, 1122..).
- Every named local / temporary object adds exactly 1 to all later ids, wherever it is declared (position sweep top / pass 1 / loop / final: identical):
  n = number of extra symbols over v1. Measured p(n): n=0 -> 8; n=1,2 -> 7 (s0 = n 2; `float texel`, `long vertex_count`, an unused long: all the same);
  n=3..6 -> NO swap in the final section (regime change: the first run 369..383 ends one block earlier, the later runs start 64 / 32 lower, the final run
  is 1586..1597 and H is 2016: s0_a trace); n=7,8 -> 16; 10 -> 15; 12 -> 14; 16 -> 13 and the second-pass base starts to change. p = 6 is never produced:
  the retail state is NOT a uniform shift of our text (this is the measured reason behind the ~900 spelling grades of the earlier lane).
- So the fitted block of the differ is not needed for anything: v1 + any one or two ordinary locals gives the same one-site state.
- Not symbols (no effect at all on the letters): scopes (`tex` block, `long i` before the loop), statics at file scope / after the early return, `== 0`,
  `i++`, inline float accessors for the offsets (also give integer moves: the volatile ruling stands), empty inline calls with constant arguments
  anywhere, `-1.0f * x` / `1.0f * x` (not folded: fmul). One function-scope `textures` array filled by assignments changes the code (6390 bytes).
- What would close it (target for a later idea): keep the first-section state of n = 0..2 and consume 2..5 more ids between the first section and
  SetAlphaRef (final run start 1683/1684 instead of 1679..1681), or stay in the n = 3..6 regime with H 32 higher. Only something that takes ids at
  expansion time in the middle of the function does that (a setter-like expansion does, a named local does not); no natural candidate found.
- FableWin: ApplyOutline 619i/2312b v ours 523i/2092b (/Od /EHsc): 38 constant float pushes = +76i (34 of them zeros = fldz, +1b each); the rest is
  +20i / ~+166b = long instructions (absolute addresses), i.e. debug-only code (tweak registration / static guards), not a helper call. The
  FableWin COMDAT list of this TU has no TU-local helper function, and GetPRenderToSurface (first emitted here) without GetPZBuffer (first emitted by a
  much later TU) fits an ASSERT in FinishOutlineRendering (104i v ~40i), not this row.

### 0x00B89C30 -- third agent (2026-11-15, acceptance-test devices): still DIFFER(6431v6431), 1 site 0x00B8AF59; the closing condition is now exact
- Tool: `w4/sw.py <base.cpp> name=ANCHOR:text|R:old=>new|D:defines ...` (one line per variant, block letters, 2 s each; anchors TOP/VP/M0/L/F/U/LIN/AT/AR/END/FN).
- NOT TU context: one, three or a full-size copy of the row defined above it (w4/t1..t4, tu_full, tu_half) leave every letter unchanged. c2's ids are per function.
- Rulers, measured again: UNINITIALISED unused locals do nothing (c1 drops them); `long unusedK = 0;` is the uniform ruler (position-independent: top, loop,
  nested block, right before SetAlphaRef all equal). The fitted `{ CRenderTarget current = ...; OldRenderTarget = current; }` block of the differ is worth n = 1
  (not 2). Uniform sweep n = 0..70 on the natural text (w4/u_N): retail's final letters CCCCSCCSSCCCCCCC never appear (n = 1, 2 and 26 give the one-block miss).
  Landed FITTED SetRenderState opt-ins (D, DB, A, DA, DBA) x n = 0..10: no match (table in ATTEMPTS.log, names oD_*, oDB_*, oA_*, oDA_*, oDBA_*).
  Dead asserts (also with an inline accessor in the condition, e.g. OldRenderTarget.GetPRenderToSurface()) at six positions: no letter moves.
- EXACT CLOSING CONDITION (diagnostic probes, NOT landable): the SetAlphaRef block's first-pass `state` id must be >= 1696 (end of the free run 1679+n..1695;
  the next id comes from the high-water mark 2048+ and swaps the operand order). Its id is 1693 + n + (number of inline PARAMETER temps alive while SetAlphaRef
  is expanded), and n <= 2 or the first section's run 369+n..383 breaks (n = 3: other regime). So: n = 2 plus ONE forwarding inline layer around the call
  (`inline void PE(unsigned long r) { GPRenderManager->SetAlphaRef(r); }`, w4/iPE_2 RELOCATION_MATCH), or n = 1 plus a layer with a local (w4/iPD_1, = acctB's x3),
  or a 3-parameter layer on n = 0/1 (acctB's m4). The parameter temps are freed when the layer ends: wrapping the three setters BEFORE SetAlphaRef
  (w4/j3) changes nothing, wrapping SetAlphaRef alone is enough. Argument spellings never create that temp (n = 2 base: DA(1) pass-through inline, DZ(),
  unsigned char cast, `true`, a non-const local, a static const, `cond ? 1 : 1`: w4/kA..kS all unchanged); a row-level local is only the uniform ruler.
  Inlinee locals / multi-return inlines / temporary objects in an argument are uniform too (w4/hB, hF, hG, hAA).
  => No device of the acceptance list reaches it: (a) no row-level spelling nests a call, (c) is uniform, (d) does nothing here, (b) does not apply.
- Helper texts checked against FableWin once more (fwsize, FW_EXTRA=/EHsc for rows / EH helpers): every setter, PrepareForRender 158i/565b, CSurface ctor/dtor,
  CRenderTarget ctor 28i/102b / dtor 25i/90b / operator= 24i/64b, GetRenderTarget 20i/56b: SAME. No combined setter (AlphaTest / filter / address) exists in
  either PDB name set, so retail has no real extra layer at this call that a debug build would show.
- EVIDENCED finding (code- and letter-neutral, kept as a PROBE in probe/hdr_const/, not in hdr/): FableWin's PrepareForRender calls the CONST
  CArray<CVertexShaderConstant>::GetAsCArray (0x02CD4880 QBE, then vector::size, GetZeroSizeArray ABE 0x02CD48F0, front() const 0x02CD4910; no non-const copy
  for that element type), i.e. CShaderRenderManager::CurrentVSConstantLayout is `const CVertexShaderConstantLayout*` (Ego_r: the registered table is
  CArray<CVertexShaderConstantLayout const*>), and the const GetAsCArray is `size() > 0 ? &front() : GetZeroSizeArray()` (19i/52b SAME, GetZeroSizeArray 8i/14b SAME;
  the repo's const text `_M_start : (const T*)this` is not the original). Three additive opt-ins in the probe headers: FABLE_RETAIL_CSHADERRENDERMANAGER_CONST_LAYOUT,
  FABLE_RETAIL_CARRAY_GETASCARRAY_CONST_FRONT, FABLE_RETAIL_STLP_VECTOR_FRONT_CONST (STLport `const_reference front() const { return *begin(); }`). With all
  three the row grades exactly as before (w4/c_v1_both, c_s0_both); with the const pointer and the repo's const text the final run loses 3 ids (p = 9 / 8).
- FableWin row size, recomputed: 619i / 2312b v ours 523i / 2092b at /Od /EHsc; 38 constant float arguments = +76i / about +156b (fldz 34x, fld 4x), so the
  unexplained rest is +20i / about +64b (not +166b): small instructions, e.g. four `lea ecx ; call` pairs + EH state stores = a scoped object with an inline
  constructor / destructor in each of the four draw blocks (the fourth block `{ const CTexture* tex ... }` has no other reason to be a block). Not identified;
  a named scoped object is a uniform symbol (w4/eS: three of them = the n = 3 regime), so it would not close the row by itself.

### 0x00B89C30 -- fourth agent (2026-11-15): still DIFFER(6431v6431), 1 site 0x00B8AF59; ~30 grades, parked again
- New families tried (w5/tg.py + w5/toggles.py, names q_*): bool-argument spellings of the final-section setters before SetAlphaRef
  (`SetAlphaBlendEnable(1)`, `SetAlphaTestEnable(1)`, n = 1 / 2): no letter change. Named COPY locals initialised from members (the lever
  that closed row 0x00B8B780): `const long steps = BlurStepCount` (changes code, 6414 bytes), a `tex` local feeding each `textures[4]`
  (final section CCCCSSSCCSSSSSSS), `const C2DBoxI& viewport = GetViewport(); OldViewport = viewport;` (moves the FIRST section by one
  block: CCCCCC + 16 S, final unchanged; + rulers r1..r6 and on the natural v1 text: never the retail pair), `const C2DBoxI viewport` copy
  (changes code). None reaches retail CCCCC+17S / CCCCSCCSSCCCCCCC.
- PDB check of possible natural inline layers around SetAlphaRef: CEnginePrimitiveRenderUtil has SetupOverlayAlphaBlendingStates(K, CRGBColour),
  DisableTextureFiltering(K), SetupStencilMask(...), ApplyVertexShader / ApplyPixelShader (static fastcall forwarders) -- all members of
  CEnginePrimitiveRenderUtil (non-static except the two forwarders), not reachable from CEngineScreenEffectOutlineGlow, and none wraps
  SetAlphaRef. Measured anyway (w5/fw_*.cpp): the static forwarders CEnginePrimitiveRenderUtil::ApplyVertexShader / ApplyPixelShader (landed
  0x00BBF4A0 text) for the final pair or all eight applies, on the fitted and the natural OldRenderTarget text: letters unchanged (no records).
- Next idea for a successor: none of the cheap families moves only the final section; the measured closing condition (an inline layer with a
  parameter around the SetAlphaRef expansion) has no PDB-backed natural source. Keep parked.

## 0x00BCC800 CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail -- RELOCATION_MATCH, test PASS, vland PASS, addmatch done (successor agent)
- What made it match: ONE helper text. `CEngineTreeFilerBase::FindNode` is, in retail,
      if (node->IsLeaf) return node;
      unsigned long index = (key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1);
      if (((CNodeHeader**)(node + 1))[index]) return FindNode(((CNodeHeader**)(node + 1))[index], key);
      return 0;
  (index named, the child table read twice, no `child` local). New overlay opt-in FABLE_RETAIL_CENGINETREEFILER_FINDNODE_INDEX
  (engine/CEngineTreeFiler.h, nested in the old INLINES opt-in as #ifdef / #else, old text untouched). Row source = the previous
  differ + that one #define; nothing else changed. No flags, no asserts, no fitted spelling.
- How it was found (method, reusable): this TU keeps UNREFERENCED OUT-OF-LINE COPIES of the pool / tree inlines. They are small
  oracles; graded with tools/pq.py (retail bytes read from the exe by VA + length; probe source = the row's include block + a
  pointer-to-member that forces the COMDAT). Before: FindID 0x00BC9B80 matched, FindNode 0x00BC9B30 key edx / index esi (retail
  key esi / index edx), AddKey 0x00BC9FB0 this/key swapped, ReturnResource 0x00BCA140 tree/key swapped, GetResource 0x00BCA210,
  GetBuffer 0x00BCA510 scratch rotation. c2prio traces (8 s each, tools/pt.sh) showed `this` 17 v key 15 in AddKey and tree 36 v
  key 34 in ReturnResource: two short, in the same direction in every copy, and FindID alone 28 v 22. One more candidate
  referenced in the index block (K 4 -> 5) gives exactly that swing, and in FindNode's own copy a live `index` candidate takes
  edx before the key. The double table read keeps `index` alive across the null test. After: all six copies RELOCATION_MATCH
  / MATCH and the row matches (frame permutation, `lea edx`, ReturnResource tail all followed, as predicted by the earlier lane).
- Probe panel: `tools/pa.sh <name> "<defines>"` (FindID / AddKey / ReturnResource / GetResource / GetBuffer), `tools/pf.sh <k>`
  (FindNode), variant texts in p3/fn/<k>.inc and p3/fi/<k>.inc (p3/mk.py builds the chain; p3/hdr is a probe-only overlay put
  in front of hdr/). NOT part of the hand-back.
- Tried without effect before the cure (probes): dead asserts in FindID / ReturnResource (FableWin sizes say FindNode, FindID,
  GetResource, AddNewResource, GetBuffer have one assert each, GetKey three, ReturnResource two: none is needed), AddKey early
  return forms (FableWin 53 bytes = early-return form, but only the two-assignment form gives retail's bytes), FindID forms
  (if (node) {}, id local, size local), FindNode forms (ternary, early return, named index used once, named word / mask / children,
  compound-assigned index, node reused, node methods, while loop), 25 compiler flags on FindNode, plain `inline`.
- Free leads for the coordinator (not my rows): with this opt-in the six unreferenced copies are matchable rows: FindNode
  0x00BC9B30 (66 b), FindID 0x00BC9B80 (115 b), AddKey 0x00BC9FB0 (125 b), ReturnResource 0x00BCA140 (195 b), GetResource
  0x00BCA210 (247 b), GetBuffer 0x00BCA510 (398 b); probe sources p3/f18_*.cpp / p3/fn18_fn.cpp (they carry probe-only
  `#define private public` lines and need real row headers). The sibling row RenderSpriteTrail 0x00BCA950 (differ in
  2026-11-08_linux_hand889) inlines the same helpers and should be regraded with the opt-in.
- Class key: FableWin mangles CNodeHeader and CListHeader as `U` (struct); the overlay opt-in declares them `class` (the repo
  default forward-declares `class CNodeHeader`). vland accepts the row (no RELOC_IDENTITY; two RELOC_NAME warnings for the
  folded bodies 0x00AC20C0 / 0x00AC20D0 = CVertexBufferBase::IsBusy / Sizeof labelled CThingFilter_IsValid). See NAME_FIXES.tsv.
- Test: host model with recording stand-ins (test/00bcc800.cpp); seven single-decision mutants all fail.

## Names
- No made-up names added. Respelled in differ/00b8b780.cpp to the PDB spellings used by the c19a hand-back (NAME_FIXES.tsv).

### 0x00B89C30 -- fifth agent (2026-11-15): RELOCATION_MATCH, test PASS, vland PASS, addmatch done -- WITH FITTED DEVICES (coordinator decides)
- src/00b89c30.cpp = the natural text v1 (`OldRenderTarget = NEngineInternal::GPDisplayManager->GetRenderTarget();`, no fitted block) plus
  (1) `static FABLE_FORCEINLINE void SetCompositeAlphaRef(unsigned long ref) { NEngineInternal::GPRenderManager->SetAlphaRef(ref); }` called as
  `SetCompositeAlphaRef(1);` (INVENTED helper, descriptive name, found in no PDB: the third agent's iPE_2 closing condition made static and named) and
  (2) `long unused1 = 0; long unused2 = 0;` after the shaders object (device c, `// Fidelity:`; window measured on this source: k = 0 / 1 one block late
  (final CCCCSSSSS / CCCCSSCSS), k = 3 no swap; only k = 2 matches). Test: test/00b89c30.cpp (host model, 3 scenarios). No header change, flags none.
- Why a forwarding inline is the only lever found (measured again, ~70 grades, w6/): the SetAlphaRef block needs ONE more value copy alive during its
  SetRenderState expansion. Things that do it: an enclosing inline with a parameter (row-local), or a local copy INSIDE SetAlphaRef's body
  (probe/hdrP_setalpharef: `const unsigned long value = ref;` or non-const, with k = 2 rulers = RELOCATION_MATCH; but fwsize 16i/43b v FableWin 14i/34b,
  and FableWin's six K-setters SetAlphaRef / SetStencilRef / SetStencilMask / SetStencilWriteMask / SetFogColour / SetTextureFactor are all 14i/34b:
  the header local contradicts the debug build, so it was NOT used). Things that do not: plain `inline` SetAlphaRef (changes code), (unsigned long) cast,
  `ref + 0`, `this->`, a local CStateInfo& , `ref ? ref : 0`, const parameter, a static_cast<CRenderStateManager*> / qualified call, an inline accessor for
  the object pointer, 1.0f / 1.0 arguments, a 2- or 3-parameter alpha-test helper wrapping Enable/Func/Ref (jumps to the no-swap regime), forwarding
  layers around PrepareForRender in the middle region (p unchanged), the const-layout / GetAsCArray / STLport front() = *begin() texts x k = 0..16
  (shift the final run and the regime break together: p = 6 never appears), /GS /Ob1 /G6 /Ot /Gy, #pragma inline_depth(3..16).
- FOR THE COORDINATOR: accept or reject the invented SetCompositeAlphaRef. It passes the owner's three questions (byte match, same behaviour, portable),
  is not a stand-in for any retail callee (retail calls nothing there), and lives in the row source, not a shared header; but no PDB has such a function
  (FableWin's TU list is complete for this TU and shows none). If rejected, the row goes back to DIFFER(6431v6431) with differ/00b89c30.cpp.
- Probe-only overlays (never hand in): probe/hdrC_const_frontbegin (const layout + GetAsCArray const + STLport front() *begin() opt-ins),
  probe/hdrP_setalpharef (SetAlphaRef body variants under PROBE_AR_*).
