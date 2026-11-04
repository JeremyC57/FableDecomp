# Chunk 741 -- NParticleEngine::CPSCEmitterGeneric rows + CPSCUpdateNormal::Update (retry)

## Results
| Address | Row | Grade | Attempts |
|---|---|---|---|
| 0x00B149D0 | CPSCEmitterGeneric::ReadBinary (slot 15) | RELOCATION_MATCH, vland PASS (src/) | 19 |
| 0x00B15BD0 | NParticleEngine::GFInterpolateArray<C3DVector> (catalog "InterpolateAnimationSequence") | RELOCATION_MATCH, vland PASS (src/) | 5 |
| 0x00B13530 | CPSCEmitterGeneric::DatabaseSerialise (slot 17) | DIFFER(5268v5268), 16 masked lines (differ/) | 11 |
| 0x00B10B10 | CPSCEmitterGeneric::GenerateParticle (private) | DIFFER(4380v4276), 794 masked lines (differ/) | 15 |
| 0x00B0DC20 | CPSCUpdateNormal::Update (slot 9, retry) | DIFFER(5601v5602), 418 masked lines (differ/) | 10 |

vland (read-only, FABLE_HDR_OVERLAY=mirror): WINS 2, both RELOCATION_MATCH + behaviour PASS (vland_result.txt). All five tests pass under g++
(test/ for the matches, differ/ for the parked rows).

## Slot / identity evidence
- slot 15 0x00B149D0 = ReadBinary: FableWin ?ReadBinary@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; vtable 0x012A002C slot 15;
  family-wide slot 15 (chunk 737); reads the fields in the landed WriteBinary 0x00B12EF0 order.
- slot 17 0x00B13530 = DatabaseSerialise: FableWin ?DatabaseSerialise@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z,
  `ret 8`, slot 17 on the landed CPSCSpline / CPSCAttractor.
- slot 9 0x00B0DC20 = CPSCUpdateNormal::Update: FableWin ?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z,
  `ret 0Ch`, stores false through the bool& when the system life runs out (as chunk 739 recorded).
- 0x00B10B10 GenerateParticle: not a vtable slot; FableWin private ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z.
- 0x00B15BD0 is a free template, not a member: Ego_r.pdb and FableWin.pdb both carry
  ??$GFInterpolateArray@VC3DVector@@@NParticleEngine@@YI?AV?$CArray@VC3DVector@@@@ABV1@0MABVC3DVector@@@Z (__fastcall, hidden return); the catalog name is wrong.

## Header overlay (hdr/), hdrcheck2 (run on $D/hdr)
```
CHANGED engine/C3DVector.h default=same[36] FABLE_RETAIL_C3DVECTOR=same[48] FABLE_RETAIL_C3DVECTOR_EMITTER=same[36]
CHANGED stlport/fable_containers.h default=same[350] FABLE_RETAIL_STLP_LIST=same[357] FABLE_RETAIL_STLP_VECTOR=same[372] FABLE_RETAIL_STLP_VECTOR_ASSIGN=same[351] FABLE_RETAIL_STLP_VECTOR_C=DIFF[350]
```
- stlport/fable_containers.h, new macro FABLE_RETAIL_STLP_VECTOR_C: STLport 4.6 _vector.c operator= body made visible (plus capacity() and
  _M_allocate_and_copy). With the body in the TU VC7.1 knows the out-of-line copy does not capture the local array and keeps `_M_start`
  in a register (retail 0x00B15840 `mov ebx,[esp+14h]` ... 0x00B1587C); declaration-only it reloads (3784v3779). Used by the matched
  0x00B149D0 and the parked 0x00B13530. No landed source defines it, so no sibling recheck is needed.
- engine/C3DVector.h, new macro FABLE_RETAIL_C3DVECTOR_EMITTER (nested in FABLE_RETAIL_C3DVECTOR, so that branch stays byte-same):
  static GetRandomPointWithinUnitSphere (FableWin ?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z, 0x00A146C0) and inline Clear()
  (?Clear@C3DVector@@QAEXXZ, stores Z, Y, X). Used only by the parked 0x00B10B10.
- SIBLINGS.tsv: none (no landed source defines either new macro; every pre-existing macro is `same`).

## Header debt
- No engine header models CPSCBase / CPSCEmitterGeneric / CPSCUpdateNormal / CParticle / CDecal (the generated CDecal.h is a flat
  struct) / CQuaternion / CRandomAcceleration / CAEnvironmentInteraction / CParticleEmitterManager: the rows declare local models laid
  out from the landed rows and the Ego_r.pdb TPI (STLport sizes).
