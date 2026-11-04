# Chunk 739 -- NParticleEngine::CPSCUpdateNormal (InterpolateTo, ReadBinary, slots 12 / 17 / 9)

Mirror: frozen repo headers (MIRROR_HEAD.txt, base_include/ unchanged) + the hdr/ overlay = mirror/ (rebuild with `S=... bash mkmirror.sh`).

## Results
| address | row | grade | attempts |
|---|---|---|---|
| 0x00B0A920 | CPSCUpdateNormal::InterpolateTo (slot 14) | RELOCATION_MATCH (retail_asm helper only), vland PASS | 3 |
| 0x00B0B860 | CPSCUpdateNormal::ReadBinary (slot 15) | RELOCATION_MATCH, vland PASS | 2 |
| 0x00B0CAD0 | CPSCUpdateNormal::UpdateAddParticle (slot 12) | DIFFER(2191v2191), parked, differ/ | 24 |
| 0x00B0D360 | CPSCUpdateNormal::DatabaseSerialise (slot 17) | DIFFER(2242v2240), 2 diff lines, parked, differ/ | 19 |
| 0x00B0DC20 | CPSCUpdateNormal::Update (slot 9) | not drafted, parked | 0 |

vland (read-only, FABLE_HDR_OVERLAY=mirror): 2 wins, both behaviour PASS (vland_result.txt). Tests: test/ (matched), differ/ (parked) -- all pass.

## Slot names (evidence)
- slot 12 0x00B0CAD0 = UpdateAddParticle: FableWin.pdb ?UpdateAddParticle@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z;
  `ret 24h` = system&, params&, two by-value C3DVectors, float; body allocates and initialises a particle (CPSCBase slot 12 0x00AF2A00 is the empty hook).
- slot 17 0x00B0D360 = DatabaseSerialise: FableWin name; `ret 8`; slot 17 is DatabaseSerialise on the landed CPSCSpline 0x00B1B2E0 / CPSCAttractor 0x00B18110;
  body = CPersistContext::Transfer of every member by its name literal.
- slot 9 0x00B0DC20 = Update(CParticleSystem&, const CParticleUpdateParameters<K,V>&, bool&): FableWin name; `ret 0Ch`; slot 9 is Update on CPSCBase 0x00AF29D0 /
  CPSCAttractor 0x00B179A0 (landed, same signature); the body stores false through the bool& when the system life runs out.
- slots 14 / 15 as chunk 737 (InterpolateTo / ReadBinary, family-wide).

## Header overlay (hdr/engine/), hdrcheck2: every changed header default=same
```
CHANGED engine/C3DVector.h default=same[36] FABLE_RETAIL_C3DVECTOR=DIFF[44]
CHANGED engine/CDataInputStream.h default=same[30] FABLE_RETAIL_CDATAINPUTSTREAM=DIFF[78]
CHANGED engine/CParticleSystem.h default=same[91] FABLE_RETAIL_CMATRIX3X4=same[91] FABLE_RETAIL_CPARTICLESYSTEM=DIFF[165]
CHANGED engine/CRightHandedSet.h default=same[37] FABLE_RETAIL_CRIGHTHANDEDSET=DIFF[40]
```
- CDataInputStream.h (FABLE_RETAIL_CDATAINPUTSTREAM): declaration ?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ (0x009950F0) + `class C3DVector;`. Used by the matched 0x00B0B860.
- C3DVector.h (FABLE_RETAIL_C3DVECTOR): GFFastOneOverSqrt (FableWin ?GFFastOneOverSqrt@@YIMM@Z, the landed 0x007AF0F0 body) + table extern
  GFastOneOverSqrtTable 0x013CE558; static GetRandomUnitVector (0x00A145C0); inline Normalise (?Normalise@C3DVector@@QAEAAV1@XZ). Only the parked 0x00B0CAD0 uses them.
- CRightHandedSet.h (FABLE_RETAIL_CRIGHTHANDEDSET): inline SetForward / SetUp, MakePerpendicular decl (0x00A22E10). Parked 0x00B0CAD0 only.
- CParticleSystem.h (FABLE_RETAIL_CPARTICLESYSTEM): AllocateParticle (0x00AF5010), PeekEmitterVelocity (0x00AF3DB0), `class CParticle;`. Parked 0x00B0CAD0 only.
Siblings: the 18 landed sources that define FABLE_RETAIL_CDATAINPUTSTREAM or FABLE_RETAIL_C3DVECTOR were rechecked with --recheck-landed against the overlay:
18/18 RELOCATION_MATCH + PASS (SIBLINGS.tsv, sib/recheck.txt). No landed source defines FABLE_RETAIL_CPARTICLESYSTEM or FABLE_RETAIL_CRIGHTHANDEDSET.

