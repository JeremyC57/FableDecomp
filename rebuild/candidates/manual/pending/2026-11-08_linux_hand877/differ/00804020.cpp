// Grade: DIFFER(-v5587) -- no compiling body (chunk 877 spent its budget converging 0x00839940); this file extends chunk 875's call skeleton with a decoded phase map to 0x00805083 + the tail call order
// Residue: no source graded. Probe quantity: phase map below (frame `push ebp; mov ebp,esp; and esp,-8; sub esp,1F0h`, ebp=this, ebx=ParentThing early); 7 STLport algorithm instances over std::vector<NPlayerTargeting::CTargetPoint> (0x1C bytes: CIntelligentPointer<CThing> 8 + Index + 4 dwords) with anonymous-namespace binders
// Next: author the body phase by phase against engine/CTCTargetingPlayer.h: model CTargetPoint (user copy ctor = the 0x00A01B10/0x00A01B50/0x00A01B90 + 5-dword copy), the vector push_back (_M_insert_overflow 0x008092D0), and declare the algorithm instances out of line (remove_if/copy_if/find/sort below) with their PDB template spellings; then converge
// CTCTargetingPlayer::UpdateTargetList() -- rebuild the player's target-point list: gather creatures in range, expand target points, filter, sort by score, keep / choose the locked, projectile and threatened targets
// Leaf: ?UpdateTargetList@CTCTargetingPlayer@@IAEXXZ
//
// Phase map (retail addresses; members per engine/CTCTargetingPlayer.h retail layout):
// 0x804030 ebx=ParentThing(+4); [esp+2C]=this; CTCBase::GetWorld 0x00686A70 -> [+34]->vfunc+4(&frame?, 1.0f).
// 0x804057 edi=+0x30 (the player-targeting def / settings, [esp+28]); copy TargetedPoint (+0x4C, CTargetPoint) into a local
//          (ctor 0x00A01B10, vtable 0x01238C6C, GetPItem 0x00A01B50 / SetPItem 0x00A01B90, 5 dwords +8..+0x18).
// 0x8040A6 bool lockedMode = [+0x38] > 8; PeekPlayer(0x004C7A10)->GetCurrentMode(0x00633BE0)==0x2E; bytes def+0x3A / def+0x38
//          -> bool useStickDir = (def+0x3A || lockedMode || mode==0x2E); position = def+0x38 ? local(+0x18C) : GetPos(0x004C73D0).
// 0x80413E origin = useStickDir ? local(+0x1A4) : PhysicsTC(+0x60)->vfunc+0x118(&tmp) (head / eye position).
// 0x80417D grid box: three __ftol2 (0x00BFEA70) of X, Y and MaxTargetingDistance(+0x48)+[0x013BAE28] -> (x-r, y-r, x+r, y+r).
// 0x8041CB range = (PeekPlayer()+0x21A ? def+0x34 : def+0x30) * [0x01238E00].
// 0x8041EE carried = ParentThing HasTC(0x46) ? CTCCarrying::GetThingInPrimarySlot 0x006A1440 : 0.
// 0x804234 bool flag = ![0x013BAF21]; list<CThingCreatureBase*> found; CFilter_Bind5 of
//          CThingFilter_IsValidTargetForPlayerCreature (0x00805890), CThingFilter_IsInSameVolumeContextAsThing (0x006A8C00),
//          CThingFilter_IsInTargetingRangeAndFOV (0x00809030); CTCBase::GetThingSearchTools 0x00686D80 ->
//          GetAllThingsInRoughArea<CThingCreatureBase,list<..>,CFilter_Bind5<..>> 0x00809A70.
// 0x80432B for each creature: HasTC(9)->CTCTargeted (+0x20 bit 9 test, LowerBound 0x0040F020); n = GetNumTargetPoints 0x007EA2D0;
//          for i<n: CTargetPoint(creature, i) ; GetTargetPoint 0x007EA490(&pos, i, 1.0f) ; candidates.push_back (inline copy,
//          _M_insert_overflow 0x008092D0). n==0: CTargetPoint(creature, -1) + GetTargetPoint(-1) the same way.
// 0x804644 if range > [0x0122F59C]: frame = vfunc this+0x90; three corners scaled by [0x0122DEE0];
//          C3DVector ctor 0x006A7150 twice; remove_if CThingFilter_IsInFOV 0x00807C30 + remove_copy 0x00807A90; erase 0x00808520;
//          remove_if 0x00809F80 / 0x00809D60 (CTargetPointFilter_IsStealableStockItem); erase.
// 0x80479D facing = PhysicsTC(+0x60) +0xC..+0x20 deltas; if def+0x3B: dot/length test vs DELTA_NOTIONAL_ZERO -> erase(remove_if 0x008078A0).
// 0x804858 if def+0x3C: vfunc this+0x84; PeekPlayer()+0x20C; CThingFilter_IsTargetableBehind -> 0x0080A910 / 0x0080A850; erase.
// 0x804909 [0x01375753] / CPlayer::IsFirstPersonTargeting 0x004874D0 pick the score weights def+0x44..+0x70 (8 floats).
// 0x804A3F build the comparator (0x34 bytes by value: two C3DVectors, weights, bool 1, float) -> introsort 0x0080C020
//          (CThingCompare_BestForTargeting2); sorted copy: allocate n*0x1C (0x00BFEA0E), uninitialized_copy 0x00807980;
//          Targets(+0x68) = copy (0x00808470).
// 0x804B2A GetWorld()->vfunc+0x24; TargetedPoint thing (+0x4C GetPItem); vfunc this+0x90 -> [esp+E0];
//          bool keepCurrent from +0x3C / PeekPlayer()+0x20A / +0x20C; vector<CTargetPoint> work (0x008085C0 range-assign).
// 0x804BFC remove the RejectedTargetPoints (+0x74) via __find 0x008066A0 + push_back / _M_insert_overflow; then compact with
//          __find + __copy 0x00806E10 + dtor 0x00A01C10 loop.
// 0x804D02 if the locked thing is alive and CTCEnemy::AreThingsEnemies 0x0076D2F0: find_if 0x00807F20 + remove_copy_if
//          0x00807B70 (enemy filter), __copy 0x00806E10, destroy tail.
// 0x804D7B index = find(work, TargetedPoint) (0x008066A0, /0x1C via 0x92492493) when the frame thing equals +0x100.
// 0x804DD5 if +0x3C: player mode checks (+0x20A, vfunc +0x84 / +0x90); def+0x40 == 1: CWorld::GetWorldFrame 0x0049D870 timing vs
//          [0x01375550]*[0x012722E8]+def+0x44 -> introsort 0x0080C020 or advance the index modulo the size; def+0x40 == 2 path
//          at 0x804F96: copy (0x00808470), per point direction sign (+-1.0f), 0x0080...
// 0x805083 .. 0x8055F2 (tail, call order): __copy 0x00806E10, dtor, GetPos 0x004C73D0, 0x008059F0, 0x008065C0, __find 0x008066A0,
//          dtor x2, operator delete 0x00BFEA14, 0x008085C0, GetPItem, 0x004C12D0, 0x00877C40, GetPItem, __find, 0x00806D80,
//          __copy, dtor, __find, 0x00806D80, __copy, dtor, GetPItem/SetPItem, 0x008085C0, GetPItem, Dead test (+0x91),
//          AreThingsEnemies, PeekPlayer, ctor/SetPItem/GetPItem/SetPItem/dtor, vfunc this+0x88 (SetTargetedPoint), 0x00445B80,
//          GetCurrentMode 0x00633BE0, (ctor, SetPItem, GetPItem, SetPItem, dtor, vfunc+0x88) x2, then the four vectors' dtor
//          loops + operator delete, the TargetedPoint copy's dtor, `mov esp,ebp; pop ebp; ret`.
