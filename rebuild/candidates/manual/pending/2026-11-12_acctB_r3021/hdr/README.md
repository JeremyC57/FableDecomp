# Chunk 3005 header overlay (grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3005/hdr`)

No header was changed by chunk 3005. This overlay is the union of the chunk 3001 overlay (row 0x00A9EB90) and the chunk 3002 overlay
(row 0x00550DC0); the only file in both, engine/CMatrix3x4.h, is taken from 3001 (a superset of 3002's copy: 3002's text plus the 3001
opt-ins). Both prior READMEs follow verbatim. Two probe opt-ins tried in engine/CRGBFloatColour.h this chunk (member operator*, converting
constructor through operator=) were byte-neutral and were removed again; NOTES.md records them.

---
# Chunk 3001 header overlay (grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3001/hdr`)

Union of the chunk 2005 and chunk 2001 overlays (their READMEs follow, unchanged) plus the chunk 3001 additions in the first table.
Every change to an existing repo header is an opt-in macro (default paths untouched); copied from the prior overlays, which were
copied from repo HEAD. Drift check against rebuild/include at hand-back: only additions / re-wrapped #if lines.

## Chunk 3001 additions
| file | opt-in | content | evidence (two sources) | used by |
|---|---|---|---|---|
| engine/C3DAnimation2.h | `FABLE_RETAIL_C3DANIMATION2_CALCLOCALBONE` (inside `..._CALCLOCAL`) | declares `void CalcLocalBoneTransformation(C3DKeyframe&, unsigned long, float) const` | Ego_r.pdb + FableWin.pdb `?CalcLocalBoneTransformation@C3DAnimation2@@QBEXAAVC3DKeyframe@@KM@Z`; retail out-of-line copy 0x00A4D330 (`ret 0Ch`, frame 0x38; 0x00A4D920's frame 0x68 = 0x38 + the 0x30 local keyframe) | 0x00A4D920 (matched) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY` | chunk 2006's SetMultiply + PreMultiply block, copied verbatim from agent2006/hdr | chunk 2006 (0x00B399A0) | 0x00AB2520 (parked) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY_SETMULTIPLY` (inside `..._POSTMULTIPLY`, needs `..._SETMULTIPLY`) | `PostMultiply(m) { CMatrix3x4 copy(*this); SetMultiply(copy, m); }` | FableWin.pdb lists ?PostMultiply@CMatrix3x4@@QAEXABV1@@Z (0x02604F80, 0x40 bytes: copy + one call) immediately ahead of ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z (0x02604FC0); retail 0x00AB2771 `rep movsd` copy of *this, then the products (66 -> 23 diff lines) | 0x00AB2520 |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DETERMINANT` / `..._SETINVERSE_NOTRANSLATE` (inside `FABLE_RETAIL_CMATRIX3X4`) | inline `GetDeterminant() const`, `SetInverseNoTranslate(m)`; SetInverse through GetDeterminant (and, with NOTRANSLATE, through SetInverseNoTranslate + the translation rows) | FableWin.pdb ?GetDeterminant@CMatrix3x4@@QBEMXZ (0x01B41130, right after SetInverse 0x01B40BE0) and ?SetInverseNoTranslate@CMatrix3x4@@QAEXABV1@@Z (0x02D840E0, 0x4C0 bytes = SetInverse's 0x550 minus the three translation rows); no retail out-of-line copy. Same cofactors as the default body. DETERMINANT is byte-neutral in 0x00AB2520; NOTRANSLATE is worse there (45 lines) | 0x00AB2520 (DETERMINANT) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_NOTRANSLATION` | probe: SetMultiply through FableWin ?SetMultiplyNoTranslation@CMatrix3x4@@QAEXABV1@0@Z (0x032289C0) | byte-neutral in 0x00AB2520; unused by any deliverable | - |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPLANE` | FableWin ?TransformVector@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z and ?TransformPlane@CMatrix3x4@@QBEXAAVCPlane@@@Z as inline bodies (probe for 0x00A12EE0; worse there: 39 v 29 frame-masked lines); includes engine/CPlane.h | FableWin.pdb names only (no retail copy) | unused by any deliverable |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR` (nested in the FABLE_RETAIL_CMATRIX3X4 TransformPoint choice) | probe: all three TransformPoint rows left to right | - | unused |
| engine/C3DKeyframe.h | `FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY` (chunk 2005 opt-in, listed there as experimental) | now USED: `PostApplyRelativeAnimation(other) { Rotation *= other.Rotation; Position += other.Position; }` (pulls in CQUATERNION_MULASSIGN + C4DVECTOR_ADDASSIGN) | FableWin.pdb ?PostApplyRelativeAnimation@C3DKeyframe@@QAEXABV1@@Z; retail 0x00A4DE26 `call 0x00A88C10` (CQuaternion::operator*=) then the four position adds | 0x00A4D920 (matched) |
| engine/C3DMesh2.h | `FABLE_RETAIL_C3DMESH2_BONES`, `FABLE_RETAIL_C3DMESH2_BONEKEYFRAME` (NEW in overlay; repo copy + opt-ins) | inline GetBone (const / non-const), GetBoneCount, GetBoneKeyframe | Ego_r.pdb / FableWin.pdb ?GetBone@C3DMesh2@@QBEPBVC3DBone2@@K@Z / QAEPAV..., ?GetBoneCount@C3DMesh2@@QBEKXZ, ?GetBoneKeyframe@C3DMesh2@@QBEPBVC3DKeyframe@@K@Z; nothing bound out of line in reloc_bindings | probe only (byte-neutral in 0x00AB2520); unused by any deliverable |

# hdr/ overlay (chunk 2005)

Each modified header started as a copy of the repo header. Every change to an existing header is an opt-in macro, so
landed rows see no difference.

## New headers
These are Ego_r layouts plus PDB method spellings:
- `engine/CEngineWeatherSettings.h`: members through RandomNumberGeneratorSeed +0xD8; sizeof 0x558.
- `engine/CTVertexWeather.h`: CTVertexWeatherBase is X, Y, Z, U1, V1, ConstantIndex, Pad; CTVertexWeather is
  CGenericVertex<Base>, 0x18 bytes.
- `engine/C3DBone2.h`: NameCRC, Parent, OriginalNoChildren, LocalizationMatrix; 0x3C bytes.
- `engine/N3DAnimationBuilder_COutputAnimation.h`: CSequenceInfo (OriginalIndex, GeneratedTransform; 0x34 bytes) and
  COutputAnimation (Sequences, GeneratedBoneCount +0xC).
- `engine/C3DAnimationBuilder.h`: the vptr, then AddedBonePrefixes at +4, and nested CBoneInfo. It declares
  BuildMeshAnimation, AssignAnimation, the three BuildBoneInfoTree overloads and GetIntersection.
- `engine/C3DMeshVolume.h`: Name, ConvexVolume, and the inline `GetConvexVolume() const` from FableWin.pdb.

## Opt-ins in existing headers
| header | opt-in | content |
|---|---|---|
| `engine/CEngineWeatherRenderer.h` | `FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS` | typed buffer members (+0xC, +0x40, +0x54, +0x88, +0xBC), MistVolumeTexture +0x110, Settings +0x114, sizeof 0x12FC, `BuildBuffers()` declaration |
| `engine/CEngineResourceManager.h` | `FABLE_RETAIL_CENGINERESOURCEMANAGER_VBPOOL` | `EVertexBufferPool GetVertexBufferMemPool()` at 0x00B3B090 |
| `engine/CGenericVertex.h` | `FABLE_RETAIL_CGENERICVERTEX_INLINE` | inline SetX/Y/Z, SetXYZ, SetU1/V1, SetU1V1 |
| `engine/GFFloatRandom.h` | `FABLE_RETAIL_GFFLOATRANDOM_INLINE` | inline body; RELOCATION_MATCHes COMDAT 0x004978A0 |
| `engine/CQuaternion.h` | `FABLE_RETAIL_CQUATERNION_MULASSIGN` | `operator*` (0x00A88B60) and `operator*=` (0x00A88C10); also `SetIdentity()` declared in the SLERP block |
| `engine/CQuaternion.h` | `FABLE_RETAIL_CQUATERNION_MULASSIGN_INLINE` | experimental, unused |
| `engine/C3DAnimationSequenceData.h` | `FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE` | GetKeyframeInterpolate (0x00A4C1F0), InlineGetKeyframeInterpolate, nested C3DVectorWord |
| `engine/C3DAnimationSequence.h` | same macro | FABLE_FORCEINLINE InlineGetKeyframeInterpolate forwarder |
| `engine/C3DAnimation2.h` | `FABLE_RETAIL_C3DANIMATION2_CALCLOCAL` | ConvertTimeToFrames, CalcLocalBoneTransformationRelative |
| `engine/C3DKeyframe.h` | `FABLE_RETAIL_C3DKEYFRAME_CTOR` | empty ctor |
| `engine/C3DKeyframe.h` | `FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY` | experimental, unused |
| `engine/C4DVector.h` | `FABLE_RETAIL_C4DVECTOR_ADDASSIGN` | `operator+=` |
| `engine/C4DVector.h` | `FABLE_RETAIL_C4DVECTOR_ASSIGN3D` | `operator=(const C3DVector&)`, W = 0 |
| `engine/CMatrix3x4.h` | `FABLE_RETAIL_CMATRIX3X4_SETIDENTITY` | inline SetIdentity (nine zeros, then three ones; 0x00AB28CF) |
| `engine/CContainmentVolume.h` | `FABLE_RETAIL_CCONTAINMENTVOLUME` | polymorphic class: ctor 0x00A752D0, Initialise 0x00A755C0, inline PeekContainmentPlanes/GetPlaneCount |
| `engine/CPhysicsMesh.h` | `FABLE_RETAIL_CPHYSICSMESH_NAVVOLUMES` | NavigationContainmentVolumes +0x54 and accessor |
| `engine/CPhysicsMeshObject.h` | `FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVVOLUMES` | GetNavigationLevelVolumes declaration |
| `engine/CPlane.h` | `FABLE_RETAIL_CPLANE_NORMAL_DISTANCE` | `bool Initialise(const C3DVector&, float)` at 0x00A42140 |
| `engine/CVectorMap.h` | `FABLE_RETAIL_CVECTORMAP_SORT_INLINE` (+ `_FORCE`) | inline Sort() |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL` | noinline `__fastcall __destroy_aux` |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_SORT_INLINE` (+ `_FORCE`) | inline std::sort with `__lg` / `__final_insertion_sort`; `__fastcall` introsort / insertion declarations |

# Chunk 2001 header overlay (grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2001/hdr`)

Every change is a new opt-in (default paths untouched) or a new class header. Copied from repo HEAD before editing.

| file | change | evidence (two sources) | used by |
|---|---|---|---|
| engine/CParticleEmitter.h | opt-in `FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATETO`: declares `void InterpolateTo(const CParticleEmitter&, float)` | Ego_r.pdb + FableWin.pdb `?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z`; `ret 8`; caller 0x00AEB674 | 0x00AED600 |
| engine/CParticleSystem.h | opt-in `FABLE_RETAIL_CPARTICLESYSTEM_INTERPOLATETO`: `void InterpolateTo(const CParticleSystem&, float)` | both PDBs `?InterpolateTo@CParticleSystem@NParticleEngine@@QAEXABV12@M@Z`; retail 0x00AF4100, sole caller 0x00AED623 | 0x00AED600 |
| engine/CParticleSystem.h | opt-in `FABLE_RETAIL_CPARTICLESYSTEM_ADDCOMPONENT`: `CPSCBase& AddComponent(const CCharString&, const unsigned long&, bool, bool)` | both PDBs `?AddComponent@CParticleSystem@NParticleEngine@@QAEAAVCPSCBase@2@ABVCCharString@@ABK_N2@Z`; the bound reloc 0x00AF4530 | 0x00AF4900 |
| engine/CParticleSystem.h | opt-in `FABLE_RETAIL_CPARTICLESYSTEM_SCALE`: `Scale` typed `C3DVector` (was `unsigned char[12]`), same offset +0x60 | Ego_r.pdb member `Scale` (C3DVector); retail ScaleX/Y/Z transfers at +0x60/+0x64/+0x68 (0x00AF4990..0x00AF49CA) | 0x00AF4900 |
| engine/CPSCBase.h | NEW: `NParticleEngine::CPSCBase` : `CParticleUpdateParamCallback<unsigned long,unsigned long>` : CBaseClassNonCopyable, 32 virtual slots | RTTI bases; retail vtable 0x0129F3F4 (implemented slots 7..13, 17, 19, 21..31) filled in FableWin.pdb definition order of the CPSCBase bodies (0x02F10210..0x02F103C0); pure slots from CPSCRenderSprite vtable 0x0129F67C + landed rows (14 InterpolateTo, 16 WriteBinary, 17 DatabaseSerialise, 18 DatabaseCopy, 20 GetComponentCost, 28 Initialise) | 0x00AF4900 (slot 17 call) |
| engine/CStringParser.h | opt-in `FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR`: the inline body of `CStringParser(const CCharString&, cb = 0, const CCharString& filename = CCharString(""), long = 0)` | both PDBs `??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z`; retail COMDAT 0x00404720 (CBaseClass(), vtable 0x0122D874, Filename(), TempCharString(), Comments(), `TempCharString = text`, Init(text chars, ...)) and the same body expanded at 0x00AF4CA1 | 0x00AF4900 |
| engine/CStringParser.h | opt-in `FABLE_RETAIL_CSTRINGPARSER_READLINE`: `CCharString ReadLineAsString()` | Ego_r.pdb `?ReadLineAsString@CStringParser@@QAE?AVCCharString@@XZ`; retail 0x009BA390 (hidden result, thiscall) | 0x00AF4900 |
| engine/CDataInputStream.h | opt-in `FABLE_RETAIL_CDATAINPUTSTREAM_READWHITESPACE`: `CCharString ReadWhiteSpace()` | Ego_r.pdb `?ReadWhiteSpace@CDataInputStream@@QAE?AVCCharString@@XZ`; retail 0x00994F60 | 0x00AF4900 |
| engine/CEngineResourceManager.h | opt-in `FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE`: StipleFadeTexture (CTexture), DitherTextureList (CArray<CTexture>), EngineCachePath (CWideString) typed at the opaque members' offsets; declares BuildHWStipplePatterns / SetupAlphaStippleTexture | Ego_r.pdb member types and both PDBs' private method names; retail +0x2AAC/+0x2AC0/+0x2ACC uses in 0x00B3C5D0 (sar 3 = 8-byte elements) | 0x00B3C5D0 |
| engine/CSurface.h | opt-in `FABLE_RETAIL_CSURFACE_AUTOSWIZZLE`: `void CopyFromMemoryAutoSwizzle(void*, long)` | Ego_r.pdb `?CopyFromMemoryAutoSwizzle@CSurface@@QAEXPAXJ@Z`; retail 0x009F3470 | 0x00B3C5D0 |
| stlport/fable_containers.h | opt-in `FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_OUTOFLINE`: STLport's `resize(size_type)` body visible but `__declspec(noinline)` (MSVC only) | retail calls vector<CTexture>::resize(4) 0x0049B7F0 out of line (both PDBs `?resize@?$vector@VCTexture@@...@QAEXI@Z`) while the same TU's BuildAlphaStippleTexture must see the body (exposure) | 0x00B3C5D0 (Inline-budget line) |
| engine/CQuaternion.h | opt-in `FABLE_RETAIL_CQUATERNION_SLERP`: declares Dot / SetSlerp / static CalculateSlerpWeights | FableWin.pdb `?Dot@CQuaternion@@QBEMABV1@@Z`, `?SetSlerp@CQuaternion@@QAEXABV1@0M@Z`, `?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z` (landed 0x00AA3390) | 0x00A9EB90 (parked) |
| engine/C3DAnimationManager.h | NEW: retail C3DAnimationManager (CBaseClassNonCopyable base; MeshBank +4, BoneIndicesBuffer +8, MemoryPool +0x14, MemoryPool2 +0x30; sizeof 0x4C) | Ego_r.pdb struct layout + landed ctor 0x00AA0F60 / CalcBlendedMeshTransformations 0x00AA09F0; `lea ecx,[esi+14h]` before Allocate at 0x00A9EBF8 | 0x00A9EB90 (parked) |

Header debt noticed (not changed): engine/CTCTrapBase.h wraps a full-layout class in an outer `#ifdef FABLE_RETAIL_CTCTRAPBASE`
whose `#else` holds the generated default plus an unreachable inner `#ifdef FABLE_RETAIL_CTCTRAPBASE` opt-in (the agentNF12 branch);
0x007891F2 compiles against the outer branch (needs FABLE_RETAIL_DEF_CLASS_BASE + engine/ETCInterfaceType.h first).

---
# Chunk 3002 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3002/hdr`)

Chunk 3002 made NO header change of its own. hdr/engine is the union of the two prior overlays it continued from (no file name
overlaps, so the union is exact): agent2022's (row 0x007F26B0; section 1 below) and agent2006's (row 0x00550DC0; section 2 below).
Both prior differ sources were re-graded with this merged overlay before any change and reproduced their recorded grades
(0x007F26B0 DIFFER(1793v1802), 0x00550DC0 DIFFER(1399v1399)). The matched row 0x007F26B0 additionally turns on the EXISTING repo
opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (rebuild/include/engine/CSpecialEffectsDef.h, chunk 782: same-TU noinline body of
CSpecialEffectsDef::GetEffectFromString 0x00669D30), no header edit.

## 1. From chunk 2022 (agent2022/hdr/README.md, verbatim)

## Chunk 2022 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2022/hdr`)

Every change to an existing header is an additive opt-in (`#ifdef FABLE_RETAIL_...`): with none of the new macros defined each copied file
preprocesses to its source text. Copied unchanged from the acctB c2007 hand-back (`pending/2026-11-11_acctB_c2007/hdr/engine`, which itself
carries the c2002 CTCTavernGame.h / CRightHandedSet.h / CInputProcessTavernGame.h): every other file under hdr/engine not listed below
(CTCTavernGame.h, CTCTavernGameCoinBase.h, Coin.h, CTCTavernGameShoveHaPenny.h, CInputProcessGameBase.h, CInputProcessTavernGame.h,
CRightHandedSet.h, ...). Row 0x007D3820 needs the c2007 CoinBase / Coin / CTCTavernGame / input-process files; land them first.

### New headers
| header | content | evidence |
|---|---|---|
| engine/CTCTavernGameCoinGolf.h (row 0x007D3820) | `CTCTavernGameCoinGolf : CTCTavernGameCoinBase`, vtable 0x0126F49C overrides, the Ego_r/FableWin method set (EnableCourse, GetCamera public; SetupForNewRound / FadeOutTableOccupants private); members ZAngle +0x148 .. CurrentCourse +0x17C, sizeof 0x180 | RTTI bases; rtti-vtables.tsv slots; Ego_r layout - 0x18; Alloc 0x004D59DF `push 180h`; ProcessInput / landed GetCamera 0x007D3560 offsets |
| engine/CLightningOrbDef.h (row 0x007F26B0) | `CLightningOrbDef : CSubDefClassBase`, StatGetTypeName inline, Transfer / Copy / GetSizeofClass; LightningDefIndex +0x28 .. ExplosionDefIndex +0x38 | RTTI bases; Ego_r layout (no container); Initialise reads +0x28 / +0x30; landed FrameUpdate +0x34 / +0x38 (the generated copy stays in _quarantine) |
| engine/CTCLightningOrb.h (row 0x007F26B0) | `CTCLightningOrb : CTCBase`, slots 6/8/9/13/21/22, Initialise, SetLifeTime, inline SetPParent / SetPTarget; members PParent +0x0C .. PSpline +0x2C, sizeof 0x34 | Ego_r / FableWin name sets; vtable 0x01240A2C; Ego_r layout; Initialise expands both setters |
| engine/CTCLightningNode.h (row 0x007F26B0) | `CTCLightningNode : CTCBase`, inline SetPParent / SetPSourceOfFaction / SetPSourceOfHit / SetPDef; members PParent +0x0C .. LightningBranches +0x2C, sizeof 0x38 | Ego_r / FableWin names; out-of-line copies 0x007F39A0 / 0x007F39F0 / 0x007F3A40 / 0x007F3A90 write +0x0C / +0x14 / +0x1C / +0x24 |
| engine/NBBBMaths_CLinearSpline.h (row 0x007F26B0) | `NBBBMaths::CLinearSpline` with the two STLport vectors (sizeof 0x18), GetLength / GetPointWithParam / AddControlPoint / ProcessPoints(long) declared | Ego_r layout + donor order; `push 18h` new and the six zero stores in Initialise; landed 0x00A580F0 |

### Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CTavernGameCoinGolfDef.h | FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF | the polymorphic retail class `: CSubDefClassBase` (members at the Ego_r offsets, inline StatGetTypeName); the generated flat layout stays the default. Evidence: RTTI bases, the landed GetSubDef 0x007D4BD0 literal, ProcessInput reads +0x34 / +0x38 / +0x3C / +0x44 / +0x48 |
| engine/C2DVector.h | FABLE_RETAIL_C2DVECTOR_ROTATE (inside the NORMALISE block) | Rotate(float) 0x00A141D0, AccurateNormalise() 0x00A14510 (declarations), inline operator-= (Ego_r / FableWin ??Z). Evidence: bound names + ProcessInput call sites 0x007D3B07 / 0x007D3C0C, in-place subtract 0x007D3B7B |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES | Rotate(float, float) 0x00A14260 declaration (Ego_r / FableWin ?Rotate@C3DVector@@QAEXMM@Z; GetCamera 0x007D3560 calls it) |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND (needs _MOVEMENT) | inline SetForceResendEveryRenderFrame(bool) (Ego_r / FableWin name; bit 1 of +0x59 = ForceResendEveryRenderFrame, `or byte ptr [eax+59h],2` 0x007F2C98) |
| engine/CTCDParticleEmitter.h | FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE (default branch) | static __fastcall Create(long, const C3DVector&, bool) 0x006E0880 declaration (Ego_r / FableWin; the bound spelling) + `class C3DVector;` forward |

Landing note: re-copy HEAD first and re-apply the blocks (other chunks may have changed the same headers since 2026-11-11). The lightning
headers serve only the parked row 0x007F26B0.

## 2. From chunk 2006 (agent2006/hdr/README.md, verbatim)

## Chunk 2006 header overlay (hdr/engine)

Every change is an addition behind an opt-in macro; with the macros undefined each header preprocesses exactly as the repo copy
(CV_BodyGuardScript.h and CQ_HobbeCaveScript.h are the pending hand-back 2026-11-11_linux_handH40g's versions, whose blocks are
also opt-in). Grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2006/hdr`.

| header | opt-in | what | evidence | used by |
|---|---|---|---|---|
| engine/CEngineInternalAttachedPrimitive.h | `FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_TRANSFORMS` | declarations of GetParentTransform 0x00B398E0, GetParentTransformAnimatedMesh 0x00B39080, GetChildInverseTransform 0x00B39900, GetChildInverseTransformStaticMesh 0x00B393D0 (reference parameter); GetOutputPrimitive / UpdateOutputPrimitive when the UPDATE opt-in is off. No layout change | Ego_r.pdb + FableWin.pdb ?<Name>@CEngineInternalAttachedPrimitive@@ spellings; retail TU order 0x00B38DB0..0x00B399A0 | 0x00B399A0 (match) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY` | `SetMultiply(const CMatrix3x4&, const CMatrix3x4&)` (this = m1 * m2) and `PreMultiply(const CMatrix3x4&)` written as `CMatrix3x4 copy(*this); SetMultiply(m, copy);`, both FABLE_FORCEINLINE | Ego_r.pdb / FableWin.pdb ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z, ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z; retail 0x00B399F9..0x00B39EDB (three expansions, `rep movsd` copy); the only nesting that gives retail's 36 x87 sums | 0x00B399A0 (match). Mutually exclusive with the chunk 764 `FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY` |
| engine/CRGBFloatColour.h | `FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR` | `CRGBFloatColour(const CRGBColour&)` written through `Initialise(r, g, b, a)` (channels converted A, B, G, R) | Ego_r.pdb ??0CRGBFloatColour@@QAE@ABVCRGBColour@@@Z and ?Initialise@CRGBFloatColour@@QAEXMMMM@Z; retail 0x00550EA8..0x00550F53 conversion order | 0x00550DC0 (parked) |
| engine/NUISystem_CTable.h | `FABLE_RETAIL_CTABLE_DRAW` | `virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*)` override | Ego_r.pdb / FableWin.pdb ?Draw@CTable@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z; CTable vtable 0x0124A224 | 0x00550DC0 (parked) |
| engine/CV_BodyGuardScript.h | `FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN` (H40g, unchanged) | the nested CBodyGuard entity class (Ego_r 0x3c bytes) incl. `SetHenchmanAsHired(bool)` and `GetGoldCost()` | hand815 / H40g README; both PDBs | 0x00E621F0 (match) |
| engine/CQ_HobbeCaveScript.h | `FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBESPAWN_MAIN` (new; the file also carries H40g's `..._CBANDITSACRIFICE_MAIN` block unchanged) | the nested CHobbeSpawn entity class: ParentClass +0x14, PMasterData +0x18, NumberToGenerate +0x1c, size assert 0x20 | struct_layouts_egor.tsv CHobbeSpawn (32 bytes, member names); both PDB name sets (Alloc SI, Main / Init EAE, GetParentScript UBE); landed Alloc 0x00D8F310 | 0x00D91E00 (match) |

Notes:
- The shared CRGBFloatColour.h declares a free `operator*`; both PDBs have the member ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z. Both
  give the same bytes in 0x00550DC0 (not changed here: header debt).
- Re-base onto HEAD before merging (other agents may have touched these headers since the copies were taken); every block is a
  pure insertion.

---
# Chunk 3021
No header changed: the overlay is the chunk 3005 overlay unchanged. The row 0x00A9EB90 RELOCATION_MATCHes with it (src/00a9eb90.cpp defines its own forced-inline helper bodies).
