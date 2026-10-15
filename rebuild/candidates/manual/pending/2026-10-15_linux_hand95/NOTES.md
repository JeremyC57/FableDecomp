# Hand chunk 95 (CTCInventoryMap .. CTCJackShield): 39 of 40 byte-exact

All 39 authored rows grade MATCH / RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under both
g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). Every row matched within two grading attempts except
00cbc660 (three). One row is skipped (two functions).

## What matched (39)

- **CTCInventoryMap** (the screen is `CTCInventoryBase` (retail 0x130 bytes) + `NHeroInformationScreens::CBase` (a bare
  vptr at +0x130))
  - 005faa60 IsTeleportValid: display-engine flag && !CBase slot 3. `if (a && !b) return true; return false;` keeps
    the two exits; slot 3 is reached through the secondary base (`lea ecx,[esi+130h]`).
  - 005fae90 RemapUIDsForLoading: remaps each saved teleporter UID through CThingManager::RemapUIDForSerialisation.
    Vector by reference (end re-read), arguments evaluated right to left.
  - 005faf10 vfunc_0 (and its twin 0061a220 in CTCInventoryQuests): the CBase slot-0 override returning the def's title
    text as CWideString. An override of a virtual first declared in the SECONDARY base gets `this` = the sub-object
    (negative member offsets); the def pointer goes through a LOCAL CDefPointer copy (null-tested AddRef, untested release).
  - 005fc840 TeleportHeroToRegionTeleporter: two IS_THING_ALIVE tests around GetRegionTeleporter/TeleportHeroToTeleporter.
  - 005fd0b0 ProcessButtonBReleased: one-shot close latch, else the qualified base call as a tail jump.
  - 006116c0 vfunc_77: void forwarder adding two `false` arguments (cannot tail-jump).
- **CTCInventoryQuests**
  - 0061a0e0 / 0061a100 vfunc_42 / vfunc_43: float getters, 0.5f / 0.035f in test-quest mode, else the base value; the
    base result must go through a NAMED float local or VC emits the sibling `jmp` (retail `call ; ret`).
  - 0061a140 ProcessInventoryInputLeft: `!ActiveTestQuestMode && categories > 0` then the base slot-41 tail jump.
  - 0061a170 vfunc_61: toggles TestQuestShowAll and rewinds both indices (PDB names, retail -0x20) -- exact MATCH.
  - 0061a1a0 / 0061a1d0 vfunc_71 / vfunc_72: wrap-around step of the module-static test-quest index.
- **CTCInventoryStats**
  - 005cc6f3 CloseInventory: stop using / kill the mannequin, InventoryOpen = false (`optimize("s")`).
  - 005cd2fe ~CTCInventoryStats: both derived vtable stores, Kill(false), the CreaturesKilled map destructor, then the
    secondary base's inline `~CBase() {}` re-storing 0x0123A3E0, then the base destructor as a tail jump.
  - 005d7fab vfunc_1 = OnSerialise: base OnSerialise then `context.Transfer("CreaturesKilled2", &CreaturesKilled)`.
- **CTCInventoryTrade** (local view `CTCInventoryTrade_Retail`: engine/CTCInventoryTrade.h is 0x2C..0x50 bytes above retail)
  - 0059b71c vfunc_36: SetNumberOfSelectedItemOwned, the empty base slot 36, ResetInventoryAnim (`optimize("s")`).
  - 0059b740 GetItemCountForCategory: ItemBuyCount / ItemSellCount by category identifier 11 / 12 (two-case switch).
  - 0059c6b0 AddItemOfTypeToInventory, 0059c6dd vfunc_80, 0059c710 vfunc_81, 0059c747 vfunc_79: category for the
    current CategoryIdent, then CCategory::AddItem / RemoveItem. `mov edx,ecx ; call ; mov ecx,edx` = the callee
    (GetCategoryIndexForTradeType 0x0059B6F4) is defined EARLIER IN THE SAME TU, kept out of line with
    `#pragma auto_inline(off)`: VC7.1 then knows it only clobbers eax/ecx.
  - 0059c8ae DeleteComponents: `delete PCurrentOpenTradeMenu; Components.pop_back();` then two front-end manager
    slots and the global node list clear (tail jump).
  - 0059d9c2 ApplyHairCard: `__forceinline` out-parameter GetInterface(0x5E) + AddAppearanceModifierWithReplace;
    `optimize("y", off)` + `("s", on)`.
