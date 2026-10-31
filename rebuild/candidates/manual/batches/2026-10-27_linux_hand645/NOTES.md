# Hand chunk 645 (CActionEventTriggerParticle .. CRotateAround): 0 of 10 byte-exact

All ten rows are parked retry rows (same-length misses from the 2026-10-30/31 sweep). Each baseline was reproduced first
(10/10 same grade as the brief), then one lever per grade. No row matched; four rows moved closer (00b847d0, 00870e00,
004888c0, plus PDB name respells on 00870e00 / 00838ec0 / 004888c0). Best sources are in differ/ with the residue on line 2.

## What matched (0)
- None.

## Parked for Fable 5.1 (10)
- 00b847d0 CEngineSubPrimitiveRenderer::RadixSort DIFFER(444v444): `xor ebx,ebx` (shift init) between the hoisted `lea eax,[esp+1028h]` and `sub eax,4` (retail after both hoisted base-pointer stores) | tried: named `radix` + `counts[radix] = counts[radix] + 1` (fixed the whole histogram loop), named radix in the scatter loop with `offsets[radix]++` or `index = offsets[radix]; offsets[radix] = index + 1` (fixed the scatter loop), pointer-to-offset form (447), shift declared at entry / before the loop / do-while (=) | next: scheduling tie in the shift-loop preheader; try a different prefix-sum loop shape that changes the hoisted set, else lane gap: preheader scheduling
- 00870e00 NCreatureAction::CActionEventTriggerParticle::SetDataFromString DIFFER(438v438): `mov esi,[esp+10h] ; test esi,esi ; ... mov ecx,esi` v ours `mov ecx ; test ecx ; ... mov esi,ecx` (the copy now sits inside the non-null arm, as retail) | tried: named `CBasicString<char>* typeString = &typeData->Data` in the arm (moved the copy into the arm), flipped null test (=), member pointer from type.PStringData (=), const reference (=), pointer declared before the test (=), PDB respell (namespace, protected virtual, anonymous-namespace GetDataFromEventString with CCharString&) (=) | next: lane gap: register colouring (VC copy-propagates the arm's pointer into the first call, giving the load ecx); the twin 0x00871290 should take the same arm-pointer form
- 004888c0 CPlayer::SwitchPlayerCharactersWith DIFFER(443v443): this in ebp / the &Controlled-vtable-other web in ebx (retail ebx / ebp), push order follows | tried: named `long number = Number;` after each SetupInitialPlayerMode() (fixed the Number loads into the dead this/other register), PDB CPlayer& parameter (=), named reference to other's character pointer (=), IS_THING_ALIVE as an inline function (411) | next: lane gap: callee-saved colouring tie between two webs
- 00ba50a0 CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks DIFFER(441v441): `mov byte [esp+0Fh],0` (failed = false) emitted after the first mesh load instead of after `mov edi,[primitive]` | tried: failed defined after bank/meshData/flag (store right, but failed then takes the dead mesh slot and flag the frame), uninitialised declaration + late assignment (same), a meshFlags copy so flag is defined after failed (same), failed before the UseBlankTextures test (store hoisted above the first `je`), failed between bank and meshData (load order changes) | next: find what makes retail give the dead mesh slot to flag while failed is defined late (a `bool` flag accessor on the mesh, or flag as a bitfield read), else lane gap: frame packing order
- 004feec0 CWorldMap::UnloadRegion DIFFER(444v444): `imul eax,eax,58h ; lea ebx,[eax+ecx]` (region in ebx, map/j in ebp) v `imul ebp ; add ebp,[Start]` (region in ebp) and every later ebx/ebp use | tried: named `CRegion* regions = Regions.Start` (=), `*(Regions.begin() + region)` (=) | next: lane gap: callee-saved colouring tie
- 0095b820 CAIStateGroup_HoverHornetQueen::Spawn DIFFER(445v445): filter index store after the PCreature load (retail between the PBrain and PCreature loads); 0xad lookup `lea edi,[eax+44h]` v `mov edi ; add edi,44h` | tried: filter built straight from the GetDefGlobalIndexFromName call (443, frame +4) | next: lane gap: scheduling + creature/map web coalescing
- 00853670 NCameraModes::CScriptModuleOpenChest::OnInitLeaf DIFFER(445v445): `mov edx,[esp+18h]` before `fstp [esp+1Ch]` (retail after); AxisX.Z store `mov [eax+8],edx` after Theta/Rad (retail before Theta) | tried: operator- through a named result (455), PerpendicularXY through a named local (451) | next: lane gap: scheduling
- 00872110 NCameraModes::CStringAndCageBase::UpdateAutoGoBehind DIFFER(447v447): side test `fld [esp+10h] ; fmul [esp+18h]` v retail `fld [esp+18h] ; fmul [esp+10h]` (behind.X * dir.Y) | tried: DotProduct(perp, behind), Z,X,Y-ordered DotProduct body, explicit Z,X,Y sum over a named perp (all identical: the order is canonical), perp named before `behind` (455), perp declared at entry (487) | next: lane gap: x87 operand order (canonical for every source spelling tried)
- 00838ec0 CThingGameObject::GetShotHitParameters DIFFER(448v448): second 0x77 key and the damage float in the dead params slot [esp+28h] (retail the dead shot slot [esp+20h]); Position/Direction copies rotate eax/ecx/edx with `mov byte [esi],1` before the last store in retail | tried: PDB respell (const, NHitLocations::CHitLocation* first parameter, CThing& shot, reference setters, EDamageAttribute) (=) | next: lane gap: frame packing + copy register rotation
- 0087bd70 NCameraModes::CRotateAround::Update DIFFER(451v451): frame 0x44 v 0x40; thing Z at [esp+8] v [esp+40h]; HeightOffset stored at once to its own word v kept in eax and stored into the dead XAxis*cos Z word | tried: HeightOffset added in a second statement (`position = position + ...` 461; `+=` 451 but frame 0x38; `position.Z +=` 417), centre as focal copy with Z replaced (475), whole thing-position copy (464) | next: lane gap: scalar temp packing (retail packs thing Z with the second sum's Z and HeightOffset with cos.Z)

## Skipped
- None.

## Names (differ files)
- 0x00870E00 `?SetDataFromString@CActionEventTriggerParticle@NCreatureAction@@MAE_NABVCCharString@@@Z` (Ego_r.pdb name set; objdump-confirmed leaf).
  0x00870C90 spelled as agent643's anonymous-namespace `GetDataFromEventString(const CCharString&, long, CCharString&)` (__fastcall).
- 0x004888C0 `CPlayer::SwitchPlayerCharactersWith(CPlayer&)` (Ego_r.pdb QAE_NAAV1@@Z); the differ uses the landed CPlayer_Methods carrier
  (so it mangles the carrier) like the landed CPlayer_InitCharacterAs.
- 0x00838EC0 `?GetShotHitParameters@CThingGameObject@@QBEXPAVCHitLocation@NHitLocations@@AAVCThing@@ABVC3DVector@@AAVCHitParameters@@@Z`
  (Ego_r.pdb + FableWin.pdb). The first parameter is the hit location, not an attacker: it is passed to 0x00868590 SetHitLocation
  (stores CHitParameters::PHitLocation +0x48) and queried with 0x00815280 IsToCauseDecapitionOnHit. The shot TC queries
  0x007BED70 / 0x007BED80 / 0x007BEE20 (bytes `mov al,[ecx+66h]`, `mov al,[ecx+67h]`, `mov eax,[ecx+38h] ; mov al,[eax+91h]`)
  keep descriptive names (IsKnockDown / IsDecapitate / IsBlockable) -- not proven.

## Label corrections
- 0x00815280: manifest "CTCShotDefinitionBase::CanBeHitByMeleeStrike" -> `NHitLocations::CHitLocation::IsToCauseDecapitionOnHit() const`
  (PDB name set; body `mov eax,[ecx+4] ; mov al,[eax+6Dh]` = PDef (+4) -> DecapitateOnHit (Ego_r CHitLocationDef +0x71, retail +0x6d);
  called on GetShotHitParameters' hit-location parameter). The body is a two-load getter: check label_trust before propagating.
- 0x00868590: manifest "CObservable::AddExclusiveObserver" -> CHitParameters::SetHitLocation(CHitLocation&).
- 0x008685F0: manifest "CVillageTaskCarryCrate::AssignCreatureToTask" -> CHitParameters::SetThingHitting(CThing&).
- 0x00868CA0: manifest "CIME::SetEditBoxRegion" -> CHitParameters::AddDamageWithAttribute(EDamageAttribute, float).

## Header debt
- 0x00838EC0 and the inherited 0x0095B820 / 0x004FEEC0 differ files keep local models; 00838ec0 still carries the inherited
  `#pragma pack(push, 1)` for CHitParameters (2-byte vector offsets; Ego_r agrees) -- replace with the byte-image + reference
  accessor lever before any landing. The 00870e00 differ dropped the inherited pack (only PData/StringLength are read).
