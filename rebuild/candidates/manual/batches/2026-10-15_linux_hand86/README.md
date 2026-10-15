# Hand chunk 86 (CTCDHeroSuit .. CTCEffectOnDie): 34 of 40 byte-exact

All 34 sources in src/ grade MATCH (5) or RELOCATION_MATCH (29) with gall.sh (grades.txt). All 34 tests in test/ print
`<addr>_TEST PASS` under g++ (tall.sh) and under VC7.1 in Wine (tall.py). Four rows are parked in differ/ and two are
skipped because each of those rows holds two functions.

## What matched (34)
- **CTCDHeroSuit / CTCGatherPointBase constructors** (004e079c, 004e0aab): /Os pass-through base argument
  (`push dword ptr [esp+8]`). When `xor eax,eax` and the vtable store come FIRST and the zero stores follow, the members are
  member objects with inline constructors (a 12-byte retail vector `CRetailVector() : first(0), last(0), end(0)`, a
  CDefPointer). An init-list scalar would sink the vtable store below them. 004e0aab is CTCGatherPointBase
  (vtable 0x012421E4), and its MaxOccupation = 1000 sits at +0x18 because the retail vector is 12 bytes.
- **CTCDriverBase** (00771200 ctor, 00771230 DrawPreparePrimitivesForRendering): the ctor's virtual call is on the
  PARENT thing (`call [edx+70h]`, SetAsDrawable(true)), not on `this`, so it stays in the ctor body. 00771230 is
  the base-class slot-11 body that 12 CTCD* vtables inherit. It takes `const CEnginePrimitivePrepareInfo&` (egor) and
  ends in a null test plus a sibling `jmp`. The retail CThing PTCGraphicAppearance sits at +0x64 (PDB +0x74).
- **CTCDParticleEmitter** (006e0590, 006e05f0, 006e0db0, 006e0fc0, 006e1020, 006e11a0, 006e1e60):
  - SetAlwaysUpdate: the emitter's AlwaysUpdate bit goes through an inline setter ON the emitter class. A direct
    bitfield store on `GetPItem()` makes the lane form `add eax,44h` once. The component's own bits are bool:1
    `shl ; xor ; and ; xor` inserts.
  - EnterTimelineEnd: the no-argument branch tail-jumps; Kill(true) stays a plain call.
  - AttachToCamera(long, float): the float is copied with integer moves, and `or cl,40h` is a bool:1 set.
  - OnSetInLimbo / SetAsDrawable: the flag byte is loaded once after the predicate's `test al,al` and tested in both
    arms. The argument is unused (`ret 4`). SetAsDrawable adds the inline "thing alive" test (non-null, dead bit +0x91).
  - SetEmitterUpdateParameters: the argument is pushed before the GetPItem call that yields `this` (a plain chained
    call), and the pending arm re-reads the counted pointer after the operator= call.
  - ~CTCDParticleEmitter (`// Leaf: ??1CTCDParticleEmitter`): there is no vtable store, so the model has no virtuals.
    The inline CCountedPointer release does `--PBlock->Count; if (PBlock->Count == 0)` (the block is re-read), calls
    the static deleter through `[eax+4]` with `ecx=[eax+8]`, deletes the POD block without a null test, and zeroes
    both members after the join. Then the members die in reverse order and the CTCBase dtor is tail-jumped.
- **CTCDRegionEntrance / CTCDiggingSpot / CTCDoor OnSerialise** (00778ef0, 007e2530, 00731170): CPersistContext::
  Transfer<bool>(name, member, default) is called with a one-byte default temporary. The default's home is the high
  byte of the `push ecx` slot, or the dead `fullState` argument slot once that slot has been tested. `this` is
  advanced in place (`add ecx,0Ch`) to form &member. In CTCDoor::OnSerialise the second default is CurrentOpen
  itself (its address is pushed again).
- **CTCDoor** (00730580 OnCreate, 007305a0 Copy): engine/CTCDoor.h is used directly. Copy takes `const CTCBase&`
  (egor) and reads it through a reinterpret_cast to the door.
- **CTCDRumble::FrameUpdate** (007f1890): a post-decrement compared with zero (`test eax,eax` on the OLD value
  before the `lea edx,[eax-1]` store), then `jge`.
- **CTCDayOrNightOnlySupport::GetSectionNameSuffix** (00702ac0): a two-case switch (`dec eax ; je` x2) that lays the
  default out first. The operator+ is `__fastcall` (hidden result in ecx, left in edx), and the static CCharString
  globals are "DayOnly" / "NightOnly".
- **CTCDeadShotInThingTracker** (007e3450 ctor, 007e3490 SetThingStuckIn, 007e34e0 FrameUpdate):
  - The ctor inlines CIntelligentPointer(item = 0): the base ctor, its vtable, then SetPItem(0).
  - SetThingStuckIn(const CThing&) builds an 8-byte stack temporary, assigns it through the inline operator=
    (SetPItem(other.GetPItem())), and destroys it through the out-of-line base dtor.
  - FrameUpdate is a three-way `||` (null, dead bit, physics Moving bit) that shares the Kill(true) tail.
- **CTCDegradableBase** (008ec7e0 IsAvailableToHit, 008ec810 OnHitByShot, 008ec830 OnHitByMelee, 008ec900
  IsFullyDegraded, 008ecc80 OnHit):
  - IsAvailableToHit calls the static frame getter through `GetWorld()->`, so GetWorld runs for nothing. The result
    is a local preset to false in bl.
  - OnHitByShot / OnHitByMelee are byte-identical forwarders to the virtual OnHit (`ret 0Ch`, no sibling jump
    because the callee pops 8). The egor function order separates them.
  - IsFullyDegraded uses an inline GetDefinition that returns a CDefPointer BY VALUE (add-ref, then release after the
    compare, with the result parked in `sete bl`).
  - OnHit inlines IsAvailableToHit and SetAvailabilityToHitFromFrame. The LastWorldFrameHit store is hoisted between
    the `cmp` and the `jle`.