- engine/CParticleSystem.h keeps Particles / Decals as opaque bytes and its members protected; the parked Update / GenerateParticle use
  local CParticleSystem stand-ins (list members, DeleteParticle, AllocateDecal, TransformVectorIfLocal etc.).
- engine/CIntelligentPointer.h defines CIntelligentPointer(T*) inline; retail calls it out of line (0x00655318) in Update.
- The shared STLport list has no erase(); retail's erase frees the node unconditionally (no null test before free).
- engine/CMatrix3x4.h lacks TransformVector (FableWin has both TransformVector(C3DVector&) and TransformVector(const C3DVector&, C3DVector&),
  whose row orders differ -- Update needs both), SetFromVectorAndAngle and Set(const CRightHandedSet&, const C3DVector&).
- CRightHandedSet has no two-vector constructor in the header (retail out of line at 0x00487050).
- The rows declare CCharString locally where fable_string.h is not needed (0x00B13530).

## Callee names (reloc bindings / PDB name sets; fixes in NAME_FIXES.tsv)
vector<C3DVector>(n) 0x007C2AB0, vector<C3DVector>::operator= 0x0075B8D0, CCountedPointer<CCardinalSpline>::Reset 0x0075FA30 / DeleteData 0x0075F830,
CDataInputStream::ReadEBOOL / ReadFloat / ReadULONG / Read3DVector (landed spellings), operator+(CCharString, CCharString) 0x0099F570,
GFIntToCharString 0x0099F830, CCRC::Calc 0x004014A0, GetRandomSeed 0x00AEAAD0, C2DVector::GetRandomUnitVector 0x00A13EB0,
C3DVector::GetRandomUnitVector 0x00A145C0 / GetRandomPointWithinUnitSphere 0x00A146C0 / AccurateNormalise 0x00A14440,
CCardinalSpline::GetSplinePosition 0x00A567A0 (PDB ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z), GFFloatRandom 0x004978A0,
GetRandomRadius 0x00B102C0, ChangeOrientation 0x00B10360, CVectorMap::LowerBound 0x00B05890, CParticleSystem::GetSystemTransform 0x00AF2250 /
GetInverseSystemTransform 0x00AF22C0 / GetComponentOfType 0x00AF4360 / DeleteParticle 0x00AF5330 / AllocateDecal 0x00AF5380 /
PeekEmitterVelocity 0x00AF3DB0, GetOneOverTickRate 0x00AEAA00, GetEnvironmentInteraction 0x00AEAB10, GetParticleEmitterDatabase 0x00AEAAE0,
GetParticleEmitterManager 0x00AEAAF0, GetTickRate 0x00AEA9F0, CPSCUpdateNormal::GetCurrentFadeValue 0x00B0A750, CMatrix3x4::SetFromVectorAndAngle
0x00A55F90, CParticleUpdateParameters::GetVectorParam 0x00B059F0, CRandomAcceleration::operator new 0x00AF8680 / ctor 0x00B0A210,
NFractal::CPerlinNoise::GetGaussian2DNoise 0x00F36540, CRightHandedSet::MakePerpendicular 0x00A22E10 / ctor(up, forward) 0x00487050,
GFROR13 0x00497890, CParticleEmitterDatabase::GetEmitterTemplateHandleFromName 0x00AE85B0, CParticleEmitterManager::CreateDecalEmitter 0x00AEABE0,
CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318, list<CIntelligentPointer<CParticleEmitter>>::push_back 0x00B0F920,
~CBaseIntelligentPointer 0x00A01C10, GetPItem 0x00A01B50, CQuaternion::SetFromAxisAngle 0x00A88D10 / operator*= 0x00A88C10 /
SetFromRotationMatrix 0x00A88DE0, CParticleEmitter::Update 0x00AEE9F0.
CAEnvironmentInteraction vtable as used by Update: +4 IsParticleCollidingAt(pos, velocity, bool, C3DVector&, C3DVector&, bool&), +8 IsWindActive,
+0xC GetWindVelocity, +0x10 GetGravityVector (GravityFactor +0x24 pairs with +0x10, WindFactor +0x20 with +8 / +0xC; chunk 739's note had the
two vector slots the other way round).

