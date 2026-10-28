# Retry chunk 540 (CTCInventory* family, 8 rows parked within 8 bytes): 1 of 8 byte-exact

All 8 parked sources were copied to probe/. Each still compiled and was regraded under RTM 13.10.3077 (1 grade each,
g0/). Each row was then worked compile-only with the local `tools/exp.py` harness (retail instruction diff,
`ONLY=`/`SHOW=` filters, `autojunk=False`). Real grades per row: 0058f649 1, 0059fc17 2, 005c0e81 3, 005c71aa 2,
005cdd86 2, 005d8978 2, 005f0dc5 1, 005fb140 1.

## Matched (1)
- 005c0e81 `CPersistTraits<NInventory::CItem>::TransferOut` RELOCATION_MATCH (587 bytes), test PASS under g++ (tall.sh)
  and VC (tall.py). What fixed it:
  - The parked source took pointers. The PDB leaf takes references (`AAVCPersistContext@@AAVCCharString@@ABVCItem`).
  - The augmentation index uses STLport `operator[]` = `*(begin() + n)`, which gives retail's byte-offset CSE
    (`shl ecx,2 ; cmp [ecx+eax],0 ... push [ecx+eax]`). The parked `_M_start[n]` CSE'd the whole address (the 2-byte gap).
  - Every callee now has its PDB spelling: `CPersistTraitsSLONG::TransferOut` (FableWin static SI),
    `CPersistTraits<CCharString>::TransferOut`, `CDefinitionManager::GetDefNameFromGlobalIndex` (const, by-value
    CDefString), `CGameDefinitionManager::Get` (static fastcall), and the `CCharString` operators returning `const&`.
  - The shared headers engine/CItem.h and engine/CDefinitionManager.h are used through the define/derive device.
  - The vland dry run is clean apart from one RELOC_NAME warning: the 0x00415DD0 label (`CDefString::operator_class_CCharString`)
    should be respelled to the PDB `??BCDefString@@QBE?AVCCharString@@XZ` (in NAME_FIXES.tsv).

## Parked (7)
- 0058f649 CTCInventory::OpenPCInventory DIFFER(883v883)
  - residue: (1) the front-end manager takes [ebp-8] and the name temporaries take [ebp-4] (retail swaps them);
    (2) the PTop_Level_List address (edi) is formed before the second name ctor;
    (3) the photojournal arrow's action.first store is scheduled above the vtable load.
  - tried: scope blocks around each name temp (the parked next step), named CCharString locals, the manager declared
    outside the block, before the list pointer or at function scope (all inert); one named temp (worse).
  - next: lane gap: frame-slot order / scheduling (no source form moved either slot).
- 0059fc17 CTCInventoryTrade::AlreadyHaveSpecial DIFFER(715v716)
  - residue: only the tattoo loop's register allocation. Retail keeps the tattoo def in edi, re-reads def->BankIndex
    on each pass, keeps the redundant second bank compare, and stores the count before `sar [ebp-0Ch],2`. Ours hoists
    &def->BankIndex into edi, folds the compare and spills the def.
  - tried: a full PDB-named rewrite (inert, NDIFF 12):
    - the private leaf;
    - the CTCInventoryItem getters returning `CDefPointer<const T>`;
    - the CTattooDef members BankIndex / RemovesAllTattoos / Permanent / CustomFileName;
    - same-TU `GetDef<CThingBaseDef>` and `GetSubDef<CHeroTitleDef>` bodies replacing the invented FindDef_a/FindDef_b
      stand-ins (without same-TU bodies, NDIFF 85);
    - the CTCHero HeroTitleObjectDef check;
    - CTCBase via its header.

    Also tried: compare-order swaps, size() on every pass (36), a signed count, cached bank locals (11, but 6 bytes
    larger), the def by reference, operator-> instead of the def local (23), and the element by const ref.
  - next: lane gap: register allocation / loop-invariant address hoisting.
