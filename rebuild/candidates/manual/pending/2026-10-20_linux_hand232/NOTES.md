# Hand chunk 232 (CTCInGameMenu .. CTCInventoryQuests): 15 of 40 byte-exact

## What matched (15)
- CTCInGameMenu
  - 0047e07f ~CTCInGameMenu -- unregister, delete the menu components, clear the two video dialogs, front-end hooks, members in reverse (local model: retail 12-byte vectors). Shape: the component clear is an inline `clear() { erase(begin(), end()); }` (end() addressed through edi); POD vector dtors `__forceinline`.
- CProgressDisplay (catalog CTCInventoryMap::ResetPrimitiveHandles)
  - 00499fb0 ~CProgressDisplay -- StopProgress, then texture/string vectors and strings in reverse, base vtable last. Shape: inline STLport `_Destroy(first,last)` element loop + `if (start) free(start)`; user inline base dtor keeps the CAProgressDisplay vtable store.
- CTCInventoryExperience
  - 0056e75f GetExperienceAbilityMovieName -- if-chain of `return CWideString(L"...")`, tail-merged ctor.
  - 0056edd2 GetExperienceAbilityColour -- three local id tables; `!=` loop bounds give retail's `jne` (the strength scan really stops after 5 of its 7 ids).
  - 0056f177 GetExperienceStatPic -- if-chain into a CCharString local; the "UI_RING_PIC" literal converts implicitly (temp in the dead arg slot); bank counted-pointer copy.
  - 0056fad7 OpenInventory -- landed SaveExperienceValuesForCountDown GetInterface model; uninitialised out local.
  - 0057229b UpdateExperienceSpendStat -- def copy local (no-null-test release) + nested if/else clamp chain.
  - 0057238f UpdateExperienceSpendAbility -- ability twin; needed `#pragma optimize("s", on)` WITHOUT ("y", off) (esp frame, size codegen).
- CTCInventory
  - 00583bd5 OpenQuickAccessMenu -- three blockers + menu open; the two const LowerBound lookups go through an inline `GetConstParent() const`.
- NInventory::CTCInventoryBase
  - 005bfb96 Copy -- `__forceinline` CDefPointer operator=; `count = v.size(); T* p = v._M_start; for (...; ++p)` for retail's count-down/pointer-walk loop.
- CTCInventoryAbilities
  - 005edd04 GetAbilityInventoryThingName -- dense switch 1..19 in source order, jump table inside the row (q.py handles it).
  - 005ee887 UpdateWillMorph -- two inline GetInterface lookups; the entry pick must be ONE ternary for the second lookup's register map.
- CTCInventoryMap
  - 00602e30 OnCreate (catalog vfunc_4) -- the landed CTCInventoryWeapons::OnCreate def dance + inline clear() of three handle vectors.
- CTCInventoryQuests
  - 0061ac60 DeactivateSelectedTestQuest, 0061ab30 ActivateSelectedTestQuest -- same-TU stand-in for GetTestQuestsToDisplay (it reads and writes the vector only through its out pointer) so the vector start stays in edi across the quest-manager calls; `"Data\Levels\Ini\" + file` through the __fastcall operator+.

