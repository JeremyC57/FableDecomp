# Hand chunk 98 (CTCParticleAttachmentTracker .. CTCPhysicsNavigator): 32 of 40 byte-exact

All 32 matched rows grade MATCH / RELOCATION_MATCH under the default flags (no pragma, no extra flag) and their behaviour tests
print PASS under both g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). 7 rows parked, 1 skipped.

## What matched (32)
- **CTCParticleAttachmentTracker** -- `SetAsDrawable` 0071a8a0: forward the drawable flag to every live attached particle
  (STLport list of CIntelligentPointer<CThing> held as its header-node pointer). Shape: begin/end node loop; `this` lives in ebp,
  end() is re-read from the member every iteration (virtual call in the body), the element's address is a reference local
  (`lea esi,[edi+8]`) used by the three GetPItem calls.
- **CTCPerceiveThing** (engine/CTCPerceiveThing.h + engine/CPerceivedThingDef.h, `_Methods` subclass):
  - 00744500 (really `SetExtendedSightRadius`, +0x44) / 00744540 `SetSightRadius` (+0x40): `-1` = take the def value; the clamp
    is the natural `if (a < b) a = b;` -- VC produces the `fst`/`fcomp` and the `fcom`/`fst`/`fstp` variants on its own.
  - 007448b0 (really `IsPlayerIgnoreFOVLineOfSight`, +0x2f): nested ifs, long 0/1 result, pointer object address in esi.
  - 007448f0 (really `GetSightRadius`) / 00744950 `GetExtendedSightRadius`: an inline `GetVisibilityMultiplier()` (day / night /
    average * 0.5f; landed standalone at 0x00744580) times the radius, the multiplier loaded first.
- **CTCPhotojournalCapture** (local model; retail 0x44: the STLport vector is 12 bytes) -- `TakeQuickScreenshotNoHud`
  0055d490 and `GlobalUpdate` 0055d4d0: plain source order; `if (FrameCounter < 20) { if (++FrameCounter == 18) ... }`.
- **CTCPhysicalShieldSpell** -- `ResumeForCutscene` 0079e520: `LoopingSound = PlaySound(GetString(PDef->LoopingSound[SpellLevel-1]),
  owner->Type == 2)` with the OWNER taken into a named local first (it is read before the GetString call) and the string a named local.
