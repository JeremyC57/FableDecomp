# Hand chunk 611 (CTCInventoryClothing .. STLport vector): 1 of 10 byte-exact

All ten rows were parked same-length misses from the 2026-10-30/31 sweep. Every row was re-graded from its best file
(each reproduced its old grade), its names re-proven against Ego_r.pdb / RTTI / reloc_bindings.tsv, then tuned one lever
per grade.

## What matched (1)
- CEffectFilter (libpk DSP, descriptive): 0x00C74BB0 `SetHighPass(long attenuation, float reference, unsigned long sampleRate)`
  RELOCATION_MATCH. The first-order high-pass coefficient set. Two levers: `float frequency = (float)pow(10.0, ...);
  frequency *= reference;` puts the rate's `mov eax,[rate] ; test eax,eax` before `fmul [reference]` (the one-expression
  product schedules the fmul first), and `float omega = frequency * 2pi; cos(omega / rate)` keeps `fmul 2pi ; fdiv rate`
  (VC7.1 rewrote the inline `frequency * 2pi / rate` into `fdiv ; fmul`). Test PASS under g++ and VC7.1.

## Parked for Fable 5.1 (9)
- 005b690b CTCInventoryClothing::RefreshDescriptions DIFFER(290v290): second pass keeps the list node in ebx and the component in edi (retail edi/ebx, as in the first pass) | tried: shared iterator + split init, separate second iterator, end-first declaration, continue form, component local (293), map[*it] (286) | next: lane gap: equal-use register colouring
- 0087ba90 NCameraModes::CScriptedControl::OnInit DIFFER(294v294): `mov edx,[ecx]` before the PCameraManager store (retail loads the vtable into the freed eax after it); later eax/ecx/edx rotate by one | tried: module reference, named module pointer, manager local, inline module Init helper, optimize("s") (275) | next: lane gap: register rotation
- 007bbc9f NThingShot::CTCShotDefinitionThrow::InitMovement DIFFER(295v299): retail keeps a second x87 copy of sin(A) (`fst qword ; fld st(0)`, four dead pops v three) and computes velocity Y as dirY*s, Z as s*dirZ | tried: s/c double with no float casts (94 -> 28 lines, frame now 0x34 as retail), repeated sin()/cos(), inline double Square, split t local, named-result operator* (319), f*X operator*, `float sf = (float)s` for the square (299v299 with the copy, but its conversion CSEs with the velocity one: 92 lines) | next: a velocity form that multiplies in double without a float conversion of s
- 007805d0 NFire::CFirePoint::SetAsSmoldering DIFFER(299v299): retail loads effects.Object into edi in both arms of the && and reuses it for the release (`dec dword ptr [edi+4] ; jne`); ours loads ecx and reloads in the dtor, temps 4 bytes lower | tried: engine/CDefPointer.h + fable_string.h explicit CCharString temporary (303), the same with a local signed-count CIVCountedPointeeBase and implicit "SINGE" (302) | next: lane gap: cross-statement CSE of the def pointer (the shared header's Release materialises `mov/dec/mov` here)
- 00793370 std::vector<CTCDivineWrath::PerEffectInfo>::_M_insert_overflow DIFFER(300v300): this in esi / new_finish in edi (retail the reverse, all four pushes at entry) | tried: landed CBaseIntelligentPointer model, new_finish from the copy | next: lane gap: LTCG register convention (helpers take register arguments)
- 007981b0 std::vector<CTCUnholyPower::PerEffectInfo>::_M_insert_overflow DIFFER(300v300): byte twin of 0x00793370 | tried: same | next: as 0x00793370
- 00860ad0 CVillageTaskPatrol::IsCreatureMoreAppropriateForTask DIFFER(302v302): the first two GetPos copies interleave load/store (retail loads all three dwords first) and fnstsw sits before the two dead fstp st(0) | tried: trivial copy (376), body-assignment copy ctor, named floats, named bool / if-return / ternary / int returns (fix the fnstsw order but give `mov al,1` and merge the early exit, 294), swapped comparison | next: lane gap: copy scheduling
- 007afe20 std::vector<CIntelligentPointer<CThingAICreature> >::_M_fill_insert DIFFER(302v302): pos in ebx, elems_after in ebp (retail reverse) | tried: old_finish declared first (288) | next: lane gap: LTCG register convention
- 007b9b40 std::vector<CIntelligentPointer<CThingObject> >::_M_fill_insert DIFFER(302v302): byte twin of 0x007AFE20 | tried: const x_copy | next: as 0x007AFE20

## Skipped
None.

## Names (proven unless marked)
- 0x005B690B ?RefreshDescriptions@CTCInventoryClothing@@QAEXXZ (Ego_r.pdb). Callees: CComponent slot 5 FindAllChildren(CCharString,
  list<CComponent*>&) (Ego_r.pdb + landed 0x00530F30 / slot list in landed CTCInventory sliders); CText slot 144
  SetValue(const CWideString&, bool, bool) (landed CText::SetValue 0x0054F410, vtable 0x01249CCC slot 144); GetItemDescription
  const virtual slot 78 (vtable 0x0124356C slot 78 = 0x005B5CF8, Ego_r UBE); STLport list/map spellings (NAME_FIXES).
  Filler virtual slots (SlotNNN) are unnamed placeholders.
- 0x00C74BB0 CEffectFilter::SetHighPass -- DESCRIPTIVE (libpk, no PDB/RTTI); class name from the landed caller 0x00C783F0.
- 0x0087BA90 ?OnInit@CScriptedControl@NCameraModes@@UAEXXZ (Ego_r + RTTI slot 21). Callees all Ego_r spellings; the module's slot 8
  is OnInitLeaf (the slot the OnInitLeaf-declaring CScriptModuleFullTransition overrides). Epsilon 0x0129BA3C: no PDB name
  (50 bound spellings) -- descriptive g_fEpsilon.
- 0x007BBC9F ?InitMovement@CTCShotDefinitionThrow@NThingShot@@MAEXXZ (Ego_r + RTTI slot 33); GetShootAtPos 0x007BEDD0 (Ego_r +
  catalog), CTCPhysicsBase::SetVelocity 0x0066F9B0 (landed); physics slot 38 name unproven (v38).
- 0x007805D0 ?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ (Ego_r); callee CThing::GetPos fixed.
- 0x00860AD0 ?IsCreatureMoreAppropriateForTask@CVillageTaskPatrol@@QBE_NABVCThing@@@Z (Ego_r); GetNearestNode is QBE.
- STLport rows: real std::vector members with the default allocator (NAME_FIXES for the bound spellings without it).

## Label corrections
- 0x00C74BB0 "CAudioChannel_ComputePeakFilterCoefficients" -> CEffectFilter::SetHighPass (descriptive).
- 0x0087BA90 catalog fine; 0x0084D8B0 is NCameraModes::CBase::GetObservedThingCurrentMeshHeight (virtual const, CBase slot 15).
- 0x007BBC9F "CTCPhysicsRigidBody_ApplyForce" -> NThingShot::CTCShotDefinitionThrow::InitMovement.
- 0x007805D0 "sub_007805d0" -> NFire::CFirePoint::SetAsSmoldering.
- 0x00793370 / 0x007981B0 / 0x007AFE20 / 0x007B9B40 catalog labels descriptive -> the STLport members above.
- 0x0066F9B0 manifest "CTCPhysicsRigidBody::SetFacingAnglesFromRHSet" -> CTCPhysicsBase::SetVelocity (already landed so).

## Header debt
- engine/CTCInventoryClothing.h flattens the vptr (`void* __vftable`), so 0x005B690B models the class locally (virtual slot 78,
  ClothingItemMap +0x160, ClothingList +0x16C); the header's `CComponent*` is a global `struct CComponent`, not NUISystem's.
- engine/CGameCameraManager.h keeps Camera/OldCamera as byte arrays: 0x0087BA90 uses a local layout model for the inline cut.
- engine/CIVCountedPointeeBase.h: unsigned IVRefCount; with the shared CDefPointer.h the release in 0x007805D0 compiles to
  `mov/dec/mov` either way (signed tried via a guard-defined local base).
- fable_string.h's CCharString(const char*, long) has no default length, so an implicit "literal" -> CCharString conversion
  (which 0x007805D0 needs) is impossible with it.
- STLport list iterator modelled with one template parameter (4.6 has <_Tp, _Traits>); only inline begin/end are affected.
