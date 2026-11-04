# Chunk 740 -- NParticleEngine::CPSCRenderMesh (InterpolateTo, ReadBinary, AddToBoundingSphere, DatabaseSerialise, PreparePrimitives)

## Results
| Address | Function | Grade | Attempts |
|---|---|---|---|
| 0x00AFF1B0 | CPSCRenderMesh::InterpolateTo (slot 14) | RELOCATION_MATCH, test PASS, vland clean (retail_asm: GFFloatToLongNear) | 1 |
| 0x00AFFFE0 | CPSCRenderMesh::ReadBinary ("vfunc_15") | RELOCATION_MATCH, test PASS, vland clean | 1 |
| 0x00B027D0 | CPSCRenderMesh::DatabaseSerialise (slot 17) | RELOCATION_MATCH, test PASS, vland clean (retail_asm: GFFloatToLongNear) | 13 |
| 0x00B01900 | CPSCRenderMesh::AddToBoundingSphere ("vfunc_19") | DIFFER(3755v3777), parked | 19 |
| 0x00B03CA0 | CPSCRenderMesh::PreparePrimitives ("vfunc_13") | DIFFER(6601v6468), parked | 10 |

Matched 3, parked 2. `vland_result.txt`: 3 x RELOCATION_MATCH + PASS, no warnings. Every attempt is in `LOG.tsv` (tags `final` are
regrades of the parked files after adding comments, not attempts).

## Slot names (vtable 0x0129F96C, rtti-vtables.tsv)
13 PreparePrimitives 0x00B03CA0, 14 InterpolateTo 0x00AFF1B0, 15 ReadBinary 0x00AFFFE0, 16 WriteBinary 0x00AFEE00, 17 DatabaseSerialise
0x00B027D0, 18 DatabaseCopy 0x00AFEB90, 19 AddToBoundingSphere 0x00B01900, 20 GetComponentCost 0x00B00DB0, 28 Initialise 0x00AFF0A0,
29 ClearAllPrimitives 0x00B01250. Evidence: the FableWin.pdb CPSCRenderMesh member set (exactly these virtuals with these signatures), the
sibling CPSC vtables (CPSCSingleSprite / CPSCLight / CPSCRenderSprite share the slot order; CPSCBase slot 19 = AddToBoundingSphere 0x00AF2A30),
chunk 737's slot-13 proof, `ret N` (13: 14h, 14: 8, 15: 4, 17: 8, 19: 10h), and 0x00B03CA0's own `call [edx+74h]` (slot 29) with
(info.Engine, handle) = ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&).

## Header overlay ($D/hdr/engine, mirror rebuilt with mkmirror.sh; hdrcheck2: default=same for all three)
- `CSymbolMap.h` behind new `FABLE_RETAIL_CSYMBOLMAP` (used by the matched 0x00B027D0): includes fable_string.h; nested
  `class CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; }`; declarations FindFirstSymbolWithVal 0x009E50C0,
  SymbolExists 0x009E51C0, GetSymbolSLONG 0x009E5120 (FableWin spellings).
- `CIVCountedPointer.h` + `CEnginePrimitiveHandle.h` behind new `FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN` (used only by the parked
  0x00B03CA0 draft): FableWin ??4?$CIVCountedPointer@VCEnginePersistentPrimitiveBase@@@@QAEAAV0@ABV0@@Z (compare, release, store, AddRef)
  and ??4CEnginePrimitiveHandle@@QAEABV0@ABV0@@Z. Retail expands this assignment in CParticle::SetPrimitiveInfo (0x00B04F2C, 0x00B0555A).
- No landed source defines either macro (SIBLINGS.tsv).

## Header debt
- No shared CPSCBase / CPSCRenderMesh / CParticle / CParticleSystem(-with-Particles-list) / CQuaternion / C3DMeshInfo / CMeshDataBank /
  CParticleEmitterManager / CEnginePrimitiveParticleMesh(Group/Line/LineGroup) headers: local models (Ego_r.pdb names, retail offsets).
  CParticle is `#pragma pack(1)` (PositionBuffer at +0x29). The shared CParticleSystem.h keeps Particles / Scale opaque.
- The parked rows use a local C3DVector / CMatrix3x4 / CRightHandedSet (the shared CRightHandedSet has no setter for ToRightHandedSet and
  CMatrix3x4.h has no TransformVector; FableWin has ?TransformVector@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z and the in-place form).
- CRGBColour copy ctor: 0x00B03CA0 needs it `__forceinline` (p4: at the low-frequency mid-colour site VC7.1 calls the copy out of line);
  the parked draft uses a local CRGBColour for that (GOTCHAS line ~700 says the same for by-value CRGBColour).

