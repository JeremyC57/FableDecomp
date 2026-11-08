// Grade: DIFFER(-v5587) -- no compiling body: phase skeleton decoded from the call sequence below; budget went to 0x00A1E5C0 (15 hunks) and the full OnHit decode
// Residue: no source graded. Probe quantity: ordered call skeleton (this file) + callee profile: CBaseIntelligentPointer ctor 0x00A01B10 x12 / GetPItem 0x00A01B50 x16 / SetPItem 0x00A01B90 x16 / dtor 0x00A01C10 x19 (CIntelligentPointer<CThing> members and temporaries of CTargetPoint), STLport vector<NPlayerTargeting::CTargetPoint> out-of-line helpers (_M_insert_overflow 0x008092D0 x3, operator= 0x008085C0 x4, copy 0x00808520 x4 / 0x00806E10 x5, copy-ctor 0x00808470 x2, uninitialized_copy 0x00807980, free x6 / malloc x1 = the allocator inline), std algorithms with filter functors (sort 0x0080C020 x3, __find 0x008066A0 x6, remove_copy(_if) / find_if pairs per filter), PeekPlayer x8 / GetPlayer x7 / IsFirstPersonTargeting x6, __ftol2 x3.
// Next: decode the phases (skeleton below) into C++ against engine/CTCTargetingPlayer.h with the real NPlayerTargeting::CTargetPoint (vector element with CIntelligentPointer members: element copy/assign/dtor are the GetPItem/SetPItem/dtor traffic) and the anonymous-namespace filter functors named in the PDB (CThingFilter_IsInFOV, CThingFilter_IsTargetableBehind, CTargetPointFilter_IsStealableStockItem, CGetBestTargetInDirection, CThingFilter_IsValidTargetForPlayerCreature, CThingFilter_IsInSameVolumeContextAsThing, CThingFilter_IsInTargetingRangeAndFOV); STLport remove_if = find_if + remove_copy_if (that pair is what retail calls).
// CTCTargetingPlayer::UpdateTargetList() -- rebuild the player's list of target points: gather candidate creatures in range, expand their target points, filter by FOV / line of sight / enmity / stealable items, sort by targeting score, keep the current target, pick the best target in the stick direction
// Leaf: ?UpdateTargetList@CTCTargetingPlayer@@IAEXXZ  (Ego_r.pdb 0x007FF350, FableWin.pdb 0x024CFE90; only caller CTCTargetingPlayer::FrameUpdate 0x00805600)
// Retail: 0x00804020..0x008055F3 (5587 bytes), push ebp frame, ends `ret` (no arguments).
//
// PHASE SKELETON (retail call order; addresses are retail):
//  0x0080403D  GetWorld 0x00686A70 ; vfunc +4 (1.0f)                                   -- world time / frame scale
//  0x008040D6  PeekPlayer ; CPlayer::GetCurrentMode 0x00633BE0 ; GetPos ; vfunc +0x118 ; __ftol2 x3   -- targeting origin, mode, integer grid cell
//  0x008041CB  PeekPlayer ; GetTC(const, 0x004365B0) ; CTCCarrying::GetThingInPrimarySlot 0x006A1440  -- held item excluded from targets
//  0x00804264  GetPlayer ; filters: IsValidTargetForPlayerCreature 0x00805890, IsInSameVolumeContextAsThing 0x006A8C00,
//              IsInTargetingRangeAndFOV 0x00809030 ; GetThingSearchTools 0x00686D80 ; GetAllThingsInRoughArea<CThingCreatureBase,list> 0x00809A70
//  0x00804373  per candidate: GetTC(0x0040F020) CTCTargeted ; GetNumTargetPoints 0x007EA2D0 ; GetTargetPoint 0x007EA490 (1.0f) x2 ; vector push_back (_M_insert_overflow 0x008092D0) x2
//  0x0080465E  vfunc +0x90 ; CThingFilter_IsInFOV(…, 0.5f) 0x006A7150 x4 with remove_if / copy_if-with-distance / find_if+remove_copy_if pairs, each followed by vector assign 0x00808520
//  0x00804868  vfunc +0x84 ; PeekPlayer ; IsTargetableBehind filter (find_if 0x0080A910 + remove_copy_if 0x0080A850)
//  0x00804931  GetPlayer x2 ; IsFirstPersonTargeting 0x004874D0 x6 (PeekPlayer each)   -- mode-dependent sort key
//  0x00804ACD  sort 0x0080C020 (CThingCompare_BestForTarget) ; malloc + uninitialized_copy + vector copy-ctor  -- snapshot of the list
//  0x00804B2C  GetWorld ; vfunc +0x24 ; vfunc +0x90 ; GetPlayer x2 ; vector operator= 0x008085C0 x2 ; __find x2 ; push_back ; copy 0x00806E10
//  0x00804D1D  CTCEnemy::AreThingsEnemies 0x0076D2F0 ; enemy filter find_if 0x00807F20 + remove_copy_if 0x00807B70 ; copy ; __find
//  0x00804DEE  GetPlayer ; vfunc +0x84 / +0x90 ; CWorld::GetFrame 0x0049D870 ; sort x2 ; vector copy-ctor ; copy
//  0x008050C3  GetPos ; CGetBestTargetInDirection 0x008059F0 ; for_each 0x008065C0 ; __find ; operator=
//  0x00805208  GetCameraFocalPos 0x004C12D0 (1.0f) ; NCameraModes::CTools::IsLineOfSight 0x00877C40 ; __find + remove_copy 0x00806D80 + copy, x2 ; operator=
//  0x008053A2  AreThingsEnemies ; GetPlayer ; vfunc +0x88 ; 0x00445B80 ; GetCurrentMode ; vfunc +0x88 x2   -- current-target bookkeeping
//  0x008055F2  ret