- 005c71aa CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight DIFFER(680v680) (was 681v680)
  - residue: two scheduling swaps in the TEXT_GUI_MENU_NONE block. Retail emits `push -1 ; lea ecx,[ebp+14h] ; push str`
    and `lea eax,[ebp-14h] ; mov ecx,esi ; push eax`; ours emits the reverse order.
  - tried: a full PDB-named rewrite (CDefClassBase slots from vtable 0x01230BCC, same-TU GetDef/GetSubDef bodies,
    CDefPointer release shapes); about 20 none-block forms (found flag, goto, braces, named temp, c_str, else arm,
    if <= 0, wide temp, ref, arrow), all inert.
  - next: lane gap: instruction scheduling in the none-text block.
- 005cdd86 CTCInventoryStats::CreateExperienceSliders DIFFER(864v864) (was 860v864)
  - residue: frame-slot assignment only (the instruction stream aligns 1:1 at the same size). Retail homes each
    slider's ternary level in [ebp-4], the colour temp at [ebp-8], and the strength value in the dead parent slot
    [ebp+0Ch]. Ours uses a fresh low slot per slider.
  - tried: the PDB-named rewrite; naming the first slider's value and inlining spacing+0x19 per slider (this closed the
    size gap); one level variable shared by all sliders, declared at the top or at first use (inert); the step at the top.
  - next: lane gap: frame-slot packing. The parked "inline per-slider helper" was not tried: no retail function backs it,
    so it would be an invented helper.
- 005d8978 CPersistContext::TransferMap<long, unsigned long> DIFFER(754v754)
  - residue: frame slots. The discarded insert result sits at -0x3C (retail -0x30), so the value, the ";" temp and
    start each sit one slot off. The value traits object takes -0x38 where retail homes both traits objects in the dead
    map slot [ebp+0Ch]. The binary save/load case blocks come out load-first.
  - tried: the PDB-named rewrite (map<unsigned long,long>, CPersistTraits<unsigned long>/<long>,
    CPersistTraitsSLONG::TransferIn, _Rb_global<bool>::_M_increment); traits objects block-scoped, declared before the
    loop, or reversed; traits temporaries (worse); case 3 before case 2 (inert); insert returning pair<iterator,bool>
    (frame +4). The old single-object traits device reached NDIFF 9 against this file's 10, but it is not a real type.
  - next: lane gap: frame-slot packing.
- 005f0dc5 CTCInventoryAbilities::AssignAbilityPC DIFFER(996v996)
  - residue: (1) the old-contents name ternary is a register select, where retail pushes the two literals in the arms
    after `push -1`; (2) the first use-param `new` temp sits in the dead container slot [ebp+10h], where retail uses the
    dead ability slot [ebp+8].
  - tried: eq/ne ternaries; implicit-conversion and two-temp ternaries; if/else of whole FindChild calls (26-31);
    named single or two param variables (107-134); scoped temps (inert); push_back by value (26).
  - next: a form that reaches push-in-arms without moving the manager slot; (2) is a lane gap (frame-slot packing).
- 005fb140 CTCInventoryMap::PanMap DIFFER(873v877)
  - residue: the region-scan loop's allocation. Retail keeps the iterator in edi and the best iterator and distance in
    frame words. Ours puts the best iterator in edi, and the changed prologue then takes a 2-byte loop-head alignment
    pad (`lea ebx,[ebx]`).
  - tried: the earlier 9 grades' forms, plus node-in-loop (worse), assignment orders, nested if, region by const ref,
    size TU (247), /Oy- (40), STLport-spelled `_Rb_tree_iterator` (inert).
  - next: lane gap: register allocation.