## Callees (names as used; NAME_FIXES.tsv for the corrected ones)
GFGetInterpolatedColour 0x00A44250; CDataInputStream ReadWithSrcChunkOverflow 0x00993CA0 / ReadColour 0x009945E0; CSymbolMap 0x009E50C0 /
0x009E51C0 / 0x009E5120; CCRC::Calc 0x004014A0; GetParticleEmitterManager 0x00AEAAF0; GetMeshBank 0x00AEAAB0; CMeshDataBank::PeekMeshInfo
0x00A24260 (and 0x00A24270, same body, landed as GetMeshInfo(long) -- no PDB spelling found; the prologue of 0x00B03CA0 reads its
+4 / +8 into CBankFile::GetEntryDataType 0x009CC210 and requires type 4); C3DBoundingSphere::AddToBounds 0x00ADDD80;
GetVectorParam 0x00B059F0; CQuaternion::operator* 0x00A88B60; GetSystemTransform 0x00AF2250; ClearParticle 0x00AF5090;
GetParticleWorldTransform 0x00B05980; ScaleParticleDimensions(float&) 0x00AF8F80; GetInterpolatedLifetimerInSeconds 0x00AF85E0 (its
result is discarded in 0x00B03CA0); GetOneOverTickRate 0x00AEAA00; GetTickRate 0x00AEA9F0 (result unused); CRightHandedSet::GetRight
0x0049CE00 (out of line in 0x00B03CA0, inline in 0x00B01900); CEnginePrimitiveParticleMeshGroup 0x00AFEAD0; CEnginePrimitiveParticleMesh
0x00B058E0; CEnginePrimitiveParticleLine 0x00AFE220; vector resizes 0x00AFE910 / 0x004428F0 / 0x00443F90; vector constructor iterator
0x004038C0 with CRGBColour() 0x004048E0; CPerlinNoise::GetGaussian2DNoise 0x00F36540; CIEngine slots 26 AddChildPrimitive, 27 / 28
SetChildPrimitive.

## Parked rows
- 0x00B01900 AddToBoundingSphere, best b16 (dl 442, 3755v3777): frame 0x298 v 0x2B0 (slot numbering throughout), a loop-head alignment
  pad, the Z row of the two trail-end TransformPoint expansions (retail E23*yy, E13*xx, E33*zz while the first expansion is x, y, z),
  the trail magnitude load order (retail Z, Y, X; every source order of GetSquaredMagnitude gives X first here), and the !CentredOnPos
  matrix block. Next idea: the PreparePrimitives p8 shape (per-member negated matrix, TransformVector + `pos +=`), then the trail end.
- 0x00B03CA0 PreparePrimitives, best p9 (struct-diff 272 lines, dl 1060, 6601v6468): frame 0x44C v 0x3C0; the CArray<long>()
  temporary passed to PrimIDList.resize keeps its inline destructor test + free (retail has none, and no EH frame); the inline
  CIVCountedPointeeBase::Release gives `mov/dec/mov/test` where retail has `dec [ecx+4]; jne`; colour arrays 4 bytes off. Next idea:
  find the resize form without a destructed temporary (STLport's resize(n) default argument evaluated in an out-of-line resize?), a
  Release written as `--IVRefCount` at the call, then the frame.

## Helper calls (every one is a bare `fistp dword`, on the proven-impossible list: `(long)x` -> __ftol2, /QIfist -> fistp qword)
- engine/GFFloatToLongNear.h: 0x00AFF1B0 (every GFInterpolate rounding, first 0x00AFF22D); 0x00B027D0 (GFRoundFloatNear and PackParam,
  first 0x00B032BD); parked 0x00B01900 (GFRoundFloatNear, GFFastArcCos, GFFastSin, first 0x00B01956) and 0x00B03CA0 (GFRoundFloatNear,
  the colour index, GFSmoothFadeValue, GFFastArcCos / GFFastSin, the alpha byte; first 0x00B03D26).
- engine/GFFastFloatToLong.h: parked 0x00B01900, CParticle::IsActive(float) (0x00B01C4D..0x00B01C65, the pointer-out fistp).
- engine/GFFloatToLongCeil.h: CParticle::GetRecordedPos in parked 0x00B01900 (0x00B01DAA..0x00B01DD0, 0x00B01F9D..0x00B01FBD) and
  0x00B03CA0 (0x00B0503C..0x00B05048, 0x00B051A4..0x00B051B0).
No `__asm` / naked / _emit / volatile in any row source or test; ReadBinary uses no helper.

## GOTCHAS one-liners
- CPSCRenderMesh packed floats: unpack `(float)packed * (1.0f / steps)` then `* (max - min) + min` rounded with GFRoundFloatNear(v, 2);
  pack as two statements `n = (GFRoundFloatNear(v,2) - min) * (1.0f/(max-min)); GFFloatToLongNear(n * steps)` (one expression folds).
- DatabaseSerialise float blocks match only as `{ float value = Unpack(...); TransferParam(context, name, value, def); member = Pack(value); }`
  with a by-value default; AlphaFadeMinimum needs the direct `GFRoundFloatNear(x * (1.0f / 127.0f), 2)` form (0x00B027D0).
- A CSymbolMap name lookup with an `unsigned`/`long` key must use the implicit `CCharString` conversion of the group literal; an explicit
  temporary pushes the key first (0x00B027D0).
- In a 6 KB function VC7.1 calls the CRGBColour copy ctor out of line at a low-frequency site (`if` inside `if`); `__forceinline` it (0x00B03CA0).
- CParticle::GetPrimID / SetPrimitiveInfo / GetInterpolatedPos must be `__forceinline` in 0x00B03CA0 (plain inline is called out of line).
- `pos -= offset` with a matrix whose rows are assigned per member (`E11 = -right.X` ...) gives retail's negate-then-row order; Set(-a, -b, ...)
  evaluates the arguments right to left (0x00B03CA0 p8).
- Retail's handle store in SetPrimitiveInfo is FableWin's counted ??4 assignment; the implicit CIVCountedPointer copy is a bare pointer move.
- Hoist `const CPerlinNoise& noise = GetParticleEmitterManager().PeekPerlinNoise();` before the noise call: retail calls the manager accessor
  before building the C2DVector argument.