## Helper calls (asm only through shared headers; none in any row source or test)
- 0x00B15BD0: GFFloatToLongNear (engine/GFFloatToLongNear.h) for the element count -- retail 0x00B15C37 / 3B / 3F `fstp; fld; fistp dword`,
  a bare dword fistp that VC7.1 cannot emit from C++ (`(long)x` -> __ftol2, /QIfist -> fistp qword). Source carries the `// Retail-asm:` line.
- 0x00B13530 / 0x00B10B10 (parked): GFFloatToLongNear inside RoundToDecimals and the encodes, same reason (as the landed CPSCEmitterGeneric
  InterpolateTo / WriteBinary rows).
- 0x00B149D0, 0x00B0DC20: none.

## Parked residues and next ideas
- 0x00B13530 DIFFER(5268v5268), 16 lines: the bool-default byte stores of UseEmitterTimelineSecs and OrientationXZ are scheduled one
  Transfer early in retail; NonUniformScalingY's top-field store swaps the OR operands (retail `and edx,1FFFFFh` 6 bytes v our 5, one
  alignment nop); the OppositeDirection `and ecx,7FFFFFFFh` placement. Tried: bool macro variants (!=0, ?:, named const default;
  `?1:0` worse), Y cast / local variants. Next: a named `const bool defaultValue = false` before the previous field for the two early
  bools; the Y encode through a separate unsigned long local.
- 0x00B10B10 DIFFER(4380v4276), 794 lines: retail converts word / byte bit-field decodes with the unsigned-long fixup (`test; jge;
  fadd 2^32`), which no bit-field spelling produced (casts, inline / reference params, templates, struct ctor, array local); no `push ebx`;
  frame 0x11c v ours larger; IsNonZero spelled with `fld st0; fadd st2; fadd st3` + fcompp (seven IsNonZero spellings tried; the
  `s = X*X; s += Y*Y; s += Z*Z; return eps*eps < s` form is closest). Next: decode each field through an accessor that takes / returns
  a plain `unsigned long` so VC loses the bit-field range; then the frame.
- 0x00B0DC20 DIFFER(5601v5602), 418 lines (102 structural): frame 0x2d0 v 0x2dc (one 12-byte temporary missing), x87 operand order in
  the two perpendicular cross products, the CreationTime fraction kept in a register (retail fstp + memory fmul), TransformPosIfLocal
  Z row order, a few copy schedules. The two perpendicular inlines differ in retail (`<=` in the decal one, `<` in the orientation one),
  and the two TransformVector uses have different Z-row orders (matching FableWin's two TransformVector overloads). Next: find the 12-byte
  temporary (gravity / wind products as named locals, ToMatrix through a temporary matrix), and model the perpendicular as a
  CRightHandedSet / C3DVector member with an explicit reference vector.

## GOTCHAS one-liners
- (chunk 741) q.py grades in $S/wine/q/<addr>: two grades of the SAME address in parallel overwrite each other and report identical
  (wrong) results -- grade one address at a time (my p1..p5 / f,g,h parallel runs were discarded and rerun).
- (chunk 741) Making STLport's _vector.c operator= body visible in the TU (FABLE_RETAIL_STLP_VECTOR_C) lets VC7.1 keep a local vector's
  `_M_start` in a register across `x = local`; with the declaration only it reloads it after the call.
- (chunk 741) VC folds `(x * (1/max)) * range` into one constant multiply; retail's two multiplies need `float unit = x * (1/max);` as its
  own statement before `unit * range`.
- (chunk 741) A bash heredoc with unquoted EOF runs backticked asm in comments as commands (`mov: command not found`) and strips them:
  write sources with the Write tool or `<<'EOF'`.
- (chunk 741) vland.py needs `source $S/wine/env.sh` in the same shell: without it verify_and_land reads the Windows default
  `D:\Documents\FableTLC/rebuild/manifest/functions.tsv` and dies with FileNotFoundError.
- (chunk 741) FableWin.pdb names a TU's inline helpers right after the function in debug-address order (0x02F3F4C0 Update is followed by
  GetEnableDecals, CQuaternion::ToMatrix, the CParticle / CDecal setters, TransformVectorIfLocal ...): list that range before modelling a big row.
- (chunk 741) VC7.1 tests (vland compiles them with cl) have no nearbyintf / lrint: use a hand round-half-even helper.

## Process
No git, no repo writes, no --land. Processes: only my own cl / vland runs. After the container restart, the chunk directory was intact;
vland was rerun with the environment sourced.
