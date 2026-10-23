# Hand chunk 319 (CIconText .. CModeJackOfBladesFlash): 14 of 40 byte-exact

## What matched (14)
- NUISystem::CList::AddScrollingChildDynamicallyAndSetOffsets 0x0053BF20: adds a scrolling child at the list's next position and alpha, then records it. It matched once the map operator[] and the direct base AddChildDynamically call were modelled on the landed CScrollableList (CDefPointer / CCountedPointer).
- CInputProcessInventoryMapScreen / ExperienceScreen / TradeScreen::ProcessInput 0x0068AE30 / 0x0068B2E0 / 0x0068B7C0: route menu, d-pad, mouse and wheel input to the front end and the inventory map. These matched with the CInputProcessGameBase fields (DefinitionManager +0x1c, PGamePlayerInterface +0x24) and the CGameEvent AddUWORD/AddFloat/AddBool model. In 0x0068B2E0, `buttons |=` had to be written as separate statements.
- NAnimationEvents::CManager::LoadBinary 0x006F9F90: reads the binary extra-event file through a memory stream. The flag bytes had to be read as `ReadEBOOL` -> `value != 0`, following the landed Save 0x006F9BE0 model.
- CKeyboardDX::CKeyboardDX 0x00AB64E0: builds the DirectInput keyboard device and the DIK table. It uses a local CKeyboard base model, with the base ctor 0x00A66930 called directly.
- CLadyGreyWifeManagerScript::Main 0x00CF8A70: a region-loaded wait loop. It needed GameCalls slot 11 IsLevelLoaded, not slot 12 IsRegionLoaded.
- CKG_ChiefBreakSiege::Main 0x00D22880: siege wait and chief control. A `__forceinline` accessor got around the per-function inline budget, so the ctor/dtor inline choices match.
- CMazeJournal::Main 0x00D2A180: the maze journal script. Reference helpers keep the virtual calls virtual, and the virtual GetPos on a temporary is hoisted.
- CMagicBarrier::Main 0x00DA4E30 (guild barrier): respawns two forcefield halves until the barrier opens. It uses the landed CATO_MagicBarrier model, with a novtable predicate holder.
- CMinionWarningGuard / CMinionFieldGenerator / CMinionTrollSummoner::Main 0x00DA9C70 / 0x00DAA260 / 0x00DAA700: minion-chase scripts in the CCorneredGuard / CDF_ShineLight handle model. The frame overlay in 0x00DAA260 was fixed by nested blocks.
- CM_HeroInsideHouse::Main 0x00E96660: haunted-house door lock. The inline budget decides whether the ctor/dtor are inlined or out of line, and a `__forceinline` accessor matched retail.

