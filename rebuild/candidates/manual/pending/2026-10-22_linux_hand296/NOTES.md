# Hand chunk 296 (CQ_MinionClifftopChaseScript .. CQuestManager): 29 of 40 byte-exact

All 29 grade RELOCATION_MATCH under RTM cl 13.10.3077 (no flags). `probe/rpair.py` pairs every built relocation with
its retail target. All 29 tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (29)
- 00da8970 CQ_MinionClifftopChaseScript::GuardComment
- 00db0660 NScript::CQ_NewOakValeIntroScript::AddGoodDeed
- 00dbe4e0 NScript::CQ_NewOakValeIntroScript::ManageQuestCoreMarkers
- 00dbf9e0 CQ_OakValeBanditRaidScript::WatchForWhistling
- 00dc37f0 CQ_OpeningGraveyardSecretPassageScript::WatchForArmour
- 00dd2020 CQ_OrchardFarmRaidEvilScript::Init
- 00dd23a0 CQ_OrchardFarmRaidGoodScript::Init (the label says ...EvilScript::Init)
- 00dd1af0 CQ_OrchardFarmRaidScript::DoMultiplierCutscene
- 00dde230 CQ_PrisonWardenGameScript::WardenKeyMiniGame (`#pragma inline_depth(0)` region from the success block)
- 00de03e0 CQ_RansomVictimScript::Init
- 00de1280 CQ_RansomVictimScript::GossipSetup
- 00deff60 CQ_SecretPassageScript::DoMission
- 00df0750 CQ_SecretPassageScript::MotherTortureThread
- 00df13a0 NScript::CQ_SummoningTheShipScript::Main
- 00e75ba0 NScript::CV_ExposeMayorScript::Main (the label says CQ_SummoningTheShipScript::Main)
- 00cdba10 NScript::CQ_SunnyvaleMasterData::Init
- 00df4240 NScript::CQ_TentacleKrakenBossFightScript::Main
- 00ec1910 NScript::CV_RandomPopulationSimScript::Main (the label says CQ_TentacleKrakenBossFightScript::Main)
- 00df5cd0 CQ_TraderConflictEvilScript::Init
- 00dfa0e0 CQ_TraderConflictGoodScript::Init
- 00e04f10 CQ_TraderEscortScript::WatchForPickpocketing
- 00ee76f0 NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Main (the label says CQ_TraderEscortScript::TurnToBalv)
- 00e0dbb0 CQ_UndeadRisingScript::LookAfterHealth
- 00e12f20 CQ_WaspBossScript::WaspIntro
- 00e1c0f0 CQ_WizardBattleScript::DoMission
- 00e1cd40 CQ_WizardBattleScript::CutsceneScreamers (`__declspec(novtable)` plus `__forceinline` dtor; rpair confirms the vtable/dtor symbols)
- 00bde390 CLocalDetailCacheMap::CQuadTreeElement::LoadStaticContents. This needed a `__forceinline long ReadLong()` that
  reads into its own local, so both inlined reads share one frame word. The count lands in ebx without a store, and
  the value's high dword reuses the edi zero.
- 004b1720 CQuestManager::AddBoast
- 004b4770 CQuestManager::~CQuestManager

## Parked for Fable 5.1 (11)
- 00dd9520 CQ_PrisonEscapeRescueMotherScript::PrisonEscape DIFFER(986v986): the fstp of the inline GetAngleXY() result
  into the outgoing float argument lands before the inline GetPos() null test (retail puts it after) | tried: named angle
  local, ternary and if forms (both polarities), reversed GetPos form (990), QFE 4035 | next: an out-of-line
  EntityTeleportToPosition argument wrapper, or moving the angle into the callee's argument list order.
- 00dfc630 CQ_TraderConflictGoodScript::WatchForHittingEnemies DIFFER(734v740): retail caches 1 in edi across the outer
  loop, pads no loop head, and has a 0x20 frame with the hit bool at [esp+0x13] | tried: for(;;)/break,
  while(!(x>=1)), guarded do-while, index signedness and scope, named bool, QFE, optimize("s")/("t",off), forceinline
  helper splits | next: a sibling landed thread with the same 1-in-edi loop shape.
- 00e06050 CQ_TraderEscortScript::WatchForSurprisingBalverines DIFFER(1007v1007): in the IsAlive loop retail hoists
  `lea ecx,[esp+0x34]` above the terminate jne and jumps to the shared ~CScriptThing call | tried: depth-0 regions
  (loop / all four exits), while(!(!IsAlive())), inverted rand branch order, STLport base split | next: a block scope
  that ends the balverine CScriptThing at the loop exit.
- 00e1cae0 CQ_WizardBattleScript::MonitorMazePhases DIFFER(564v568): the handle local's implicit dtor (0x007E74D0, no
  vptr store) is inlined at 3 of 9 returns (retail: 7, out of line only on the two MazeDefeated returns) | tried:
  novtable+forceinline (RELOCATION_MATCH, but it emits a wrong ??_7 store in the ctor, so refused), forceinline holder
  (matches only through a shadow ??1 call, refused), depth-0 holder | next: per-return inline_depth regions around the
  two MazeDefeated returns only.
