# Hand chunk 99 (CTCPhysicsNavigator .. CTCRockTrollShield): 37 of 40 byte-exact

All 37 src/ rows grade MATCH / RELOCATION_MATCH (grades.txt) and every behaviour test prints `<addr>_TEST PASS` under g++ and
under the lane's VC7.1 cl.exe (tall_vc.txt). The CTCPhysicsBase / CTCPhysicsStandard rows share one local model written by
gen/std.py (the engine headers are flattened PODs without virtuals; the landed constructor 0x00723FD0 does the same).

## What matched (37)
- **CTCPhysicsNavigator** (2): SetPositionToNavigateTo 0x00726920 (slot-41 bool gate, then the POD C3DVector copy through a
  `lea` base and `Moving = true`); vfunc_34 0x00727120 (ForcedMoveToPos = pos, then the UsePhysics / Set bitfields merged into
  one byte store). Lesson: a PLAIN POD C3DVector assignment gives the `lea reg,[this+N]` base + carrier-register copy; a
  memberwise user copy constructor/assignment gives direct `[ecx+N]` stores instead.
- **CTCPhysicsStandard / CTCPhysicsBase** (16): vfunc_70 0x00723C00 (RHSet.Forward by value: the interleaved dword copy needs
  C3DVector's memberwise user copy ctor, the POD copy is an esi block copy); vfunc_53 0x00724C50 (`OnGround | OnMesh` bitwise
  gives `movzx ; shr ; or ; shr 4 ; and 1`); vfunc_84 0x00724D00 (PushableByHero = bool); vfunc_86 0x007236A0 (mesh-info word
  `movzx`'d into virtual slot 85); vfunc_38 0x007235E0 (SetAsMoving, then `FeelsGravity = GetAirResistance() > 0.0001f` with
  a separate `and 0FBh` else-exit); SetFacingAnglesFromRHSet 0x007238E0 / 0x007239C0 (byte-identical slots 74 / 75: virtual
  UpdateOldRHSet, POD set copy, SetAsMoving(true)); UpdateOldRHSet 0x00723930 and its base sibling
  CTCPhysicsBase::UpdateOldPosition 0x006AF740 (`call GetWorld ; call GetFrameNumber` with no `mov ecx,eax` = a STATIC
  member called through the `GetWorld()->` object expression; the one-bit `OldMoving = Moving` copy); ResetRHSet 0x00723980;
  GetFacingVector 0x00723C20 (lerp through a NAMED local + memberwise copy ctor); the propagated "GetFacingVector" lerps
  0x0049D150 (CTCPhysicsBase::GetInterpolatedPosition), 0x004C22C0 (C3DVector::Interpolate), 0x004C2EF0 / 0x00AFE320
  (unidentified interpolators named by address) -- all `return (b - a) * t + a;` over three inline value-constructing
  C3DVector operators.
- **CTCPhysicsRigidBody** (2 + 3 relabelled): vfunc_70 0x00759B20 and vfunc_75 0x00759A00 (retail RHSet at +0x68: the PDB's
  three 16-byte vectors are 12 bytes; vfunc_75 tail-jumps SetAsMoving with the argument slot rewritten);
  CProng::GetRotatedPosition 0x006E7FD0 (copy, Rotate about (0,0,1), return through the memberwise copy ctor: X via x87,
  Y/Z via integer moves); CRigidBodyFileRecord_0081e4f0::CopyFrom 0x0081E4F0 (POD key copy, `_snprintf(name, 32, "%s", ..)`
  + forced terminator, two words, three zeros); CMarkedBuffer_00c44550::GetMark 0x00C442F0 (out-reference filled in source
  order and returned: `SBufferMark& GetMark(SBufferMark&)`).
- **CTCPreCalculatedNavigationRoute** (3 + 1 relabelled): constructor 0x004E0CDF (size TU; intelligent pointer, `unsigned
  __int64` UID and the inline zeroing vector all from one `xor eax,eax`); GetRouteDestination 0x008004D0 (typed IP
  `operator->` -> GetPosition reference copied by value); HasRoute 0x00800500 (inlined IS_THING_DEAD + `empty()`, `long`
  result); NPlayerGui::CAutoPickupBar constructor 0x006543FF (the landed CMultiStatBar recipe: `optimize("y",off)` +
  `("s",on)`, list allocator homed in `[ebp+0Bh]`).
- **CTCProjectileTrap** (1): constructor 0x00787DDC (size TU; the three scalar initialisers precede the vtable store).
- **CTCQuestCard** (1): SetRegionName 0x00702560 (`name != RegionName` with the argument as `this`, `=`, tail-jumped
  player-GUI refresh with the slot rewritten to 0); uses engine/CTCQuestCard.h through an inline accessor.
- **CTCQuickAccessMenu** (1 + 1 relabelled): constructor 0x006259E0 and CTCFineDialogue constructor 0x007E2730 (init lists
  after the vtable store, body `AddToGlobalUpdateTCs()`).
