# Chunk 738 -- NParticleEngine::CPSCRenderSprite (InterpolateTo, ReadBinary, slots 19 / 17 / 13)

Mirror: frozen repo `rebuild/include` (MIRROR_HEAD.txt) in `base_include/` + the `hdr/` overlay = `mirror/` (`bash mkmirror.sh`).

## Results
| address | row | grade | attempts |
|---|---|---|---|
| 0x00AF9840 | CPSCRenderSprite::InterpolateTo | RELOCATION_MATCH (retail_asm) | 2 |
| 0x00AFA2B0 | CPSCRenderSprite::ReadBinary | RELOCATION_MATCH | 1 |
| 0x00AFAE50 | CPSCRenderSprite::AddToBoundingSphere ("vfunc_19") | DIFFER(2406v2405), parked | 10 |
| 0x00AFBCE0 | CPSCRenderSprite::DatabaseSerialise ("vfunc_17") | RELOCATION_MATCH (retail_asm) | 6 |
| 0x00AFCC30 | CPSCRenderSprite::PreparePrimitives ("vfunc_13") | DIFFER(5338v5274), parked | 10 |

vland (FABLE_HDR_OVERLAY=mirror, LANDVERIFY_WORK=vwork): 3 wins, all behaviour PASS, no purity / shared-header warnings
(`vland_result.txt`). The behaviour tests are built the vland way (VC7.1 `cl /Od /W3`, link, run under Wine: `vt.sh`) and also pass
under g++. The first vland run gave TCC_FAIL for two tests (C99 `std::nearbyint`, `map::at`, absent from VC7.1); fixed.

## Slot names (evidence)
FableWin.pdb name set for CPSCRenderSprite (every slot name below is in it) + `ret N` + the landed same-slot siblings:
- slot 13 PreparePrimitives: chunk 737's proof (CPSCSingleSprite / CPSCLight slot 13), `ret 14h`, same five-argument signature.
- slot 17 DatabaseSerialise: landed slot 17 of CPSCAttractor 0x00B18110 and CPSCSpline 0x00B1B2E0, `ret 8`, reads CPersistContext
  Mode and calls Transfer<T> with retail's field names.
- slot 19 AddToBoundingSphere: CPSCBase 0x00AF2A30 and CPSCLight 0x00B18AF0 slot 19, `ret 10h`, returns bool, calls
  C3DBoundingSphere::AddToBounds.
- slot 14 InterpolateTo / slot 15 ReadBinary / slot 16 WriteBinary (0x00AF9620, landed) as the 737 siblings.
- slot 18 DatabaseCopy: the landed 0x00AF9410 already defines DatabaseCopy but is catalogued "vfunc_18" (NAME_FIXES.tsv).

## Header overlay (hdr/engine/), hdrcheck2: every changed header default=same
- CSymbolMap.h (FABLE_RETAIL_CSYMBOLMAP): CSymbolInfo {GroupName, EntryName, Val}, const FindFirstSymbolWithVal 0x009E50C0,
  GetSymbolSLONG 0x009E5120, SymbolExists 0x009E51C0. Used by matched 0x00AFBCE0.
- GFPackFloat.h (NEW): __forceinline GFRoundFloatNear / GFPackFloat / GFUnpackFloat (FableWin names; out-of-line copies 0x00AF8D50,
  0x00AF8DC0). Compiled out of line, GFPackFloat grades RELOCATION_MATCH against the landed 0x00AF8D50 and GFUnpackFloat is
  instruction-identical to 0x00AF8DC0. Used by matched 0x00AFBCE0 and both parked drafts. No asm of its own.
- CIVCountedPointer.h (FABLE_RETAIL_CIVCOUNTEDPOINTER): operator= (FableWin ??4?$CIVCountedPointer@VCEnginePersistentPrimitiveBase@@..).
- CEnginePrimitiveHandle.h (FABLE_RETAIL_CENGINEPRIMITIVEHANDLE): __forceinline operator= = Clear() + counted-pointer assignment
  (FableWin ??4CEnginePrimitiveHandle@@QAEABV0@ABV0@@Z). __forceinline because the plain inline one was called out of line in the
  5 KB row; retail inlines it at all three sites.
- C3DBoundingSphere.h (FABLE_RETAIL_C3DBOUNDINGSPHERE): AddToBounds(const C3DVector&, float) declaration (0x00ADDD80).
The last three are reached only by the parked drafts: the coordinator may drop them if those rows stay parked.
hdrcheck2 output: `CHANGED engine/C3DBoundingSphere.h default=same`, `CHANGED engine/CEnginePrimitiveHandle.h default=same`,
`CHANGED engine/CIVCountedPointer.h default=same`, `CHANGED engine/CSymbolMap.h default=same`, `NEW engine/GFPackFloat.h`.
No landed source defines any of the four macros (SIBLINGS.tsv), so there were no landed includers to recheck.

