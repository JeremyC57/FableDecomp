# Hand chunk 2004 (CAIStateGroup_FlyingThingBase .. CGraphicDataBank): 4 of 4 byte-exact

All four rows grade RELOCATION_MATCH under VC7.1 RTM from the exact files in src/ (gall.sh), behaviour tests PASS under g++ (tall.sh)
and VC7.1 (tall.py), and the read-only verifier (vland.py, overlay on) reports 4 WINS. Every call target / global was checked against
retail (rel.py: each relocation of the built leaf against the retail target at the same offset).

## What matched (4)
- **0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable** (1352 bytes, retail_asm through the shared GFFloatToLongCeil helper).
  Fresh virtual deed from the world's exclusive opinion source, else the nearest one in a perception-sized C2DBoxI
  (GetPBestThingInRoughArea with CThingFilter_IsOpinionSourceWithFreshDeed / CThingCompare_Nearest), else the head pending hero
  deed (hero reactee unless blocked while following, else the aware perceived thing; ReactionRestrictToTarget check).
  Shape: the search body visible (FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) keeps `thing` in edi with a frame home (declaration
  only: memory, 1347 bytes); the target test is the FableWin free comparison `deed->PTarget == &GetCreature()` (EQUAL_RAW);
  the two `ThingReactingTo = ...` sites split expanded / called by VC7.1 itself; GetPos().ToC2DCoordI() for the box centre.
- **0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath** (1314 bytes). The natural STLport deque<CBezierCurve>
  push_back / back expansion plus one lever: the STLport _M_push_back_aux_v body visible but noinline (it is in this TU in
  retail, landed 0x00913D60), which makes the second push_back's 12-dword copy load-ahead.
- **0x009FED30 CGraphicDataBank::LoadGraphicFrame** (1206 bytes). Matched on the second grade: the top surface's byte length in a
  named local (the GetSurfaceLevel temporary dies before the test) and the mip reduction counting down (no aligned loop head).
- **0x00725500 CTCPhysicsControlled::PerformImpulseBasedMovement** (1324 bytes). Ten graded variants in pass 1, each lever measured
  first (c2prio priorities, fragment / layout probes): step copy after the two magnitudes, response / counter declared after it,
  `collided = true` in both non-cylinder arms (prio 60 > pos 27 -> bl), named `thing`, function-scope old_pos,
  `if (!collided && special_move)` after the if / else (moves the normal test out of line), `0.0f - normal.Dot(C3DVector(0,0,1))`,
  C3DVector NORMALISE_SQUARE_HELPER.

## Length check (brief item)
0x009FED30: the function ends with `ret 20h` at 0x009FF1E3; 0x009FF1E6..0x009FF1EF are int3 padding and 0x009FF1F0..0x009FF21F is a
separate 48-byte function that calls LoadGraphicFrame (`call 0x009FED30`, `ret 14h`) before the next catalog start 0x009FF220. The
oracle's 1206 is the true length, so no SPLITS.tsv entry. All four rows start at real function starts (each preceded by `ret` /
int3 padding).

## Names (all PDB-proven unless listed)
Row names and callees: LABELS.tsv (two sources each). Wrong landed spellings found: NAME_FIXES.tsv (10 addresses: GetSightRadius,
GetHeadPendingDeed const return, CIntelligentPointer<const CThing> ctor / operator=, CPixelFormat::IsEquivalent, CSurface::Unlock,
ModifyResponseForNearVerticalSlopes, CTCPhysicsStandard::UpdateVelocityForImpassability, GetRandomFlightPos access / reference,
the filter's operator() parameter).
**Descriptive:** HeightProbeOffset 0x0138C500 (no PDB data names on this machine; the established bound spelling is reused).
Verifier warnings for the coordinator: RELOC_CONFLICT GetExtendedSightRadius (the landed 0x00864620 binds it to 0x007448F0, which
is GetSightRadius -- NAME_FIXES); RELOC_NAME DELTA_NOTIONAL_ZERO 0x0129BA3C labelled 'EPSILON' in a label file (the shared
C3DVector.h name is DELTA_NOTIONAL_ZERO).

## Header debt
All header work is in hdr/ (README.md): 7 new headers (CAIStateGroup_DeedReactionRoot, CThingFilter_IsOpinionSourceWithFreshDeed,
CTCOpinionSource, CTCRegionFollower, CBezierCurve, CAIStateGroup_FlyingThingBase, CTCPhysicsControlled) and opt-in blocks in 16 copied
repo headers. CTCOpinionSource keeps three container / smart-pointer members as opaque words (named in comments). CGraphicInfo gets a
`friend class CGraphicDataBank` under its opt-in (the original access grant is not in the name sets).

## Notes on the asm helper (0x009609B0)
The only retail asm is the existing shared engine/GFFloatToLongCeil.h helper (proof, FABLE_PORTABLE twin and dual-path test live
there; the row adds a `// Retail-asm:` evidence line so crawl/purity grades it retail_asm). No new helper and no asm in any source or
test.

## Parked for Fable 5.1 (0)
None.

## Skipped (0)
None.

## Label corrections
See NAME_FIXES.tsv; also the catalog label of 0x009F2DC0 (CVertexBufferWin32::DoUnlock) is CSurface::Unlock.
