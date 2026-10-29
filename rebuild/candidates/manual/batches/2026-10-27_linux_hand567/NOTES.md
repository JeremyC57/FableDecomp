# Hand chunk 567 (CThing .. CTCTavernGame): 2 of 12 byte-exact

12 rows: 2 RELOCATION_MATCH (src/ + test/), 3 parked DIFFER (two re-splits whose true extents are byte-exact, one
771v771 near miss), 7 SKIP (mid-function fragments). No retail_asm rows. vland dry run: 2 RELOCATION_MATCH, behaviour
PASS, no RELOC_CONFLICT / RELOC_DUPLICATE / RELOC_IDENTITY.

## What matched (2)
- **_global** `AddToRelevantBuilding` 0x00502B00 (906 bytes, `__fastcall`, `ret 0Ch`) -- region population's home
  chooser: a GFRandom(2) coin from the world seed; the first building with LiveSpaces > 1 and no occupants wins; else
  the building with the most free places, where an empty one takes the creature if it is multi-family, the creature is
  of type 1, or the coin says so, and a mixed household takes the sex it is short of; then HomeBuilding is set, the
  creature joins the occupant list, and a now-full building is erased from the candidate vector. Shape: the assignment
  tail is at function scope after `if (chosen == 0) return;` (inside an `if` block VC7.1 packed its def pointer into
  the dead `buildings` argument slot: frame 0x1C instead of 0x20); the two sex-balance tests are one `||` (one shared
  break block); the def pointer's release zeroes the pointer; STLport list needs its allocator default argument or the
  mangled name loses `V?$allocator@...`.
- **CTCInventoryStats** `AddHeroAbility` 0x005D64DD (477 bytes, size TU) -- one tick-slider row of the stats screen.
  Shape: `optimize("y", off)` + `("s", on)`, the GetTC / PeekTC templates (the abilities lookup is const, hence the
  const LowerBound), GetAbilityLevel called twice (first result dropped), the slider component read into a block local
  for the dimming store.

## Parked for Fable 5.1 (3)
- 0055a5d0 NUISystem::CDraggable::OnLeftClicked DIFFER(82v697): the row runs past this function's `jmp [edx+0x258]` tail into CScrollBar overrides 0x0055A630 / 0x0055A660 / 0x0055A740 / 0x0055A7C0; true extent 0x0055A5D0..0x0055A621 byte-exact (pfx2 0 mismatches) | tried: true-extent source (WasDragged store first) | next: re-split the catalog row at 0x0055A5D0 (82 bytes) and land as is
- 005e9840 CPersistContext::TransferVector<unsigned long> DIFFER(180v533): the row runs past the jump table into 0x005E9900 (vector length) and the getters 0x005E9930..0x005E9A52; true extent 0x005E9840..0x005E98F3 (code + `mov edi,edi` + 5-entry table) byte-exact (pfx2 0 mismatches) | tried: true-extent source (closed-enum switch with `__assume(0)`, explicit instantiation) | next: re-split the catalog row at 0x005E9840 (180 bytes) and land as is
- 004cebb0 CSubtitledMessage::GetMessagesFromTextName DIFFER(771v771): not-found path only, 5 `lea` offsets: ours builds the ToWideString result at +0x64 and `text` at +0x68, retail the reverse (text in narrator's dead slot +0x64, the temporary in method's +0x68) | tried: cast/no cast, direct-init, const ref (`const CWideString&`), nested block, forceinline helper, explicit operator+, flat push_back, `index == 0` early return, no-else tail, POD tag, named message, whole temporary expression, const text, a dummy read of narrator | next: VC7.1 dead-arg-slot assignment order -- try declaring the else branch's locals in a different scope arrangement (the chunk-567 lesson: function scope vs if-block changes slot packing), or a different parameter type for `method` (an enum vs long changes when its slot dies)

## Skipped (7)
- 004ca019: 9 bytes into CThing::ConstructFromParams 0x004CA010 (landed).
- 005239cd: real start 0x005239C0 (label Thing_SavePersistentState, not landed; the row label
  CThingDeadCreature_SavePersistentState is descriptive).
