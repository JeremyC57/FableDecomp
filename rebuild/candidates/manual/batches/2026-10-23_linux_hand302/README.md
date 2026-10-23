# Hand chunk 302 (CTCHeroStats / CTCInGameMenu / CTCInventory* / CTCInventoryMap / CTCHitOnCollision, 241-1024-byte band)

40 rows: 17 matched (RELOCATION_MATCH, g++ + VC7.1 behaviour tests pass), 21 parked in `differ/`, 2 skipped.

## What matched
- 0047d78b CTCInGameMenu::CloseInGameMenu (/GS)
- 0056f69f CTCInventoryExperience::AddMagicCategoryToList
- 0056f8a4 CTCInventoryExperience::AddMagicCategoryToListPC
- 0057b783 CTCHeroStats::Vomit
- 0057bc6e CTCHeroStats::SetMarriageState
- 0057be71 CTCHeroStats::CheckForNewExpressions
- 0057c2e6 CTCHeroStats::SetMorality
- 0057cb7e CTCHeroStats::ProcessDrunkenness
- 0057cdb5 CTCHeroStats::CTCHeroStats (ctor; declaration-order initialiser list, `resize(n, 0)`)
- 0057d5a0 CTCHeroStats::FrameUpdate
- 0058216e CTCInventory::SetEditBoxValues
- 00582414 CTCInventory::DeactivateAllEditBoxes
- 00589d30 CTCInventory::CloseInventory
- 005b37f7 CTCInventoryClothing::InitialiseMannequin
- 005b4a95 CTCInventoryClothing::GetCurrentTotalArmorValue (test rewritten with dyadic floats: x87 vs SSE truncation differed)
- 005be3b1 CTCInventoryBase::GetPrerequisitesString
- 005be62c CTCInventoryBase::GetPrerequisitesString (overload)

