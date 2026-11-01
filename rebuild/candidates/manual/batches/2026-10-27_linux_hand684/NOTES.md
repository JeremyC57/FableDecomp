# Chunk 684 notes

## Names and evidence
| address | name | evidence |
|---|---|---|
| 0x00BDA150 | `CEngineLandscapeMemoryManager::ProcessDisposalList()` (private) | proven: Ego_r.pdb / FableWin.pdb; Ego_r donor 009C2A30 sits between EnablePoolAllocation 009C29D0 and AllocateIndexBuffer 009C2B20 = retail 0x00BDA070 / 0x00BDA150 / 0x00BDA360; body drains the three disposal lists of the landed FreeDisposalList 0x00BD9CE0. Callers: the three Allocate* members (landed as `ProcessDisposalLists`). |
| 0x00C77D90 | `MixChannels_00c77d90(const float* position, float* gains, unsigned long count, unsigned long channels)` | descriptive (libpk audio, no PDB/RTTI); the binding name of the landed callers CChannelMixer_00c783b0::Mix 0x00C783B0 / CEffectPart260::Configure 0x00C783F0, kept under one-address-one-name; the counts are unsigned (retail `jae`/`jb`/`jbe`), so the leaf is `?MixChannels_00c77d90@@YAXPBMPAMKK@Z` (binding has `PAM0JJ`). |
| 0x00BA9A70 | `CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(unsigned long, unsigned long, bool&, const CGraphicDataBank*, bool)` (static) | proven: Ego_r.pdb / FableWin.pdb; Ego_r donor order GetDiffuseTexture 00a5fff0 / GetBumpTexture 00a60220 / GetReflectionTexture 00a60440 / Get2DAlphaMaskTexture 00a60640 = retail 0x00BA9360 / 0x00BA95D0 / 0x00BA9830 / 0x00BA9A70. |
| 0x00BA95D0 | `CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial*, CCompositeSurfaceMap*, unsigned long, bool&, const CGraphicDataBank*, bool)` (static) | proven as above; reads C3DMeshMaterial::BumpMapBankIndex (+0x14), passes `bump = true` to GetComposedTexture 0x00BEBED0. |
| 0x00BA9360 | `CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(...)` (static) | proven as above; DiffuseMapBankIndex (+0x10), `bump = false`. |
| 0x00C677C0 | `CEaxContext::SetEnvironmentSize(unsigned long slot, float value)` | descriptive (libpk EAX, no PDB/RTTI): the landed CEaxContext object (vtable 0x012B9340, SetDecayTime 0x00C633C0 family), the range is EAXREVERB_MIN/MAXENVIRONMENTSIZE (1..100), the flag bits are the EAX 4.0 EAXREVERBFLAGS scaling rules, callers are the property dispatchers 0x00C68A50 / 0x00C68F90. |
| 0x00CB28F0 | `GetImeId(unsigned int uIndex)` (`__stdcall`, ImeUi.cpp file static in the SDK) | proven: FableWin.pdb + Ego_r.pdb name sets; DXSDK ImeUi.cpp text (_CHT_HKL / _CHT_HKL2 / _CHS_HKL layouts, the five IME file names 0x012BFA88..0x012BFAC8, MAKEIMEVERSION table); callers CheckInputLocale 0x00CB37A0 (landed) and 0x00CB31C0. |
| 0x00A5B870 | `NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, unsigned long count, float t)` | proven: Ego_r.pdb (KKM) overload; donor order GFInitVectorMath / (KKM) / (K1KKM) / (K1KPBMKK) / GFFinishPartialSlerp = retail 0x00A5B850 / 0x00A5B870 / 0x00A5BBA0 / 0x00A5BEF0 / 0x00A5C2B0; grade retail_asm (SSE kernel 0x00A5B8A6..0x00A5B8DC). |
| 0x0077A890 | `CTCDExperienceOrb::Create(CThing& creature)` (static) | proven: FableWin.pdb / Ego_r.pdb, the CThing& overload beside the landed 0x00779E90 overload it calls; caller CThingCreatureBase::OnDie 0x00664300 (landed as `CreateExperienceOrbs`). Parked. |
| 0x00C74510 | `CDspGain::ApplyRamps(const float* feedback, const float* feedforward, float* state, const float* source, float* block, bool hasInput, unsigned int count)` | descriptive (libpk reverb): callee of the landed CDspGain::Apply 0x00C74CE0 / Crossfade 0x00C74D30; the sixth argument is a byte test (bool). The body is a 1-offset IIR (state 1.0 = rest): first order inline (pure gain copy / ScaleCopy when a1 == b1 == 0), second order via the ops table's slot 8 when there is input, inline decay loop otherwise. Parked. |

## Label corrections (catalog)
- 0x00BDA150 `CDrawResourcePool_FlushCycle` -> `CEngineLandscapeMemoryManager::ProcessDisposalList` (NAME_FIXES also covers the two reloc bindings).
- 0x00C677C0 `HeroAttribute_SetValueWithValidation` -> `CEaxContext::SetEnvironmentSize` (descriptive).
- 0x00C74510 `Audio_IIRFilterProcess` -> `CDspGain::ApplyRamps` (binding, bool sixth argument).
- 0x0077A890 `_global::Create` -> `CTCDExperienceOrb::Create(CThing&)`.
- 0x00A5B870 `_global::GFInterpolateVectors` -> `NVectorMath::GFInterpolateVectors` (KKM).
- 0x00BA9360 / 0x00BA95D0 / 0x00BA9A70: right names, missing class `CEnginePrimitiveManagerMeshBase`.
- 0x00C77D90 `Audio_MixChannels` -> binding `MixChannels_00c77d90` (descriptive, kept).
- 0x00CB28F0 `GetImeId`: right.

## Header debt
- `engine/CEngineResourceManager.h` (generated from Ego_r) is 0x230 bytes above retail: retail SatRGBATexture +0x2ADC (header +0x2D0C), ZeroRGBATexture +0x2AD4, StipleFadeTexture +0x2AAC; 0x00BA9A70 models the manager as a pad + `CTexture SatRGBATexture` with a `// Layout:` note.
- `ETextureType` enumerator 4 (bump) is proven only by the GetBlankTexture argument of 0x00BA95D0 (`TEXTURE_TYPE_BUMP = 4`); the PDB enum text was not located.
- `g_ExperienceOrbInitialVelocity_01382a68` (0x01382A68, C3DVector (0.3, 0, 0.4) in .data) is a placeholder name in the parked 0x0077A890.
- The texture-family model (`tmp/texfam_model.inc`, shared by the three matched rows and the parked 0x00BA9830) carries CGraphicInfoFrame / CFrameData / CGraphicDataBank layouts as `// Layout:` comments; no `engine/` header reproduces them yet.
- 0x00C74510 adds a ninth CDspOps slot (+0x20, the biquad with input) to the landed CDspOps model.
- The ImeUi SDK revision matching retail returns `dwRet[0]` on success, zeroes both entries on every failure and frees a null buffer on Malloc failure (differs from the public June-2010 text); documented in the source header.

## Manifest / donor splits
- None needed: every row is one retail function with one name.