- **CTCPhysicsBase** (engine/CTCPhysicsBase.h, `_Methods` subclass + vtable-shaped views for the virtual slots):
  - 006af700 `ConstructFromParams`, 006af790 `ResetInterpolation` (`GetWorld()->GetFrame()` = a STATIC member called through an
    object expression: the world call is made and dropped; the OldMoving = Moving copy through ONE reference to the flag byte),
    006b0210 `ApplyAcceleration` (inline C3DVector +=), 006b01d0 `ResetPosition` (virtual SetPosition slot 30).
  - 006aff00 `UpdateCollisionGroundIneractions`: thing read into a local first, then
    `thing->OnCollision(3, pos, GetWorld()->GetWorldMap()->GetGroundFaceNormalAt(pos, 0))` -- the nested call's arguments are pushed
    before the world-map virtual is called.
  - 006b0150 (vfunc_45 = `ClearGraphicPersistenceFlags`), 006b14b0 (vfunc_39 = `HasPositiveMass`, long result),
    006b0190 `IsPhysicsMeshTypeSphere` (`!info() && (info() || !(thing->Flags & 8))` through an inline: the re-asked slot-93
    call's non-null arm jumps to `return 1`), 00880800 `HasCollidedWithThingAt`.
  - 006b1450 (vfunc_71 = `GetRHSet(float)`): return a global CRightHandedSet; C3DVector with a user memberwise copy constructor
    gives retail's "first float through the x87, the other five dwords through ecx/edx".
- **CTCPhysicsControlled** (engine/CTCPhysicsStandard.h base + the PDB members from +0x88):
  - 00725e60 (vfunc_79 = `GetRenderZOffset(float t)`), 00725250 (vfunc_81 = `AddImpulseVelocity`), 00725000
    (vfunc_34 = `SetForcedMoveToPos(pos, usePhysics)`: bool:1 bit insert then `|= 1`).
  - 00724f50 constructor: the bit pair and the two float zeros are BODY assignments in that order, all before the vtable store.
  - 007254b0 `AdjustThingListForDebugHelpers`: `things.erase(things.begin(), things.end())` with `__forceinline` erase and an
    early-out trivial copy (`if (first == last) return result; ... memmove ...`) -- reproduces `cmp eax,eax ; jne`.
- **CTCPhysicsLight** (engine/CTCPhysicsBase.h base, AffectedByGravity bool:1 at +0x40): 00723400 constructor, 007232e0 `OnCreate`
  (`AffectedByGravity = GetMass() > 0` -- the branchy per-arm bit insert), 00723320 `ResetPosition`, 00723360 `Move`,
  007233b0 `SetPosition` (the world map's slot 16 returns a LONG narrowed to CThing::SetCurrentMapNumber's 16-bit parameter --
  that is what schedules `push eax` before `mov ecx,ebx`).
- **CTCPhysicsNavigator** -- 007266c0 constructor (Navigating a plain initialiser before the vtable, AdditionalImpulse a C3DVector
  member built by its inline value constructor after it, the bits body assignments), 00726700 (vfunc_31 = `ResetPosition`),
  00726780 (vfunc_81 = `AddImpulse`) on engine/CTCPhysicsNavigator.h.

## Parked for Fable 5.1 (7)
- 00724fc0 CTCPhysicsControlled::ClearMovement (vfunc_35) DIFFER(48v48): lane hoists `mov dl,[ecx+0A0h] ; and dl,0FEh ; or dl,2` above the three `xor`-register ImpulseVelocity zero stores (retail keeps source order: stores, bit RMW, Velocity stores) | tried: separate Z/Y/X stores, chained `X = Y = Z = 0`, bool:1 vs unsigned char:1 bitfields | next: lane gap: load scheduling across independent stores (same as 00724f80)
- 00724f80 CTCPhysicsControlled::OnCreate DIFFER(53v53): lane hoists `mov cl,[esi+3Ch]` above the three ImpulseVelocity zero stores (retail loads both flag bytes after them) | tried: bool:1 / unsigned char:1 bit-field views (give `or byte ptr [m],1` memory RMW, 41 bytes), /G5 /G6 /G7, plain byte `|=`/`&=` (best, same instruction set), C3DVector temporary assignment (89 bytes) | next: lane gap: load scheduling across independent stores
- 004cb6e0 CTCPhysicsBase::GetMapwhoPosition DIFFER(51v51): `fld [esi+10h] ; fadd [eax+4]` / `fld [esi+0Ch] ; fadd [eax]` where retail loads the returned offset first (`fld [eax+4] ; fadd [esi+10h]`) | tried: named C2DVector local (retail operand order but reads the frame, 54 bytes), const-ref local, inline helper taking the call result by reference (both operand orders: VC commutes), helper taking floats by value (67 bytes) | next: helper parameter order (offset first, position second) or an inline `C2DVector + C3DVector` operator
- 006afcb0 CTCPhysicsBase::GetMapwhoPosition DIFFER(53v53): same operand-order swap as 004cb6e0 (`fld [esi+0Ch] ; fadd [eax]` vs retail `fld [eax] ; fadd [esi+0Ch]`), the named-result `fxch st(2)` tail already matches | tried: the four forms of 004cb6e0 | next: as 004cb6e0
- 007447e0 CTCPerceiveThing::UpdateDistanceFromPerceivedThing DIFFER(77v77): epilogue interleave only -- retail `pop esi ; fstp st(0) ; pop ebx ; fstp st(0) ; fstp st(0)`, the lane pops esi and ebx before the three x87 pops | tried: inline expression (owner load hoisted to the prologue), perceived/owner named locals (fixes the load order), named delta local | next: lane gap: x87 pop scheduling in the epilogue
- 0072ac40 CTCPhysicalShieldSpell::FadeOutEffect DIFFER(75v75): retail `mov eax,ecx` copies the thing to eax and keeps the zero-initialised component in ecx (`xor ecx,ecx`, `mov ecx,[eax+4]`); the lane keeps the thing in ecx and the component in eax (`xor eax,eax`, `mov eax,[eax+4] ; mov ecx,eax`) | tried: out-parameter GetInterface template (inline -> called out of line, __forceinline -> 75v75), pointer-returning GetInterface (84 bytes), CThing thiscall member instead of a static __fastcall helper | next: lane gap: register colouring (or a CThing member returning the component through a reference parameter)
- 00c462e0 _unattributed::CFloatQuad::Interpolate (catalogued CTCPhysicsControlled::GetRenderZOffset) DIFFER(40v51): retail loads all four corners, spills the first row's lerp to an 8-byte frame (`sub esp,8 ; fstp [esp] ; fstp st(0)`) and re-reads it twice (`fsub [esp]`, `fadd [esp]`); the lane keeps it on the x87 stack | tried: value locals + by-value Lerp, reference-parameter Lerp (35 bytes), value locals + reference Lerp, target-first helper nested call; /Op (86 bytes) | next: an 8-byte two-float struct local (C2DVector of the two row results) -- the frame size suggests one

## Skipped
- 007460d0 CTCPerceiveThing::vfunc_10: the 48-byte row holds three functions separated by int3 padding (`mov eax,[ecx] ; jmp [eax+24h]`, `mov al,1 ; ret`, `mov eax,[ecx] ; mov eax,[eax+4] ; add eax,8 ; ret`).

## Label corrections
- 007266c0 "CTCPhysicsBase::CTCPhysicsBase" -> CTCPhysicsNavigator::CTCPhysicsNavigator (stores vtable 0x01266314).
- 00744500 "CTCPerceiveThing::SetSightRadius" -> SetExtendedSightRadius (writes +0x44 from the def's +0x34).
- 007448b0 "IsPlayerExtendedLineOfSight" -> IsPlayerIgnoreFOVLineOfSight (tests +0x2f; the landed 0x00744830 tests +0x2d =
  LineOfSight and 0x00744870 tests +0x2e = ExtendedLineOfSight -- 0x00744830's landed name is therefore also off).
- 007448f0 "GetExtendedSightRadius" -> GetSightRadius (scales +0x40).
- 00c462e0 "CTCPhysicsControlled::GetRenderZOffset" -> an unattributed four-float bilinear interpolation (no callers);
  CTCPhysicsControlled's own render-Z lerp is slot 79 (0x00725e60).
- vfunc names: 006b0150 slot 45 ClearGraphicPersistenceFlags, 006b14b0 slot 39 HasPositiveMass, 006b1450 slot 71 GetRHSet,
  00724fc0 slot 35 ClearMovement, 00725000 slot 34 SetForcedMoveToPos, 00725250 slot 81 AddImpulseVelocity, 00725e60 slot 79
  GetRenderZOffset, 00726700 slot 31 ResetPosition, 00726780 slot 81 AddImpulse.
- Callees: 0x00723FD0 "CTCActivationTrigger::CTCActivationTrigger" = CTCPhysicsStandard::CTCPhysicsStandard; 0x006AF620
  "CThingTrackNode::DrawIsDrawable" = CTCPhysicsBase GetWorld()->GetWorldMap(); 0x00686A70/0x00686A80/0x00686D40
  "CViewBase::GetDisplayEngine" = this-ignoring world / player-manager accessors; 0x0049D870 "GFGetPreMainMemoryUsed" = the
  static frame-counter getter; 0x004C73D0 "CVertexBufferWin32::DoSizeof" = CThing::GetPosition; 0x004374F0
  "C3DMeshInfo::HasPhysicsMesh" = CPlayerGui::IsOpen; 0x0099EAE0 "_Cons_val<...>" = CCharString::~CCharString;
  0x006AFE00 "CTCPhysicsBase::GetAirResistance" returns PhysicsDef +0x38 = PDB CPhysicsDef::Mass.
