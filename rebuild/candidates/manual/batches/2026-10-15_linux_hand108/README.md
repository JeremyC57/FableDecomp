# Hand chunk 108 (CThingBodyReorienter .. CThingCreatureBase): 32 of 40 byte-exact

All 32 src/ rows grade MATCH/RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). No pragma/flag sweeps needed except `#pragma optimize("s", on)` on the def assignments.

## What matched (32)
- **CDefClassBase-derived def assignments (15; catalog "CThingBuildingDef::operator=", all propagated).** Real classes found
  by matching the copied member signature against Ego_r.pdb (vectors/maps 16 -> 12 bytes in retail) and through the
  incremental-link `Copy` thunks (`jmp operator=`, vtable slots): CCombatAbilityStrafeDef 00453f63,
  CCombatAbilityUseProjectileWeaponDef 00453f93, CEntitySoundDef 00463ba2, CCarryableDef 004dfeb8, CChestDef 004e0569,
  CFishDef 004e12b8, CBettingDef 004e1918 (byte-identical twins), CFireheartMinigameDef 004e19e2, CBuyableHouseDef
  004e2391, CCreatureGeneratorDef 004e2447, CBossDef 004e2c08, CTurncoatDef 004e2ce4, CClockDef 004e4450,
  CExperienceDef 004e4d88, CFlammableDef 004e5839. Shape: `CDefClassBase::operator=(other)` (0x00431F10) then memberwise
  in declaration order under `#pragma optimize("s", on)`; CDefClassBase is a packed 0x25-byte record (`_body[0x25]`), so
  derived bytes at +0x25/+0x26 sit right behind it. A member record whose base has its own operator=
  (CCombatAbility*Data, CFireEffectCreationSet) must get a written-out `__forceinline` operator= (the DDK out-lines the
  implicit one); the record's `this`/`other` then live in ebx/edi exactly like retail. A member record modelled as a
  `char[]` aligns to 1 and shifts the next member (CSoundMap needed pointer members to align to 4). The 12-byte POD
  C3DVector copies as three `movsd`.
- **CThingCreatureBase (7).** SetLastActionHandedness 006619f0 (`GetWorld()->GetWorldFrame()` with a STATIC frame
  getter: the object call survives, result unused); PerformBlockedReaction 00661ef0 (action pointer as a named local ->
  loaded into ecx; vtable slot 51 OnBlowBlocked); GetStunTimeSeconds 00662040 / GetDazedTimeSeconds 006620a0 (a by-value
  CFloatRange local copied first, `GFFloatRandom(max - min, GetWorld()->GetPRandomSeed()) + min`); MakeNoise 00662110
  (`CThingNoise::Create` static __fastcall, -1.0f pushed first, dead bit +0x91); UpdateLastAttackingCreature 006624a0
  (named `world` local + inline `GetFramesSince(WF)` puts the WF load between GetWorld and GetWorldFrame);
  SetAttackedByCreature 006624f0 (landed SetPTarget intelligent-pointer temporary). Retail layout is 0x1c below the PDB
  around +0xe0..+0x100 (PDef +0xe0, PLastAttackedByCreature +0xf4, WF +0xfc, PCurrentAction +0x100), so local models.
- **CThingBuilding (5).** IsPointInside 0082d000 (physics component as a named local, mesh object by value, bool in bl
  across its destructor); DrawPreparePrimitivesForRendering 0082d080 (slot 36, base call then graphic appearance);
  WorldUpdate 0082d810 (slot 19: CThingGameObject::WorldUpdate then a flag/global `||` chain); Set/GetIsAvailableForResidency
  0082e090 / 0082e0e0 (landed GetInterface out-parameter template, interface 0xd5 = CTCBuyableHouse, IsResidential +0x49 from
  engine/CTCBuyableHouse.h).
- **CThingComponentSet (3).** Add 00870480 (`resize(Size + 1, CEntry())` then `back() = entry`; CEntry is a packed 9-byte
  record), TransferBinaryOut 008708f0 (WriteLong(Size), slot-4 Write(GetData(), Size*9), WriteLong(0x01AB6CCE) -- the
  "address" is an out-of-image end marker), HasDriver 00870980 (SGI `find_if` inline front end over the /Gr `__find_if`,
  value-initialised empty predicate, const-ref tag with a user constructor, result compared in a separate statement).
- **CThingCompare_BestForTargeting2 (1).** Copy constructor 007f9730: memberwise, floats as dwords, the last member assigned in
  the body (in the initialiser list it is stored first).
- **CThingBodyReorienter (1).** UpdateBodyReorienation 00863e10: plain PDB layout, re-tested done flag, `% period == 0`.

