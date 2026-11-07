# Chunk 860 -- chunk 857's six parked rows, second pass

Result: 0 of 6 matched. All six are parked in differ/ with lines 2-3 and a passing behaviour test (`differ/<addr>.test.cpp`,
prints `<addr>_TEST PASS` under VC7.1). Read-only vland: 6 DIFFER, 6 test PASS, 0 RELOC_IDENTITY (tmp/vland.log).
Attempts: 1,160 grades in total (ATTEMPTS.log). Per row: 348 / 175 / 185 / 129 / 170 / 150. Most came from three new
tools in bin/ (they build variants inside probe/):
- `comb.py`: random combinations of per-site alternative spellings.
- `multi.py`: lists of edits.
- `micro.py`: compiles a micro TU with VC7.1 and disassembles it.

## Lever found this chunk: callee visibility decides stack-slot sharing (005AECC0: md 349 -> 76)
VC7.1 shares an address-taken variable's frame slot with later locals only when it can prove nobody holds the address. That
covers parameters too: their home slots are reused once they are dead. The proof needs every callee that receives the address
to be visible in the TU and not to capture it.

Micro probes (probe/micro, deleted):
- m2/m3: `Ext(item)` with `Ext(C* const&)` declared only. No later local reuses item's slot (or count's).
- m5: the same call through a visible `#pragma auto_inline(off)` template body. `shop` reuses [ebp+8].
- m6: that body forwards the reference to a declared-only function. The reuse is gone again.

Retail TUs get this from STLport templates and from earlier-defined functions of the same .cpp. Our drafts declare them only.

## Per row

### 0x009D8CF0 CRenderManager2D::ClearRenderTarget -- DIFFER(3385v3385) md 12 (6 instructions)
- **Change:** `indices[]` filled by six statements, plus a `CShaderRenderManager& shaders = CShaderRenderManager::Get();` local for
  the three shader calls.
- **Effect:** fixes the bookmark load order in the seven stencil blocks, the stencil else block, TextureOperation[1] and
  AlphaArgument1[0].
- **Residue:** retail colours state.BookmarkMask edx and CurrentBookmarkMask ecx (`or ecx,edx`) in the three texture blocks
  SetTextureAlphaOperation(0), SetTextureAlphaOperation(1) and SetTextureArgument1(0). Ours swaps them.
- **Search:** about 370 knob combinations (flag tests, arm order, locals, scopes, vertex-fill forms, stage literals) move only
  the stencil flip. That is GOTCHAS's count effect. None moves these three colourings.

### 0x00A8B990 C3DMesh2::InitialiseBones -- DIFFER(1608v1624) md 317 (61 lines ignoring registers)
- **Bone-loop registers:** retail gives the local bones vector's _M_finish edi (eos ebp) and spills bone.Info to [esp+0x34].
  Ours gives Info edi and leaves _M_finish in memory.
- **Frame layout:** the spilled `bone` takes retail's bottom slot. used_bone_names shares that slot, so every later slot sits
  0xC higher in retail.
- **Copy loop:** in retail, used_bone_names.begin() is read after the map ctor and stepped as a pointer.
  - m2_2 (`CCharString* used_name = used_bone_names.begin(); *used_name++ = ...`) reproduces that and gets retail's length (1624).
  - Its md is 340, so it is not kept; it is noted on line 3.
- **Search:** 175 grades over declaration order, Reset / ctor / raw-pointer forms, parent local, PUSHBACK wrapper, name
  temporary, loop forms, and copy-loop pointer / index / iterator forms. None changes which value gets edi.
- **Chunk 857 lesson:** a visible noinline `reserve` gave _M_finish a register in 0x00AC8C80. Here no out-of-line call takes
  &bones; the only one in the loop is bone_names' `_M_insert_overflow` 0x00433530. The same lever may still apply through that
  call. Untested.

### 0x005AECC0 CTCInventoryTrade::SellItem -- DIFFER(2446v2446) md 76 (52 bytes)
- **STLport map bodies:** same-TU `_M_lower_bound` / `_M_upper_bound` / `_M_find` / `operator[]` / `erase(key)` bodies under
  `#pragma auto_inline(off)`, in the local std::map model. These free item's and count's argument slots.
  - Retail homes shop, the GetTC type temporary and later temporaries at [ebp+8] / [ebp+0xC]. Result: md 80.
