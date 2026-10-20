# Hand chunk 209 (CTCInventoryBase .. CTCMeleeTrap): 19 of 40 byte-exact

## What matched (19 RELOCATION_MATCH, each with a behaviour test passing under g++ and VC7.1/Wine)

- NInventory frame-pointer size TUs (`optimize("y",off)` + `("s",on)`): 0056fdc3 GetExperienceStatDescription, 005721fa UpdateExperienceSpendLevelUp, 005746f7 SpendExperienceOnStat, 0057713f CTCInventoryExperience::OnCreate, 0059dabb ShowWarningIfNeeded, 005aebfe ProcessTradeAction, 005bd273 GetInventoryText, 005ccbbd GetHeroSexualityAsString.
- NInventory no-frame size TUs (`optimize("s",on)` only): 005747a3 SpendExperienceOnAbility, 005bd4ea OpenInventory (`#pragma pack(push,4)` around the class: a double member would pad the vptr).
- Speed TUs (no pragma, inline bitset::test with `puts("bitset")`/noreturn abort): 005d8d50 AddToInventory, 005d9c90 CTCInventoryItem::OnInitialActivate (vfunc_6), 005fcb10 CTCInventoryMap::OnInitialActivate, 005fd0f0 FindQuestsForActiveRegion, 00602f30 ProcessButtonAReleased, 00753840 TrackNearestInterestingThing, 00753ba0 ImplementHeadSettings, 007f1410 GetShouldStrike, 007f14f0 CTCKrakenTentacle::FrameUpdate (vfunc_9).

Landing notes:
- Pragmas per TU as above; no non-default compiler flags anywhere (every `// Flags:` line is absent/empty).
- 005bd4ea holds PDef through a raw `AddRef`/`Release` inline pair: a CDefPointer local with a destructor forces a frame pointer in a no-frame TU.
- 005aebfe/005bd273 use the landed 0042b374 manager by-value model (CRefCounted DecRef/GetRefCount long form); 005bd273 had a `volatile` cast in an early draft, removed (byte purity) and re-verified.
- Name conflicts to watch: 0x5D9AA0 is `GetTattooCardDef` (catalog says GetHairCardDef; 0x5D9A10 is the hair one); 0x6868A0 is `CTCBase::FrameUpdate` (catalog `$E2`); 0x686870 `CTCBase::OnInitialActivate`.
- Retail CTCInventoryBase layout is the PDB layout minus 4 per preceding vector (documented in the 005bd4ea header).

## Parked (21 DIFFER)

- 0056ef6f CTCInventoryExperience::UpdateExperienceSpendPool DIFFER(207v206): RMW vs forwarded compare value | tried: direct/ref/pointer forms, /Og off | next: likely lane scheduling; try an RTM 3077 toolset
- 00574576 CTCInventoryExperience::OpenInventoryOld DIFFER(182v182): frame slot order of the string temporary vs component local | tried: plain, chained, block-scoped, inline-helper forms | next: slot-order gap; RTM toolset
- 005786b5 CTCInventoryExperience::BuyStat DIFFER(230v226): biased ebp frame (`lea ebp,[esp-70h]`) | tried: /G5 /G6 /G7 /GB | next: lane cannot emit biased frames; needs RTM
- 0059d1b1 CTCInventoryTrade::FinalRefreshSubtitles DIFFER(194v191): string local homed in dead map arg slot, temporaries in dead component slot | tried: plain and named-owner forms | next: QFE slot-sharing gap
- 0059d761 CTCInventoryTrade::FinalRefreshSubtitles DIFFER(194v191): byte twin of 0059d1b1 (PC_SUBTITLE literal) | tried: same two forms | next: land together with 0059d1b1
- 0059da10 CTCInventoryTrade::IsItemAffordable DIFFER(173v171): bool return `setge al` with price in eax vs lane `xor eax,eax` + price in ecx | tried: three compare/negation forms | next: try returning through a long/ternary form or RTM
- 005b3c95 CTCInventoryClothing::OnItemAdded DIFFER(236v229): zero-register CSE of two and-zero stores | tried: four store orderings/types | next: lane CSE gap
- 005b3d7a CTCInventoryClothing::SetThingAsWearingClothingSuit DIFFER(225v212): suit map address hoisted, def spilled | tried: map walk, flat header, CDefPointer walk | next: hoisting gap; RTM
- 005bdf08 CTCInventoryBase::GetNumberOfItemsOfTypeInInventory DIFFER(178v169): def out-slot release form (RMW dec on re-read slot) + zero-register liveness | tried: outlined/inline/member Release/local copy/hand-written release | next: RTM; or a different release idiom in the landed CDefPointer header
- 005beb57 CTCInventoryBase::GetMiniMapGraphic DIFFER(233v233): third key local in [ebp-8] vs dead arg slot [ebp+8] | tried: temporary, scoped, pointer-def, hero-block forms | next: QFE slot-sharing gap
- 005c3180 CTCInventoryWeapons::GetActiveProjectileWeaponDamage DIFFER(188v186): by-value def temporary reuses the out-slot word | tried: forceinline dtor form | next: QFE slot-sharing gap
- 005cbc36 CTCInventoryWeapons::OnItemAdded DIFFER(198v180): weapon def spilled + `+2Ch` hoisted | tried: dtor, hand-written, class-local forms | next: hoisting/spill gap; RTM
- 005cdccb CTCInventoryStats::FrameUpdate DIFFER(187v187): type word and frame local share [ebp-4] | tried: unsigned/long subject (the long form gives the conversion temporaries and the right instructions) | next: QFE slot-sharing gap; 187v187 same instructions
- 005d9070 CTCInventoryItem::GetDescription DIFFER(172v172): text id register / push order around the bank lookup | tried: chain, locals, order forms | next: scheduling only
- 0061a9d0 CTCInventoryQuests::MoveToNextTestQuest DIFFER(168v168): callee-saved register assignment swapped | tried: size()/begin-end, dtor local, named index | next: register assignment only
- 0061aa80 CTCInventoryQuests::MoveToPreviousTestQuest DIFFER(182v176): callee-saved register assignment swapped | tried: two forms | next: sibling of 0061a9d0
- 00751bc0 CTCLook::IsInsideAnimationRange DIFFER(171v169): frame-slot order + def load sunk below Distance2D | tried: named-local, temporary, reference forms | next: QFE slot-order gap
- 00752f30 CTCLook::UpdateHeadCreatureAnim DIFFER(226v226): type ebx / this edi swapped | tried: two forms | next: register assignment only
- 00788406 CTCMeleeTrap::TriggerTrap DIFFER(236v239): vtables and name in registers, this spilled | tried: one form | next: register allocation; worth a second pass
- 007f2410 CTCLightningOrb::OnDie DIFFER(224v205): && with by-value GetPDef copy + string temporary: guard-flag arrangement | tried: plain landed UndeadReturnToEarth form | next: needs the exact landed conditional-temporary idiom; worth a second pass
- 007fba20 CTCKraken::AddStrikeExplosion DIFFER(222v222): this in ebp vs argument cached | tried: push_back, insert(end(),x), word-test forms | next: register assignment only

