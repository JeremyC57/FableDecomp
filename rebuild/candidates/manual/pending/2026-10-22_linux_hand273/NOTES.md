# Hand chunk 273 (CTCVillage .. CThingPlayerCreature): 18 of 40 byte-exact

## What matched (18)
Every row below grades RELOCATION_MATCH (gall.sh), and its behaviour test passes under g++ (tall.sh) and VC7.1 (vct.sh).
- 0045912f CThingObjectDef::Transfer
- 004c9d60 CThing::AddTC: tc stays in edi only with an address-taken copy local made right after CreateTC, used for the update-list push_back. Also needed: a __forceinline bitset test, a noreturn abort, a __forceinline CVectorMap::Add/push_back, and a Resort() that sorts without testing Dirty.
- 004e493e CThingBuildingDef::operator=
- 004e9813 CThingBuildingDef::operator=
- 00522880 CThingManager::SaveMainPlayerCreatureState: const-escape stand-ins. The InsertGrow stand-in leaks `this`; the SaveThingList writer stand-in fills first/last.
- 00581765 CTattooDef::IsInvalidCustomTattoo
- 0065b400 CTCVillage::CTCVillage
- 00663610 CThingCreatureBase::PreloadBasicAnimations: the test defines `__declspec(x)` for g++ (mkt.py now does this).
- 00666b50 CThingCreatureBase::GetAppropriateCombatAnimationFromList
- 006a5460 CThingPhysical::GetFirstCollisionBetweenPoints
- 006a62e0 CThingPhysical::InternalModifyHealth
- 006cf1d0 CTCWife::~CTCWife (`// Flags: /GS`)
- 0071b840 CTattooDef::CopyCustomTattooToUserDir
- 0076df90 CTCWeaponTrail::GetStrikeEventIndexFromEventName
- 007b5ca0 CTCWaterWader::IsInWater
- 008326a0 CThingAICreature::UpdateMovement: __forceinline out-parameter GetInterface for the eax-component form.
- 00832e60 CThingAICreature::OnDie: a same-TU GetDef stand-in plus the const-object def release.
- 00a3b730 CTextureRenderer::DoRenderSample

## Parked for Fable 5.1 (21)
- 0054f6e0 CText::~CText DIFFER(506v510): in the inlined member handle destructors, the vtable store and the primitive load are in the wrong order. | tried: explicit Release order, member dtor shapes | next: a user dtor on CEnginePrimitiveHandle that stores the vptr first
- 0065a41c CTCVillage::GetFineAmountForCurrentCrimes DIFFER(383v415): register allocation (this/village/fine), and the null-village exit is merged into the release path. | tried: exit duplication, named locals | next: write every exit out in full, with the village held in a CDefPointer-style local
- 0065aa6f CTCVillage::IsTimeToGoToTavern DIFFER(413v415): register allocation; retail reuses the dead villager argument slot for home, the type and the tavern start. | tried: scoped locals, `push 23h; pop edi` const local | next: put the home/start locals in their own `{}` blocks
- 0065c5bb CTCVillage::UpdateMirroredCrimeData DIFFER(421v421): register allocation only (retail keeps this in ebx; ours puts the hero there). | tried: hero accessor, named locals | next: an inline GetHero() accessor
- 00680050 CThingObjectDef::Validate DIFFER(432v439): retail places the empty-string arm of the inline c_str/length out of line after the epilogue; the static-graphic index goes through edx (ours ecx). | tried: if/return c_str, an inline CRC helper | next: c_str/length as one out-parameter inline helper
- 006a9690 CThingPlayerCreature::ResolveRelativeMovementAcceleration DIFFER(458v450): the flag clear is load/and/store where retail uses RMW `and byte ptr`; the RotateAboutZ `this` lea comes before the float push. | tried: bool bitfield, ternary angle, inline ClearAccelerating | next: flags as an unsigned char member written `&= 0xfe` through a reference
- 006ab430 CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude DIFFER(404v408): which copy of the merged returns stays inline; the sneak component register. | tried: exit reorders | next: write the exits in retail's order with the fabs test first
- 006ab5d0 CThingPlayerCreature::GetCurrentMaxMovementSpeed DIFFER(412v412): the band speed is homed in memory where retail keeps it on the x87 stack. | tried: switch/if forms | next: return the band speed from an inline float helper
- 006d0630 CTCWife::ReceivedNiceGiftIsOpinionReactionHappening DIFFER(472v459): block order (retail puts the not-nice branch after the shared block) and registers. | tried: branch order, goto-free exits | next: invert the nice test
- 006d5b50 CThingAICreature::UpdateAITurning DIFFER(475v479): the turn-state/flags registers are swapped; retail shares one C3DVector frame temporary. | tried: by-value results, copy ctor | next: one named C3DVector result assigned in every arm
- 00703590 CThingObject::DrawPreparePrimitivesForRendering DIFFER(492v492): scheduling only; the orientation interpolator's vptr store sinks past the 0x47 bit test. | tried: const-this cast, GetCarrier helper, user ctors, named bool | next: advisor request sent; try the interpolators as members of one local struct
- 0076e2f0 CTCWeaponTrail::CalculateInterpolatedWeaponHelpers DIFFER(421v421): retail holds the parent in eax via lea (ours ebp); the else-call register. | tried: accessor forms | next: an inline GetParent()
- 007df770 CTCWallMountEffects::vfunc_9 DIFFER(436v437): lane gap (a register-cached frame local); the advisor confirmed that every control-flow-preserving spelling gives the same bytes. | tried: 27 grades (advisor) | next: none (lane gap)
- 00808e40 CThingFilter_IsValidTargetForPlayerCreature::operator() DIFFER(507v488): `this` is spilled; the literal/key temporaries sit in the parameter slot. | tried: same-TU Compare, named keys | next: const char* conversions for the literals
- 0082d3b0 CThingBuilding::PeekPCameraManagerSetDefinition DIFFER(459v455): the retry fixed the block layout (const char* fallback name); one residue remains: the region-def add-ref is `inc dword ptr [edi+4]` in retail, load/inc/store in ours. | tried: prefix/postfix/base-cast increments, a CDefPointer member copy, a mutable count, no pack, an inline AddRef | next: add-ref through an out-of-TU-style helper on the region (`region.GetDef()` returning a CDefPointer)
- 00838ec0 CThingGameObject::GetShotHitParameters DIFFER(448v448): choice of dead argument slot (params vs shot) and copy-register rotation. | tried: statement order, pointer locals | next: scope the key/damage locals
- 0083d990 CThingPhysicalSwitch::ConstructFromLevelScript DIFFER(464v460): slot packing (token vs value) and EndThing block layout. | tried: early-return base test, inverted arm, reader helper | next: token as a named CCharString declared before the loop
- 008565e0 CThingFilter_CombatIsStrikeToStrike::operator() DIFFER(383v406): the def temp/assignment chain (three count stores). | tried: copy/assign forms | next: a by-value CDefPointer temporary assigned into a zeroed local
- 0085d180 CThingMap::InitMapwhoLevels DIFFER(429v481): retail inlines vector<CThingMapCell>::resize only on the size -1 path, packs its temporaries into a 0x24 frame, and keeps the def row pointer in ebp. | tried: in-class resize, auto_inline(off) resize, __forceinline single-arg resize | next: resize(1) as an explicit erase/insert in the body, with the other call through a declared-only resize
- 009c8de0 CTextEntry::ReadFromDataStream DIFFER(459v452): temporaries in the parameter slot, count caching, index CSE. | tried: named locals | next: `*(begin()+i)` indexing
- 009faf70 CTexture::GenerateMipmapsWithGPU DIFFER(407v407): register assignment only. | tried: ++size forms, unsigned level, scoped target | next: none obvious

