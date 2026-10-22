# Hand chunk 294 (CPhysicsMesh .. CQ_BreakSiegeScript): 28 of 40 byte-exact

All 28 grade RELOCATION_MATCH under RTM cl 13.10.3077 and carry a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (28)
- **CPlayerGui** (4): AddScreenMessage 0x00438AA0, DrawInventory 0x00438CB0, Update 0x0043A080; CPlayerGuiDef::CPlayerGuiDef 0x00459BB6 (`#pragma optimize("s",on)`).
- **CProgressDisplay** (1): StopProgress 0x00499D90.
- **CPlayer** (4): RemoveMode 0x00634190, AddMode 0x006345C0, InitModeZTarget 0x006355D0, InitModeLightning 0x00636480.
- **CQ_ArenaScript** (3): Init 0x00CFA700, AnimateCrowd 0x00F1EC70 (jump table), GivePrizeFund 0x00F21300 (jump table).
- **CQ_BanditCampScript / BossBattle** (4): Init 0x00D006D0, WatchForCostume 0x00D00FD0, CheckAnyBanditsKilled 0x00D032F0; CQ_BanditCampBossBattleScript::Init 0x00D03610.
- **CQ_BountyHuntScript** (3): Init 0x00D12D00, WatchForEnableFisherCreekHutDoor 0x00D151D0, WatchForDeputy1Killed 0x00D16570.
- **CQ_BreakSiegeScript** (1): Init 0x00D1CC40.
- **CQ_FireHeartScript entity scripts** (3): CPuzzleBlockMoon::TurnIntoASunSign 0x00D38000, CPuzzleBlockSun::TurnIntoAMoonSign 0x00D387A0, CProphetCage::Main 0x00D38F70.
- **CQ_GuildTrainingPreMeleeScript** (1): CPreMeleeDummy::Main 0x00D51FC0.
- **Prison race** (1): CPrisonRaceWarden::Main 0x00DDD030 (the CBowerstoneIntroGuard explicit-destroy view model; out-of-line-constructed hero handle as a storage view with Construct()).
- **CQR_EscortTraderScript** (1): Main 0x00E1E110.
- **CQ_AmbushTradersScript** (2): AskBanditHireQuestion 0x00F07ED0, ProcessInitialSpyExtraction 0x00F0F680.