- **pair ctor:** an STLport `pair` ctor makes RefreshEntriesAmountsAndAvailabilities return `pair<unsigned long,unsigned long>`
  through a hidden pointer (`lea eax,[ebp-54h] ; push eax`). This gives retail's length.
- **GetMenuBranchFromDefIndex:** the landed body (0x0059B79A, rebuild/src/compiled/00/59/), retyped to the PDB signature, frees
  menu_type's slot. Result: md 76.
- **Residue:** frame 0x58 v 0x54. One 4-byte slot of the bottom temporaries is still unshared. Next: add the bodies of the
  other earlier-defined TCInventoryTrade.cpp callees (all landed) the same way.
- **Model debt:** the source still carries 857's local models (std::map with two template parameters, CCharString /
  CWideString, members public where the PDB has AAE). Those mangle differently from retail's bindings. The vland check warns
  SHARED_HEADER_AVAILABLE for 10 classes.
  - Before this row can land, rebase it onto engine/ and stlport/ headers. stlport/fable_map.h would need retail branches for
    `_M_lower_bound` / erase(key) bodies.

### 0x0049A0B0 CProgressDisplay::InitialiseTextures -- DIFFER(2928v2920) md 582 (142 lines ignoring registers)
- **Change:** every Initialise block stores the C2DExtentsI Y before X ([esp+0x54] then [esp+0x50]). A `{ Y = y; X = x; }` ctor
  and `size.Y = 0x40; size.X = size.Y;` give md 630 -> 582.
- **Residue:**
  - frame 0xd8 v 0xb0;
  - retail materialises the extents constants in eax / ecx;
  - register roles in the guild-seal and text-tag tail;
  - 8 bytes of length.
- **Model debt:** the source still uses 857's local models (8 shared headers available). Next: rebase onto the headers and try
  the visible-callee lever on the earlier-defined CProgressDisplay callees.

### 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData -- DIFFER(3495v3491) md 1268
- **Change:** positions / scalings and their palettes are declared at their resize. Each resize then folds to retail's
  `push &x ; push FrameCount ; push 0 ; call 0x00A552B0 / 0x00485F3A`. Length 3613 -> 3495.
- **Why md rose:** md went from 1192 to 1268 because difflib misaligns two large x87 blocks.
  - ck_27 (md 1189) is the md-best, but it is 106 bytes long and does not fold the resizes.
- **Retail puzzle:** retail declares the track vectors at the top (their zero stores come early) yet still folds. VC7.1 must know
  they are untouched, which the visible-callee lever would explain.
- **Other residue:**
  - retail's entry `xor ebx,ebx` zero register and four pushes;
  - the keyframe loop's single induction pointer;
  - the x87 GFMax / fcomp chain;
  - frame-slot order.

### 0x00BB30A0 CEngineClothRenderer::Render2DClothPrimitive -- DIFFER(3522v3538) md 167 (36 lines ignoring registers)
- **Change:** `surfaces` from a ternary (md 172 -> 167).
- **Frame:** 0x38 v retail 0x34, so every esp operand is 4 off. Retail keeps preloading / two_sided in cloth's argument slot and
  lights in mesh's. The texture getters are CEnginePrimitiveManagerMeshBase statics from another TU, so the visible-callee
  lever does not reach them.
- **GetRGBColour:** retail holds R in bl and G in a spilled byte, and stores R/G/B together after the third __ftol2. Ours
  stores each byte at once. Initialise-based CRGBColour ctors and named-byte forms are all identical to ours.
- **Fog bookmark:** the flip is the CRenderStateManager.h lane gap.

## Callee names with evidence
New labels are in LABELS.tsv: 0x00A55070, 0x00A552B0, 0x00A54BB0, 0x00A54CB0 and 0x005B2779; each replaces a forge
autoname. Wrong landed spellings are in NAME_FIXES.tsv:
- InitialiseFromChunk -> InitialiseFromFileData;
- GetMenuBranchFromDefIndex's landed (CMenuTree*, long*) model;
- SetUserVertexShaderConstant `J PBV` -> `K ABV`;
- CalculateNextTextTag QAE -> AAE;
- 0x009F2F90 GetRect -> CSurface::GetDimensions.

The engine-row names are chunk 857's proven LABELS (unchanged).

## Header debt
- None beyond hdr/README.md.
- The 005AECC0 / 0049A0B0 rebases (model debt above) need STLport map retail branches (`_M_lower_bound` / `_M_upper_bound`
  bodies, `erase(const key&)` via equal_range) in stlport/fable_map.h, behind a new opt-in.