## Helper calls (asm only through shared headers; none in any row source or test)
- 0x00B0A920: GFFloatToLongNear (engine/GFFloatToLongNear.h) -- retail 0x00B0A982 `fstp [esp+3Ch] ; fld [esp+3Ch] ; fistp dword ptr [esp+10h]` and every later
  unsigned GFInterpolate rounding: a bare dword fistp, proven impossible from VC7.1 C++ (__ftol2 / fistp qword / cvttss2si).
- parked 0x00B0CAD0: GFFastFloatToLong (engine/GFFastFloatToLong.h) in CParticle::SetCreationTime / SetAngleChange (0x00B0CBD0 / 0x00B0D310 `fistp dword ptr [edx]`),
  same reason (the landed out-of-line copies 0x00B0A4A0 / 0x00B0A420 use the same helper).
- 0x00B0B860, 0x00B0D360: none.

## Header debt
- No engine header for CPSCBase / CPSCUpdateNormal / CParticle / CQuaternion: local models as in the landed CPSCUpdateNormal rows (ctor 0x00B0C9C0, DatabaseCopy 0x00B0B370).
- engine/C3DVector.h has 4-byte alignment; retail C3DVector is 1-aligned, so the parked CParticle model wraps its three C3DVector members in pack(1)
  (byte evidence: PositionBuffer +0x29, Velocity +0x59, Acceleration +0x65 with Lifetimer +0x72 still naturally aligned).
- The rows declare CCharString locally (no engine header; fable_string.h's model is pulled only where CParticleSystem.h is included).

## Callee names (all from reloc_bindings / PDB name sets; fixes in NAME_FIXES.tsv)
CCharString(const CCharString&) 0x0099EC30, operator= 0x0099EFB0 (??4CCharString@@QAEABV0@ABV0@@Z), ~CCharString 0x0099EAE0, CCharString() 0x0099E4B0;
CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, Read3DVector 0x009950F0; CPersistContext::Transfer<T> (K 0x004106F0, CCharString 0x004109A0, _N 0x004045C0,
M 0x00410620, C3DVector 0x004859AD, J 0x00410BE0); CCRC::Calc 0x004014A0; GFFloatRandom 0x004978A0; GFROR13 0x00497890 (?GFROR13@@YIJK@Z) inside the inline
GFRandom(range, seed) (PDB ?GFRandom@@YIKKPAK@Z); NParticleEngine::GetTickRate 0x00AEA9F0; GetRandomSeed 0x00AEAAD0 (fix); CParticleSystem::AllocateParticle 0x00AF5010,
PeekEmitterVelocity 0x00AF3DB0 (fix); CQuaternion::SetFromAxisAngle 0x00A88D10 (new), operator* 0x00A88B60, SetFromRotationMatrix 0x00A88DE0;
C3DVector::GetRandomUnitVector 0x00A145C0, AccurateNormalise 0x00A14440; CRightHandedSet::MakePerpendicular 0x00A22E10; CParticleUpdateParameters<K,K>::GetVectorParam 0x00B059F0;
CPSCUpdateNormal::GetCurrentFadeValue 0x00B0A750 (protected const, landed). Descriptive: the local up-vector / cross-product names in 0x00B0CAD0.

## Parked rows
- 0x00B0D360 DatabaseSerialise (best differ/00b0d360.cpp = attempt a): the only residue is the RandomisePosDistVaryEnable (bit 0 of +0x10) merge at 0x00B0DA8E:
  retail `mov cl,[esi+10h] ; xor cl,[esp+28h]`, ours loads the bool first. A function-scope bool (e/f), an outer block spanning the next transfer (j/k), or a
  named function-scope local (l) all give retail's merge but move either the bool or the next float default out of the dead `context` arg slot (frame +4 or
  a 0x14 slot). Tried: != / ?: / !! conversions, separate default local, nested block, inline helper (x, much worse), unsigned char bit-fields (y).
  Next idea: a shape where the bool outlives its store yet the RandomisePosDistVaryScale default still lands in the arg slot (e.g. the float default as a
  block-scope named const, or the Lionhead macro shape from another bit-0 DatabaseSerialise once one lands).
