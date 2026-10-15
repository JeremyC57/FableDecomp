# Hand chunk 122 (CAIStateGroup_SetupWares .. CAnimComponentFlags): 33 of 40 byte-exact

All 33 matched sources grade RELOCATION_MATCH in `grades.txt` (gall.sh) and their behaviour tests print
`<addr>_TEST PASS` under both g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). No pragma/flag was needed
except 005c2013 (`optimize("y",off)` + `optimize("s",on)` in-source).

## What matched (33)

- **CAIStateGroup_* interface lookups** (00967250 TavernServer::IsServingPointValid, 00938580 UsesSeat::IsSeatValid,
  00944d10 WorkHammerAnvil::OnActivate, 009445f0 WorkSawingWood::OnActivate, 00944690 WorkSawingWood::OnReplace,
  00945530 SitForMeal::ReleaseControlOfPot, 00945620 SitForMeal::Cleanup, 00927ea0 SetupWares::InitCreaturePosition,
  009685c0 TavernKeeper::InitStateGroup, 0094dbc0 TrollBase::ForceRiseUp, 008e8a10 VersusScorpionKing::FindChargeEvent,
  009635c0 VersusScorpionKing::IsStillRunnable, 00911a00 WanderAroundInitialPos::IsRunnable,
  0096e610 SummonerSummon::Update): all use the landed out-parameter `GetInterface(T**, type)` template (0x007504E0 /
  0x00930DC0 shape) + `IS_THING_ALIVE`. Shape lessons: the bool result of GetInterface used as an `if`/`&&` link gives
  the third `je fail` (the bit test), an untested call reads the unwritten frame slot on the miss path; a call whose
  argument is the brain's creature needs the creature in a named local taken before the pushes (00944d10);
  `HasMode() ? false : true` for the byte `neg ; sbb ; inc` (009635c0); `d = dx*dx; d += dy*dy` for left-square-first
  (00911a00); a named zero C3DVector to keep its stores before the creature load (009685c0); the 0x118-byte RiseUp
  action needs its own extra word (0094dbc0); a CCharString local in its own block shares the lookup key's slot
  (00944690). Headers used where they exist (`_Methods` shells over the flat PDB structs; PBrain read from the
  flattened base bytes).
- **CAIStateGroup animation actions** (009454c0 SitForMeal::StandAroundAndEat, 00942920 TellStory::StandAndAct,
  00942820 / 009428a0 WorkAtSchool CrouchAndTalk / Teach): the landed GoToPub::SitIdle PlayAnimation shape with the
  wait call (`false` pushed first); `result == 2 ? true : false` after an inner block for the post-destructor
  `sete al`; three implicit-conversion CCharString temporaries for PlayLoop (built right to left).
- **CRegion::~CRegion** (0051a110, catalog "~CAIStateGroup_SummonerCharge"): a non-polymorphic class whose member
  destructors are inline: STLport map clear + header free, strings, a CDefPointer released through a local copy,
  two vector buffers; retail offsets (12-byte vectors) instead of the PDB header's 16-byte ones.
- **CANavQuadTreeNode / CActionInputControl**: 0040ed10 GetDirection (global epsilon squared read twice, four-way
  quantisation) matched first try.
- **NTCScriptedControl actions**: 00712bb0 CActionDoCreatureAction::Update, 00870d70 CActionEventAddObject::
  SetDataFromString (two separate `if (!x) return false;` for retail's false-tail placement; default-constructed
  CCharString homed in the dead argument slot), constructors 009034f0 CActionPlayAnimation, 009051f0 / 00905290
  CActionMoveTo (novtable CActionBase + GetScriptThing straight into the member + inline CScriptThing default
  constructor), 00713180 CActionSummonerLightningAttackTarget (embedded CActionTurnToFacePosition with SCALAR
  position members), 00717bc0 CActionOverheadTalkToThing (catalog "CActionTalkBase::CActionTalkBase"; local
  CArray with an inline destructor loop, `last` cached).
