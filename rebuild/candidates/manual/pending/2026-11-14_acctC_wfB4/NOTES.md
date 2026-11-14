# wfB4 NOTES (author agents, 2026-11-14) -- three parked render rows; 0x00BCC800 MATCHED by the successor; rows 1 and 2: residue class measured

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
