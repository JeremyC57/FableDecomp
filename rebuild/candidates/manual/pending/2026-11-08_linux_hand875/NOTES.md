# Hand chunk 875 (CThingGameObject .. CNavigatorAStar): 0 of 3 byte-exact

Three ~5 KB rows. All budget went first to the most promising one, CNavigatorAStar::CalculateAndMoveToPosProximity (0x00A1E5C0):
from DIFFER(5476v5437) / ~600 diff lines to DIFFER(5423v5437) / 15 hunks / 115 lines with an identical call profile. The other two
rows were decoded (OnHit fully, UpdateTargetList to a phase skeleton) but have no compiling body.

## What matched (0)

## Parked for Fable 5.1 (3)
- 00a1e5c0 CNavigatorAStar::CalculateAndMoveToPosProximity DIFFER(5423v5437): line-of-sight block 0x00A1EFF0 (retail copies C2DVector(position) as dwords through the [esp+40h] temp and loads destination X-first + fxch; ours: Y-first fld/fstp, no temp), x87 stack in both IsRouteClear expansions (0x00A1ED18 `fadd [mem]`; 0x00A1F65A / 0x00A1F9D1 the GetSmallestNodeSize scale kept live, two `fstp st(0)`), eax/ecx/edx rotation after them (recalc block 0x00A1F099..0x00A1F180, front GetNodeAt 0x00A1F073, MoveTowardNextWayPoint 0x00A1FAAD), pads 0x00A1F2F9 / 0x00A1F6EA | tried: 75 grades: IsNonZero forceinline opt-in (no /d2inlS/T reaches it), IsEqual body in TU, operator=(C3DVector) opt-in, IsCurrentNavigabilityBetter same-TU body under auto_inline(off) (edx kept across the call), recalculate-flag structure, merged arrived test, block-2 iterator + temp way point (fixed 2 hunks), `LastWayPointVisited = position` (fixed 1), `arrivalDistance = Peek...(); arrivalDistance += proximity` (fixed the entry scheduling hunk), 6 LOS spellings, 3 way-point ctor forms, 3 front-node forms, 3 recalc declaration orders, 5 start orders, trivial-copy C2DVector probe (hdrp, worse); c2prio priorities/rotation read | next: find the spelling that keeps the int-copied position temp in the LOS block (the rotation downstream should follow it), then the x87 choices in the two expansions
- 00839940 CThingGameObject::OnHit DIFFER(-v5195): no body | tried: full decode (differ/00839940.cpp: every branch, callee, TC id, member offset) | next: write it on the landed CThingCreatureBase::OnHit 0x00667D30 model; keep the statement order (the last six HasTC are out-of-line bitset::test calls = inline budget)
- 00804020 CTCTargetingPlayer::UpdateTargetList DIFFER(-v5587): no body | tried: callee profile + ordered phase skeleton (differ/00804020.cpp) | next: model NPlayerTargeting::CTargetPoint (CIntelligentPointer members) and the PDB filter functors, then decode phase by phase

## Skipped
None.

## Label corrections
- 0x006CD240 catalog `SortTreeRecursively` is std::list<CNavWayPoint>::size (folded STLport COMDAT; called x5 on Route from 0x00A1E5C0).
- 0x00A57770 reloc_bindings `GetPointOnLine` row (caller 0x00A571A0) is ?GetSquaredDist@C2DLineF@@QAEMABVC2DVector@@@Z (landed row already right).
- 0x00A77780 / 0x00A471D0 / 0x00A1A1A0 / 0x00437160: landed callee spellings wrong, see NAME_FIXES.tsv.
- Manifest hints in OnHit to distrust: 0x00868860 "HasStateZeroOrOne" (a CHitParameters predicate, probably the melee/physical strike test),
  0x00814BE0 "GetMember_dword0x18" (the hit-location TC's default location), 0x007331E0 "GetMember_byte0x63" (CTCCombat blocking test),
  0x00780410 "CMusicManager::ResetMusicEntriesOffsets" (CTCFlammable::SetOnFire, reloc_bindings agrees), 0x0083AF60 "CThing::GetPBaseDef"
  (called on the CHitParameters: GetPHitMaterialDef by value).
