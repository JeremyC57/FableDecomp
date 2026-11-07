# Hand chunk 839 -- notes from the authoring agent

Eight new rows (no prior source anywhere), all with PDB names from the brief; every name was checked against two sources
before authoring (Ego_r.pdb / FableWin.pdb name set + a landed caller or the body's member reads, as recorded on each
source's `Identity` line). 3 matched (RELOCATION_MATCH), 5 parked with their best source and test in differ/.

## What matched

### 0x00E7AB50 NScript::CV_ExposeMayorScript::PhaseFour (src/00e7ab50.cpp, a1 RELOCATION_MATCH)
Shape: stage-3 body under `if (StageNumber == 3)`, translator's terminate check opening every block, level wait, the ghost
cut-scene once, objective 4, the evidence wait, then the married arm (movie + CWideScreenMagicPauseEntities, GREYMARRIED,
TakeObjectFromHero, RemoveQuestCardFromHero, Path 2) or the choice arm (GREYCHOICE, GiveHeroYesNoQuestion, answer wait,
yes: THUNDERDEAD / GREYAGREE + take + Path 2, no: GREYREFUSE + Path 1); `StageNumber = 4` after the if. Both answer arms open
with a terminate check that VC7.1 hoists above the `answer == 1` test. Include set = the landed DoMission / CEM_Sheriff model
with FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR/DTOR (COMDAT calls 0x006E7B60 / 0x006E7B80) and
FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR.
Callees (evidence: the retail call targets + the landed rows' bindings): CScriptBase::IsActiveThreadTerminating 0x00CB7940,
CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, PlayCutscene 0x00E78100 (this chunk), the movie ctor /
dtor 0x006E7B60 / 0x006E7B80, CGameScriptInterfaceBase slots 7, 11, 39, 70, 114, 125, 184, 294, 296, 370, 379, 655 (bin/slot.py
against the header). Layout: engine/CV_ExposeMayorScript.h (Game +0x40, MasterScriptData +0x44, Path +0x48, StageNumber +0x4c,
MayorExposeGhostCutscenePlayed +0x55, Evidence +0x98), CQ_SunnyvaleMasterData DefeatedThunder +0x61 / KilledThunder +0x63 /
HeroMarriedLadyGrey +0xd7. Label: catalog module `CV_ExposeMayorScript` lacks the NScript namespace (LABELS.tsv).

### 0x00E78100 NScript::CV_ExposeMayorScript::PlayCutscene(CCharString, bool, CCharString) (src/00e78100.cpp, c1 RELOCATION_MATCH)
The landed CV_SickChildScript::PlayCutscene 0x00ECE460 body on the CQ_FireHeartScript 0x00D39330 shared-header model: csargs
map, hero STB under StartScriptingEntity(..., SCRIPT_AI_PRIORITY_HIGHEST), `if (!arg.EqualsCaseInsensitive("NULL"))
csargs["$ARG1"] = arg;`, cast HERO / PRIS / SHER / LADY / RHODRI (= seh_Suspicious +0x58, seh_Sheriff +0x78, seh_LadyGrey +0x68,
seh_Rhodri +0x88), `if (fade) { movie; pause; fix; RunCutsceneMacro; unfix } else { fix; run; unfix }`. Key spelling: the
"NULL" test must be EqualsCaseInsensitive (263 -> 0 diff lines; `!=` picks the out-of-line operator!=(const char*), an explicit
CCharString("NULL") temporary loads it through eax). Callees as on the source's Callees line (CBaseObject 0x0099A380 /
0x0099A3B0 / 0x0099A430, _stricmp 0x00BFEBA8, map operator[] 0x009AC700 / 0x00CD3D2E, _M_erase 0x009AA830 / 0x00CD344A, the
movie PImp dtor 0x006E7AB0 (declared-only specialization inside the expanded implicit movie dtor), RunCutsceneMacro
0x00CBFB7D). `__forceinline` STB operator= (FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN model).

### 0x00DE6730 NScript::CQ_RansomVictimScript::PlayCutscene(CCharString, CCharString) (src/00de6730.cpp, c1 RELOCATION_MATCH)
Same model, no fade argument (`ret 8`), cast HERO / GUARD / CHIEF / VICTIM / BROTHER / BANDIT = seh_Guard +0x58, seh_Father
+0x68, seh_Victim +0x78, seh_Brother +0x88, seh_Ransommer +0x98 (engine/CQ_RansomVictimScript.h), camera fixed around the
macro. LABEL CORRECTION: the catalog / batch label `CV_ExposeMayorScript::PlayCutscene` is wrong -- this is
`?PlayCutscene@CQ_RansomVictimScript@NScript@@QAEXVCCharString@@0@Z` (two by-value strings; the Expose-Mayor one is 0x00E78100
with three parameters). LABELS.tsv + NAME_FIXES.tsv.

## Parked for Fable 5.1

### 0x00E76E90 NScript::CV_ExposeMayorScript::HeroFlashesLamp(long) -- differ/00e76e90.cpp, DIFFER(1613v1612), 10 diff lines
Shape: `if (flashes == 0) { T; point = GetThingWithScriptName("EM_FlashLightPoint"); if (IsDistanceBetweenThingsUnder(hero,
point, MaxFlashDistFromStables)) { T; HeroFlashedLightsCorrectly = true } else { T; ResetLamp = true } } else { CTimer timer;
timer = 3; if (lit) { T; while (timer.GetTime() > 0) { NSF; T; if (!lit) { T; HeroFlashesLamp(flashes - 1) } } T } else {
mirror } ResetLamp = true }`. Retail expands the recursive call one level into itself (two copies C1 / C2) -- reproduced with
`#pragma inline_recursion(on)` (documented pragma, 9 landed rows); without it the call stays a call (md 378). CTimer model:
inline ctor `TimerIndex(pDSTGame->RegisterTimer())` (0x0143E8F8, slot 0x15c), dtor `DeregisterTimer` (slot 0x160),
__forceinline operator=(long) / GetTime (engine/CTimer.h). IsDistanceBetweenThingsUnder 0x00CBE2FF __fastcall
(?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z, the landed 0x00D2C530 spelling).
Residue: in C2 (the unlit arm's expanded copy) the shared expanded ~CTimer exit block that both inner arms' in-loop terminate
checks reach is laid out by retail right after C2's first arm (0x00E7740D), by VC7.1 here at the end of C2 -- a block-order
difference inside an inlined body, 1 byte longer. Everything else (top level, C1, the ctor/dtor expansion mix) matches.
Tried (21): see STATUS.tsv; the per-arm store forms (b12, b25), arms swapped (b21), `continue` form (b24) are all worse or equal.

### 0x00E482F0 NScript::CV_BanditTollScript::CheckForLeaderDead() -- differ/00e482f0.cpp, DIFFER(1753v1627), 176 lines
Shape: LeaderDead wait, SetQuestAsCompleted(GetActiveQuestName(), false x3), `CScriptThing Leader = GetNearestWithScriptName
(hero, "TauntingBandit")`, `if (Leader.IsAlive()) { T; conversation = AddNewConversation(Leader, false, false);
AddPersonToConversation(c, hero); AddLineToConversation(c, "TEXT_QST_011_BANDIT_ON_KILL_LEADER_10", false, Leader, hero);
BanditsFlee = true; IsFactionSet = false; three wait blocks (TauntingBandit / TollBandit / BlockingBandit): outer `while
(x.IsAlive()) { NSF; T; inner while (ref(x).IsAlive()) { NSF; T } T; x = GetNearest(...) } T` }`, then the three sign tags
(BT_Sign_Far / _Close / _Closer -> TEXT_QST_011_SIGN_FAR_GONE / CLOSE_GONE / CLOSER_GONE) and DeactivateQuestLater(name, 0).
Flags: engine/CV_BanditTollScript.h LeaderDead +0x4e, BanditsFlee +0x4f, IsFactionSet +0x51. IsAlive: inline
`PImp.Data && PImp.Data->IsAlive()` gives retail's 2 expanded + 4 called (0x004AB130) mix; the inner waits test through a
same-type `reinterpret_cast<const CScriptThing&>` (stays virtual, as retail) -- the CHTE_FreedomFighter 0x00D6A560 lever,
noted on line 2 (review at landing).
Residue: CScriptThing::operator= (CCountedPointer<CScriptThing> assignment) is expanded by retail at the first site only and
called (0x008AB980) at the two loop re-assignments; plain inline expands none (md 371: frame 0x40 vs 0x44, the dead
`mov [esp+14h],eax` Game spill missing), `__forceinline` expands all three (md 176, handed back). Not already in GOTCHAS:
the PImp-level site-1 lever (a7) and the for-loop skeleton (a8) -- both worse.

### 0x00DE4710 NScript::CQ_RansomVictimScript::DisappearFollowers() -- differ/00de4710.cpp, DIFFER(1913v1428), 473 lines
Shape: `for (;;) { T; if (IsRegionLoaded("WitchwoodCavern") && !RescuedVictim) { T; GetFollowingEntityList(hero, followers);
num = size; seh_followers.resize(num); per follower { T; release the old STB if valid; while (!StartScriptingEntity(f, seh,
HIGHEST)) { NSF; T } T; EntitySetAlpha(f, 0, true); SetIsThingForcePushable false; EntitySetAsRespondToHit false;
EntitySetAsDamageable false; EntitySetTargetable false; EntitySetAsDisplayingEmoteIcon false; EntitySetAsHiddenOnMiniMap true }
T; while (IsRegionLoaded(...) != false) { NSF; T } T; per follower { T; release; the seven calls with the opposite values } T }
NSF; T }`. Retail skeleton: `xor edi,edi` zero register, initial T `jne` to the first expanded exit pad, body, NSF, T `je` head.
Callees: 0x00CD319B ~vector<STB> (catalog placeholder StdList_Dtor_00cd319b is wrong -- LABELS/NAME_FIXES), 0x008AC970
~vector<CScriptThing>, STB::Release / IsValid inline, vector<STB>::resize (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE).
Residue: documented per-exit vector-destructor expansion split (retail: 4 expanded pads for the depth <= 1 exits + one shared
pad calling 0x00CD319B / 0x008AC970 for the 6 deeper exits; ours expands all 12 frees). New thing tried: reference locals
for the entity (a2, best) and the `for (; !T; NSF)` frame-loop skeleton (a3, worse); parked early as the documented blocker.

### 0x00D0EE70 NScript::CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre() -- differ/00d0ee70.cpp, DIFFER(2536v1989), 640 lines
Shape: Gate1 open wait (`GetThingWithScriptName("Gate1").IsOpenDoor()`, slot 64), `killed = 0; for (; !T; NSF) { if
(IsLevelLoaded("BanditCampMain")) { T; wait while loaded; T } vector<unsigned long> kills; hero MsgGetThingsKilled (slot 55)
-> killed += size, clear; vector<CScriptThing> things; GetFollowingEntityList -> per thing T + tally; T;
GetHeroSummonedCreaturesList -> same; T; if ((float)killed >= pGlobalScriptDef->BAC_EnemiesToKillInFirstArea (+0xe78)) { T;
{ movie; StartMovieSequence; CWideScreenMagicPauseEntities; STB seh_Hero; StartScriptingEntity(hero, seh_Hero, 4);
FadeScreenOut(0.5f, 0.5f, CRGBColour(0,0,0,255)); Pause(1.0f); map<CCharString,STB> cast; RunCutsceneMacro
("CS_BANDITCAMP_ALARM_OUTER", cast, 0, 0, false, true); RemoveThing(GetThingWithScriptName("Gate2Guard1"), false, true) }
Gate2Open = true (+0x4c); GetAllThingsWithScriptName("DefensiveGuardBandit", bandits); per bandit T + GiveThingBestEnemyTarget
(b, hero); T; leader = CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", GetThingWithScriptName("DefensiveGuardLeader").GetPos(),
"", false); GiveThingBestEnemyTarget(leader, hero); return } }`.
Callees / spellings: 0x00CDBF70 / 0x00CDBFB0 map<CCharString,STB>::map() / ~map() (declared-only explicit specializations;
the landed 0x00E0A820 / 0x00E12330 spellings -- the landed 0x00D246C0 / 0x00D29DB0 bind them as CCutsceneCastMap::Construct /
Destroy / ??0CCutsceneCastMap, NAME_FIXES), 0x0046496F vector<unsigned long>::erase(first,last) (landed 0x00D032F0 spells it
ThingIDVector::erase), 0x0045019A ~vector<unsigned long> (placeholder ReleaseFirstMember_0045019a), 0x00450C72
vector<unsigned long>(const allocator&) COMDAT (catalog CIndexBuffer::CIndexBuffer is a twin label), 0x008AC970
~vector<CScriptThing>, 0x004AA840 ~CScriptThing, 0x007E74D0 ~STB (virtual), 0x00CBFB7D RunCutsceneMacro, _CIpow-free.
One symbol, one address: the map ctor/dtor COMDATs 0x00CDBF70 / 0x00CDBFB0 are the same symbols the landed 0x00E0A820 /
0x00E12330 bind (same addresses, no conflict); the PlayCutscene rows of this chunk expand their own map ctors inline (no call).
Residue: documented per-exit destructor expansion split (~CScriptThing 1 expanded + 3 called, ~vector<CScriptThing> 6 called +
2 expanded at the second for-loop's exits, ~vector<unsigned long> 2 + 2 plus the allocator-ctor COMDAT). New things tried:
the explicit-specialization declarations for the map ctor/dtor (fixed those calls), the inner cut-scene block (one movie
dtor), `inline CScriptThing::~CScriptThing() { }`; parked early as the documented blocker.

### 0x00775350 CTCShop::UpdateStock() -- differ/00775350.cpp, DIFFER(1971v1988), 791 lines
Shape (decoded from the bytes; no prior source): `CDefPointer<const CThingBaseDef> thingDef; GetDefinitionManager().GetDef
(ParentThing->GetDefGlobalIndex(), &thingDef); CDefPointer<const CShopDef> shopDef; if (!thingDef->GetSubDef(&shopDef)) return;
day = GetWorld().PeekGameTimeManager().GetDayCount(); daysSince = day - DayLastChecked;` per Stock entry: `daysSince >
100000` -> bulk revert of MaxStock towards FindDefaultStockItem(key)->MaxStock (period 0: one step of daysSince * revert;
else lastRestock = ((day - 100000) / period) * period, above DayLastChecked: two steps `(lastRestock - DayLastChecked) * revert
* days` and `(day - lastRestock - 100000) * revert * days`, else one), otherwise per day d: revert one step, then on
`d % RestockPeriod == 0` Quantity = max(Quantity, MaxStock) else GFFloatRandom(1.0f, GetPWorldSeed()) (result dropped) and
Quantity = max(Quantity - SalesPerDay, 0); then a sorted vector<long> of the def's DefaultStock types, every Stock key not
binary_search-found with Quantity == 0 and (RestockPeriod == 0 or MaxStock == 0) collected and erased (erase-by-key); the
special item: DaysSinceSpecialItemPlaced += daysSince, erased past NumDaysToKeepSpecialItem; with none and GoldDonated >
MinAmountBeforeSpecialItem: `chance = (float)pow(1 - ProbabilityOfSpecialItemPerDay, (float)daysSince)`, roll
`GFFloatRandom(1.0f, seed) > chance` (inlined body) -> GoldDonated -= MinAmount, candidates = def types whose
Item.PriceMultiplier is within 0.0001f of SpecialItemPriceMult, random_shuffle, SpecialItem = candidates[0],
Stock[SpecialItem] = *FindDefaultStockItem(SpecialItem) (28-byte rep movsd), .Quantity = 1.0f, DaysSinceSpecialItemPlaced =
0; finally DayLastChecked = day.
Names: CTCShop members from engine/CTCShop.h (Stock +0x34, SpecialItem +0x40, DaysSinceSpecialItemPlaced +0x44,
DayLastChecked +0x48, GoldDonated +0x4c); CShopDef members (Ego_r.pdb CShopDef: DefaultStock +0x40, MaxStockRevertPerDay
+0x88, NumDaysToKeepSpecialItem +0xc0, MinAmountBeforeSpecialItem +0xc4, ProbabilityOfSpecialItemPerDay +0xc8,
SpecialItemPriceMult +0xcc) and CGameTimeManager::DayCount +0x14 are local models (no engine header; header debt);
`MoveTowards` 0x007720D0 keeps the catalog's descriptive label (unbound, no PDB name); `GFFloatRandom` 0x004978A0 /
`GFROR13` 0x00497890 / `GetPWorldSeed` 0x00686DA0 / `GetDef<CThingBaseDef>` 0x0042AF3C / `GetSubDef<CShopDef>` 0x005130A0 /
`_M_insert_overflow` 0x00440990 / `sort<long*>` 0x00778540 / `map<long,CShopItem>::operator[]` 0x00778280 /
`_Rb_tree::erase(iterator,iterator)` 0x007775B0 as in NAME_FIXES.tsv (several carry wrong landed spellings).
Residue: function-wide inline budget split -- retail expands the MoveTowards helper at 2 of 5 sites and calls 0x007720D0 at
3, and expands both erase(key) bodies and random_shuffle; VC7.1 here expands the helper x5 under every linkage tried and
then calls random_shuffle / erase(key) out of line. Not in GOTCHAS: a TU-local non-inline static helper (auto-inline) and the
genuine STLport chains (hdr opt-ins) -- the chains made the budget effect visible (a4 -> a8). The early-return CDefPointer
release shape (`dec [ecx+4]; jne`) is tail-merged differently here (register choice) -- secondary.
FindDefaultStockItem's result is dereferenced without a null test (retail too: the landed 0x00772250 falls back to the
category stock entry).

## Skipped
None.

## Label corrections
- 0x00DE6730 -> NScript::CQ_RansomVictimScript::PlayCutscene (catalog: CV_ExposeMayorScript::PlayCutscene, wrong).
- 0x00E7AB50 / 0x00E78100 -> NScript:: namespace added to the catalog module.
- 0x00CD319B -> std::vector<CScriptGameResourceObjectScriptedThingBase>::~vector (placeholder StdList_Dtor_00cd319b).
- 0x0045019A -> std::vector<unsigned long>::~vector (placeholder ReleaseFirstMember_0045019a).
- 0x00450C72 -> std::vector<unsigned long>::vector(const allocator&) (catalog CIndexBuffer::CIndexBuffer, twin label).
- 0x007720D0 -> keeps _global::MoveTowards (descriptive).
Wrong landed spellings for 0x0046496F, 0x008AC970, 0x007E74D0, 0x00CDBF70, 0x00CDBFB0, 0x004978A0, 0x00497890, 0x00686DA0,
0x00440990, 0x00778280, 0x0042AF3C: NAME_FIXES.tsv.

## Header debt
- stlport/fable_map.h FABLE_RETAIL_STLP_TREE_ERASE_KEY, stlport/fable_containers.h FABLE_RETAIL_STLP_BINARY_SEARCH /
  FABLE_RETAIL_STLP_RANDOM_SHUFFLE (hdr/, opt-in only; used by the parked 0x00775350).
- engine/CShopDef.h does not exist (engine/_quarantine/CShopDef.h only): the UpdateStock row models the six members it reads.
- No engine/CGameTimeManager.h: DayCount +0x14 modelled locally (the landed CTCShop::FrameUpdate 0x00775B40 model).
- GFFloatRandom's inline body (engine/GFRandom-style header) would let rows that expand it share one definition.
- A per-site vector-destructor lever (the DisappearFollowers / FirstAreaMassacre blocker) is still not expressible in
  stlport/fable_containers.h.