## Parked for Fable 5.1 (24)
- 005fbea0 CTCInventoryMap::GetRegionTeleporter DIFFER(242v248): found/key slot sharing, hoisted Bind2 address stores, this in ebp | tried: Bind2 refs, operator&& chain, function-scope filter locals (fixes the aligned frame) | next: lane gap: 8-byte filter temporaries inside a loop force an `and esp,-8` frame under 3077
- 005fbd90 CTCInventoryMap::TeleportHeroToGuild DIFFER(258v258): `mov eax,[eax+20h] ... mov ecx,eax` v retail `mov ecx,[eax+20h]` between the pushes | tried: local-head list ctor, same-TU GetAllThings stand-in, inline search helper, named search-tools local | next: an inline `GetThingSearchTools()` returning `GetWorld()->ThingSearchTools`
- 005fbfa0 CTCInventoryMap::TeleportToActiveRegion DIFFER(274v277): frame 0x1C v 0x18 (retail packs thing/key/TC into one slot) | tried: function-scope filters | next: lane gap: same 8-byte loop-temporary family as 005fbea0
- 005fb7e0 CTCInventoryMap::ConstructRegionDescriptionMapKeys DIFFER(276v270): final def release `mov eax,[esp+10h] ; mov edx,[eax+4] ; dec edx` v `mov ecx,[esp+10h] ; dec [ecx+4]` | tried: ternary temps, if/else + implicit names, Release(), DecRefToZero, forceinline dtor | next: lane gap: def-release load register
- 005785b3 CTCInventoryExperience::BuyAbility DIFFER(262v258): retail biased frame `lea ebp,[esp-70h]` | tried: speed, y-off, y-off + s | next: lane gap: biased ebp frame
- 005ee77c CTCInventoryAbilities::GetExperienceCostToUpgradeAbility DIFFER(258v267): bare dword `fistp` after a float store/reload | tried: one C++ form | next: lane gap: inline-asm FloatToLong (asm_bake family)
- 005ee99d CTCInventoryAbilities::IsAbilityQuickAccessEventInQueue DIFFER(269v254): `this` kept in edx across GetAbilityPDef | tried: natural, same-TU GetAbilityPDef + end()-begin() | next: lane gap: LTCG register knowledge of the callee
- 005eede2 CTCInventoryAbilities::InitialiseAbilities DIFFER(279v260): "HERO_ABILITY" literal CSE'd into ebx, this esi, i in ebx; retail pushes the immediate twice and sinks `push ebx` | tried: forceinline operator=, block-scoped count, function-scope count | next: register-allocation probe (named type-name local?)
- 005f3bcf CTCInventoryAbilities::CTCInventoryAbilities DIFFER(303v303): vector<long> fill-value temporary in [ebp-4] v retail's dead `thing` slot; one `lea` scheduled earlier | tried: by-value forceinline wrapper, default-argument value | next: lane gap: temporary homing order
- 005f4b30 CTCInventoryAbilities::GetPrioritisedAbilityDefs DIFFER(300v277): padding loop's null-def temp: retail hoists its zero store and drops its dtor | tried: forceinline dtor (fixed the first loop), const null local, same-TU push_back stand-in | next: a non-RAII null element (CDefPointerSlot view) pushed by const&
- 005f694d CTCInventoryAbilities::ResetAssignedAbilities DIFFER(292v290): final def release through eax (`dec [eax+4] ; mov ecx,eax`) v ecx | tried: explicit swap (fixed), named pointer in dtor, DecRefToZero, forceinline | next: lane gap: def-release load register
- 005bcd2d CTCInventoryBase::ResetInventoryAnim DIFFER(268v258): our /Os build keeps an ebp frame (retail esp) so the last set cannot live in ebp | tried: POD set, non-POD set + size TU, const& result, speed TU | next: lane gap: /Os frame-pointer choice with an address-taken 24-byte result
- 005be028 CTCInventoryBase::OnItemAdded DIFFER(236v241): retail spills `this` to [ebp-8] and keeps parent/events/type in esi/ebx/edi | tried: zero-preloaded GetInterface, named parent local | next: register-allocation probe
- 005be284 CTCInventoryBase::SetNumberOfSelectedItemOwned DIFFER(298v301): global esi/edi swap (this v hero) | tried: converted CThing* temp for IS_THING_ALIVE (fixed the hero copy), ternary pick, named self | next: register-allocation probe
- 005bf654 CTCInventoryBase::AddItemToInventory DIFFER(301v297): both title-def releases load eax not ecx | tried: forceinline zeroing dtor, nested success path (fixed the layout), DecRefToZero | next: lane gap: def-release load register
- 005c0121 CTCInventoryBase::OnCreate DIFFER(296v297): literal CSE register ebx v edi and the fetched def through eax | tried: explicit names, implicit names | next: register-allocation probe
- 005b36e9 CTCInventoryClothing::OnCreate DIFFER(280v270): element read before GetPosition (retail after, start in edi) + long `--def->m_refCount` release | tried: by-value accessor, named position ref, direct start index | next: inline `begin()`-based accessor with the start CSE'd from size()
- 005b413d CTCInventoryClothing::SetAsWearing DIFFER(311v286): frame packing (modifier in the dead `defName` slot, wear flag in its high byte) | tried: early return (dtor duplication), key copy + nested wear | next: lane gap: live-range frame packing
- 005b4795 CTCInventoryClothing::SetUpSelectedClothes DIFFER(272v269): modifier out slot not packed onto the def slot; esi/edi swap | tried: out-slot views, block-scoped def slot (worse) | next: lane gap: live-range frame packing
- 005b5be6 CTCInventoryClothing::OnItemHighlighted DIFFER(285v274): item index enregistered in edi (retail keeps it in [ebp+8], def cached in edi) | tried: out-slot views, named thingDef local | next: register-allocation probe
- 005b6767 CTCInventoryClothing::GetItemDescriptionArmourRating DIFFER(282v282): `mov eax,[g_pMainGameComponent]` scheduled before the difference load in retail | tried: implicit symbol, named bank local (after / before the difference) | next: lane gap: scheduling only
- 005b690b CTCInventoryClothing::RefreshDescriptions DIFFER(290v290): edi/ebx swap in the second pass only | tried: shared iterator/end locals (fixed the slots), function-scope component, lookup on *it | next: register-allocation probe
- 00591a66 CTCInventory::OnSerialise DIFFER(281v281): `cmp al,bl` v `test al,al` on the saved IsSaving bool | tried: alloc-proxy vector (fixed the store order), body-ordered zero stores, bool local | next: `unsigned char`/const flag, or the vector in a block after the call
- 0061a8a0 CTCInventoryQuests::GetTestQuestsToDisplay DIFFER(283v290): filter-loop registers (finish v element offset in ebx) | tried: _M_insert_overflow stand-in, no stand-in | next: register-allocation probe