## Parked for Fable 5.1 (12)
- 0x00A6D0E0 CPhysicsMesh::GetHelperDataFromFileData DIFFER(930v984): preamble register/frame choice (retail this in edi, root chunk in esi, frame 0xfc; ours this in ebx, 0xf4) and the chunk walk diverges, 54 bytes short | tried: 2 probe spellings of the root/helpers chunk temporaries and entry construction | next: model the root chunk as a named CIVCountedPointer held across both HasHelpers/GetHelpersChunk calls with the entry temporaries scoped per loop; compare against a landed STLport map-of-entries push_back.
- 0x00A6D670 CPhysicsMesh::GetContainmentDataFromFileData DIFFER(898v969): ours gets an aligned ebp frame (and esp,-8) that retail does not have, 71 bytes short | tried: entry temporaries as named locals / ctor temporaries / scoped blocks | next: find which 8-byte local forces alignment (likely a double/__int64-shaped temp in the CNavVolumeEntry or the GFCharStringToInt result) and remove it.
- 0x00A120D0 CPhysicsMeshObject::GetSphereIntersectionFromTo DIFFER(987v886): as 0x00A12450 | tried: shares the 0x00A12450 spellings | next: solve the sibling first.
- 0x00A12450 CPhysicsMeshObject::GetPointIntersectionFromTo DIFFER(971v870): x87 cofactor operand order (mirror fld/fmul) plus a larger spill frame (0x58 vs 0x4c), 101 bytes long | tried: term orders, parenthesisations, a CMatrix3x4 inverse helper, named cofactor locals | next: try a landed CMatrix3x4::GetInverse-style __forceinline helper from the engine headers; else lane gap (x87 canonicalisation).
- 0x00A42470 CPlane::GetIntersectionWithTriangle DIFFER(750v750): one x87 operand pair in the third plane distance | tried: every term order / parenthesisation / Dot helper | next: lane gap unless a landed C3DVector dot spelling reproduces the load order.
- 0x00487180 CPlayer::GetFreeCamera DIFFER(612v612): scheduling only (Y store vs hoisted push 0; one shl scheduled later) | tried: 12 Normalise/lerp/store-order spellings | next: try the camera built through a landed CCamera setter chain rather than a local struct copy.
- 0x00489D40 CPlayer::CreateCharacter DIFFER(815v815): esi/edi swap (holy site vs player number) and frame-slot layout | tried: about 30 probes (ctor/member/named rh forms, scope blocks, declaration orders, named number locals) | next: declare the holy-site out-local in the outer scope with the debug-branch vectors in a sibling block; look for a landed function with the same GetHolySite + GetMainPlayerID pairing.
- 0x00439BB0 CPlayerGui::UpdateMoney DIFFER(583v592): the bar value is rounded by an inline fistp helper (float spilled to a frame word, fld, fistp) | tried: casts, a __forceinline round helper, a long local | next: needs the engine's round-to-int helper (asm_bake family in the landed tree): lane gap.
- 0x00D17A80 CQ_BountyHuntScript::WatchForBoasts DIFFER(705v785): the boast vector is destroyed differently on each exit (out-of-line scalar-deleting pair dtor 0x004DA68A + ~_Vector_base 0x004D9C5D on the loop-top return; inline elements + out-of-line base on the boast arms) | tried: in-body inline_depth pragmas, scoped vectors | next: per-exit split needs a model of where the vector's dtor is a call vs inline; try a __forceinline-less wrapper type around the vector.
- 0x00DDC070 CPrisonRaceWinningPostGuard::Main DIFFER(611v597): final idle loop entered via `jmp` to its bottom test in retail; ours duplicates the top test | tried: while/for(;;)/goto-entered do-while idle loops, a for(;;) wait loop, the implicit-destructor model (600v597) | next: give the idle loop a preceding statement that shares the bottom test (as the warden 0x00DDD030 `do ... while` with an initial NewScriptFrame).
- 0x00E20210 CQR_EscortTraderScript::WatchForTraderHit DIFFER(979v956): retail keeps ebp pushed at entry and merges the loop-bottom terminate return into the entry return block | tried: WatchForDeputy1Killed shape, for(;;) single exit | next: the CBowerstoneIntroGuard explicit-destroy view model for the by-value CScriptThing argument.
- 0x00F07B80 CQ_AmbushTradersScript::WatchForBanditSpeechMarkers DIFFER(845v842): which of the three inline ~CScriptThing exit copies survives the tail merge | tried: return placement, marker scope | next: reorder the returns so the post-wait-loop return is the textually last exit.

## Skipped
None.

## Label corrections
- 0x00F0F910 is CQ_AmbushTradersScript::WatchForArcherSpawnConditions.
- 0x00E1E880 / 0x00E1ED50 / 0x00E1E7A0 are the CQR_EscortTraderScript entity-script binding Alloc functions (M_BanditTrigger / AttackingBandit / TraderToEscort); manifest says CTriangleList.
- 0x00E1F0C0 / 0x00E1E470 are CQR_EscortTraderScript::DoMission / WatchForRegionLoads.
- 0x00437570 is the CPlayerGui graphics release (manifest: _Cons_val).
- 0x00CD23B9 / 0x00CD2770 are CScriptGameResourceObjectScriptedThingBase::IsScripting / Release.
- 0x00A0BEB0 is CCamera::GetHorizontalFOV (manifest: CMap::DrawGetSeaLevel).
- 0x008ADF10 is also map<CCharString,bool>::operator[] (folded with the C2DVector instantiation the manifest names).
- 0x004DA68A is the pair<long,CCharString> scalar deleting dtor; 0x004D9C5D is ~_Vector_base.
- 0x0074F680 is the boast vector ctor.
- 0x0099A330 is a CPlayer +4 bool getter (manifest: GetCanItemBeQuickAssigned).
- 0x006A4D00 / 0x004C73D0 are GetPosition (manifest: DoSizeof).
- 0x00449700 is GetMainPlayerID.
- 0x00CDBF70 / 0x00CDBFB0 are the cutscene cast-map ctor/dtor (manifest: StdMap_*).
- 0x006E7B60 is the movie-handle ctor; 0x007E72A0 the scripted-thing ctor (manifest: CCarriedReadableDef).
- 0x00A9B3D0 / 0x00AAA830 / 0x00AAA8C0 are C3DMeshFile chunk getters (GetVolumesChunk/GetHelpersChunk fold, GetFirstVolume, GetNextVolume); manifest says FindFirstBone/FindNextBone, C3DMeshFileSubMeshChunk.
