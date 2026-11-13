# DOSSIER 0x00A059E0 -- CRenderStateManager::SetDefaultRenderStates (7958 bytes)

Prior material: $W/prior/ is empty and /opt/fable_lane/w22/agent2099 (the earlier chunk that owned this row) left only
CHUNK_BRIEF.md (src/ test/ probe/ hdr/ empty). Closest family scout: /opt/fable_lane/w22/bwf2/00a04630/{DOSSIER,OUTLINE}.md
(SetupRenderStates, same TU, same "helpers defined in the TU, auto-inlined" model) -- read it.

## 1. Retail shape (xdis 00a059e0 0x1f26 = $W/dis_all.txt; first insn 0xA059E0, last `ret` 0xA078F5, then int3 to 0xA07900 Init)
- __thiscall, plain `ret` (no args). NO frame pointer: `sub esp,18h ; push ebx ; push ebp ; push esi ; push edi`, ebp is a
  scratch/constant register => default /Oy (BASE_FLAGS /O2). No SEH, no /GS cookie, no destructible locals. Speed code.
  The prologue is interleaved with block 1 (`mov edx,[ecx+2A10h]` is the FIRST instruction, before `sub esp`).
- Epilogue 0xA078EE `pop edi ; pop esi ; pop ebp ; pop ebx ; add esp,18h ; ret`.
- ZERO calls, ZERO jumps outside the body, no jump tables. The whole function is 77 inline expansions of
  CRenderStateManager::SetRenderState(value, CStateInfo&) (+ AddStateToUpdateList) -- 37 straight, one 8-stage loop of 23, 6 for
  stage 0 after the loop, 11 straight. `this` stays in ecx throughout (never moved to esi).
- Register constants: ebx = 1 (whole function; also the `DirtyListFlag = true` byte `mov [eax+10h],bl`), ebp = 0 until the loop
  (the loop reuses ebp as the stage pointer; re-zeroed with `xor ebp,ebp` at 0xA074A8 after the loop).
- Frame 0x18 = 6 dwords at [esp+10h..27h] (after the 4 pushes): see OUTLINE "Frame".

## 2. Name / signature (manifest label CORRECT)
- `?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ` = `public: void __thiscall CRenderStateManager::SetDefaultRenderStates(void)`.
- Sources: (a) Ego_r.pdb ghidra_out/egor_pdb_names.tsv 0x00B70490 (Ego_r address, between RealiseRenderState 0x00B70370 and Init
  0x00B72200 -- same neighbour order as retail); (b) FableWin ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv 0x030EFD60
  (debug body 1060 b / 344 i) + thunk 0x017913EB, _fablewin_api.tsv same; (c) callers (xref.py): CRenderStateManager::Init
  0x00A07900 (call 0xA07965; landed rebuild/src/compiled/00/a0/CRenderStateManager_Init_00a07900.cpp names it
  "SetDefaultRenderStates @ 0x00A059E0") and CSurfaceCompositor::StartComposition 0x00F39730 (call 0xF39824, not landed).
- Declared already: rebuild/include/engine/CRenderStateManager.h line 138 `void SetDefaultRenderStates();` (public, non-virtual).

