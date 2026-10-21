# Chunk 252 -- hand authoring notes (40 rows, 321-400 bytes)

22 matched (RELOCATION_MATCH, all with behaviour tests), 18 parked, 0 skipped.

## What matched

| address | function | key to the match |
|---|---|---|
| 00789e60 | CTCHeroRangedAttacks::IsAbleToUseRangedWeapon | out-param GetInterface (const LowerBound 0x4365B0) for the 0xA7 / 0x46 lookups |
| 007ca670 | CTCHeroReceiveItems::PrepareToShowItems | inline `Set` members for the in-place box / RH-set writes |
| 007ca9e0 | CTCHeroReceiveItems::GiveItems | list iterator with a user copy ctor (built in its argument slot) |
| 007e0750 | CTCHeroSpecialMovement::FrameUpdate | inline SetSprinting/SetRunning; free `float&` clamp helpers |
| 00801a50 | CTCIdleScheduler::ChooseNewIdle | same-TU GetDef/GetSubDef stand-ins; thing local named after the def-pointer decls |
| 00583cd4 | CTCInventory::RemoveItemFromInventory | same-TU GetDef/GetSubDef stand-ins; `__forceinline` def-pointer dtor |
| 005be119 | CTCInventoryBase::GetInventoryShoppingValidity | `const C2DVector&` bound to a temp; same-TU IS_THING_ALIVE x3 + LowerBound stand-ins |
| 005bf77d | CTCInventoryBase::GetResurrectionItemInInventory | same-TU GetDef/GetSubDef/GetAllItems stand-ins; hoisted count; free() vector dtor |
| 0056fc5d | CTCInventoryExperience::GlobalUpdate | current fade as a list iterator; raw node compares in the advance loop |
| 005724cd | CTCInventoryExperience::InitialiseExperienceSpendValues | as TU-mate 0x5721FA; inline GetExperiencePointsAvailableToSpend |
| 0057832d | CTCInventoryExperience::CTCInventoryExperience | STLport vector base + alloc-proxy ctor; allocator user ctor |
| 005fad40 | CTCInventoryMap::TeleportHeroToGuildSealRecallLocation | **/GS**; CGameEvent AddFloat/AddBool payload as landed 0x690B20 |
| 0061b3c0 | CTCInventoryQuests::DrawPreparePrimitivesForRendering | virtual IsActive on the 2nd base; for_each(mem_fun_ref); ReleaseTemporary |
| 005cc862 | CTCInventoryStats::GetFatnessAsString | as landed sibling 0x5CCBBD: one return per band, double literals |
| 0059d820 | CTCInventoryTrade::InitialiseSellableItems | out-param GetInterface (const LowerBound 0x4365B0) into one zeroed pointer |
| 005a3436 | CTCInventoryTrade::SellHighlightedItem | non-const LowerBound 0x40F020; virtual RemoveItem slot 79; free() vector dtor |
| 005c44fa | CTCInventoryWeapons::AugmentWeapon | three inlined GetInterface (0x40F020); pack(1) intelligent pointers (ranged +0x141) |
| 005c69aa | CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions | named end in both child loops; free() vector dtors; list(alloc) ctor |
| 00731680 | CTCJackShield::EnableShield | speed TU inline bitset test; implicit `const char*` -> CCharString temp |
| 00751820 | CTCLook::GetHeadSettingsSlowInFastOut | body twin of 0x751970; named new difference |
| 00787f29 | CTCMeleeTrap::InitialiseStrikeAnimation | named manager / world locals; same-TU CBasicString::operator== + Compare 0x411570 stand-ins |
| 0072e130 | CTCMeshAutomaticEntityCreator::CreateParticle | first try: intelligent-pointer temp in the loop gives the `and esp,-8` frame; inlined AddOwnedEntityTC |

## Parked for Fable 5.1