- 00bdf100 CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren DIFFER(637v610): structure is right; register
  allocation inside the inlined _M_insert_overflow differs (retail: zero in ebx reused for new_start, this in ebp,
  src+8 cursor) | tried: out-of-line overflow (448), forceinline copy at the collection site only | next: model the
  inlined overflow on a landed STLport vector<C3DBoundingSphere> push_back.
- 006509f8 NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter::DrawAt DIFFER(549v549): one swapped pair in the
  epilogue (retail does `fstp [ebp+8]` then `lea ecx` for ~CWideString; ours schedules the lea first) | tried:
  if/return, ternary, result variable, default-then-override (541), inverted condition, inline helper (529),
  optimize("y",off), QFE | next: compare against a landed size-TU float return with a CWideString local.
- 00650c1d NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList::DrawAt DIFFER(639v635): x87/slot allocation in the
  icon loop (retail keeps drawPos.Y promoted and drawPos.X in memory, height in memory, width in st0; frame 0xc0 with
  an 8-byte hole at [ebp-0x6c]) | tried: vector-operator size math (682), scalar w/h (616), flipped branch (615), vector
  bottom-right sum (630), font local (631), scoped "5" label block (639), cursor-first operator+ | next: C2DVector
  `size` combined with the scoped label block and the scalar drawPos.
- 006507d5 NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt DIFFER(553v547): retail keeps the font in edi and
  spills the seconds to [ebp+8]; ours keeps pos/seconds in edi and spills the font (slots shift) | tried: ternary
  colour argument (two temps), per-channel ternary (setcc), default-then-override, copy-init/const-ref ternary local
  (extra copy), if/else assignment (extra copy), non-forceinline helper | next: start from the Counter's q2 frame
  and vary where `pos` is first read. The forceinline if/return colour helper is already right.
- 004b2280 CQuestManager::SetQuestAsFailed DIFFER(527v527): retail keeps questName in edi and this in ebp (ours
  swapped) and homes the interface keys at [esp+24h] (ours [esp+2Ch]) | tried: card at top, reference local, const
  self, out-pointer scopes, KickOffDeathScreen GetInterface form (535), QFE | next: a same-TU stand-in for the
  failure-screen callee.
- 004b2890 CQuestManager::PostBoastTransfer DIFFER(569v569): register colouring at two call sites
  (GetAcceptedBoastsForScript args edx/ecx in retail, ecx/eax in ours; GetInterface key eax v edx) | tried: quest
  local, GetName() accessor, const self, inline find bounds, GetInterface form, list as param class | next:
  vary the stand-in's signature (by-pointer list).
- 004b3a20 CQuestManager::DeactivateQuest DIFFER(693v693): __find's empty iterator tag is homed in the live `reason`
  arg slot [esp+34h] in retail, at [esp+13h] in ours (the flag then shifts to [esp+12h]) | tried: POD tag, named tag
  local, iterator_category() helper, find() wrapper, direct call | next: none known. The tag homing looks
  unreachable from source.

## Skipped
None. Every row was attempted.

## Label corrections
- 00ec1910: "CQ_TentacleKrakenBossFightScript::Main" is NScript::CV_RandomPopulationSimScript::Main.
- 00e75ba0: "CQ_SummoningTheShipScript::Main" is NScript::CV_ExposeMayorScript::Main.
- 00dd23a0: "CQ_OrchardFarmRaidEvilScript::Init" is CQ_OrchardFarmRaidGoodScript::Init.
- 00dd9520: "CQ_SecretPassageScript::PrisonEscape" is CQ_PrisonEscapeRescueMotherScript::PrisonEscape. It is spawned
  from CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0 (CSpawnedFunc vtable 0x012DB954).
- 00ee76f0: "CQ_TraderEscortScript::TurnToBalv" is NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Main.
- 0x00DF0180 is the SecretPassage WatchForLeaving thread. Landed 0x00E76040 belongs to CV_ExposeMayorScript.
- 0x00F35B10 / 0x00F35B30 are CEntityScriptBase::SetPredicate / IsActiveThreadTerminating.
- 0x004B8C00 and its siblings ("SortTreeRecursively" / "FrameUpdate") are list clear() instantiations.
- 0x0049D870 "GFGetPreMainMemoryUsed" is CWorld::GetWorldFrame.
- 0x004BA390 "remove_if" (propagated) is list<CCountedPointer<CActiveQuest>>::remove.
- 0x004B8770 "vector_pop_back_shift" is vector::erase.
- 0x009F0500 "NQuestManager::GetAnimationEntry" is the static CCountedPointer manager's section-handle lookup
  (manager global 0x013CAA68).
- 0x00BE0140 is vector::resize(n). 0x00BDFE70 "std_vector_insert_fill_stride" is _M_insert_overflow.
- 0x0041A7E0 (labels: operator+ / ConsoleSaveCamera) is a C2DBoxF(const C2DVector& topLeft, const C2DVector& size)
  constructor (thiscall, `ret 8`).
- Retail CObjectCacheGroupCollection is 0x38 bytes: the PDB layout without SourceObjectCount (vtable 0x012A7DDC).
- Retail CQuestInfoElement* classes are the PDB minus 4 after the 12-byte handle vector. CounterList: Graphic +0x1c,
  Value +0x20. Counter: PCounter +0x1c, Graphic +0x20, Value +0x24, MaxValue +0x28. Timer: TimerIndex +0x1c,
  Graphic +0x20.
