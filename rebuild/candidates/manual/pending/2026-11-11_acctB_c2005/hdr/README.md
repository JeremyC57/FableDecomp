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