## 3. Callees
None called. Inlined setters (all members of CRenderStateManager; PDB spellings, header line refs):
| setter (PDB name) | header status | retail out-of-line copy in THIS TU (zero callers, oracle) |
|---|---|---|
| SetDitherEnable(bool) | FABLE_FORCEINLINE (l.~199) | none |
| SetZBufferEnable / SetZBufferWriteEnable(bool), SetZBufferTestFunc(ZBUFFER_FUNC) | FORCEINLINE l.154-156 | none |
| SetPolyFillMode / SetPolyShadeMode / SetPolyCullMode | FORCEINLINE l.151-153 | none |
| SetAlphaTestEnable, SetAlphaRef(unsigned long), SetAlphaTestFunc, SetSource/DestAlphaBlendMode, SetAlphaBlendEnable, SetSpecularEnable, SetClippingEnable, SetFogEnable | FORCEINLINE l.157-166 | none |
| SetTextureFactor(unsigned long) | declared only (l.~228) | 0x00A09050 |
| SetFogColour(unsigned long), SetFogStart(float), SetFogEnd(float) | FORCEINLINE l.~196-198 | 0x00A08EE0, 0x00A08D90, 0x00A08E00 |
| SetFogDensity(float), SetFogMode(FOG_MODE), SetFogVertexMode(FOG_MODE) | declared only | 0x00A08E70, 0x00A08F50, 0x00A08FD0 |
| SetVertexBlending(VERTEX_BLEND_MODE) | declared only | 0x00A08D10 |
| SetColourWriteEnable(bool) or (bool,bool,bool,bool) | opt-in OUTLINE_INLINE (`enable?15:0`) / COLOURWRITE4 | none (both fold to 0xF) |
| SetDepthOffset(float), SetSlopeScaledDepthOffset(float) | declared only | 0x00A09CA0, 0x00A09D10 (unconditional, no caps test) |
| SetPointSpriteEnable/SetPointScaleEnable(bool), SetPointSize/Min/Max(float), SetPointScaleConstantFactor/DistanceFactor/DistanceSqrFactor(float) | declared only | 0x00A09340, 0x00A093C0, 0x00A09440, 0x00A094B0, 0x00A09520, 0x00A09590, 0x00A09600, 0x00A09670 |
| SetBlendOp(BLEND_OP) | FORCEINLINE l.163 | 0x00A096E0 |
| SetLightingEnable(bool) | FORCEINLINE | 0x00A090C0 |
| SetNormalizeNormals(bool) | declared only | 0x00A09140 |
| SetAmbient/Diffuse/SpecularMaterialColourSource(MATERIAL_COLOUR_SOURCE) | declared only | 0x00A091C0, 0x00A09240, 0x00A092C0 |
| SetTextureOperation/AlphaOperation(long,TEXTURE_OP_MODE), SetTextureArgument1/2, SetTextureAlphaArgument1/2(long,TEXTURE_ARG_MODE) | FORCEINLINE l.178-183 | Argument2 0x00A09880 |
| SetTextureFilterModeMin/Mag, SetTextureUVModeU/V/W, SetTextureMipMapMode, SetTextureCoordIndex(long,long) | FORCEINLINE l.167-173 | UVModeW 0x00A09800, CoordIndex 0x00A09980 |
| SetTextureMipmapLODBias(long,float) | FORCEINLINE | none |
| SetTextureTransformMode(long,TEXTURE_TRANSFORM_MODE) | FORCEINLINE (DEFAULT_TRANSFORM opt-in = same text) | none |
| SetTextureBumpEnvMatrix(long,float,float,float,float) | declared only | 0x00A09A80 (ret 14h) |
| SetTextureCoordWrapUVW(long,bool,bool,bool) | declared only | 0x00A09760 (ret 10h) |
| SetTextureMaxAnisotropy(long,long) | FORCEINLINE | 0x00A09900 |
| SetTextureBorderColour(long,unsigned long) | declared only | 0x00A09A00 |
| SetStencilEnable/Func/Ref/Mask | opt-in STENCIL_INLINE | none |
| SetStencilOpFail/ZFail(STENCIL_OP_MODE) | declared only (both branches) | none |
| SetStencilOpPass, SetStencilWriteMask | opt-in OUTLINE_INLINE | WriteMask 0x00A09C30 |
Setter names verified in FableWin.exe.signatures.tsv (all `?Set...@CRenderStateManager@@QAEX...`). No spelling disagreements.
The gap 0x00A08D10..0x00A09D80 (manifest-gaps.tsv row `00a079b0 ... 00a08d10 4203`) holds these 32 out-of-line setter copies
in HEADER DECLARATION ORDER, each 16-aligned, xref = 0 callers: they are ordinary member functions DEFINED in
CRenderStateManager.cpp AFTER SetDefaultRenderStates / Init / CDeviceState ctor and still inlined here (c2 sees the whole TU).
Disassemble any with `python3 $S/w17/xdis.py <addr> 0x80` to check a helper text (free oracle).
Important parameter fact from 0x00A09A80: SetTextureBumpEnvMatrix(stage, a, b, c, d) writes a->TextureBumpEnvMat00 (+0xF8C),
b->TextureBumpEnvMat10 (+0x10CC), c->TextureBumpEnvMat01 (+0x102C), d->TextureBumpEnvMat11 (+0x116C), in that order.
0x00A09760 SetTextureCoordWrapUVW body: `value = (u?1:0) | (v?2:0) | (w?4:0)` (setne / neg-sbb-and 2 / neg-sbb-and 4) on
TextureCoordWrapMode[stage] (+0x44C); with all-false args it folds to `xor edi,edi`.

