# Chunk 742: particle emitter ReadBinary + particle-group manager render passes

The work was done in the Linux Wine lane with VC7.1 cl 13.10.3077. The read-only vland run gave WINS: 2, both RELOCATION_MATCH
with the test passing (`vland_result.txt`). No `__asm` appears in any row, and no inline-asm helper header was needed.

## Results
| address | row | grade | attempts | where |
|---|---|---|---|---|
| 0x00AEFAA0 | NParticleEngine::CParticleEmitter::ReadBinary | RELOCATION_MATCH | 2 | src/ test/ |
| 0x00BBC460 | CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive (slot 5) | RELOCATION_MATCH | 3 | src/ test/ |
| 0x00BBE090 | CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail | DIFFER(2432v2464), 46 lines | 15 | differ/ |
| 0x00BC3F30 | CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive (slot 5) | DIFFER(3272v3357), 242 lines | 11 valid + 3 void | differ/ |
| 0x00BC3F3D | bogus catalog row | not a function | - | - |
| 0x00BE0EB0 | CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive (slot 5) | DIFFER(2889v2889), 14 lines | 13 | differ/ |
| 0x00BC25F0 | CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive (slot 5) | NOT_ATTEMPTED | 0 | - |

Three of the 0x00BC3F30 grades (b1, b2, b3) ran in parallel and are void: q.py shares `$S/wine/q/<addr>` for every run of the
same address. They were regraded one at a time as b1s, b2s and b3s.

## Identities and slot names
- **Slot 5 is RenderSubPrimitive** in all four manager vtables:
  - ParticleGroup 0x012A5BB8 → 0x00BBC460
  - MeshGroup 0x012A63B8 → 0x00BC25F0
  - LineGroup 0x012A6590 → 0x00BC3F30
  - 2DParticleGroup 0x012A22D0 → 0x00BE0EB0
  
  All four are `?RenderSubPrimitive@<Class>@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z` in both FableWin.pdb and
  Ego_r.pdb, with `ret 8`.
- **0x00BBE090** is
  `?RenderSingleSpriteTrail@CEnginePrimitiveManagerParticleSpriteTrailGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z`
  in both PDBs. The landed RenderSubPrimGroups 0x00BBC130 reaches it for primitive type 20, through manager slot 29.
- **0x00BC3F3D is bogus.** It is the `push esi` inside the prologue of 0x00BC3F30:
  ```
  0x00BC3F30  mov eax,[0x0139B864]
              sub esp,0x1C
              push ebx
              xor ebx,ebx
              cmp eax,ebx
  0x00BC3F3D  push esi
  ```
  The epilogue pops four registers and returns with `add esp,0x1C; ret 8`, which is the frame 0x00BC3F30 opened. The vtable slot
  points at 0x00BC3F30.

## Header overlay (`hdr/engine`, every change behind a retail branch; hdrcheck2 reports `default=same` for all)
| header | branch | change |
|---|---|---|
| CParticleEmitter.h | FABLE_RETAIL_CPARTICLEEMITTER | Declares `ReadBinary(CDataInputStream&)` and forward-declares `CDataInputStream`. |
| CParticleSystem.h | FABLE_RETAIL_CPARTICLESYSTEM | Declares `ReadBinary(CDataInputStream&)` (0x00AF4660). Adds inline `IsEnabled()`, whose evidence is the FableWin COMDAT right after CParticleEmitter::ReadBinary. |
| CEngineComponent.h | FABLE_RETAIL_CENGINECOMPONENT | Adds `PeekRenderEnabled()`. |
| CEngineLightingManager.h | FABLE_RETAIL_CENGINELIGHTINGMANAGER | Exposes +0xCE..+0xEB (FadedShadowDiffuseFactorEnabled, MistEffectColour, CurrentThemeChannel, StateUpdateFlags, SelfIlluminationAmbientPercentage). Adds the __forceinline SetEnableFadedShadowFactor, SetCurrentEnvironmentThemeChannel, SetSelfIlluminationAmbient and RestoreDefaultPrimitiveParameters. |
| CEnginePrimitiveRenderUtil.h | FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL | Declares ApplyVertexShader / ApplyPixelShader (SI), GetCurrentCamera (SI), SetupBlendOpState and SetupAlphaBlendingStatesFromBlendMode. The rows define the bodies. |
| CEngineResourceManager.h | FABLE_RETAIL_CENGINERESOURCEMANAGER | Adds `PeekGraphicBank(EGraphicBankNo) const` (FableWin 0x02C85CC0, inline in retail) and a forward declaration of the enum EGraphicBankNo. |
| CMatrix3x4.h | FABLE_RETAIL_CMATRIX3X4 | Adds the __forceinline `PostScale(float)` (all twelve elements, column by column) and `PostTranslate(const C3DVector&)`. |