## Header debt (local models in the sources)
- CPSCBase / CPSCRenderSprite: no shared header. Retail layout = Ego_r.pdb offsets (0x54 bytes; no STL containers in the class):
  SpriteBankIndex +0x0C, TrailBankIndex +0x10, Start/Mid/EndColour +0x14..+0x1C, unsigned long bit-fields +0x20..+0x34 in FableWin
  order, the four one-bit flags in the byte +0x38, PrimHandle +0x3C, ChildPrimIndex +0x44, TrailPrimHandle +0x48,
  ChildTrailPrimIndex +0x50. Worth a generated header (all five rows and the landed ctor / WriteBinary / DatabaseCopy share it).
- CParticle (pack(1)): Angle +0x24, AngleChange (schar) +0x28, PositionBuffer[4] +0x29, Lifetimer +0x72, CreationTime +0x74,
  MaxLife +0x76, FlickerIndex +0x78, FlickerTimeOffset +0x79, Active +0x7A, PrimIDList CArray<CArray<long> > +0x84,
  ParentHandleList CArray<CEnginePrimitiveHandle> +0x90 (Ego_r +0x94: CArray is 12 bytes in retail). Inline methods used:
  IsActive(), IsActive(float), GetInterpolatedLifetimer, GetInterpolatedPos, GetRecordedPos (landed 0x00AFE380), GetAngle,
  GetAngleChange (x 2/127), GetFlickerIndex / TimeOffset, GetMaxLife, GetPrimID(long, long, long&), SetPrimitiveInfo.
- CParticleSystem: the shared header's retail branch has no particle list / ClearParticle; the drafts model Particles
  (std::list +0x1C), the +0x50 flag byte, GlobalAlpha +0x54, FadeAlpha +0x58, NormalisedTime +0x5C locally.
- CGraphicDataBank / CGraphicInfo / CGraphicInfoFrame (as chunk 737), CParticleEmitterManager (+8 PerlinNoise; GetDefaultBlendMode 2,
  GetDefaultBlendOp 0 inline), NFractal::CPerlinNoise, CEnginePrimitiveParticleSprite / Line / SpriteGroup / SpriteTrailGroup
  (Ego_r layouts; group types 0x1B / 0x1A, ctor (bank, flags, blend mode, blend op) from FableWin).
- The parked drafts now use the shared CMatrix3x4.h (FABLE_RETAIL_CMATRIX3X4 X form) and C3DBoundingSphere.h (same bytes).

## Callee names used (one address, one name)
GFGetInterpolatedColour 0x00A44250; CDataInputStream::ReadColour 0x009945E0 (+ chunk reader 0x00993CA0); CPersistContext::Transfer
<bool> 0x004045C0 / <float> 0x00410620 / <CCharString> 0x004109A0 / <long> 0x00410BE0 / <CRGBColour> 0x006420F0; CCharString
0x0099E4B0 / 0x0099EBF0 / 0x0099EC30 / 0x0099EAE0 / 0x0099EFB0; CSymbolMap 0x009E50C0 / 0x009E5120 / 0x009E51C0;
NParticleEngine::GetTickRate 0x00AEA9F0, GetOneOverTickRate 0x00AEAA00, GetGraphicBank 0x00AEAAA0, GetParticleEmitterManager 0x00AEAAF0;
CGraphicDataBank::GetNoFramesInGraphic slot 20 (0x00A013E0); C3DBoundingSphere::AddToBounds 0x00ADDD80; CParticleSystem::
GetSystemTransform 0x00AF2250, ClearParticle 0x00AF5090, ScaleParticleDimensions C2DVector& 0x00AF8F00 / float& 0x00AF8F80;
NFractal::CPerlinNoise::Get2DNoise 0x00F36540; CEnginePrimitiveParticleLine ctor 0x00AFE220, CEnginePrimitiveParticleSprite ctor
0x00AFE2A0; vector<CArray<long> >::resize 0x00AFE910, vector<long>::resize 0x004428F0, vector<CEnginePrimitiveHandle>::resize(n)
0x00443F90; CIEngine slots 26 AddChildPrimitive / 27 SetChildPrimitive (5 args) / 28 SetChildPrimitive (4 args); this->slot 29
ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&); __ftol2; _CIfmod. Wrong landed spellings: NAME_FIXES.tsv.