## 4. Members (engine/CRenderStateManager.h, FABLE_RETAIL_CRENDERSTATEMANAGER branch = Ego_r layout)
- +0x2008 StateUpdateList[256], +0x2808 StateStackSize, +0x280C StateUpdateListSize, +0x2814 CurrentBookmarkMask,
  +0x08 StateStack[1024] (CStateStackElement {State +0, Value +4} -> `[ecx+edx*8+8]` / `+0Ch`), +0x2818 DeviceState (CDeviceState,
  member offsets in the header l.55-131; OUTLINE lists every one touched). CStateInfo +4 DesiredState, +8 BookmarkMask, +0x10 DirtyListFlag.
- Static tables (private, header l.~330-350), values read from retail .data:
  0x01396DC8 D3DTextureUVModeTable, 0x01396DD8 D3DAlphaFuncModeTable, 0x01396DF8 D3DZBufferFuncModeTable, 0x01396E18
  D3DStencilFuncModeTable, 0x01396E58 D3DTextureFilterModeTable, 0x01396E64 D3DTextureMipMapModeTable, 0x01396E70
  D3DTextureOperationModeTable, 0x01396ED0 D3DBlendOpModeTable, 0x01396EE8 D3DTextureArgumentModeTable, 0x01396F38
  D3DStencilOperationModeTable, 0x01396F68 D3DAlphaBlendModeTable, 0x01396F9C D3DPolyShadeModeTable, 0x01396FA4 D3DPolyFillModeTable,
  0x01396FB0 D3DPolyCullModeTable, 0x01396FCC D3DTextureTransformModeTable -- all already address-commented in the header.
  NEW addresses (header has no comment; derived here): 0x01396F58 = D3DFogTableMode[4] = {0,3,1,2} (D3DFOG_NONE, LINEAR, EXP, EXP2;
  used by FogMode +0x190 / FogVertexMode +0x1A4, and by the out-of-line SetFogMode 0x00A08F50); 0x01396FBC = D3DVertexBlendModeTable[4]
  = {0,1,2,3} (VertexBlendMode +0xDC; out-of-line SetVertexBlending 0x00A08D10); 0x01396FF4 = D3DMaterialColourSourceTable[3] = {0,1,2}
  (material sources +0x244/+0x258/+0x26C; 0x00A091C0..); 0x01396E38 = {1..8} probably D3DShadowFuncModeTable (unused here).
- Opt-ins: FABLE_RETAIL_CRENDERSTATEMANAGER (required: whole retail class, CStateInfo, SetRenderState/AddStateToUpdateList).
  Optional: _STENCIL_INLINE (Stencil Enable/Func/Ref/Mask), _OUTLINE_INLINE (SetColourWriteEnable(bool), SetStencilOpPass,
  SetStencilWriteMask), _COLOURWRITE4. Do NOT use EARLY_RETURN unless a probe says so (default SetRenderState text matched 0x00A04630's
  family). Every setter the header only DECLARES must be defined by the row TU (plain `void CRenderStateManager::SetX(...) {...}`,
  like 0x00A04630's Setup* helpers; mirror retail by placing them after SetDefaultRenderStates, or before -- c2 inlines either way;
  if a probe shows otherwise, put them before). Do not both opt in to a FORCEINLINE version and define the same setter.