I rechecked the landed includers of the changed branches with vland `--recheck-landed`. All 7 are RELOCATION_MATCH with the test
passing (`SIBLINGS.tsv`, `sib/recheck.txt`).

## Header debt (local models in the rows)
- CEngineSubPrimitiveBase.
- The list node.
- CEngineInternalPrimitiveParticleGroup and its 2D / line / sprite-trail primitives.
- The particle group managers: CEnginePrimitiveManagerParticleGroup, LineGroup, 2DParticleGroup and SpriteTrailGroup.
- CGraphicFrameData, CGraphicFrame, CGraphicInfo and CGraphicDataBank.
- The CEngineCamera members +0xC0..+0x174. The camera's retail branch is still opaque.

Retail offsets:
- Particle-group primitives are Ego_r minus 0x14, after the STLport containers.
- Line-group manager members are Ego_r minus 4 (the CArray is 0xC in retail).

## Parked rows: residue and next idea
- **0x00BBE090 (46 lines).** Every remaining line is in the blend-op and blend-mode SetRenderState blocks.
  - Cause: the bookmark test/or operand order is a known lane gap, documented in CRenderStateManager.h.
  - Effect: retail's order differs between the cases, so VC7.1 cross-jumps them at a different point. Retail keeps case 1 of the
    blend-op switch whole and merges case 2 with default.
  - Next idea: none in the source until the lane gap is solved.
- **0x00BC3F30 (242 lines, -85 bytes).** The same lane gap is at work.
  - Cause: retail's additive and inverse-source-colour blend-mode cases have opposite test/or orders.
  - Effect: the dest-blend tails merge only after the `or` (0x00BC44AB `jmp 0x00BC453E`), while ours merge at the compare.
  - Knock-on: register choices differ. Retail holds lights in edi, &ConstantBlock in edi and the node in ebp. The camera-matrix
    `add eax,0x114` is scheduled later.
- **0x00BE0EB0 (14 lines, same size).**
  - The lane gap remains in two blocks (0x00BE1620, 0x00BE16C6).
  - One `mov ebx,1` is scheduled one x87 instruction away from retail (0x00BE0FE4).
  - Next idea: the lane-gap pattern moved from 5 blocks to 2 under unrelated FP respellings. More neutral respellings of the
    camera and scale code may land on retail's pattern.

## 0x00BC25F0 MeshGroup outline (for a follow-up chunk)
1. Return early if `!*(long*)0x0139B7B0`.
2. Get the mesh: `C3DMeshInfo::GetMesh` 0x00A25E70 (out pointer, flags `primitiveRenderer(0x01436E80)->+4 ? 0x11 : 0`) on group
   +0xAC. On failure, call 0x00A23C50(1) and release the counted mesh.
3. Pass 6 (when the group manager's byte +0x21 is set, the displacement renderer 0x01436E38 gets 0x00B86A00(group+0xB0), a
   `ret 4` stub in retail):
   - Set blend op, source and dest blend.
   - ApplyVertexShader(this+0x2C) and ApplyPixelShader(this+0x54).
   - Upload the camera NoScaleViewTransform copy as SetUserVertexShaderConstant 0..3 (0x00989B00).
