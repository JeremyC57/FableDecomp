# Chunk 724 round 3 (agent724c) -- notes

Overlay: `hdr/` = round-2 overlay (agent724b) + this round's headers. Grade with `FABLE_HDR_OVERLAY=$D/hdr`.
`hdrcheck2.py hdr` prints default=same for every repo header this round changed (CThing, CThingPhysical, CPersistContext,
CSymbolMap, CTCHeroRangedAttacks, CTCTargetingPlayer, CDecal, CParticleSystem, CIVCountedPointer, CEnginePrimitiveHandle,
CIEngine, fable_user_profile.h). Inherited from round 2 and NOT default=same: NCameraModes_CBase.h (the repo now has the
chunk-725 header, landed after round 2; the round-2 overlay version is a different, larger model -- it must be merged as a
FABLE_RETAIL_NCAMERAMODES_CBASE branch before anything using it lands; this round's additions are already inside that macro)
and CVertexCompressionParams.h (not touched here).

## 0x008771A0 NCameraModes::CTools::GetThingPointIntersectionFromTo -- RELOCATION_MATCH (15 attempts)
Shape: closest-hit scan over a thing vector: squared |to-from| as the initial best, AccurateNormalise'd direction; per live,
physical (type flag 2), filter-passing (flag 8 only when the test has 8) thing: camera mesh (CTCCameraCollision) point
intersection first (normal negated), else the hit-location line test; a thing that cannot come between camera and target is
re-tested from `to + direction * radius * 2.5` to `to - direction * 0.2`. Levers: (1) `(to - from).GetSquaredMagnitude()` as
an unnamed temporary before the `direction` local (246 -> 164); (2) a named `start` local for the retest (-> 124); (3) one `bool
hit` for every path with nested ifs (-> 67); (4) no accept flag: `if (cc || able) { if (retest) copy; } else copy;` (-> 2);
(5) `hit = true` first in the mesh branch (-> RELOCATION_MATCH). Headers: new CTCCameraCollision.h, CTCHitLocations.h,
NCameraModes_CTools.h; CThing.h GetTypeFlags + CThingPhysical.h IsThingAbleToComeBetweenCameraAndObservedThing (retail branches).

## 0x00B1E380 CPSCDecalRenderer::DatabaseSerialise -- DIFFER(3698v3698) dl=11 (17 attempts)
Shape: persist every field with a block-scoped copy (`{ T v = Field; Transfer(name, v, default); Field = v; }`), packed
fixed-point fields round-tripped through GFRoundFloatNear(x * (1/N) * (max-min) + min, 2) and back with GFFloatToLongNear; the
sprite name through the graphic symbol map (save: FindFirstSymbolWithVal, load: SymbolExists/GetSymbolSLONG); legacy FaceMe2D/3D
/ CrossedSprites / RotateAroundCentre / SelfIlluminating / SpriteAlignment bools when SpriteFlags loads as -1. Levers: include
engine/CPersistContext.h directly (fable_persist_context.h's quoted include resolved the repo copy and lost the inline 2-arg
Transfer: 1312 -> 461 after the next lever); implicit `const char*` -> CCharString in FindFirstSymbolWithVal (temp built before
the other args); parenthesised `(v * (1/N)) * (max-min) + min` (keeps two fmuls); block-scoped values (frame 0x54 -> 0xc); one
reused legacy bool + function-scope alignment local (33 -> 11). Residue: the SizeFadeMinimum (+0x28 bits 25..31) store picks
edx for the old word (`shl eax ; and edx,1FFFFFFh ; or eax,edx` v retail `and eax ; shl edx ; or eax,edx`): one byte longer
and shifts the next GFRoundFloatNear loop's alignment padding; small probes all give retail's form, so it is context-driven
(tried unsigned/char casts, named locals, `& 0x7f`). PoolIncreaseSizeEnable merge loads the new byte before [esi+34h].
Next idea: reorder the AnimationTimeSecs / SizeFadeMinimum blocks' local declarations, or route SizeFadeMinimum through a
shared inline setter.

## 0x00879900 NCameraModes::CProjectileWeapon::Update -- DIFFER(3403v3415) dl=295 (11 attempts)
Shape: see the header comment of differ/00879900.cpp. Levers that moved it (structural lines, stack offsets masked: 196 -> 55):
PeekTC of TCI_HIT_LOCATIONS inside the condition (one UsingComponents test, not two); single GetPos copy then `.Z = position.Z`
for the LOS start; a named CThingFilter_IsNot object (retail keeps the observed pointer as the filter in a stack slot); x/y
longs for the 2-cell C2DBoxI; DELTA_NOTIONAL_ZERO squared at run time; left = target + up - side, right = target - up + side,
written inline (unnamed `C3DVector(0,0,1) * radius` / `side * radius`). No effect: named translation temp, named sway float,
interleaved zeroing in the else branch; worse: bounding sphere straight into targetPos (no centre copy). Residue: frame 0x164
v 0x130 (52 extra bytes of locals), the sphere-corner block's memory temps, the zoom block's `distance -= offset` spill,
`HeightParam += LockDeltaHeightParam` operand order. New/changed overlay: NCameraModes_CBase.h (FABLE_RETAIL_NCAMERAMODES_CBASE:
CPhysicsState, SetPosition/SetObservedPos/SetFOV/GetLeftStickVector inline, GetWorld/GetCamTools), NCameraModes_CProjectileWeapon.h
(Update, GetPhiForPosition, GetPhiForTargetedThing, GetHeightParamForTargetedThingWithPhi), NCameraModes_CTools.h (vector
IsLineOfSight), fable_user_profile.h (CameraSensitivity/NewProfile + inline IsCameraFPBowXY/YZInverted, GetCameraSensitivity),
CTCHeroRangedAttacks.h / CTCTargetingPlayer.h retail branches (CTCBase / CTCTargetingBase bases).

## 0x00B1F200 CPSCDecalRenderer::PreparePrimitives -- DIFFER(3178v3195) dl=439 (11 attempts)
Shape: see differ/00b1f200.cpp. The biased-ebp frame (`lea ebp,[esp-64h]`) is what VC7.1 emits by itself for this body (no
flag needed). Levers (structural 589 -> 243): __forceinline cosine-fade table helper; white CRGBColour temporaries inside the
colour ternaries (retail builds them in the dead `handle` arg slot); std::min for the pool alpha / pool size / last-frame
selects (retail's `lea eax,[a] ; jcc ; lea eax,[b] ; mov eax,[eax]`); CIVCountedPointer::operator= + CEnginePrimitiveHandle::
operator= (FableWin ??4) as __forceinline retail branches (plain inline stays out of line in this 3 KB body); a forceinline
GetFirstPrimID-style test (outer size, inner `size() > 0`, `setge` on the kept id); group descs as temporaries (both share
[ebp-90h]); bank entry stride 0x2C; GFFastFloatToLong for GraphicFrame (pointer-out fistp); GFFloatToLongFloor for the frame;
unsigned colour position with a named index. Worse: CIVCountedPointer::Release with the null store inside the if; reading
decal->MaxLife in each division. Residue listed in the Grade line. New overlay: CDecal.h (FABLE_RETAIL_CDECAL, typed
NParticleEngine::CDecal), CParticleSystem.h (FABLE_RETAIL_CPARTICLESYSTEM: typed lists + GetDecalList/GetGlobalAlpha/
GetFadeAlpha/ClearDecal/ScaleParticleDimensions), CIEngine.h slots 23..30 (FableWin signatures), CIVCountedPointer.h /
CEnginePrimitiveHandle.h operator= branches. Local models (no header exists): CEnginePrimitive, CEnginePrimitiveParticleDecal
(landed 0x00B1FE80 layout), CEnginePrimitiveParticleDecalGroup, CEnginePrimitivePrepareInfo (Ego_r Engine +0x34), the decal
bank object behind 0x013D2E08 (unproven), CPerlinNoise, GFSinTable (0x013CD550, unproven name).

## GOTCHAS (one line each)
- VC7.1 constructs an implicitly converted `const char*` -> class temporary (non-explicit ctor) before evaluating the other call arguments; an explicit `CCharString("x")` argument is built after the later pushes. (0x00B1E380)
- `x * c1 * c2` folds to one constant multiply; `(x * c1) * c2` keeps two fmuls (constant pool entries 1/N and max-min). (0x00B1E380)
- A quoted `#include "engine/X.h"` inside a repo header resolves next to that header first, so it silently bypasses the FABLE_HDR_OVERLAY copy; include the overlay header directly from the row. (0x00B1E380)
- An accept flag (`bool accept = ...; if (accept) copy`) and `if (a || b) { if (retest) copy; } else copy;` compile differently; retail matched the latter. (0x008771A0)
- In a ~3 KB body VC7.1's inline budget leaves plain-inline helpers (handle Clear/operator=, small static helpers) out of line; retail inlined them -> `__forceinline` in a retail header branch. (0x00B1F200)
- MSVC lays an overload set of virtuals out in reverse declaration order: declare CIEngine's 4-arg SetChildPrimitive before the 5-arg one to get slots 28 / 27. (0x00B1F200)
- `extern const float DELTA_NOTIONAL_ZERO` is not folded: `DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO` stays `fld ; fmul` at run time (a literal 0.0001f*0.0001f folds). (0x00879900)