## Names
### Proven (two sources each; exact spellings, checked with objdump -t)
- 005c0e81 leaf `?TransferOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABVCItem@NInventory@@@Z`.
- 0x009E2560 `?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z` (FableWin.pdb + call shape).
- 0x009E2590 `?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z` (FableWin order +0x30 + call shape).
- 0x0040E630 `?TransferOut@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABV3@@Z`.
- 0x009ACCC0 `?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z`; 0x00415DD0 `??BCDefString@@QBE?AVCCharString@@XZ`.
- 0x009AD390 `?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z` (donor order + body).
- 0x0042B312 `GetDef<NUISystem::CUIMiscThingsDef>`, 0x0042AF3C `GetDef<CThingBaseDef>`, 0x0042B1A6 `GetSubDef<CWeaponDef>`,
  0x0042AF9E `GetSubDef<CObjectAugmentationsDef>`, 0x005B1E5A `GetSubDef<CHeroTitleDef>` (literals / RTTI-proven callers).
- 0x005D9A10 / 0x005D9AA0 `CTCInventoryItem::GetHairCardDef` / `GetTattooCardDef` (static SI, `CDefPointer<const T>`).
- 0x005D9630 `?GetDetails@CTCInventoryItem@@SI?AVCWideString@@J@Z` (donor order, 0x70 spacing).
- 0x005BE8D3 `?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z`.
- 0x00765F90 `?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z`.
- 0x0059FC17 `?AlreadyHaveSpecial@CTCInventoryTrade@@AAE_NJ@Z` (private); 0x0059B679 `?IsConflicting@CTattooDef@@QBE_NABV1@@Z`.
- 0x0071DD50 `CTCHeroMorph::GetTattoos(vector<CDefPointer<const CTattooDef> >&) const`; 0x0071C2D0 `HasAPermanentTattooCoveringArea`.
- 0x004A93C0 `??8?$CBasicString@D@@QBE_NABV0@@Z`; 0x004292C0 `std::_Rb_global<bool>::_M_increment`.
- 0x005D8978 `??$TransferMap@JK@CPersistContext@@...ABJ@Z` (map<unsigned long,long>, CreaturesKilled2);
  0x005D8185 / 0x005CC6C0 `CPersistTraits<unsigned long>` / `<long>::TransferOut`.
- 0x005CDD86 leaf `?CreateExperienceSliders@CTCInventoryStats@@QAE?AV?$vector@PAVCComponent@NUISystem@@...J_N@Z` (equals Ego_r);
  0x006D6510 `?GetStatExperience@CTCHeroExperience@@QBEJW4EHeroExperienceStatCategory@@@Z`; 0x0056EA9A
  `?GetOriginalExperience@CTCInventoryExperience@@QAEJJ@Z`; 0x00427DC9 `?CreateLevelSlider@CManager@NUISystem@@...`;
  0x00429C6F `?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z` (FableWin).
- 0x005FB140 leaf `?PanMap@CTCInventoryMap@@QAEXVC2DVector@@@Z`; 0x006BBF40 `?GetNameGraphicOffset@CRegion@@QBE?BVC2DVector@@XZ`;
  members pmap_real +0x1B8 (retail +0x188), MapList, MapRegions (Ego_r).
- Members: CTattooDef BankIndex +0x34 / RemovesAllTattoos +0x3C / Permanent +0x3D / CustomFileName +0x4C;
  CHairCardDef HairObject +0x28; CTCHero HeroTitleObjectDef +0x14 (interface 0x29); CTCHeroExperience
  ExperiencePointsAvailableToSpend +0x14 (all Ego_r; retail offsets equal for these).

### Probable
- 0x00706000 `CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier(long, EHeroAttachableAppearanceModifierType) const`:
  the Ego_r signature, called on interface 0x5E with (HairObject, 0). The address is TLC-only.