## Skipped
- 004ca019 CThing::ConstructFromParams: mis-cut row. The real entry is 0x004CA010; the row starts 9 bytes in, after `push ebx; xor ebx,ebx; cmp al,bl`.

## Label corrections
- 0x00449970 "Catch@0040d959" = CPlayerManager::GetMainPlayer
- 0x00436AC0 "Vector_InsertN" = vector<CThing*> growth (_M_insert_overflow)
- 0x005D8E90 = CTCInventoryItem::GetInventoryOwner
- 0x0049D810 "CVertexBufferWin32::DoSizeof" = CWorld::GetPRandomSeed
- 0x00669CB0 "GetInterfaceFromMap" = the animation def's combat-info getter
- 0x00A14440 "GetScaled" = C3DVector::Normalise; 0x00A14040 "Math_CartesianToSpherical" = C3DVector::RotateAboutZ
- 0x00868CA0 "CIME::SetEditBoxRegion" = CHitParameters::AddDamageWithAttribute
- 0x008685F0 "CVillageTaskCarryCrate::AssignCreatureToTask" and 0x00868590 "CObservable::AddExclusiveObserver" = CHitParameters setters (hitting thing, attacker)
- 0x009B9BA0 "CThingCreatureBase::SerialiseGameState" = CStringParser::FindKeyword
- 0x00838930 = CThingGameObject::ConstructFromLevelScript
- 0x00686F10 "CMeshVolumeMap::Find" = the __fastcall TC factory CreateTC(name, thing) called by AddTC
- 0x004CD650 "CGuiVarTransferBool::TransferToGui" = CVectorMap<ETCInterfaceType,CTCBase*>::Resort (sort + Dirty = false)
- 0x004CC820 "ListNode_InsertTCNode" = list<CTCBase*>::push_back; 0x004CC150 = list<CTCBase*>::remove; 0x004CC090 = bitset<274>::set
- 0x0085E8D0 = vector<CThingMapCell>::resize(n, x); 0x0085E870 / 0x0085ED20 = resize of the level-info and cell-row vectors
- Earlier in this chunk: 0x686D30 / 0x686A70 CTCBase world getters, 0x658C5B GetTavernTimes, 0x7EE540 TC 0x23 GetVillage, 0x5937E1 gift-map find, 0x530EC0 CComponent::LoadAndReturnDef, 0x52CCF0 ~CChangingStateComponent, 0x661E80 a CThingAICreature flag query, 0x4C7AB0 CThing::GetWorld, 0x4FB150 GetCurrentRegionIndex, 0x4AB920 / 0x4AB990 camera-set GetDef, 0x843610 UseWallMount ctor, 0x998F60 FilesAreIdentical, 0x401630 ~CDiskFileWin32 copy, 0x5812DF map operator[], 0x6C5630 GetDeedDef, 0x4292C0 tree increment, 0x7DF380 ~CCreatureAction_DeclarePlayerACriminal, 0xA14480 C3DVector length, 0x57A753 TC4 OnHealthChanged, 0x7EA270 TC9 SetFlags, 0x49D9D0 GetMainPlayer, 0x487DC0 GetHeroThing, 0x99EAE0 ~CCharString; CWeaponDef::operator= and CShopDef::operator= labels are wrong