- 00537e70: inside NUISystem::CList code from 0x005379F5 (enters on the parent's ebp frame, ends in its `leave`).
- 00564d5d: real start 0x00564D50 (42 bytes, uncatalogued; `if (!n) return dst;` then the rep movsd/movsb copy
  returning dst). Name unknown -- not authored (never invent a function).
- 005eadd6 / 005eaf46 / 005eb08d: inside the landed CTCTavernGame::SetStateAfterYesNoAlt 0x005EADC0 /
  SetStateAfterYesNo 0x005EAF30 / SetStateAfterOK 0x005EB080.

## Names
### Proven (two sources each; spellings confirmed with objdump -t of our objects)
- 0x00502B00 `?AddToRelevantBuilding@@YIXPAVCThingAICreature@@AAV?$map@PAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@AAV?$vector@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@3@PBVCDefinitionManager@@AAVCWorld@@@Z` (Ego_r + FableWin name sets; graded)
- 0x005D64DD `?AddHeroAbility@CTCInventoryStats@@QAEXPAVCComponent@NUISystem@@W4EHeroAbility@@AAM@Z` (PDB + donor order; graded)
- 0x0055A5D0 `?OnLeftClicked@CDraggable@NUISystem@@UAEXXZ` (PDB; true extent byte-exact)
- 0x005E9840 `??$TransferVector@K@CPersistContext@@QAEXPBDAAV?$vector@KV?$allocator@K@std@@@std@@@Z` (PDB; true extent byte-exact)
- 0x004CEBB0 `?GetMessagesFromTextName@CSubtitledMessage@@SIXAAV?$CArray@VCSubtitledMessage@@@@PBD1W4ETextGroupSelectionMethod@@@Z` (PDB; 771v771)
- 0x0051BF90 map<CThingBuilding*, list<CThingAICreature*> >::operator[] `??A?$map@...@QAEAAV?$list@...@1@ABQAVCThingBuilding@@@Z` (PDB name sets, VC7.1 `ABQ` form; body = lower_bound + insert)
- 0x00661FF0 `?GetSex@CThingCreatureBase@@QBE?AW4ESex@@XZ`, 0x00662000 `?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ` (adjacent pair in both PDBs, Ego_r 0x53CE40/0x53CE50, FableWin 0x1D9A430/0x1D9A450; bodies read the creature def +0xC0 Sex / +0xC4 CreatureType = Ego_r CThingCreatureDef 196/200 less 4)
- 0x0082D2D0 `?PeekPSimDefinition@CThingBuilding@@QBE?AV?$CDefPointer@$$CBVCSimBuildingDef@@@@XZ`
- 0x01244344 `??_7?$CIntelligentPointer@VCThingBuilding@@@@6B@` (RTTI)
- 0x00A01B10 `??0CBaseIntelligentPointer@@QAE@XZ`, 0x00A01B90 `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z`, 0x00A01B50 `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`, 0x00A01C10 `??1CBaseIntelligentPointer@@UAE@XZ`
- 0x0049D810 `?GetWorldSeed@CWorld@@QBEPAKXZ`, 0x00497890 `?GFROR13@@YIJK@Z`, 0x004CDAE0 `?GFRandom@@YIKKPAK@Z` (inline in 0x00502B00)
- 0x00410DE0 `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`; 0x0040F020 / 0x004365B0 CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound non-const `QAEPAU..` / const `QBEPBU..`
- 0x0056F2BD `?GetExperienceAbilityName@CTCInventoryExperience@@QAE?AVCWideString@@J@Z`, 0x0056EDD2 `?GetExperienceAbilityColour@CTCInventoryExperience@@QAE?AVCRGBFloatColour@@W4EHeroAbility@@@Z`
- 0x005EDE93 `?GetAbilityLevel@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z`, 0x005EDEA3 `?GetAbilityMaximumLevel@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z`
- 0x006D7B80 `?GetAbilityPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroAbility@@AAM@Z`
- 0x00427216 `?CreateTickSlider@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@ABVC2DCoordI@@JJABVCRGBFloatColour@@MM_N32M@Z`
- 0x00429C6F `?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z`; 0x0041E5F2 `?GetInstance@CManager@NUISystem@@SIPAV12@XZ`
- 0x0042D5B1 `??A?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@QAEAAVCUIState@NUISystem@@ABK@Z`; 0x0099B510 `??1CWideString@@QAE@XZ`
- 0x0055AF60 `?OnLeftClicked@CClickable@NUISystem@@UAEXXZ`; 0x0055C860 CMouseCursor::TurnToClosedHand (slot 150, donor order)
- 0x005156D0 `?clear@?$vector@KV?$allocator@K@std@@@std@@QAEXXZ`; 0x0046486F `??$GFSerialiseVectorBinaryOut@K@@YIXAAVCDataOutputStream@@AAV?$vector@KV?$allocator@K@std@@@std@@@Z`; 0x004648A1 `??$GFSerialiseVectorBinaryIn@K@@YIXAAVCMemoryDataInputStream@@AAV?$vector@KV?$allocator@K@std@@@std@@@Z` (by jump-table case: save stream +0x28 / load stream +0x24)
- 0x00404500 `?CheckCRC@CPersistContext@@IAEXPBD@Z`; 0x009BAC80 `?TransferVectorSaveText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorGenericType@@@Z`; 0x009BB110 `?TransferVectorLoadText@...@IAE...` (protected)
- 0x004014A0 `?Calc@CCRC@@SIKKPBXK@Z`; 0x009CC210 `?GetEntryDataType@CBankFile@@QBEKK@Z`; 0x009C8890 `?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z`
- 0x0099EBF0 `??0CCharString@@QAE@PBDJ@Z`, 0x0099EAE0 `??1CCharString@@QAE@XZ`, 0x0099EFB0 `??4CCharString@@QAEABV0@ABV0@@Z`, 0x0099E900 `??9CCharString@@QBE_NABV0@@Z`, 0x0099E480 `?ToWideString@CCharString@@QBE?AVCWideString@@XZ`, 0x0099BFF0 `??H@YI?AVCWideString@@PBGABV0@@Z`
- 0x004CDBD0 `??0CSubtitledMessage@@QAE@ABVCWideString@@@Z`, 0x004D1FE0 `??0CSubtitledMessage@@QAE@ABV0@@Z`, 0x004D1DB0 `??1CSubtitledMessage@@QAE@XZ`, 0x004CEA60 `?GetMessageFromTextIndex@CSubtitledMessage@@SI?AV1@KPBD@Z` (static member, not a free function)
- 0x004D2760 `?_M_insert_overflow@?$vector@VCSubtitledMessage@@V?$allocator@VCSubtitledMessage@@@std@@@std@@IAEXPAVCSubtitledMessage@@ABV3@ABU__false_type@2@I_N@Z`, 0x004D2880 `?push_back@?$CArray@VCSubtitledMessage@@@@QAEXABVCSubtitledMessage@@@Z`
- Members: CThingAICreature::HomeBuilding +0x168 (engine header), CSimBuildingDef Property/WorkSpaces/LiveSpaces +0x3C/+0x40/+0x44 (Ego_r less 4, landed sibling 0x005003E0), CThingCreatureDef Sex +0xC0 / CreatureType +0xC4.

### Probable
- 0x009CC410 `?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z` (PDB QBEKK + semantics; retail position differs from donor order).
- 0x013B8A50 `CSubtitledMessage::MostRecentGroupQueried` (name proven by the PDB dynamic initializer / atexit names; access level unproven, declared private: `?MostRecentGroupQueried@CSubtitledMessage@@0VCCharString@@A`).
- ESex enumerators SEX_NONE / SEX_MALE / SEX_FEMALE = 0 / 1 / 2 (landed sources; 1/2 semantics from this body's balance logic).
- ETextGroupSelectionMethod enumerators GROUP_SELECT_NONE / RANDOM / RANDOM_NO_REPEAT / SEQUENTIAL / FIRST_ONLY (FSE mod scripts).
- 0x004291DE `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` (could be ReleaseData; see Review).

### Descriptive
- `g_MostRecentGroupIndex_01378e24` (0x01378E24, .data, initial -1): the last group pick.
- ECreatureType value 1 (compared, no enumerator name proven); local names chosen, coin, mostFree, spaces, males, females.
- 0x00564D50 (the SKIP row's real function) has no name.

## Label corrections
- 0x005E9840 "CPersistContext::Transfer<signed_char>" -> CPersistContext::TransferVector<unsigned long>.
- 0x004CEBB0 "_global::GetMessagesFromTextName" -> CSubtitledMessage::GetMessagesFromTextName (static, `SI`).
- 0x0082D2D0 "PeekPMaterial" -> CThingBuilding::PeekPSimDefinition.
- 0x0051BF90 "std::map::insert" -> map<CThingBuilding*, list<CThingAICreature*> >::operator[].
- 0x00661FF0 / 0x00662000 "CPixelShader::CountReferences" -> CThingCreatureBase::GetSex / GetCreatureType; the labels_chunk218 "CThingCreatureBase::GetMovementMode" for 0x00662000 is wrong.
- 0x0049D810 "CWorld::GetPRandomSeed" -> CWorld::GetWorldSeed.
- 0x00429C6F "CCountedPointer_SetPItem" -> CCountedPointer<NUISystem::CComponent>::Reset.
- 0x005EAF46 "CTCTavernGame::SetStateAfterYesNoAlt" -> fragment of SetStateAfterYesNo 0x005EAF30.
- 0x005239CD / 0x00537E70 / 0x00564D5D / 0x004CA019 / 0x005EADD6 / 0x005EB08D: fragments, not functions.
- 0x0050F3A0 "CLandscapeBackgroundPatch vector deleting destructor": slot 0 of the CIntelligentPointer<CThingBuilding> vtable (likely an ICF-folded `??_E` body shared by several classes -- not renamed).

## Header debt
- No engine header for CSimBuildingDef / CThingCreatureDef / CThingBuilding: local models (retail offsets, Ego_r names).
- engine/CThingAICreature.h types HomeBuilding as `unsigned char[8]`: the source re-declares the class (rename trick)
  with `CIntelligentPointer<CThingBuilding> HomeBuilding` and asserts the offset against the header.
- CWorld / CThingCreatureBase / CManager headers are included under a rename (#define X XLayout) because the sources
  only need a method declaration; the classes they declare are empty method shells.