- 00c3db80 CCombatActionManager::CreateIcicleAction (label: not CTCHeroStats) DIFFER(335v335): vptr register swap eax/edx on the two virtual calls; /EHsc | tried: decl order, const& receivers, split statements | next: lane gap, try a 5.1 advisor on the virtual-call receiver form
- 00c3f800 CCombatActionManager::InitializeActorArrays (label: not CTCHeroStats) DIFFER(330v356): dead count==0 operator-new split inside new[] not reproduced; /EHsc | tried: explicit new[] forms, helper | next: model the vector-new helper exactly (0xC3C8C0 is FreeList::Init(block,size,count))
- 0047ed76 CTCInGameMenu::SetToSave DIFFER(366v366): one fsubp / push-ecx scheduling swap before SetPosition | tried: named y (fixes the order but breaks frame slot reuse), temps | next: named y plus a scoped block for the slot
- 0058301a CTCInventory::CreateEditBoxActions DIFFER(321v321): address-taken edit-box copy stored before the vptr load (retail after) | tried: cast placement, local box pointer, statement order | next: build the push_back argument at the call
- 00583a92 CTCInventory::UseContextSensitiveItem DIFFER(312v323): item def / held thing not placed in the dead arg slot; def releases not inlined as retail | tried: holders, stand-ins | next: CDefHolder copy pattern of 0x8C4170
- 00587a41 CTCInventory::DeleteMenuComponents DIFFER(338v338): case-3 discarded HasTC pair `mov eax ; lea ecx,[eax+20h]` vs ours `mov ecx ; add ecx,20h` | tried: HasTC forms | next: discarded GetInterface with the bitset pointer named
- 005f0c5f CTCInventoryAbilities::AssignAbility DIFFER(356v358): find/erase tail key temp copy + map homed in the dead arg slot | tried: key by value / const& | next: by-value Contains helper
- 005f11a9 CTCInventoryAbilities::UnAssignAbility DIFFER(358v375): head CSE of element addresses; removed-ability home slot swapped with the string temp | tried: element refs, index loops | next: named element pointer per access
- 0056f30b CTCInventoryExperience::CloseInventory DIFFER(396v396): zero register (ebx) allocation from the top + zero-vector store placement | tried: init order, zero locals | next: const& idiom on the vector
- 005fb4b0 CTCInventoryMap::TeleportHeroToBoastingArea DIFFER(379v379): /GS; filter temp / out-pointer frame slots swapped | tried: decl order, scopes | next: share the out slot with the float payload temps (block scope)
- 006061b0 CTCInventoryMap::FrameUpdate DIFFER(322v322): active-quest-card loop node/pointer esi-edi swap | tried: loop forms | next: const& node binding
- 005cda50 NInventory::CTCInventoryStats::GlobalUpdate DIFFER(359v366): ebx/edi swap, offset temp elided, final def release dec+cmp vs dec/jne | tried: holders | next: `__forceinline` release + non-template stand-in
- 005cb128 CTCInventoryWeapons::RemoveItemFromInventory DIFFER(392v392): `this` esi vs edi swap only | tried: const& weapon refs, inline helper, owner local, shared carrying ptr, weapon local, predicate helper, true virtual | next: advisor
- 007f0b10 CTCKrakenTentacle::DoDieFX DIFFER(340v340): esi/edi swap + zero-store order | tried: 9 decl-order / const& / statement variants, non-template GetSubDef stand-in | next: advisor
- 00641ae0 CTCLight::DrawFlare DIFFER(331v331): scratch-register permutation in the flare block (halo edx/ecx) | tried: 19 variants (named temp + start pointer fixed the index store) | next: advisor
- 00752300 CTCLook::ImplementBodySettings DIFFER(381v383): creature pointer in ecx vs eax + `mov ecx,eax` | tried: const& creature, local, fastcall test, IsThingAlive helper, if/else, def ref | next: advisor
- 007534b0 CTCLook::IsPosInViewRange DIFFER(384v384): C3DVector frame copy scheduling (ld,ld,st,ld,st,st vs ld,st x3) | tried: ternary, const&, operator=, POD, 3-float ctor | next: copy through a by-value helper
- 007cc2b0 CTCMultiArrow::CreateAmmoEffect DIFFER(347v347): frame-slot packing only -- retail shares the effect intelligent pointer with the def/string word (3-word frame), ours gives the inlined GetInterface key that word (4-word frame) | tried: 18 variants (decl order of def/index/emitter, key as parameter / reference / KeyOf temp, const& bound temp, copy-init, CDefOut+holder, no pack(1), combined condition, explicit helper string) | next: advisor on what makes VC7.1 allocate the 8-byte local first

## Skipped

None.

## Label corrections

- 0x5BF3C6 is CTCInventoryBase::RemoveItemFromInventory(long, long) (43 bytes), not GetItemDescription.
- 0x7F0B10 is CTCKrakenTentacle::DoDieFX, not CTCKraken::CreateHeadEffect (that is 0x7FBB00, `ret 4`).
- 0x7F07A0 is CTCKrakenTentacle::KillRippleFX, not CTCKraken::EndHeadEffects.
- 0x669580 is CDefClassBase::GetSubDef<CSpecialEffectsDef> (COMDAT; the manifest's CAICreatureWillPowerIndicatorDef is folded).
- 0x669D30 is CSpecialEffectsDef::CalcAndLookup(const CCharString&, long*) (manifest "CCRC::CalcAndLookup" propagated).
- 0x787580 is the interface-0x79 component's bool method; CTCSwitchableNavigation::InitialActivate is a no-arg slot.
- 0x4C73D0 is CThing::GetPosition, not CVertexBufferWin32::DoSizeof.
- 0x99A2D0 is CBaseClass() (labelled NHeroInformationScreens::CBase).
- 0x6426A0 is vector<CPrimitive*>::resize(n, const T&).
- 0x4C0050 is CTCGraphicAppearance::AddPrimitive (unnamed).
- 0x686D40 is the CTCBase player-manager getter (labelled GetDisplayEngine).
- 0x661E80 is the creature combat test (labelled CText::GetAlignment).
- 0x70B470 is CTCAnimation::GetAnimateThing const (labelled CWorld::DrawGetEnvironment); 0x70B460 the non-const twin.
- 0x6F90B0 is the animation-manager getter (labelled GFGetPreMainMemoryUsed).
- 0x6F9B20 is the manager's event-name query (long, vector<CCharString>&) (labelled RenderSubPrimitive).
- 0xA242C0 is the world-bank (long) query (labelled CTCInventoryAbilities::GetAbilityLevel).
- 0x414EA0 is vector<CCharString>::~vector (labelled CCombatWheel::ResetRings).
- 0x4A93C0 is CBasicString<char>::operator== (length test + Compare 0x411570).
- 0xC3DB80 / 0xC3F800 are not CTCHeroStats; 0xC3C8C0 is FreeList::Init(block, size, count), not a dtor.
- 0x6D6510 / 0x6D64A0 are CTCHeroExperience getters.
- 0x579B0A (and siblings) are map ctors; 0x5792BF is the list(alloc) ctor.
- 0x6061B0 is CTCInventoryMap::FrameUpdate; 0x61B3C0 is CTCInventoryQuests::DrawPreparePrimitivesForRendering.
- 0x43F360 is _For_each; 0x70C050 is CAnimInit; 0x59D9AD is the trade-list reset; 0x487DC0 is GetHeroCreature.
- 0x5BC4D8, 0x5BC4C4, 0x5BCE2F and 0x42B0A2 carry propagated labels that do not fit their bodies (see the src headers).