- **CTCInventoryWeapons**
  - 005c2e02 vfunc_36: empty base slot 36 + ResetInventoryAnim (pass-through pushes, `optimize("s")`).
  - 005c2e82 vfunc_83: active weapon / confiscated def -> SetInLimbo(true), else the base slot 83 (`||` chain with a
    static `__fastcall` helper; PActiveRangedWeapon is genuinely unaligned at +0x141).
  - 005c30d3 RefreshActiveWeaponData: per active weapon IS_THING_ALIVE + GetPItem + the base per-item update.
  - 005c9962 RefreshSheathedWeapons: GetInterface(0x29) with the result ignored (component local initialised to 0),
    then sheath / unsheath; frame pointer kept, `this` in [ebp-4].
  - 005c99be GetTimeWieldedWeapon: a local vector<CItem> filled by GetAllInventoryItemsOfType; first item's per-item
    data +0x10 or 0; the result local is the zero register for the vector's three stores.
  - 005cac8d vfunc_31 = CloseInventory: `for_each(handles, mem_fun_ref(&CEnginePrimitiveHandle::Clear))` (`__fastcall`
    SGI algorithm, hidden functor result in the `push ecx` slot), SetupActiveWeaponsOnCarrier, InventoryOpen = false.
  - 005cb742 ReturnAllConfiscatedItems: base call, then re-equip each confiscated weapon and zero it.
  - 005cbcea vfunc_33: `*handled = false;` then SetThingAsActiveWeapon for augmentations.
  - 005cbd12 SetHighlightedWeaponAsActiveWeapon: signed `> 0` guard on GetHighlightedItem.
- **CTCJackPillar / CTCJackShield** (engine/CTCBase.h)
  - 007319a0 OnInitialActivate and 00731970 vfunc_6 (same slot): engine-globals slot-9 query, Enable(false) /
    EnableShield(false) + ParentThing slot 28.
- **Propagated labels that turned out to be other classes** (see Label corrections)
  - 00cbc660 CVectorMap<u64, CPersistInfo>::Clear: `clear()` then swap with an empty temporary; only a std::swap-style
    `Swap(T&, T&)` helper gives retail's load/store interleaving and ebp/edi colouring; `__forceinline` _Tidy/destructor.
  - 005dedc0 CVectorMap<ulong, CAnimationEntry>::SortAndTrim: `if (Dirty) { sort(begin, end, Compare); Dirty = false; }
    Data.DeallocateUnusedMemory();` with the `__fastcall` SGI sort taking the empty comparator by value.
  - 005c2aa9 vector<NInventory::CItem>::resize(n) = `resize(n, CItem())`; `optimize("y", off)` + `("s", on)`.
  - 00c265b0 pool-resource Release(manager): same-TU register knowledge again (ecx not reloaded after a callee defined
    above with auto_inline off).

## Parked for Fable 5.1 (0)

(none)

## Skipped

- 0061b530 CTCInventoryQuests::vfunc_38: the 96-byte row holds two functions -- slot 38 (0x0061B530, 40 bytes) and,
  after int3 padding, slot 39 (0x0061B560). Both are `if (ActiveTestQuestMode) jmp Move{Previous,Next}TestQuest; switch
  (InventorySelectionState) { case 0: jmp base; case 1: jmp [vtable+120h/11Ch]; }` and should be re-cut and authored
  separately.

## Label corrections

- 00cbc660 "CTCInventoryMap::ResetPrimitiveHandles" -> CVectorMap<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>::Clear (callers in the NScript persist module).
- 005dedc0 "CTCInventoryQuests::ProcessButtonBReleased" -> CVectorMap<unsigned long, CAnimationEntry>::<sort-if-dirty + trim> (callers CAppearanceDef::Transfer, CAnimatingObjectDef::Transfer).
- 005c2aa9 "CTCInventoryStats::OnSerialise" -> std::vector<NInventory::CItem>::resize(unsigned int) (callers CCategory::CCategory, GFSerialiseVectorBinaryIn<NInventory::CItem>).
- 005d7fab "CTCInventoryStats::vfunc_1" -> CTCInventoryStats::OnSerialise(CPersistContext&, bool) (slot 1, calls CTCInventoryBase::OnSerialise).
- 005cac8d "CTCInventoryWeapons::vfunc_31" -> CTCInventoryWeapons::CloseInventory (slot 31, as CTCInventoryStats 0x005CC6F3).
- 00c265b0 "CTCInventoryWeapons::ReturnAllConfiscatedItems" -> a handle-pool resource entry's Release(manager) in the late 0x00C26xxx module (class name descriptive: CPooledResource).
- Callee labels found wrong on the way: 0x005BC4C0 "$E2" (= the empty CTCInventoryBase slot-36 `ret 4`), 0x005BCE2F
  "CWorldMap::DrawGetNoMaps" (= CTCInventoryBase category count), 0x005CBDFA "CVillageTaskCarryCrate::IsCreatureAssignedToTask"
  (= the weapons screen's `thing == pointer->GetPItem()` helper), 0x005BF4ED "CTCInventoryStats::HandleMinigameCompletion"
  (= CTCInventoryBase slot 83), 0x005BF414 "CTCInventoryStats::BuildRenownString" (= a base per-item update taking CThing*),
  0x004518E1 "CActiveFile::OnReadFinished" (= the CreaturesKilled map destructor), 0x005C00A0 "~CTCActionUseBuyOrSteal"
  (= ~CTCInventoryBase), 0x005C1E30 "CActiveFile::OnReadFinished" (= vector<CItem> destructor), 0x004855A3
  "CGuiControlTreePane::SortTreeRecursively" (= the 0x013B8A98 node list's clear), 0x00C265A0 "CManager::GetKeyPressed" and
  0x00C26560/0x00C26580 "CIsThingAlive::operator()" (= the pool entry's IsActive / Activate / Deactivate).