- Locally modelled classes in landed siblings that HAVE a shared header (do not copy): Init 0x00A07900 (`struct CStateInfo`,
  `CRenderStateManager_Methods`), ForceResend 0x00A044E0, RealiseRenderState 0x00A058C0.

## 5. Callee-body visibility
- All setters + SetRenderState + AddStateToUpdateList are visible and inlined (no call in retail). Header FORCEINLINE versions cover
  ~40 of them; the rest have retail out-of-line copies listed above (same TU => define them in the row file). No STLport,
  no CIntelligentPointer, no strings, no vector maths, no x87 (floats are only bit-copied: `*(unsigned long*)&f`, the header idiom;
  retail stores the immediate float bits to a stack slot and reloads it as a dword, e.g. 0xA0627B `mov [esp+18h],3DCCCCCDh ;
  mov edi,[esp+18h]`; precedent comment in header for 0x00B24E7E / 0x00B705DE). No fistp/SSE => no Retail-asm lines.
- Similar function in another TU: CEngine::SetDefaultRenderStates 0x00B24850 (not landed) expands many of the same setters.

## 6. Siblings (same TU CRenderStateManager.cpp, retail 0x00A044B0..0x00A09D80)
- Landed in rebuild/src/compiled/00/a0/: ~CRenderStateManager 0x00A044B0, ForceResendRenderStatesToHardware 0x00A044E0,
  RestoreCaptureBlock 0x00A05840, RealiseRenderState 0x00A058C0, ctor 0x00A05970, Init 0x00A07900, scalar deleting dtor 0x00A07980.
  All default flags (no `// Flags:` line, no pragmas, except Init's `#pragma auto_inline(off/on)` around its stand-in).
- Shared-header + setter-expansion idioms to copy: rebuild/src/compiled/00/bf/CEnginePrimitiveRenderUtil_SetupOutlineSmoothAlphaStencilMaskStates_00bf01e0.cpp
  (STENCIL_INLINE + OUTLINE_INLINE), rebuild/src/compiled/00/f3/CSurfaceCompositor_SetBlendMode_00f38a10.cpp (`// Flags: none`,
  COLOURWRITE4), rebuild/src/compiled/00/b5/CEngineScreenEffectColourFilter_RenderColorFilteredQuad_00b5af80.cpp (STENCIL_INLINE),
  rebuild/src/compiled/00/9d/CRenderManager2D_Render2DDrawList_009da9f0.cpp (`#define FABLE_RETAIL_CRENDERSTATEMANAGER` first).
- Not landed: 0x00A04630 SetupRenderStates (scouted bwf2/00a04630), 0x00A079B0 CDeviceState ctor (scouted bwf2/00a079b0).

## 7. Risks
- Call ORDER is retail's (OUTLINE), not header/DeviceState order. TextureFactor is set TWICE (0xFF000000 then 0xFFFFFFFF; second block
  0xA05FC1 reuses eax, no lea). AlphaTestFunc default is index 1 (ALPHA_TEST_LESS -> D3DCMP_LESS) per the .data read -- do not "fix".
- Lane gap (header comment l.20-22): the bookmark-test operand order (`mov esi,mask ; mov edx,[eax+8]` vs the other three forms)
  varies block by block; OUTLINE tags each block A/B/C/D. Expect residues there that no spelling moves.
- Loop floats: LODBias 0.0f and the 4 BumpEnv args are stored ONCE before the loop (hoisted, right-to-left: [14h],[24h],[20h],[1Ch],[18h]);
  outside the loop each float setter re-stores [esp+18h].
