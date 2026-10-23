# Hand chunk 318 (CExpression_StealScript .. CIconText): 20 of 40 byte-exact

## What matched (20)
- CGameScriptInterface::TurnCreatureInto 0x0089F4A0: replace a live creature by one built from a named def (AI creature re-created with its script name, player creature re-created with copied interfaces and re-bound to the main player), then kill the old thing. Shape: the landed CreateObject sibling model (named result, inline copy/dtor, out-of-line operator=) plus a GetDef stand-in. The eight interface copies needed ONE shared `const CTCBase* tc` with an out-param `GetInterface(&tc, type)` (the landed 005b37f7 idiom). Per-call temporaries rotate the scratch registers (8 grades).
- CFinalTabletSarcophagus::Main 0x00CFFB60: plain inline ~CScriptThing, with `inline_depth(0)` on the second CreateObjectOnEntity statement.
- CGate1GuardInner::Main 0x00D02B80: writing the `!Gate1Open` arm first puts the duplicated terminate check where retail has it. The movie is destroyed inline.
- CGuardHoldingBandit::Main 0x00D1E3E0: CIsThingConscious filter and inline CCharString == "EXPRESSION_FOLLOW" (the WatchForExpressionFollow model).
- CFH_Villager::Main 0x00D37530: the loop group test is written `while (!Succeeded) { if (MyID < 3) { if (G1) break; } else if (G2) break; ... }`, which gives retail's cl register and exit-block order.
- CGameflowScript::DeclareGossipCategories 0x00D3B410: 28 rumour/gossip GSI calls with implicit const char* -> CCharString temporaries.
- CHeroBed::Main 0x00D446D0: three STLport vectors with `__forceinline ~vector` (free 0x00BFEA14).
- CHTG_Prisoner::Main 0x00D71B30: the escape loop of quest state 5. The inline GetDataString/GetPos are written `!Data ? global : Data->vslot()` to get retail's branch order. The MoveToPosition arm also clears ReturningToParty; this was found from a tail-merged jump that was the only residue.
- CHSB_MagicBarrier::Main 0x00D81C50: inline CScriptThing operator= (the source Info/Data are read into locals first) and `inline_depth(0)` on the two OFF assignments.
- CHS_Screamer::Main 0x00D83FA0: out-of-line handle and CTimer ctor/dtor with auto_inline(off) CTimer bodies, which keep the ebp cache.
- CGuildmaster_HS::Main 0x00D85B40: inline CTimer ctor/dtor with `inline_depth(0)` at the last two exits.
- CGuardOutsideWoods::Main 0x00D874F0: switch on NumberOfWarnings, with both attack tails written out in full.
- CFadeinSummoner::Main 0x00D9F6C0: inline CScriptThing copy for the by-value SetIsPushableByHero; `IsDead() { return !IsAlive(); }` gives the virtual IsAlive call; the flag store is written before SetTime.
- CGuardNotThisWay::Main 0x00DA68E0: `__forceinline ~CTimer` with `inline_depth(0)` on the wait returns.
- CGuardToLeadTheWay::Main 0x00DA7D30: inline operator=/~CScriptThing in the search loop, and the by-value spawn argument as an out-of-line CScriptThingArg.
- CGraveyardDemon::Main 0x00DC3FC0: handle model plus a hand-destroyed movie view (inline at two exits, out of line at one).
- CFireHeart::Main 0x00DF2F90: a same-TU auto_inline(off) ~CTimer body keeps the beat timer in ebp.
- CHKC_VillagerScreamer::Main 0x00E1B1F0: the screamer local sits in its own {} block so it shares the GetScriptThing temp slot.
- CFishCompCompetitor::Main 0x00E7E640: implicit handle dtor inline on the normal and first exits, `inline_depth(0)` on the later returns; a named `{}` pointer local for SizeComment picks eax.
- CGlobal_OpenChestScript::Main 0x00EEC890: a canOpen flag (then = main path, else = locked) gives retail's block order; `cancelled = A || B;` keeps the bl store inside the temp scope.

