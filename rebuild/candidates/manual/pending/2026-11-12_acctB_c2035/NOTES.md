# Hand chunk 2035 (CGameCameraBase .. CThingPlayerCreature): 1 of 2 byte-exact (1 attempted)

Every grade and probe used `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2035/hdr` (hdr/README.md: two opt-in additions). gall.sh /
tall.py need that export (without it the row is SRC_FAIL: GetRotationAngle not a member).

## What matched (1)
- 006fdd60 CGameCameraBase::Update (RELOCATION_MATCH 1659v1659, test PASS g++ + VC7.1; 7 grades, all in pass 1; matched at grade 6,
  grade 7 = the same body after the header-comment / include cleanup). The per-frame orbit camera: LastInternalCam / LastPointingAt
  copies, the spherical offset (GFCos / GFSin of Phi / Theta in turns, times Rad), IS_THING_ALIVE on the player character, target =
  PeekPos + (0,0,1), the controlled creature's velocity ((Position - OldPosition) * ConstantFPS) and facing (PhysicsTC slot 70),
  the fixed-camera release after DivideByGameSpeed(frame - TriggerFrame) > 0.5 (BuildingUID 0) with Rad re-measured, the
  LocalThingPosList pass (sum of target-relative positions, running dot of consecutive ones while >= 0; dot < 0 -> theta offset
  0.08 - ThetaFixed, else look from behind the flattened, normalised sum), easing of Rad / Theta / Phi (Phi through GetRotationAngle
  * PhiChanger), new eye = target + offset, PointAt, the IsThingCollision override (DrawCoordList.front(), ActAsFixedCam), and the
  line-of-sight fix-up (FindLastUnblockedGroundPositionAndReportCollision + 1.5 Z).

### Row extent (SPLITS.tsv)
The oracle length 1659 is right (ret 0x006FE3DA, int3 to 0x006FE3DF). The reservation's 2018 bytes ran into 0x006FE3E0..0x006FE53F:
22 uncatalogued, unreferenced STLport list<C3DVector> inline COMDATs of this TU (empty, iterator ctor / * / ++, front, begin / end by
hidden pointer, allocator<C3DVector>::allocate (n*12, malloc) / deallocate (free)); the next catalog row is 0x006FE550.

