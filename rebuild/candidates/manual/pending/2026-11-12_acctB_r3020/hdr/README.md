## Chunk 3020 changes (row 0x00A12EE0 only; every change opt-in, default paths untouched)
| file | opt-in | content | evidence | used by |
|---|---|---|---|---|
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPLANE` (REWRITTEN; chunk 3001's probe body and its TransformVector(src, dst) removed) | `FABLE_FORCEINLINE TransformPlane(CPlane&)`: `C3DVector normal = plane.GetNormal(); C3DVector point = plane.GetNormal() * plane.GetDistance(); TransformVector(normal); normal = normal.AccurateNormalise(); TransformPoint(point); plane.Initialise(normal, normal.Dot(point));` | FableWin.pdb ?TransformPlane@CMatrix3x4@@QBEXAAVCPlane@@@Z is the first COMDAT after GetNavigationLevelVolumes (0x031D2030) and 48i/139b at /Od; this text 48i/140b (fwsize). FITTED statement order: normal copy first (Dot operand order), point second (schedule). Forced: plain inline is called out of line in the row (grade 21) | 0x00A12EE0 |
| engine/CPlane.h | `FABLE_RETAIL_CPLANE_ACCESSORS` (new) | inline `GetNormal()` (unless `_LINE` declares it) and `GetDistance()` | FableWin.pdb ?GetNormal@CPlane@@QBEABVC3DVector@@XZ (8i/14b), ?GetDistance@CPlane@@QBEMXZ (9i/17b); no retail out-of-line copy | 0x00A12EE0 |
| engine/CIVCountedPointer.h | `FABLE_RETAIL_CIVCOUNTEDPOINTER_GET` (new; repo copy + opt-in) | `T* Get() const { return static_cast<T*>(Object); }` | FableWin.pdb ?Get@?$CIVCountedPointer@$$CBVCPhysicsMesh@@@@QBEPBVCPhysicsMesh@@XZ (0x031D3000) and ?IsNull@.. (0x031D2FE0), both emitted right after GetNavigationLevelVolumes; `const CPhysicsMesh* mesh = Mesh.Get(); if (mesh)` fixed the latch reload order (0x00A13294) | 0x00A12EE0 |
| (not changed) engine/C3DVector.h, engine/CCountedPointer.h | -- | probes only: Dot / operator*(float) texts, FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (repo opt-in; neutral in this row, used by the differ source because Ego_r + FableWin both show `~CCountedPointer() { ReleaseData(); }`) | | |

Session 2 (row matched, RELOCATION_MATCH 1144v1144): NO header change; the closing lever is in the row source (`const CPlane& source_plane`). Probe-only
blocks used for diagnosis (a no-zz TransformPoint, `normal * distance`) were removed again; the table above is the final state.

Regrade note: the rewritten TRANSFORMPLANE block is used by no landed row (it was a chunk 3001 probe). TransformPoint is used through the
existing nested opt-in `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR` (chunk 3001; rows left to right), which is a THIRD TransformPoint text in
the repo next to the default and `_SUMS` (header debt: "one retail body, several contexts" -- the x87 order is c2's record-id hash, see NOTES).

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