Groups: register-assignment/scheduling only (0056ef6f, 005d9070, 0061a9d0, 0061aa80, 00752f30, 007fba20, 00788406); QFE/RTM frame-slot sharing or order (00574576, 0059d1b1, 0059d761, 005beb57, 005c3180, 005cdccb, 00751bc0); loop-invariant hoist / def spill (005b3d7a, 005cbc36, 005bdf08); lane gaps (005786b5 biased frame, 005b3c95 zero-register CSE, 0059da10 bool return); idiom not found (007f2410).

## Skipped

None.

## Label corrections (catalog -> evidence)

- 0x485815 GetSubDef<CCreatureDef>; 0x5BADE1 GetSubDef<CInventoryItemDef> (label CAbilityDef); 0x42B1A6 GetSubDef<CWeaponDef>; 0x42AF3C generic CDefinitionManager::GetDef<T> (label is the Drunkenness instantiation); 0x669580 GetSubDef<CSpecialEffectsDef>.
- 0x4ABD90 `__fastcall _Destroy` over 28-byte elements; 0x5FC510 releases the MapScreen primitives; 0x66E799 stores +0x28 of NHeroInformationScreens::CManager; 0x66E7BD GetCurrentScreen; 0x9ACCE0 GetDefClassTypeNameFromGlobalIndex.
- 0x56FFC5 GetTCInventoryAbilities; 0x579F8A vector<ExperienceSpendInfo>::resize; 0x579114/0x57A124 undo map find/insert; 0x5797A4/0x579C35/0x5797FA SExpSpendUndo ctor/copy/dtor; 0x579E4E pair converting ctor.
- 0x686870 CTCBase::OnInitialActivate; 0x6868A0 CTCBase::FrameUpdate (label $E2); 0x51ED80 GetThingWithUID; 0x4CC340 IS_THING_ALIVE<CIntelligentPointer>.
- 0x5BDE96 CCategory::GetNumberOfItemsOfTypeInCategory; 0x5BC4C4 CTCInventoryBase trading-thing getter (label DrawGetEnvironment); 0x5D9AA0 GetTattooCardDef (label GetHairCardDef); 0x5DA530 IsRangedWeaponClass (label HasState/Close); 0x5B1A8D subtitle map find; 0x405F00 IntToWideString; 0x5CC772 GetXString; 0x5D83B3 map<unsigned long,long>::find (label CMemoryAllocatorVariableSize_FindNode); 0x9C95E0 NGameText::CDataBank::GetTextBySymbol; 0x99F570 CCharString operator+ (__fastcall).
- 0x80CBF0 CExplosionDriver::AddExcludedThing; 0x669D30 GetEffectFromString; 0x6E0880 CreateEffectThing; 0x70B460 CTCAnimation::GetAnimation; 0x70C4E0 FindCombinationAnim; 0x70D780 AddCombinationAnim; 0x70D8C0 RemoveCombinationAnimsOfType; 0x5DC340/0x5DC2E0 animation-table GetAnimation/HasAnimation; 0xA242C0 CMeshDataBank::GetAnimation; 0x787C35 SetStrikeTimes (label ConstructString).
- 0059d761 is the PC_SUBTITLE byte twin of FinalRefreshSubtitles (0059d1b1); 007f14f0 vfunc_9 = FrameUpdate; engine/CTCInventoryItem.h +4 "slot" is ParentThing.

Files: `src/` (19), `test/` (19), `differ/` (21, line 1 `// Grade:`), `STATUS.tsv`, `grades.txt`, `payload.json`, `oracle.tsv`.
