# Chunk 857 notes

Matched: 2 of 8 (both RELOCATION_MATCH). Parked: 6 (differ/, lines 2-3 carry the residue and next idea).
Attempt counts are in STATUS.tsv; the brief's 50-attempt floor was not reached on any parked row (see "Budget" below).

## 0x00AC8C80 QuadricOptimiserInternals::COptimisedPrimitive::ComputeVertexCost -- RELOCATION_MATCH
- Name: Ego_r+FableWin `?ComputeVertexCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCOptimisedVertex@2@@Z`;
  the catalog's COptimisedPrimitive_ComputeBoundingBox is a forge autoname (LABELS.tsv).
- Shape: GetFaceNormal / MakeOrthogonal carry their real bodies as noinline same-TU functions (VC7.1 reads their register
  clobbers); tangent lengths spelled sqrt(Y*Y+Z*Z+X*X); a separate `start` iterator; the `normals` vector is filled and never read.
- Callees: GetFaceNormal 0x00AC7CB0, MakeOrthogonal 0x00AC7200 (catalog Vector_ProjectOntoPlane), vector<C3DVector>::reserve
  0x00920FF0, _M_insert_overflow 0x0066E370, set<unsigned long> iterator operator++(int) 0x00AD0A70 (NAME_FIXES).

## 0x00A1C010 CNavigatorAStar::CullWayPoints -- RELOCATION_MATCH
- Name: Ego_r+FableWin `?CullWayPoints@CNavigatorAStar@@AAEXJ@Z`; catalog AStar_process_node_queue is wrong (LABELS.tsv).
- Shape: private inline IsRouteClear body in the same TU (retail expands 0x00A1A260 into this row; every return becomes a jump);
  local CNavigatorAStar : CNavigatorBase model with retail offsets (Route +0x8C, CullFront +0x90, CullBack +0x94, Culled +0xC0).
  GFGetSign (0x00437160, NAME_FIXES) inline fastcall.
- Header debt: the CNavigatorAStar local model should become engine/CNavigatorAStar.h retail branch (offsets above, sizeof 0xC4).

## 0x00A8B990 C3DMesh2::InitialiseBones -- parked, md=317
- Name: Ego_r+FableWin `?InitialiseBones@C3DMesh2@@AAE_NABVC3DMeshFileSubMeshChunk@@@Z` (catalog AddSubMeshBoneVertices is wrong).
- `// Flags: /Gr` (the C3DMesh2 TU; landed precedent 0x00A8DDC0); with /Gr VC7.1 expands vector<CCountedPointer<C3DBone2>>
  _M_insert_overflow as retail does.
- Residue: retail keeps bones._M_finish in edi and _M_end_of_storage in ebp across the loop, bone.Info at [esp+0x34]; ours keeps
  Info in edi. Tried: Reset vs ctor, for vs while, parent local, raw pointer, IsNull test, bone hoisted out of the loop, declaration
  order, /d2inlS / /d2inlT diagnostics (no effect / worse).
- Callee names: 0x00A8CB70 DeleteData, 0x00A8DB30 __destroy_aux, 0x00553E60 map<ulong,ulong>::operator[] -- NAME_FIXES.tsv.
- Local models: C3DBone2 and C3DMesh2 (no retail-branch header yet: header debt, C3DMesh2 BoneCount +0x98 / Bones +0x9C).