- 0x00613630 STLport `_Rb_tree_iterator<pair<const long,CRegion>, _Nonconst_traits<...> >::operator++`.
- 0x00581353 `vector<CDefPointer<const CTattooDef> >::~vector` (label says StdList_Dtor; caller destroys GetTattoos' result).
- 0x005D8730 `map<unsigned long,long>::erase(const unsigned long&)`; 0x0045CA09 `insert` is modelled returning the
  iterator (retail homes a 4-byte result; STLport's `insert(const value_type&)` returns `pair<iterator,bool>`). It may
  be an ICF-folded body.
- 0x013B8BA8 anonymous-namespace `NEWLINE` CWideString (FableWin dynamic initializer). The mangled name is TU-hashed,
  so it has no exact spelling.

### Descriptive
- `g_pMainGameComponent` 0x013B86A0; the binary map helpers 0x0045C91D / 0x0045C95A; the CPersistContext text helpers
  0x009BAB40 / 0x009BB0D0 (EntryTransferred / CleanText); enumerator names for EHeroExperienceStatCategory (0..2) and
  EHeroAttachableAppearanceModifierType (0).

## Header debt
- The NUISystem::CComponent / CList / CZoomingComponent / CManager / sound-system / HUD virtual slots are placeholder
  names (`c000`, `l000`, `z000`, `h000`, `s000`, `SetContents_slot60` ...) in 0058f649, 005f0dc5, 005fb140 and 005cdd86. They are not
  relocations, but a shared NUISystem::CComponent vtable header would remove them.
- The CThing interface map: `CInterfaceMap::LowerBound` 0x0040F020 is retail `CVectorMap<ETCInterfaceType, CTCBase*,
  CKeyPairCompareLess<...> >::LowerBound`, and `CPresenceBits::test` 0x00410DE0 is `std::bitset<N>::test`. The landed
  tree uses the same placeholders. The TLC spelling uses STLport iterators, so the Ego_r (Dinkumware) mangled name
  cannot be copied.
- engine/CItem.h is in the global namespace with a flattened CCountedPointer (`PPerItemData_Data/_Info`, type
  `CPerItemData*` global). 005c0e81 needs one cast to NInventory::CPerItemData.

## Label corrections (also in NAME_FIXES.tsv, 29 rows)
- 0x005D8978 catalog `TransferMap<long,long>` -> `TransferMap<long, unsigned long>` on map<unsigned long,long>.
- 0x0042B312 SpecialAbilitiesDrunkennessDef / GetDef_0042b312 -> GetDef<NUISystem::CUIMiscThingsDef>.
- 0x0042AF3C GetDef_0042af3c -> GetDef<CThingBaseDef>; 0x0042B1A6 GetSubDef(CAbilityDef) -> GetSubDef<CWeaponDef>.
- 0x005D9AA0 catalog GetHairCardDef -> CTCInventoryItem::GetTattooCardDef.
- 0x005D9630 GetItemDefDescription -> CTCInventoryItem::GetDetails; 0x005BE8D3 GetItemDescription -> GetPerItemDataForItem.
- 0x009AD390 ActualToUniqueIndex -> GetGlobalIndexFromClassIndex; 0x00415DD0 operator_class_CCharString -> ??BCDefString.
- 0x009E2560 / 0x009E2590 address-named TransferOut_/TransferIn_ -> CPersistTraitsSLONG statics.
- 0x004292C0 TreeNext_004292c0 -> std::_Rb_global<bool>::_M_increment; 0x004A93C0 CStringData::Equals -> CBasicString<char>::operator==.

## Review at landing
- 005c0e81 has no same-TU bodies, and the dry run shows no RELOC_CONFLICT or RELOC_DUPLICATE.
- If a parked row ever matches: 005c71aa and 0059fc17 carry same-TU noinline bodies of 0x0042B312, 0x0042B1A6,
  0x0042AF9E, 0x0042AF3C and 0x005B1E5A, and 0058f649 carries 0x0040F020. Those bodies are already landed, so they would
  raise RELOC_DUPLICATE against the landed leaves. Settle by keeping the landed body.
- The CDefPointer explicit specializations (two-step release for by-value results, one-step for out-param locals) are a
  device: no single CDefPointer class reproduces both shapes.