- **Persistence**: 007b5740 CActivateQuestDef::Transfer (two inline CPersistContext::Transfer<T> of the landed
  CReadableDef shape, CDefString traits forwarding to the string's own load), 005dbda0 CAnimComponentCombat
  FlourishTargetOffset::TransferBinaryIn (landed inline stream Read for one byte; the by-value C2DVector assigned
  straight from the call), 005dc3c0 CAnimComponentFlags::Initialise (CStringParser local with an explicit
  `CCharString("", -1)` temporary; the parsed long is the local of an inline helper so VC homes it in the dead
  argument slot).
- **Entity scripts**: 00ce1110 CActiveEntityScriptBase constructor (`__forceinline` CSpawnedFuncBase constructor,
  64-bit EntityUID, inline CScriptThing copy), 00e33720 CAT_ConvoySpy::Init (CGameScriptInterface slots 360/70/599/
  844 from rtti-vtables.tsv; the by-value CScriptThing copy needs the counted pointer as its own copy-constructed
  member).
- **005c2013** (catalog "CActiveFile::ReserveSpace") = STLport `vector<T20>::reserve`, size TU with frame pointer.

## Parked for Fable 5.1 (7)

- 00945a90 CAIStateGroup_SitForMeal::GetPCookingPot DIFFER(127v121): `sub esp,8` + the shifted `[esp+N]` offsets v retail `push ecx` (lookup key, predicate and find_if result overlaid in ONE frame word) | tried: GetPHome accessor, iterator in its own block, search in a (force)inline helper | next: lane gap: frame-slot sharing between an inlined callee's local and a later hidden-return temporary
- 00946380 CAIStateGroup_SitForMeal::GetPCookingPot DIFFER(122v116): `sub esp,8` v `push ecx` (same overlay, uninitialised predicate) | tried: predicate with user constructor, key taken from the inlined parameter, named predicate + iterator locals | next: lane gap: frame-slot sharing (as 00945a90)
- 00945b10 CAIStateGroup_SitForMeal::GetPFreeChair DIFFER(126v122): `sub esp,8` v `push ecx` (key and returned iterator overlaid; no predicate temporary here) | tried: early-return form, out pointer scoped to a block, iterator with a user destructor | next: lane gap: frame-slot sharing (as 00945a90)
- 00962b80 CAIStateGroup_VersusEnemyBase::OnReplace DIFFER(129v129): `mov eax,[edi+4]` / `mov edx,[edi+4]` swapped against retail's edx-then-eax for the two PBrain loads after RemoveMode | tried: without the world local, inline LeaveCombat helper, inline RemoveModeIfSet helper | next: lane gap: register allocation tie
- 009395f0 CAIStateGroup_SitRoundCampfire::GetPNearCampfire DIFFER(122v122): retail loads each copied position's three dwords before storing them (interleaving the next PBrain load) and stores the result zero after the creature load; the lane alternates load/store | tried: memberwise copy constructor, POD vector, radius init-list vs body (body moves the radius store to retail's place) | next: lane gap: struct-copy scheduling
- 00964360 CAIStateGroup_StealStealableItem::FindPStealableItem DIFFER(107v107): same copy scheduling (`mov ecx,[eax] ; mov edx,[eax+4] ; mov eax,[eax+8]` then three stores) plus the zero store one instruction early | tried: memberwise copy ctor, POD + body assignment, by-value constructor parameter (the empty filter as a Bind2 temporary fixed the frame layout) | next: lane gap: struct-copy scheduling
- 00a763f0 CANavQuadTreeNode::operator== DIFFER(116v116): retail `mov ecx,[esp+8]` before `fld [esi+4]`, the lane after it | tried: by-reference NearlyEqual helper, pointer local for the other node | next: lane gap: one-instruction scheduling

## Skipped

None: every row is a single function (no int3/nop padding followed by a second body inside a row).

## Label corrections

- 0051a110 "CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge" -> CRegion::~CRegion (39 callers in
  CWorldMap / region vector code; member types and order match the CRegion PDB layout with 12-byte retail vectors;
  sizeof 0x58 = the `add edi,58h` stride of CWorldMap's region loop).
- 00717bc0 "CActionTalkBase::CActionTalkBase" -> NTCScriptedControl::CActionOverheadTalkToThing constructor (calls
  the CActionTalkBase constructor 0x00717950, stores vtable 0x012659FC).
- 009428a0 "CAIStateGroup_WorkAtSchool::CrouchAndTalk" (propagated from the byte-identical 0x00942820) -> plays
  ST_TEACH_*, named `Teach` (descriptive).
- 005c2013 "CActiveFile::ReserveSpace" -> STLport vector<20-byte record>::reserve (named CRetailVector20::reserve).
- Callee labels found propagated: 0x007B8570 "CTCInventoryBase::GetSelectedCategory" (tavern-table seat check),
  0x007FEF40 "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" (action-point release), 0x008FCF40
  "CXMVPlaybackInfo" (CAIStateGroupBase::Terminate), 0x00692620 "CWorld::DrawGetEnvironment" (action-queue id
  getter), 0x004D1DB0 "~CPatchTesselationEdgeStrip" (~CSubtitledMessage), 0x0071A160 "CCombatWheel::CopyAssign"
  (CArray<CSubtitledMessage>::operator=), 0x0044FC00 "CPersistTraits<float>::TransferBinaryIn" (CDefString load),
  0x00995030 "CPersistTraits<CFloatRange>::TransferBinaryIn" (two-float C2DVector read), 0x00A44740
  "CCreatureAction_CastBattleCharge" (script-process root constructor), 0x00939AF0 / 0x00964230 thing-search
  template names (instantiation labels propagated), 0x005C1F1F "CActiveFile::OnReadFinished" (vector _M_clear).