### Grade log (pass 1; passes 2 / 3 unused because the row matched)
| # | change | result |
|---|--------|--------|
| 1 | first full draft | DIFFER 1652v1659, 379/460 equal (block layout, frame, x87 orders, end copies) |
| 2 | Rad / Theta easing as `+=` member expressions (GFGetInterpolatedValue kept Rad on the stack); `if (dot >= 0) {flatten} else {offset}` (retail lays the flatten arm after the easing block, `test ah,1 ; je`); `dot` declared after `last` | DIFFER 1659v1659, 425/460 |
| 3 | same-TU noinline body of the landed GetRotationAngle 0x006FD4F0 (retail TU; Update its only caller) | DIFFER 1659v1659, 439/460: behind / offset now share frame words with the later temporaries, empty() test scheduled as retail |
| 4 | FableWin Normalise body (GFSqr, FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER: FableWin COMDAT order Normalise 0x01ACDFD0, GFSqr 0x01ACE080, GFFastOneOverSqrt 0x01ACE0A0) for the X, Z, Y squares; blocked branch `new_pos = collision + (0,0,1.5)` + one MoveTo after the if (VC7.1 duplicates it into both exits; retail's edx / eax / ecx copy registers) | DIFFER 1661v1659: only 0x006FE22E `fst [esp+10h] ; fmulp st(1)` (ours `fstp ; fmul [esp+10h]`) |
| 5 | field-wise zero init of `total` (u1) | DIFFER 1659v1659: that line right, but the flattened squares (Y first) and Y*r operand order wrong (my probe indicators did not yet cover them) |
| 6 | back to the ctor init; loop body reads the element through `const C3DVector& pos = *it;` | RELOCATION_MATCH |
| 7 | header comment / include cleanup (removed the unused GFGetInterpolatedValue include and a duplicate CWorldMap.h include) | RELOCATION_MATCH |

Probes on top (free: own object only, deciding instructions read with tools/ind.sh, never a full diff against retail): about 70
compiles (tools/pc.py, sweep.sh), 3 c2prio traces (tools/c2p, copied from agent2027), 1 c2 record dump (tools/c2h, copied from
agent3005). Measured: outer dead `long` locals k=0..8 (entry and else arm) move nothing here (the r3004 key is not this tie); the
flattened Normalise's three x87 orders (X/Y square order, X*r, Y*r) follow the c2 candidate ids of total.X / total.Y relative to that
Normalise's one_on_magnitude: a direct field reference of total (field-wise init, `total.X += ...`, `total.Y = 0`) creates that
field's candidate before inlining (early id), inline `this->` references only after; retail needs X early / Y late, which the named
`pos` reference produced (its record changes the allocation order) with no instruction change elsewhere.

### Names (two sources each)
- Update: Ego_r / FableWin ?Update@CGameCameraBase@@UAEXXZ + vtable 0x01264A8C slot 0 (+ Ego_r donor order = retail order).
- Callees: CWorld::GetFrame (static __fastcall) 0x0049D870, CWorld::PeekMyPlayer 0x0049D9D0, CBaseIntelligentPointer::GetPItem
  0x00A01B50 via the FableWin-only inline CPlayer::GetPPlayerCharacter (+0x34), IS_THING_ALIVE<CThingPlayerCreature*> (FableWin
  ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z, expanded), CThingPhysical::PeekPos 0x006A4D00,
  CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetPhysicsTC (+0x60), CTCPhysicsBase::GetPosition / GetLastPosition (+0x0C /
  +0x18) / GetFacingVector (slot 70), DivideByGameSpeed 0x00415FD3 (landed definer's global spelling; no PDB record under either
  spelling), CGameCameraBase::GetRotationAngle 0x006FD4F0 (private, see NAME_FIXES), CCamera::PeekWorldPos / PointAt 0x00A0C2B0 /
  MoveTo (FableWin), CGameCameraBase::IsThingCollision 0x006FDA20 (private; LABELS), CWorld::GetWorldMap (slot 12), CWorldMap::
  IsLineOfSight 0x0063AEE0, FindLastUnblockedGroundPositionAndReportCollision 0x0063D960, C3DVector::Normalise / GFSqr /
  GFFastOneOverSqrt (FableWin, expanded), GFSin / GFCos (both PDBs), CMainGameComponent::GetConstantFPS / ConstantFPS 0x01375550,
  GFastOneOverSqrtTable 0x013CE558. Every call target and global checked against retail (tools/rel.py: all `same`; every __real
  constant's value checked against the retail pool).
- Descriptive names: the row's locals only (frame, offset, target, velocity, facing, theta_offset, behind, total, last, dot, rel,
  pos, new_pos, collision) -- no PDB local names exist for them. No descriptive function names.

### Header debt / notes for landing
- The landed GetRotationAngle 0x006FD4F0 is public in its local class (QAE); both PDBs say private (AAE). This row defines the AAE
  body in its own TU (noinline, needed for the frame); respell the landed definer (NAME_FIXES.tsv).
- 0x00415FD3: MULTINAME conflict (?DivideByGameSpeed@@YGMM@Z vs ?DivideByGameSpeed@CDisplayEngine@@SGMM@Z) still open; this row uses
  the definer's global spelling.

## Parked for Fable 5.1 (0)

## Not attempted (1)
- 006aca30 CThingPlayerCreature::OnCollisionWithObject (2026 bytes): optional second row, not started (after row 1 my remaining
  context was below the brief's "well over half" threshold). No source exists; it goes to a fresh agent. Pointer: the retail
  CThingPlayerCreature branch of rebuild/include/engine/CThingPlayerCreature.h already lists 0x006ACA30 among its evidence rows.

## Skipped
- none.

## Label corrections
- 0x006FDA20 catalog "CGameCameraBase_TracePhysics" (_global) is ?IsThingCollision@CGameCameraBase@@AAE_NXZ.
- 0x006FD4F0 landed QAE spelling should be AAE (private).
- 0x0049D9D0 is bound once as ?GetCurrentPlayer@CWorld@@QBEPAVCPlayer@@XZ (site 0x0095F9A0); the PDB name is PeekMyPlayer.