## Parked for Fable 5.1 (19)
- 00d91960 CHobbeRoaming::Main DIFFER(1101v1085): the final `while (!IsActiveThreadTerminating()) NewScriptFrame();` duplicates its entry test (16 bytes); retail jumps into the bottom test | tried: closing-loop and exit forms (8 grades) | next: lane gap: loop-entry rotation of the closing idle loop
- 00d91e00 CHobbeSpawn::Main DIFFER(-v1408): decode only; a mix of inline and out-of-line vector<CScriptThing> and CTimer exit copies | tried: decode | next: the 00e7e640 per-return inline_depth(0) recipe once the exit map is charted
- 00eebdf0 CExpression_StealScript::Main DIFFER(1239v1240): register allocation only; constant 0 is cached in edi early (one zero use too many) and framesLeft is enregistered in ebp (retail keeps it in memory) | tried: 14 grades of local/scope/reuse forms | next: lane gap: whole-function register priority
- 00d73f40 CHTG_Executioner::Main DIFFER(1250v1256): retail parks the two inline handle-dtor exit copies (vector-loop and HTG wait-loop returns) after the hit-test jmp inside the main loop; ours places them at the jne sites | tried: do/while main-loop forms, inline_depth(0) on each hit-arm return, the first hit-arm return without the pragma (4 grades) | next: lane gap: block placement of a shared Terminate/failure block
- 00e82950 CFisticuffsClubRing::Main DIFFER(1574v1574): jump targets only; the two inline ~CTimer exit copies swap which terminate checks they serve | tried: while-loop form, forceinline timer accessors, pragma'd out-of-line exits (9 grades) | next: lane gap: block placement of a shared Terminate block
- 009c47a0 CFileInstaller::AddFilteredFiles DIFFER(-v1920): decode only; a 1.9 KB path filter (CWideString/CCharString ToUpper, ConvertPathToFullPath, ScanNextFile loop, inline std::map FindNode) | tried: decode | next: recover the calling convention and the map node type first
- 009fde80 CFrameData::BeginLoading DIFFER(-v1280): decode only; inline CResource LRU relinks, a texture-allocation retry loop that halves the dimensions, an async bank read swapping three CCountedPointer handles | tried: decode | next: chart the CFrameData / CGraphicInfo / bank-entry layouts
- 009fed30 CGraphicDataBank::LoadGraphicFrame DIFFER(-v1206): decode only; a 9-argument texture loader (surface lock, chunked decompression, deque insert, mip copies, free) | tried: decode | next: surface/texture layouts
- 004959e0 CFrontEndManager::GetProfileNames DIFFER(-v1536): decode only; /GS (cookie 0x0139C8A8) with an aligned 0x524-byte frame and a CAFile find loop | tried: decode | next: lane gap: FrontEnd /GS TU family
- 00595cc1 CFrontEndManager::ConstructFileDescription DIFFER(-v1613): decode only; size-optimised /GS + EH TU (`leave` epilogue) | tried: decode | next: lane gap: FrontEnd /GS TU family
- 00597df7 CFrontEndManager::GetSavedGameNames DIFFER(-v1644): decode only; size-optimised /GS + EH TU | tried: decode | next: lane gap: FrontEnd /GS TU family
- 00598463 CFrontEndManager::RefreshAvailableSavedGamesForProfile DIFFER(-v1335): decode only; size-optimised /GS + EH TU | tried: decode | next: lane gap: FrontEnd /GS TU family
- 00598a1c CFrontEndManager::Init2 DIFFER(-v2007): decode only; size-optimised /GS + EH TU | tried: decode | next: lane gap: FrontEnd /GS TU family
- 006fdd60 CGameCameraBase::Update DIFFER(-v1659): decode only; x87-heavy (162 x87 instructions) | tried: decode | next: lane gap: large x87 body
- 0069ae80 CGameCameraManager::CGameCameraManager DIFFER(-v1936): decode only; a ~60-member constructor with three STLport list/tree header nodes (malloc), a counted-pointer camera registration and global flag resets | tried: decode | next: build the member layout from CGameCameraManager.h plus the retail container sizes
- 00446ef0 CGamePlayerInterface::SetControlScheme DIFFER(-v1184): decode only; four STLport vector clears, a def lookup, then a 28-byte-record loop building ~14 polymorphic input-action classes into counted pointers, routed to three lists or a sorted pair vector (Reallocate + make_heap) | tried: decode | next: recover the action-class vtables 0x012317A8..0x012318A4
- 0088edc0 CGameScriptInterface::CameraDoConversation DIFFER(-v1568): decode only; x87-heavy (154 x87 instructions) | tried: decode | next: lane gap: large x87 body
- 006cddf0 CGroundNavigatorExternalInterface::IsMovementBlockedAt DIFFER(-v1056): decode only; x87-heavy navigation geometry (80 x87 instructions) | tried: decode | next: lane gap: large x87 body
- 00541a00 CIconText::FitInWindow DIFFER(-v1120): decode only; aligned-stack x87 word wrap over CWStringParser chunks with CWideString temporaries | tried: decode | next: x87 compare idioms + CWideString temp model

## Skipped
- 00d2988b CHookCoastTeleporter::Main: a bogus mid-function row. The real function starts at 0x00D29880; the row begins 0xb bytes in and uses edx/ebx undefined.

## Label corrections
- 0x00CFF9A0 is CQ_AwakeningTheOracleScript::PlayCutscene.
- 0x00D872D0 is probably CQ_HeroSoulsGuildmasterScript::PlayCutscene.
- 0x0099A2E0 is ~CBase, not CBase::CBase. 0x0099A2D0 is the out-of-line CBase ctor.
- 0x006E7B40 is the CScriptThing default ctor.
- 0x00DF20F0 is CQ_SummoningTheShipScript::MakeBriarRoseComment.
- 0x00E83230 is labelled CCreatureAction_TrollWhackGroundBase::HandleVillageSelection, but it is a CFisticuffsClubRing member called from Main.
- 0x00D2988B is not a function start (see Skipped).
- From TurnCreatureInto 0x0089F4A0:
  - 0x004C73D0 "CVertexBufferWin32::DoSizeof" is CThing::GetPosition.
  - 0x004C7E50 "GFPredicateAnd<...>" is CThing::GetScriptThing.
  - 0x00437CD0 "CWorld::DrawGetEnvironment" is CPlayerGui::GetPHeroInfoScreensManager.
  - 0x006AC910 "_global::Create" is CThingPlayerCreature::Create.
  - 0x00449970 "Catch@0040d959" is CPlayerManager::GetMainPlayer.
  - 0x00487B80 "CVillageTaskCarryCrate::AssignCreatureToTask" is a CPlayer member that takes the new player creature, called just before SetControlledCreature (probably SetPlayerCharacter). The label was propagated from a byte-identical body.
- From CHTG_Prisoner::Main 0x00D71B30:
  - 0x007E74D0 "CScriptGameResourceObjectMovieBase::~..." is the scripted-thing handle dtor.
  - 0x00F35B10 "CWorldMap::AllocateMapThingUID" is CEntityScriptBase::RegisterFilter.
  - 0x00F35B30 "CCreatureAction_TrollWhackGroundBase::Initialise" is IsActiveThreadTerminating.
  - 0x00CD23B9 / 0x00CD2770 are the handle's IsScripting / Release.