## Skipped
- 005d9380 CTCInventoryItem::vfunc_4: three functions in one row (0x005D9380, then 0x005D93B0 after int3 padding, then 0x005D9430); ledger.

## Label corrections
- 0x00499FB0 CTCInventoryMap::ResetPrimitiveHandles -> CProgressDisplay::~CProgressDisplay (only caller ??_G 0x0049B7D0 = slot 0 of CProgressDisplay vtable 0x012388B4).
- 0x00602E30 CTCInventoryMap::vfunc_4 -> CTCInventoryMap::OnCreate (vtable 0x01257A9C slot 4; calls CTCInventoryBase::OnCreate).
- 0x00485AA7 CGuiControlListBox::SelectEntryFromData -> vector<NUISystem::CComponent*>::erase(first, last).
- 0x005BC6ED "_Dest_val<...CRegisteredMusicEntry>" is a CTCInventoryBase screen hook called from CTCInventoryExperience::OpenInventory.
- 0x005795C3 "CTCCarryable::OnKill" is a map clear (CTCInventoryExperience::TutorialSoundHandles +0x134).
- 0x005EE3B3 "CCreatureAction_CastStrengthMultiStrikeProper ctor" is CTCInventoryAbilities::GetUIMiscThingsDef() (CDefPointer by value).
- 0x005F9DF8 "CArray<pair<long,long>>::push_back" is vector<EHeroAbility>::resize(n, value); 0x005F8F89 "CRGBColour_CopyAssign" is vector<EHeroAbility>::erase(first, last); 0x005F9BCD is vector<EHeroAbility>::push_back.
- 0x005BD993 "CRenderTarget::DetachTarget" is a NInventory::CCategory getter (per-slot item maximum).
- 0x0071BE80 "CTCHeroMorph::SetAge" is used as the will-morph amount setter (name unproven); 0x006D6520 "CTCInventoryAbilities::GetAbilityLevel" is a CTCHeroExperience stat-level getter (called on the 0x68 component).
- 0x004A0940 "NQuestManager_ActivateQuestWithFilters" is called on CTCBase::GetWorld() with the test quest's HSP name (a CWorld HSP loader).
- 0x0057914C "BinaryTree_FindLowerBound_Ability" is map<EHeroAbility, SExpSpendUndo>::find (node pointer result).
- 0x00416359 "CWorld::DrawGetEnvironment" is a CMainGameComponent getter (`mov eax,[ecx+20h]`: the game player interface).