- **CTCRandomAppearanceMorph** (1 + 1 relabelled): Copy 0x00728400 and CCountedRecord_00af6690::operator= 0x00AF6690 (inlined
  CCountedPointer::operator= reading the source OBJECT then block into locals).
- **CTCRegionDisplay** (2 + 1 relabelled): constructor 0x006398B0; IsDrawing 0x00639B70 (`if (alpha > 0 && name != L"")
  return 1; return 0;`); CTCScriptedControl constructor 0x007156B0 (intelligent pointer + two inline VC6 lists, the two
  mallocs' arguments popped together).
- **CTCRegionFollower** (1): IsRegionFollower 0x006AEB60 (null guard, iterator walk over a one-pointer list, GetPItem on the left).
- **CTCRockTrollShield** (1): vfunc_9 0x007AED80 (rechargeCounter countdown resetting health to 5 and the counter to 0 again).

## Parked for Fable 5.1 (3)
- 007268d0 CTCPhysicsNavigator::OnCreate DIFFER(69v69): retail `mov byte [esi+88h],1 ; xor eax,eax`, lane `xor eax,eax ; mov byte [esi+88h],1` | tried: Z/Y/X member stores, X=Y=Z and Z=Y=X chains, inline ClearImpulse helper, Navigating before ForcedMoveToPosSet, C3DVector(0,0,0) temporary | next: lane gap: scheduling (zero-register hoist)
- 00c6b720 SBodyRecord_00c55030::Reset DIFFER(35v37): retail copies through edx (source) / esi (dest) / edi (carrier) with `xor eax,eax` inside the copy, lane uses eax / edx / esi and zeroes after | tried: float zeros, long zeros, named source pointer, zero-constructed C3DVector temporary | next: lane gap: register allocation (try the record as part of its 0x24-byte owner TU or a differently typed rest vector)
- 00728470 CTCRandomAppearanceMorph::vfunc_4 DIFFER(71v71): retail `mov eax,[eax+4] ; mov [edi+28h],eax`, lane `mov ecx,[eax+4] ; mov [edi+28h],ecx` | tried: HasTC + GetTC find, named thing local, entry-returning find, out-parameter GetInterface with the member as the out pointer (inline and __forceinline) | next: lane gap: register choice for the found component (try GetInterface into a local then a separate member store)

## Skipped
None: no row spans two functions.

## Label corrections
Each chunk label below is propagated (BSim / byte-similar bodies); the identity comes from the bytes, Ego_r.pdb, RTTI and landed callers.

| Address | Chunk label | Identity |
|---|---|---|
| 0049d150 | CTCPhysicsStandard::GetFacingVector | CTCPhysicsBase::GetInterpolatedPosition(float) (Position / OldPosition lerp) |
| 004c22c0 | CTCPhysicsStandard::GetFacingVector | C3DVector::Interpolate(const C3DVector&, float) const |
| 004c2ef0 | CTCPhysicsStandard::GetFacingVector | unidentified vector-pair interpolator (vectors at +4 / +0x10) |
| 00afe320 | CTCPhysicsStandard::GetFacingVector | unidentified packed interpolator (vectors at +0x29 / +0x35) |
| 006af740 | CTCPhysicsStandard::UpdateOldRHSet | CTCPhysicsBase::UpdateOldPosition (vtable 0x0125D2C4 slot 96) |
| 006e7fd0 | CTCPhysicsRigidBody::GetFacingVector | CDefendingCombatantInfo::CProng::GetRotatedPosition(float) |
| 0081e4f0 | CTCPhysicsRigidBody::UpdateInterpolation | packed rigid-body file record CopyFrom (caller CAFile_ConstructRigidBody) |
| 00c442f0 | CTCPhysicsRigidBody::GetFacingVector | CMarkedBuffer_00c44550::GetMark |
| 00c6b720 | CTCPhysicsRigidBody::GetFacingVector | SBodyRecord_00c55030::Reset |
| 006543ff | CTCPreCalculatedNavigationRoute::CTCPreCalculatedNavigationRoute | NPlayerGui::CAutoPickupBar::CAutoPickupBar (vtable 0x0125A768) |
| 007e2730 | CTCQuickAccessMenu::CTCQuickAccessMenu | CTCFineDialogue::CTCFineDialogue (vtable 0x01270274) |
| 007156b0 | CTCRegionDisplay::CTCRegionDisplay | CTCScriptedControl::CTCScriptedControl (vtable 0x012658E4) |
| 00af6690 | CTCRandomAppearanceMorph::Copy | unidentified record operator= (counted pointer +4, byte +0xc, word +0x10) |
| 00686800 / 00686ed0 (callees) | CEngineSubPrimitive2DClockSprite / std::list::push_back | CTCBase::CTCBase(CThing*) / CTCBase::AddToGlobalUpdateTCs |
| 00686a70 / 0049d870 (callees) | CViewBase::GetDisplayEngine / GFGetPreMainMemoryUsed | descriptive: a this-free getter of [0x013B8A1C]+0x24 (modelled CTCBase::GetWorld) / a static getter of the global 0x013B89BC (modelled CWorld::GetFrameNumber) |