- **Small constructors** (004d8193 CTCCameraPointBuilding, 004db15e CTCShotDefinitionDrainLife, 004db3db CTCDiceGame,
  00746820 CTCBed, 007509a0 CTCDiscovery, 007bad20 CTCTrespassable, 007e0b50 CTCEffectOnDie):
  - Plain init-list scalars are scheduled around the derived vtable store.
  - CTCDiscovery's Discoverer really sits at the unaligned +0x0d (pack(1)).
  - The pointer members of CTCShotDefinitionDrainLife and CTCDiceGame use the out-of-line CIntelligentPointer(CThing*)
    called with 0.
  - In CTCEffectOnDie, ebx is the shared zero and the two bools follow the CCharString ctor call.
- **CTCDrainLife::SetAICast** (007c3040): `CGameDefinitionManager::Get().GetDef(index, PDef)`. The arguments are
  pushed before the static singleton call (`add ecx,0Ch` forms &PDef in place), and the bool result is dropped.

## Parked for Fable 5.1 (4)
- 004df14b CTCDPhysicalObstruction::CTCDPhysicalObstruction DIFFER(91v91): `mov ecx,edi` for line1's CBaseClass call is scheduled before the three vector zero stores (retail: between the 2nd and 3rd store) | tried: the 12-byte vector ctor as an init list and as body assignments (3 grades) | next: lane gap: member-ctor `lea/mov ecx` scheduled before a run of register-zero stores (wave 24i)
- 007509d0 CTCDiscovery::SetDiscovered DIFFER(90v90): retail keeps `this` in esi and the discoverer in edi, the lane the reverse | tried: plain argument, named local copy of the discoverer, named CIntelligentPointer local | next: lane gap: register allocation (esi/edi swap)
- 0078e7c0 CTCDivineWrath::Attackable DIFFER(91v94): retail loads ParentThing into ebx before the target GetPItem call and loads arg1 before arg2 for both `__fastcall` AreThingsEnemies calls; `this` in esi (lane: edi); retail shares one `mov al,1` tail after `test al,al ; jne` where the lane emits `setne` | tried: if/return-true chain, single `||`/`&&` return expression | next: try AreThingsEnemies modelled as a static member with explicit locals for the two arguments (ParentThing loaded into a local before the first test); otherwise lane gap: __fastcall argument evaluation order
- 00793cc0 CTCUnholyPower::Attackable DIFFER(91v94): byte-identical to 0078e7c0, same differences | tried: the same source as 0078e7c0 | next: lands with 0078e7c0 when that one matches

## Skipped
- 00991620 CTCDParticleEmitter::EditFrameUpdate: the 80-byte row holds two functions (the int3 padding is followed by a second body at 00991640).
- 007312f0 CTCDoor::vfunc_10: the row holds two functions (`mov eax,[ecx] ; jmp [eax+24h]`, int3 padding, then `mov al,1 ; ret` at 00731300).

## Label corrections
- 004e0aab "CTCDHeroSuit::CTCDHeroSuit" is CTCGatherPointBase::CTCGatherPointBase (vtable 0x012421E4).
- 004db15e "CTCDiceGame::CTCDiceGame" is NThingShot::CTCShotDefinitionDrainLife::CTCShotDefinitionDrainLife (vtable 0x012403F4).
- 004d8193 "CTCEditor::CTCEditor" is CTCCameraPointBuilding::CTCCameraPointBuilding (vtable 0x0123ECBC).
- 00746820 "CTCEditor::CTCEditor" is CTCBed::CTCBed (vtable 0x01268964).
- 007bad20 "CTCEditor::CTCEditor" is CTCTrespassable::CTCTrespassable (vtable 0x0126DD9C).
- 00771230 "CTCDNavigationSeed::vfunc_11" is CTCDriverBase::DrawPreparePrimitivesForRendering (inherited by 12 CTCD* vtables).
- 00793cc0 "CTCDivineWrath::Attackable" is CTCUnholyPower::Attackable (byte-identical body).
- 008ec810 / 008ec830 "CTCDegradableBarrier::vfunc_29/28" are CTCDegradableBase::OnHitByShot / OnHitByMelee.
- Vtable-slot names: 00730580 = CTCDoor::OnCreate (slot 4), 007305a0 = CTCDoor::Copy (slot 12), 00731170 / 00778ef0 /
  007e2530 = OnSerialise (slot 1), 007e34e0 / 007f1890 = FrameUpdate (slot 9).
- Callee labels:
  - 0x004045C0 "Transfer<signed char>" is the byte-identical Transfer<bool>.
  - 0x00686A70 / 0x00686A80 "CViewBase::GetDisplayEngine" are CTCBase::GetWorld (non-const / const).
  - 0x0049D870 "GFGetPreMainMemoryUsed" is the static world-frame getter.
  - 0x0075C180 "CTCOpinionSource" is CTCCameraPointDefinitionBase's ctor.
  - 0x0099EAE0 "_Cons_val<...>" is CCharString::~CCharString.
  - 0x00686830 "~CChunkedFileChunk" is CTCBase::~CTCBase.
  - 0x006E2570 "CParticleEmitter::SetUpdateParameters" is CParticleUpdateParameters<unsigned long, unsigned long>::operator=.
  - 0x00AED140 is ClearAllParticles (labelled ClearAllDecals).
  - 0x007C3A60 "GetDef<CThingPhysicalSwitchDef>" is the byte-identical GetDef<CSpecialAbilitiesDrainLifeDef>.