- 0x00B0CAD0 UpdateAddParticle (best differ/00b0cad0.cpp = attempt x, same size 2191): structure, calls and stores all line up; residue (349 masked lines) is
  (1) register allocation -- retail keeps the seed in ebp the whole function, ours puts it in esi and spills it, then CSEs the constant 1 into ecx
  (MaxLife fallback, Active, the StayWithEmitter test); (2) frame slots -- retail places the C3DVector / quaternion temporaries at 0x48 above the orientation
  (0x38) and the forward / qy pair at 0x18, ours the other way round (declaration order does not move them); (3) x87 stack order in forward ^ up.
  Wins along the way: the scalar up vector (upX/upY/upZ floats, kept on the x87 stack as retail), `fabs(X) > fabs(Y)` compare shape, chained
  `X = Y = Z = 0` ClearAcceleration (stores Z, Y, X), inline Activate copying [0] to [1..3] (needed __forceinline in this 2 KB body -- plain inline stays a call).
  Next idea: an inline CParticle initialiser taking the seed / life, or the seed fetched through GetRandomSeed() inside each GFFloatRandom call, to change
  the seed's live range so the allocator picks ebp.
- 0x00B0DC20 Update: not drafted (5602 bytes, ~1300 instructions); the session went to the two closer rows. Decoded head for the next agent:
  frameTime = 0x00AEAA00(); env = 0x00AEAB10() (virtuals +0x8 bool, +0xC / +0x10 C3DVector by hidden pointer = gravity / wind);
  lifeTime = LifeTimer * frameTime; if UseSystemLifeSecs { if !(lifeTime < SystemLifeSecs) { alive = false; return; } t = clamp(lifeTime / SystemLifeSecs, 0, 1)
  when SystemLifeSecs > 0; if SystemAlphaFadeEnable system.FadeAlpha(+0x54) = min + (1-min) * GetCurrentFadeValue() }; system +0x50 bit 3 = UseSystemLifeSecs,
  +0x5C = t (SetNormalisedTime); airFactor = AirResistance > 0 ? pow(1 - AirResistance, frameTime) : 1; force = wind * WindFactor * frameTime
  (+ gravity * GravityFactor * frameTime when env->+0x8()); Emitter2D (+0x40 bit 11 of the parent emitter) rotates force by
  CMatrix3x4::SetFromVectorAndAngle(C3DVector(1,0,0), -0.25f) 0x00A55F90; acceleration = UseAccelerationParam && GetVectorParam(..., AccelerationParam) ?
  param : ParticleAcceleration, times ParticleAccelerationScale; maxLife = (long)(GetTickRate() * ParticleLifeSecs); seed = GetRandomSeed(); then the
  particle-list loop (DeleteParticle 0x00AF5330 when the Lifetimer passes maxLife, CRandomAcceleration 0x00AF8680 / 0x00B0A210 with noise 0x00F36540,
  GetSystemTransform 0x00AF2250, AllocateDecal 0x00AF5380, decal emitters through 0x00AEAAF0 / 0x00AEABE0 / list push_back 0x00B0F920).

## GOTCHAS one-liners
- (chunk 739) A CPSC bit-field InterpolateTo matches with the DecalRenderer family overloads as is: `(unsigned long)Bit` through GFInterpolate(K,K,M); the C3DVector overload must be __forceinline (plain inline is called out of line at 0x00AF3CF0 in a 2.6 KB body).
- (chunk 739) DatabaseSerialise bool bit-fields: per-block `{ bool v = X; Transfer(name, v, false); X = v; }` homes v and the next float default in the dead context slot as retail, but a bit-0 field then loads v before the old byte (retail xors v as a memory operand); every scope change that fixes the merge moves a slot.
- (chunk 739) VC7.1 keeps a C3DVector built from three scalar float locals on the x87 stack across an if/else; the same vector as a C3DVector local (operator^ by reference) goes to memory (0x00B0CAD0 up axis).
- (chunk 739) `X = Y = Z = 0.0f` stores Z, Y, X with one zero register (CParticle::ClearAcceleration in 0x00B0CAD0); Initialise(0,0,0) stores X, Y, Z.
- (chunk 739) mkmirror.sh exits silently when $S is not exported in the same shell: export S before `bash mkmirror.sh` or the mirror keeps the old overlay.

## Process
No git, no repo writes, no --land. Processes: only my own cl/vland runs.