## Parked for Fable 5.1 (26)
- 005430f0 CIconText::InitIcons DIFFER(22v1081): first-pass stub, body not reconstructed | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 009f57a0 CInputManager::Update DIFFER(88v1072): stub | tried: first-pass stub | next: full reconstruction (replay-package loop)
- 0068f470 CInputProcessCombat::ProcessCombatInput DIFFER(1365v1361): same instructions; frame 0x50 v 0x48 (TC-key temps and the __true_type tag overlay; locked target homed early in retail) | tried: bool HasTC, unsigned long HasTC, derived target type | next: lane gap: stack-slot overlay heuristic
- 00688360 CInputProcessControlCreature::ProcessInput DIFFER(1714v1690): same blocks; eax/edx/ecx rotation for the g_pPlayerGui loads, and the ExpressionShiftMode tail is cross-jumped | tried: 8 variants (local gui ptr, per-slot helpers, inverted held branch) | next: lane gap: register allocation / tail merging
- 006a40d0 CInputProcessJumpingAndRolling::ProcessRolling DIFFER(13v1579): stub | tried: first-pass stub | next: full reconstruction (x87, C2DVector::Dot)
- 0068dd00 CInputProcessUseEnvironment::ProcessInput DIFFER(1469v1471): frame 0x44 v 0x40, TC keys / IS_THING_ALIVE temp overlay, 0xd key CSE into ebp | tried: 8 variants, including split `&&` conditions | next: lane gap: stack-slot overlay
- 00be6880 CLandscapeBackgroundPatch::BuildDefaultSection DIFFER(19v1776): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 0079f970 CLightningBeam::UpdatePositions DIFFER(8v1052): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 00a495f0 CLipSyncSampleData::ReadFromFile DIFFER(47v1261): stub | tried: first-pass stub | next: full reconstruction (check EgoCore lipsync format first)
- 0053b91e NUISystem::CList::Initialise DIFFER(1416v1422): retail promotes constant 0 into ebx (bool spilled, frame 0x28); ours hoists lea [edi+0x17c] (frame 0x24) | tried: forceinline operator= / GetPtr / local bar ptr, ?: middle index | next: lane gap: zero-constant ebx promotion
- 00bfad90 CLocalDetailPrimitiveMeshZSpriteBatch::RenderUpdate DIFFER(18v1151): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 00bfa2a0 CLocalDetailPrimitiveRepeatedMesh::Load DIFFER(93v1533): stub | tried: first-pass stub | next: full reconstruction
- 00ea5230 CLostTraderBanditSpawn::Main DIFFER(1117v1117): same length; retail hoists lea ecx,[attacker] above the terminate jne in the two wait loops | tried: loop-form variants | next: lane gap: scheduling
- 00e37e40 CM_AssassinTrigger::Main DIFFER(1097v1103): Knothole bool-set shape (mov0/jne/mov1 v jne/mov1/jmp/mov0), placement of the wait-loop exit blocks, one hoisted mov edi | tried: out-of-line dtor, inverted guard, nested item scope (fixed frame) | next: an explicit bool temp with if/else assignment
- 00e96110 CM_QuestControl::Main DIFFER(1360v1356): retail puts 0 in ebx and the timer index in edi; ours gives ebx to the timer index | tried: game local, long colour ctor, `== false` | next: lane gap: zero-constant ebx promotion
- 00e95770 CM_UndeadHere::Main DIFFER(1144v1144): same length; edx/eax choice in the AXE branch around SetCreatureCreationDelayFrames + CreateCreature | tried: local game ptr variants | next: lane gap: register allocation
- 00e03f70 CMagicBarrier::Main (Darkwood) DIFFER(1986v1906): retail calls ~CScriptThing 0x004AA840 out of line (inline budget); fstp scheduling | tried: inline/out-of-line dtor forms | next: model the inline-budget order (more inlines before the respawn block)
- 004184bd CMainGameComponent::Init DIFFER(24v1285): stub | tried: first-pass stub | next: full reconstruction (frame-pointer /Os TU)
- 00428494 NUISystem::CManager::CreateMenuEntryComponent DIFFER(45v1062): stub | tried: first-pass stub | next: full reconstruction (frame-pointer /Os TU)
- 006fa4e0 NAnimationEvents::CManager::Load DIFFER(1405v1444): frame slot order (exists flag +0x2b v +0xf); inner END_EVENTS while loop not inverted | tried: __forceinline clear, tag ctors, animID copy | next: a do/while with a duplicated test, CStringParser loop forms
- 00a276d0 CMeshDataBank::InitialiseEntry DIFFER(37v1330): stub | tried: first-pass stub | next: full reconstruction (check EgoCore mesh format)
- 0082afa0 CMiniMapDisplay::InitialiseRegionExitMarkers DIFFER(16v1393): stub | tried: first-pass stub | next: full reconstruction
- 008e8060 CModeBriarRoseConcentrate::OnActivate DIFFER(5v1784): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 008de080 CModeDragonStrafeBurninate::FrameUpdate DIFFER(5v1437): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 008de7a0 CModeIceTrollGroundSmashIcicleAssault::FrameUpdate DIFFER(5v1239): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)
- 008dcc80 CModeJackOfBladesFlash::Flash DIFFER(1v1136): stub | tried: first-pass stub | next: full reconstruction (x87-heavy)

## Skipped
- None. The odd-address rows 0053b91e, 004184bd and 00428494 each hold one function from a packed frame-pointer /Os TU.

## Label corrections
- 0x00687D50 CInputProcessGameBase::IsGameActionEventInQueue (manifest: CPersistContext::Transfer)
- 0x00687CF0 CInputProcessGameBase::IsEventGameAction, 0x00687D10 GetButtonPressureForEvent (manifest: _Cons_val / operator_new)
- 0x00487DC0 CPlayer::GetHeroCreature (manifest: _Cons_val)
- 0x009B9BA0 CStringParser::NextItemIs (manifest: CThingCreatureBase::SerialiseGameState)
- 0x009E5200 CSymbolMap::CSymbolMap (manifest: CParticleUpdateParameters)
- 0x006FD040 CSymbolMap::~CSymbolMap (manifest: ~CPatchTesselationEdgeStrip)
- 0x006E7B60 CScriptGameResourceObjectMovieBase ctor (manifest: CCarriedReadableDef)
- 0x008AC970 ~vector<CScriptThing> (manifest: ~vector<CConversation>)
- 0x009B9880 CParsedItem::GetString (manifest: CModeBlock::GetAnimFilter)
- 0x0040DCE0 IS_THING_ALIVE<CThing*> (manifest: CThingBuilding*)
