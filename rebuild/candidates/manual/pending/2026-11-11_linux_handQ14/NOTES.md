# Chunk Q14 notes: 40 parked x87-order near misses

Note: `gall.sh` does not set the overlay; run it as `FABLE_HDR_OVERLAY=$A/hdr bash $A/gall.sh` (without it the match SRC_FAILs on the missing opt-in).

Result: 1 RELOCATION_MATCH (0x00876CA0), 39 parked in `differ/` (38 DIFFER, 1 SRC_FAIL). Grade with
`FABLE_HDR_OVERLAY=$A/hdr` (see hdr/README.md). Grades per row are in STATUS.tsv and ATTEMPTS.log.

## Match
- **0x00876CA0 NCameraModes::CTools::PotentialFieldBufferObjects** (Ego_r.pdb 0x0085D8B0) -- RELOCATION_MATCH on shared headers
  (CThing / CThingGameObject / NCameraModes_CTools + new opt-in FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD). Levers: the landed
  twin 0x00876E10's divide-by-step return (reciprocal multiply, Z first) instead of the draft's named `inv`, and the object position
  copied by value through the header's copy ctor. Callees: GetMeshExtent 0x00875F00, CThing::GetPos 0x004C73D0 (agree with
  reloc_bindings). Globals g_PotentialFieldSampleStep / g_PotentialFieldSampleOffsets[4] are descriptive names (no retail name found).

## Rows worked on shared headers (closest first)
- **0x00B80CE0 CEngineClothSimulationMeshBase::GetDeltaTransform** -- 682v682 (same length). Semantics fixed: delta =
  LastTransform * inverse(CurrentTransform) = `SetInverse(CurrentTransform); PreMultiply(LastTransform)` on HEAD's CMatrix3x4
  (FABLE_RETAIL_CMATRIX3X4 + _PREMULTIPLY). Only PreMultiply's E12/E13 row sums differ (retail a.E12*l.E11, a.E22*l.E12,
  a.E32*l.E13; header gives E12, E32, E22). Compile-only probes: parenthesised `(t3)+(t2)+(t1)` / `(t2)+(t1)+(t3)` give retail's
  E12/E13 load order, but an E12/E13-only reorder (m5) moved the mismatch into rows E21+ (0x00B80E9D on). Next: try the
  parenthesised forms for just those two rows as a FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY_* opt-in (4 grades left), and write
  engine/CEngineClothSimulationMeshBase.h (CurrentTransform +0x18, LastTransform +0x48 in retail; the PDB's +0x1c/+0x4c assume a
  16-byte ParticleSims CArray, retail's is 12; bases CEngineSimulationBase / CParticleContext have no headers yet). Still a local
  class model -> not landable even at a full match until that header exists.
- **0x00987040 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator()** -- 370v374. Return polarity fixed with
  `!RestrictAngle ? true : offset.Normalise().Dot(SideVec) > MinCosine`; remaining: retail keeps the factor alive across the test.
- **0x00A58790 NBBBMaths::CSphere::GetFirstIntersectionWithFiniteLine** -- 699v707 on new engine/CSphere.h. Residues: (1) retail homes
  the squared length in the dead end-argument slot (`fst [esp+2Ch]`) and copies it with integer moves; (2) dir*t operand order
  (dir components' candidate ids 7-9 below t's 16).
- **0x008FA660 NCameraModes::CPhysicsForceModelBase::FrameUpdate** -- 356v366 on engine/NCameraModes_CPhysicsForceModelBase.h:
  retail materialises the first negated force as a temporary and copies it into the sum (no copy elision).
- **0x0066C900 CRegionMinimap::IlluminateRegionMinimapTheme** -- 346v354 on shared headers (local model was 348v354): light copy and
  Z liveness.
- **0x00A0C1B0 CCamera::MoveSideways** -- 158v152: the cross-product free-arm operand order.
- **0x00AC70F0 QuadricOptimiserInternals::CQuadric::GetInverseMatrixPart** -- 229v237: x87 operand order.
- **0x00A0CA90 CWParsedItem::InitAsFloat** -- 172v187 (leaf `?InitAsFloat@CWParsedItem@@QAEXPBGK@Z`): divisor homing.

## Probed, parked (still local models)
- 0x00844740 CCreatureAction_DropGift::PerformAction 342v350 and 0x008EF200 Coin::Coin 223v231: retail does not CSE the repeated
  product (`sin*0.25`, radius squared via GFSqr `?GFSqr@@YIMM@Z` per FableWin); every spelling tried is CSE'd.
- 0x0083B6B0 family (0x0083B6B0 308v309, 0x0083DC80 304v309, 0x00837B30 309v314): HEAD's CTCPhysicsBase interpolated
  GetPosition(float) (FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY) gives the t-in-register shape but regressed to
  296v309 on the local model; these need a full port to CThingPhysical / CEnginePrimitiveStaticMesh / CIEngine headers.
- 0x0089CE70 CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay: SRC_FAIL here -- the draft needs a prior agent's
  FABLE_RETAIL_CTCPERCEIVETHING_SENSES overlay (earlier 616v616). Its callee bindings (0x744440 / 0x744450 / 0x744950 / 0x7445D0 /
  0x643B3B) are unproven.
- 0x0049C490: the draft spells it FindThingsInRoughArea<CThing, HasPhysicsMesh> (non-const); the proven retail name (naming wave 1)
  is `??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@...@std@@VCThingFilter_HasPhysicsMesh@@@CThingSearchTools@@QBEXABVC2DBoxI@@...@Z`
  (const member, const CThing). Respell before landing.

## The other 26 rows
Regraded once as-is (residue on differ line 2, earlier notes kept as `// Earlier grade note:`). All are local padded models; each
needs migrating to the shared engine headers before its x87-order residue is worth chasing.

## Names
- LABELS.tsv: 0x00876CA0, 0x0063AB10 GetGroundFaceNormalAt, 0x00A58790 NBBBMaths::CSphere, 0x008FA660, 0x00B80CE0, 0x00A0C1B0.
- NAME_FIXES.tsv: 0x0063AB10 is `?GetGroundFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z`; landed 0x00723A10 spells it
  `?Helper_use@@YIXPAUCFlushGroundHelper@@@Z`.
- 0x004C73D0 = ?GetPos@CThing@@QBEABVC3DVector@@XZ (already a recorded label; the drafts' GetPosition spelling is wrong).