## Parked for Fable 5.1 (8)
- 0082d6b0 CThingBuilding::GetMaterial DIFFER(74v64): def-pointer release `mov eax,[esp+4] ; test ; mov esi,eax ; ... mov edx,[eax+4] ; add eax,4 ; dec edx ; mov [eax],edx` vs retail `mov esi,[esp+4] ; test esi,esi ; dec dword ptr [esi+4] ; jne` | tried: raw-pointer inline GetBuildingDef with CDefPointer dtor, dtor through a local copy, CDefVirtual+CRefCounted base (GetSubDef family model), detach-and-release helper (not inlined) | next: lane gap: register choice for the def-pointer slot load (wave-24k "CDefPointer local cached in esi" gap)
- 0082d7d0 CThingBuilding::GetFloorMaterial DIFFER(74v64): same release shape as 0082d6b0 | tried: raw-pointer helper, `__forceinline` detach (`def.Object = 0` + explicit release: right release but the dtor re-reads the slot), handle without destructor (load/dec/store again) | next: lane gap: as 0082d6b0
- 0082d870 CThingBuilding::UsesHighDetailQuadTree DIFFER(74v66): one extra `mov dword ptr [esp+4],0` (the detach store; everything else identical) | tried: raw-pointer helper (76v66), detach store without destructor | next: lane gap: as 0082d6b0 (the store is what makes the lane load straight into esi)
- 0082d8c0 CThingBuilding::GetReverbEnvironment DIFFER(94v84): same release shape as 0082d6b0 | tried: raw-pointer helper, `optimize("s")` + `__forceinline` (frame pointer, `dec [eax] ; cmp [eax],0`) | next: lane gap: as 0082d6b0
- 0083c1e0 CThingSwitch::GetSwitchDef DIFFER(82v76): `inc [eax+4] ; mov eax,[esp+4] ; dec [eax+4] ; mov ecx,[esp+4]` (slot re-read after the add-ref) vs retail one load into ecx and `inc edx ; mov eax,edx ; dec eax ; mov [ecx+4],edx ; mov [ecx+4],eax` | tried: `++`/`--` counted pointer, load/inc/store copy + local-copy destructor | next: lane gap: CDefPointer local re-read from its slot (wave 24k)
- 006c5630 COpinionDeedDefTable::GetDeedDef DIFFER(84v78): same as 0083c1e0 (ushort def-index table at +0x2e of an interface-0x51 component) | tried: the 0083c1e0 form | next: lane gap: as 0083c1e0
- 007f9f00 CThingCompare_BestForTargeting2::operator() DIFFER(94v94): `pop ebx` scheduled after `fnstsw ax` in retail, before it in ours | tried: `GetValue(posB) < GetValue(posA)` (wrong evaluation order, int exits), `if (GetValue(posA) > GetValue(posB)) return true; return false;` | next: lane gap: epilogue scheduling (compiler build)
- 006621c0 CThingCreatureBase::IsFreeToFinishAction DIFFER(68v68): last test `jge` to the false tail with `mov al,1` inline vs retail `jl` to a trailing `mov al,1` after the shared `xor al,al` | tried: local + if chain (77), member reads + `||` return (81), `!(...)` guard block with explicit null test (64, right tail order, missing the second null test), guard block + inline IsPerformingAnyAction (68) | next: guard block + the IsPerformingAnyAction body written inline as `if (!(PCurrentAction && !PCurrentAction->Finished)) return true;`, or the `||` return inside the guard block

## Skipped
- none (no row spans two functions).

## Label corrections
- 00453f63/00453f93/00463ba2/004dfeb8/004e0569/004e12b8/004e1918/004e19e2/004e2391/004e2447/004e2c08/004e2ce4/004e4450/004e4d88/004e5839
  "CThingBuildingDef::operator=*" -> CCombatAbilityStrafeDef, CCombatAbilityUseProjectileWeaponDef, CEntitySoundDef, CCarryableDef,
  CChestDef, CFishDef, CBettingDef, CFireheartMinigameDef, CBuyableHouseDef, CCreatureGeneratorDef, CBossDef, CTurncoatDef,
  CClockDef, CExperienceDef, CFlammableDef ::operator= (in that order).
- 0x00431F10 "CThingBuildingDef::operator=" is CDefClassBase::operator= (copies PDefManager +0x1c, GlobalIndex +0x20, three flag bits +0x24).
- 0x00454886 "CActiveFile::operator=" is std::vector<long>::operator=; 0x00432B38 "CTCCarryable_assign" is map<CCharString,long>::operator=;
  0x004E4EEC "CTCCarryable_Swap" / 0x004E4DC7 "CTCCarryable::operator_assign_4" are map<long,float> / map<long,long>::operator=;
  0x004E5878 "CTCCarryable::operator_assign" is map<CDefString,long>::operator=; 0x0045CFAD "CSoundMap" is CSoundMap::operator=.
- 00662040 "GetDazedTimeSeconds" -> GetStunTimeSeconds (reads StunTimeMin/Max, retail +0x70/+0x74); 006620a0 keeps GetDazedTimeSeconds.
- 0082d8c0 "CThingBuilding::GetFloorMaterial" -> GetReverbEnvironment; 0082d6b0/0082d7d0/0082d870/0082d080 "vfunc_70/40/57/36" ->
  GetMaterial / GetFloorMaterial / UsesHighDetailQuadTree / DrawPreparePrimitivesForRendering (descriptive for 70/57).
- 0083c1e0 "CThingBuilding::GetFloorMaterial" -> CThingSwitch::GetSwitchDef; 006c5630 "CThingBuilding::GetFloorMaterial" -> a
  def-by-deed-type getter of an unresolved interface-0x51 component (named COpinionDeedDefTable::GetDeedDef here).
- 007f9730 "CThingCompare_BestForTargeting2" is its copy constructor; 007f9f00 "operator_less" is operator()(CThing*, CThing*).
- Callees: 0x00838BA0 "CThingCreatureBase::EditWorldUpdate" is CThingGameObject::WorldUpdate (vtable 0x01244F7C slot 19);
  0x006691C0 "CAIStateGroup_MinionPutOutFires::Cleanup" is a CThingCreatureBase clear-last-attacker helper; 0x00993EB0 "WriteCBYTE"
  is CDataOutputStream::WriteLong; 0x00870530 "CWorldDriver_FindClosestMeshVolume" is the SGI __find_if over CThingComponentSet
  entries; 0x00870370 is CSmallVector<CThingComponentSet::CEntry,8>::resize; 0x004C7590 "CRenderTarget::DetachTarget" forwards to
  the thing's graphic appearance (+0x64).