## 0x009D8CF0 CRenderManager2D::ClearRenderTarget -- parked, md=32 (3385 = 3385 bytes)
- Name: Ego_r+FableWin `?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z`; header already declares it.
- Shape: flags 0 -> 7; AlternateRenderTargetClearEnabled off -> CDisplayManager::ClearRenderTarget; else CScopedShadersEnable(true,
  false), CRenderStateScopedCaptureBlock, Fog / Specular / AlphaBlend / AlphaTest off, colour write 0xF or 0 (if/else on flags & 4,
  each a full SetColourWriteEnable), z (flags & 1: enable, write, ALWAYS; else disable), stencil (flags & 2: enable, ALWAYS,
  REPLACE x3, mask 0xFF, ref 0; else disable), viewport save through `C2DBoxI old = GetViewport()` with FABLE_RETAIL_C2DBOXI_COPY
  (memberwise copy = retail's field-by-field save), SetViewport(C2DBoxI(0, 0, dims)), four CTVertexRHWColSpecTex1 at
  (+-1, +-1, 0.99999994f) with RHW 1 / colour / UV 0 in a loop (UV stored first), indices {0,1,2,0,2,3}, empty constant layout +
  scoped layout + ApplyVertexShader(VertexShader2D) + PrepareForRender(0), texture stage 0 select-arg1 diffuse / stage 1 disabled,
  DrawAsIndexedTris (5-argument overload), viewport restore.
- Residue (32 lines): bookmark test load order in the 7 stencil blocks and the stencil-else block (retail loads state.BookmarkMask
  first), and ecx/edx swapped in the AlphaOperation[0..1] / Argument1[0] / AlphaArgument1[0] blocks. Swapping the test / or operands
  in the helper, `!= 0` conditions, local display reference, declaration moves and plain `inline` setters all failed (the plain
  inline ones fall out of the inline budget). This is the documented CRenderStateManager.h lane gap.

## 0x005AECC0 CTCInventoryTrade::SellItem -- parked, md=429 (first draft)
- Name: Ego_r+FableWin `?SellItem@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z`; the catalog's SellItemPC belongs to 0x005B01FE
  (PC strings and PC callees); evidence in LABELS.tsv.
- /Os TU with a frame pointer (`#pragma optimize("s", on)` + `("y", off)` as every landed CTCInventoryTrade row).
- Retail members: Components vector +0x0C, ItemsToBuy +0x144, ItemsToSell +0x150, SellListsByCategory +0x180, BuyListsByCategory +0x18C,
  NumSellableThings +0x1BC, TotNumClothesToSell / WeaponsToSell / ItemsToSell +0x1D4 / +0x1D8 / +0x1DC (Ego_r names, retail offsets).
- Descriptive: the four static CCharString amount names 0x013B8B68 / 0x013B8B70 / 0x013B8B74 / 0x013B8B78 (g_Sell*AmountName),
  CTCShop::AddItemSold 0x00774D20, CThing::GetInterface inline model, slot names 9 / 93 / 95 / 144 / 156 taken from the engine
  NUISystem headers (FindFirstChildOfType, Make(Un)AvailableDynamically, SetText, SetItemCount).

## 0x0049A0B0 CProgressDisplay::InitialiseTextures -- parked, md=798 (first draft)
- Name: vtable slot 0x012388C4 + `?InitialiseTextures@CProgressDisplay@@UAEX_N0@Z`; argument names descriptive.
- Strings decoded (UTF-16): splash.tga, LoadProgress\Fable_BG_PC_0 + n + .dds, guild_seal_[0]n.dds, slot_left/mid/right_bar_off.dds,
  Ybar_seg.dds, fable_BG_bar_border.dds, rc_seal_border.dds, TextTags.txt; text tags via CPersistContext::TransferVector "TextTags" /
  "TextTagsPC"; tip text / title statics 0x013B89D8 / 0x013B89DC (descriptive g_ProgressTipText / g_ProgressTipTitle).

## 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData -- parked, md=1535 (first draft)
- Name: Ego_r+FableWin `?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z`;
  the catalog's C3DKeyframe_InitialiseAsConstant is wrong (LABELS.tsv).
- Algorithm (from retail): keyframes from the chunk matrices with scale (1,1,1,0); PositionFactor / ScalingFactor = max |component| / 32767;
  rotation track: unique quaternions (CQuaternion::Equals 0x00A88C50) + byte palette, mode 0 (single identity, IsIdentity 0x00A88CB0),
  1 (single), 3 (paletted when 16*unique + palette < 16*frames and unique <= 255) or 2; positions and scales likewise with C3DVectorWord
  (6 bytes, ftol quantised), zero test |v|^2 <= 1e-4^2, scale test |v - 1|^2; the scaling track is computed but not stored (retail);
  pool copies through CSimpleMemoryPool::AllocateType; IdentityFlag when all three modes are 0.

## 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive -- parked, md=641 (skeleton)
- Name: Ego_r+FableWin PDB (8 arguments, `ret 20h`). Only the render-state prefix and the RenderPolygons call are modelled; see differ/ line 3.

## Budget
- The two matched rows and 0x009D8CF0 / 0x00A8B990 took most of the session; the four large rows (2.4-3.5 KB each, GUI / animation /
  cloth code with no retail-branch headers) got first compiled drafts only. They are honest starting points, not near misses.
- Tests: src/ rows have standalone tests (test/00ac8c80.cpp, test/00a1c010.cpp). The parked rows have no tests yet.

## vland (read-only, final overlay)
- 0x00A1C010 and 0x00AC8C80: RELOCATION_MATCH + PASS. Warnings only: STANDIN_LEAF (catalog names wrong, corrected in LABELS.tsv),
  RELOC_NAME 0x00AC7200 labelled Vector_ProjectOntoPlane (LABELS.tsv: MakeOrthogonal), and RELOC_NAME 0x0129BA3C labelled 'EPSILON'
  where the shared header binds ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f) -- label candidate, one source (the header binding), not in LABELS.tsv.