4. Pass 7: alpha test, alpha ref 0x10, alpha func GREATER, then the same shader and constant upload.
5. Other passes:
   - SetUpPointLights, then the clamped point-light vertex shader (CArray at +0x20).
   - Blend op and blend mode, each behind the switches `*(long*)0x0139B7AC` and `0x0139B7A8`.
   - SetupPixelShader(0, +0x34/+0x3C or +0x44/+0x4C).
6. Constant 4 = (4.0f, 0, 0, 0).
7. Loop over the mesh's primitive batches (mesh +0x90 / +0x94, stride 0x60, batch capped at 10):
   - CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360 and AttachTextureToStage.
   - SetSelfIlluminationAmbient(material +0x24) and UpdateGlobalLightingStates.
   - Select the batch's vertex and index buffers.
   - Per particle mesh: three matrix rows (+0x48..+0x74) and the colour /255 with doubled alpha into ConstantBlock at manager
     +0x5C, 4 per mesh.
   - When the batch is full: SetWorldTransform(group+0xB0), SetUserVertexShaderConstants(5, n, ConstantBlock) 0x009896D0,
     PrepareForRender(0x11), DrawCurrentBuffersAsIndexedTris 0x00A0AC60.
8. Tail draw.

## Callees bound (spellings from reloc_bindings / shared headers)
- CShaderRenderManager:
  - ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140
  - SetWorldTransform 0x009881F0, ResetWorldTransform 0x00988290
  - EnableVertexShaders 0x00987FE0, DisableVertexShaders 0x009880E0
  - EnablePixelShaders 0x00988110, DisablePixelShaders 0x00988190
  - SetVertexShaderConstantLayout 0x0098B5E0
  - SetUserVertexShaderConstant 0x00989B00, SetUserVertexShaderConstants 0x009896D0
  - The PrepareForRender updaters
- CRenderStateManager::RestoreCaptureBlock 0x00A05840
- CRenderManagerCore::DrawCurrentBuffersAsIndexedTris 0x00A0AC60
- CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80
- CEnginePrimitiveManagerParticleGroup: RenderSubPrimGroups 0x00BBC130, SetUpPointLights 0x00BBC0B0, SetupPixelShader 0x00BBCC70
- CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail 0x00BCA950
- CGraphicInfo::GetFrame 0x009FF8D0
- 100.0f is 0x01230A08

## GOTCHAS one-liners
- VC7.1 commutes and reassociates float `*`, `/` and `+` operands under /O2. Match retail's order by splitting statements
  (`s = a*b*c; s /= d`; scale passed as `s / d`), by copying a local before an in-place transform (`depth = pos.Z`), or by a
  by-reference helper operand (`PostTranslate(const C3DVector&)` turns `fld v ; fadd st(1)` into `fld E4k ; fadd v`).
- The bookmark test/or operand-order lane gap is not local. Unrelated FP respellings moved it in 0x00BE0EB0 (5 blocks → 2), and it
  decides where VC7.1 cross-jumps identical SetRenderState tails (0x00BC3F30: -85 bytes; 0x00BBE090: switch-case merging).
- `CEngineResourceManager::PeekGraphicBank(0)->PeekGraphic(i)` gives retail's prologue load order, where `GraphicBanks[0]` does
  not (the same lever as chunk 723's RenderSingleSprite).
- In a constant-upload loop, a reference per constant block (`CEngineParticleConstantBlock& b = ConstantBlock[n]`) gives retail's
  one-address-per-block code. Indexing each field recomputes `n*16+0x84`.
- q.py works in `$S/wine/q/<addr>`: never grade two sources of the same address in parallel.
- vland tests compile with VC7.1 cl:
  - `vector::insert(it, const char*, const char*)` does not compile.
  - `T()` does not zero a struct with a std::vector member. Give it a constructor.
- FableWin's empty CRenderStateManager::SetStippleEnable sits right after the 2D RenderSubPrimitive. It is a no-op call in the
  source and emits nothing in retail.