## Parked for Fable 5.1
- 0047e651 CTCInGameMenu::VideoOptionsChanged DIFFER(752v752): SetPosition `push ecx` scheduled before the X `fsubp` (retail after) | tried: C2DVector ctor/copy forms, temp orders, RTM+QFE (6 grades) | next: advisor (submitted with its 3 twins)
- 0047e9ae CTCInGameMenu::GlobalUpdate DIFFER(860v860): only the same SetPosition push ecx/fsubp swap; /GS | tried: inline text-bank accessor fixed the GetTextBySymbol load order (4 grades) | next: whatever fixes 0047e651 applies verbatim
- 0047f30f CTCInGameMenu::VideoOptionsNotAccepted DIFFER(636v636): SetPosition swap (twin of 0047e651) | tried: 4 grades | next: advisor
- 00576c4f CTCInventoryExperience::UndoBuyAbility DIFFER(642v642): SetPosition swap | tried: 5 grades | next: advisor
- 00576ed1 CTCInventoryExperience::UndoBuyStat DIFFER(622v622): SetPosition swap + one static-dialog/vtable load order | tried: 3 grades | next: advisor
- 005726a0 CTCInventoryExperience::UpdateAbilityDisplay DIFFER(824v824): action.ActionType store hoisted above the vtable load | tried: component local orders, inline setter in the argument (12 grades) | next: store through a forceinline copy-ctor argument
- 0057df85 CTCHeroStats::KickHeroToMostRecentSafeRegion DIFFER(876v876): frame-slot rotation of the marker-search temporaries (region/string/HasDefName) and the marker load after the minimap store; /GS | tried: TU pragmas (size + FPO default gives retail's `lea ebp,[esp-78h]`), block-scoped marker, kick copy, hero conversion temp, inline GetPlayerGui (10 grades) | next: marker declared in a helper scope so its word is shared with the AddFloat home
- 00584b86 CTCInventory::AssignThingPC DIFFER(854v854): name ternary register select vs push-in-arms; GetDef args vs PeekWorld order; use-param new-temps not packed into the dead thingID arg slot | tried: 16 grades | next: advisor (shares residue with 005f0dc5)
- 00589a30 CTCInventory::OpenPCMapMenu DIFFER(549v549): esi/edi reload order after the inline strcmp's repe cmpsb | tried: RTM + QFE (7 grades) | next: strcmp as an out-of-line wrapper
- 0058f649 CTCInventory::OpenPCInventory DIFFER(883v883): front-end manager / name temporaries frame swap, `lea edi` placement, action store scheduling | tried: 6 grades | next: scope blocks around the name temps
- 00591b7f CTCInventory::UpdateContextSensitiveItems DIFFER(729v735): escape-analysis gap: retail treats the candidate vector as non-escaping but reloads items' start, with items below candidates in the frame | tried: same-TU stand-ins (14 grades) | next: lane gap
- 005b3e4e CTCInventoryClothing::GlobalUpdate DIFFER(532v543): list.empty() materialised with sete before the list dtor; default-anim re-store; disjoint frame words | tried: 16 grades | next: bool local for empty()
- 005b43a2 CTCInventoryClothing::AddClothingToThing DIFFER(945v945): list ctor's empty allocator temp homed in the live `thing` slot's top byte (lane gives it its own word) | tried: allocator arg removed | next: lane gap (dead-slot packing)
- 005ee49a CTCInventoryAbilities::IsAbleToUseAbility DIFFER(555v549): key/out-pointer share the dead checkCost arg slot; merged release `lea` | tried: 10 grades | next: lane gap (arg-slot packing)
- 005eea9b CTCInventoryAbilities::OnSpecialAbilityUsed DIFFER(684v684): stats pointer and interface key share one frame word; SIB swap | tried: out-param, else-zero, HasTC/GetTC, value getter, scope block (8 grades) | next: const& SIB lever for the swap
- 005f0dc5 CTCInventoryAbilities::AssignAbilityPC DIFFER(996v996): name ternary register select vs push-in-arms; first use-param new-temp at [ebp+0x10] where retail packs it into the dead ability arg slot [ebp+8] | tried: named literal, no CSE, speed pragma, QFE, `const long&` ctor, inline AddLongParam (12 grades) | next: advisor with 00584b86
- 005f1320 CTCInventoryAbilities::UnAssignAbilityPC DIFFER(530v518): twice-used literal CSE'd into ebx spills the loop index | tried: distinct literal object (521v518, frame slots only) (6 grades) | next: lane gap
- 005f6bd6 CTCInventoryAbilities::SetAbilityAsAvailable DIFFER(588v618): register allocation (ability in ebx, zero in edi), level re-reads, i+1 in the dead slot | tried: 3 strategies | next: byte-offset loop form
- 005fb140 CTCInventoryMap::PanMap DIFFER(873v877): best node vs iterator in edi | tried: copy ctor, operator=, declaration order, while form (9 grades) | next: iterator const& lever
- 005fbaa0 CTCInventoryMap::GetRelativePosOnMiniMap DIFFER(632v634): vector start/size cached across virtual calls | tried: 5 grades | next: lane gap (whole-program caching)
- 007c81f0 CTCHitOnCollision::OnCollision DIFFER(762v768): iterator-tag byte vs direction frame order, begin() before ebp, Normalise scheduling | tried: 6 grades | next: scope blocks

## Skipped
- 00c3dfd0 "CTCHeroStats::InitializeVectorArrays": third-party library code at 0xC3DFD0 (not a CTCHeroStats method; manifest label propagated)
- 00c41060 "CTCHeroStats::SetupCharacterStateInitial": third-party library code at 0xC41060 (same)

## Label corrections
- 0x5EE01A is SetAbilityLevel (manifest: OnRegistedZSortingPatchRelocation)
- 0x56FFC5 is GetTCInventoryAbilities (manifest: GetTCHeroExperience)
- 0x47E941 is the NotAccepted callback (manifest: VideoOptionsChangedConfirmed)
- 0x41CC2B / 0x41CC14 are GetScreenHeight / GetScreenWidth (manifest: CTCLook::GetHeadAngularVelocityMaxXY)
- 0x79A610 is the spell-cost getter (manifest: CPixelShader::CountReferences)
- 0x42BE50 is NUISystem::CAction::CAction (manifest: CRumbleDef); 0x42BED4 / 0x42CD84 are the action-param pair ctor / copy ctor
- 0x99AED0 is the CWideString default ctor (manifest: CCharString); 0x405F00 is IntToWideString, __fastcall (manifest: CGuiControlNumber::SetSLongValue)
- 0x4292C0 is the _Rb_tree increment (manifest: GetNoAllocatedAreas)
- 0x70B460 is GetAnimation (manifest: CWorld::DrawGetEnvironment)
- 0x5BA7F9 / 0x5F8150 are find (manifest: _Umove); 0x5BAAE3 / 0x5F8750 are vector erase (manifest: GetNewTreeNode)
- 0x429B02 is list::erase (manifest: LinkedList_Unlink)
- 0x5BB3B3 is the set ctor (manifest: CopyBackBufferToTexture); 0x5BAF94 is set insert (manifest: bad_alloc); 0x581114 is the crime map ctor (manifest: CopyBackBufferToTexture)
- 0x686D50 is GetPlayerManager (manifest: GetDisplayEngine); 0x449960 is GetMainPlayer (manifest: Catch@0040d959)
- 0x706800 is GetAppearanceModifierList (manifest: CArray_InsertNode)
- 0x7C8130 is a 4-byte std::find, __fastcall (manifest: vector_find_Value, wrong element size)
- 0x686800 is CTCBase::CTCBase(CThing*) (manifest: CEngineSubPrimitive2DClockSprite); 0x6869D0 is CTCBase::GetDefinitions (manifest: _Cons_val)
- 0x99A2D0 is the primitive-handle base ctor (manifest: NHeroInformationScreens::CBase)
- 0x49D810 is the CWorld random seed (manifest: DoSizeof); 0x49D870 is the world frame count, fastcall (manifest: GFGetPreMainMemoryUsed)
- 0x57F58E is a marriage-list op (manifest: SortTreeRecursively); 0x4FB470 is CWorldMap::GetNoMaps (manifest: DrawGetNoMaps)
- 0x5791E5 is CCountedPointer<CAction>::Reset (manifest: vector<CCharString> Reset)
- 0x5819F2 is the button-slot mapper (manifest: CPersistContextReadSection)
- 0x434840 is CFadeInFadeOutBase::Start (manifest: CEngine::StartNewScene)
- 0x594BE8 is the candidate vector dtor (manifest: OnReadFinished); 0x594ED4 is std::sort, __fastcall (manifest: Heap_CalculateDepth)
- 0x5F9EA5 is map<EHeroAbility, CComponent*>::operator[] (manifest: map<EUsedKeys,long>)
- 0x686D20 is CTCBase::GetWorldMap, 0x4FB150 is CWorldMap::GetCurrentRegionNumber, 0x686D90 is CTCBase::GetThingSearchTools (manifest: GetDisplayEngine / DrawGetEnvironment / _Cons_val)
- 0x4378B0 is a CPlayerGui method taking a CRegion& (region title; manifest: _Cons_val); 0x57A5DD is the MirroredCriminalStatus default ctor