## Helper calls (each one: why it is unavoidable)
Every helper is an existing shared header with an `__asm` path and a FABLE_PORTABLE C++ twin; no row source or test has asm.
- GFFloatToLongNear (engine/GFFloatToLongNear.h): 0x00AF9840 GFInterpolate rounding (retail 0x00AF99B4 `fistp dword ptr [esp+14h]`),
  0x00AFBCE0 GFRoundFloatNear / GFPackFloat rounding (0x00AFC5B0 `fistp dword ptr [esp+1Ch]`), both parked drafts (unpack rounding,
  colour-ramp index, alpha, GFSmoothFadeValue). Reason: a bare `fistp dword` into a frame word; VC7.1 C++ `(long)x` is a __ftol2 call
  and /QIfist emits `fistp qword` (GOTCHAS impossible list).
- GFFastFloatToLong (engine/GFFastFloatToLong.h), parked drafts only: CParticle::IsActive(float) (0x00AFD424 `fistp dword ptr [edx]`,
  through a pointer). Same reason.
- GFFloatToLongCeil / GFFloatToLongFloor (engine/GFFloatToLongCeil.h / Floor.h), parked drafts only: GetRecordedPos's index
  (0x00AFDCE5) and the animation frame (0x00AFD846): `fadd/fsub half ; fistp dword ; fild ; fadd/fsub one ; integer compare` blocks.
  Same reason (bare dword fistp; the integer compare of the float bits is part of the asm block).
Everything else is C++: the `(unsigned char)(x * fadeAlpha)` alphas are retail's __ftol2 calls, fmod is _CIfmod, fabs is the intrinsic.

## Parked residues and next ideas
- 0x00AFAE50 AddToBoundingSphere, DIFFER(2406v2405), 10 grades: frame (0x1C8, ebp-0x68) and instruction stream match; registers are
  rotated by one around the frame-graphic aspect (the two movzx), the IsActive(float) lea (eax v ecx), the pos copy and the `this`
  reload; the first (pos) TransformPoint has retail's X row as E11*x, E31*z, E21*y and Z row E33*z, E13*x, E23*y while the tail
  transforms keep the shared X form (the same "two contexts" note as CMatrix3x4.h / parked 0x00B087C0). Next: a two-argument
  TransformPoint(in, out) or a ctor-temp pos for the first transform only.
- 0x00AFCC30 PreparePrimitives, DIFFER(5338v5274), 10 grades: the code lines up with retail's (calls, x87 order, inline
  GetPrimID / SetPrimitiveInfo / handle assignment). Residue: (1) retail has no destructor for the three default `CArray<long>()`
  temporaries passed by const& to resize 0x00AFE910 (ours: `mov eax,[tmp] ; test ; je ; push ; call` x3 = +57 bytes); a named
  const local and the one-argument resize keep it; removing the temporary entirely (a global) did not realign the frame either.
  (2) frame 0x334 v 0x328 with a different slot permutation of the same code (e.g. particle [ebp+38h] v [ebp+2Ch]); declaration
  placement of colours / pos / tail / particle / lifetimer changes nothing. (3) blend-field register order and the zero register
  of the single-sprite SetPrimitiveInfo. Next: find the TU or build reason for (1) first; (2) likely follows from it.
- The AlphaFadeMinimum / SizeFadeMinimum unpack: 0x00AFBCE0 matches only with `GFRoundFloatNear(x / 127.0f, 2)`, while the parked
  0x00AFCC30 is closer with GFUnpackFloat(x, 7, 0, 1, 2) (dl 595 -> 585); one TU, so one of the two is still unproven.

## GOTCHAS one-liners
- A packed-float unpack `x / (2^n - 1) * (max - min) + min` written as one expression folds the two constants into one reciprocal
  multiply under VC7.1; retail keeps `fmul 1/2047 ; fmul 20` apart: put the normalised value in its own statement (GFPackFloat.h).
- Behaviour tests are compiled by vland with VC7.1 `cl /Od /W3` (not g++): no `std::nearbyint`, no `std::map::at`; round half
  to even by hand.
- A `GFGetInterpolatedColour(...)` result that retail copies through `mov edx,[eax]` into a frame word is an assignment to a
  colour declared outside the loop, not a loop-scope initialisation (which receives the result in place).
- In a 5 KB body VC7.1 left the in-class CEnginePrimitiveHandle::operator= out of line; retail inlined all three: __forceinline.
- 0x00AEAA00 is NParticleEngine::GetOneOverTickRate (FableWin order SetTickRate, GetTickRate, GetOneOverTickRate); several landed
  binders spell it GetParticleUpdatePeriod / GetParticleFrameTime / GetParticleClock.
