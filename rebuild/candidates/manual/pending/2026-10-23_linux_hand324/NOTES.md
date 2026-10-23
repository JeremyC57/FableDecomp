# Hand chunk 324 (CTCCreatureFlyInCircles .. CTCInventory): 8 of 40 byte-exact

## What matched (8)
- CTCInGameMenu 0047eee4 ConstructSaveGameFileDescription: /GS biased frame from optimize("s") alone; `*(begin()+i)` operator[]; per-arm one-statement GetEntry temps tail-merge.
- CPlayer::~CPlayer 00488020 (catalog label CTCFireheartMinigame::~CTCFireheartMinigame is wrong; vtable 0x01231CC4).
- CTCHeroOnlineScoreboard 00561980 / 005621b0 (DownloadMyClansLogos / DownloadLogosForThisClansMembers): implicit const char* -> CCharString at the reply lookups; same-TU noinline stand-ins.
- CTCHeroOnlineScoreboard 00562710 InitialiseActivityInfo: resize(39) then one `ActivityInfo[n].Field = "..."` statement per store in retail order (generated from the disassembly), byte fields last.
- CTCInventory 00582ace / 0058315b ConstructStyleEntry(PC): size TU; cached loop end.
- CTCHeroMorph 0071c360 InitialiseCustomTattoos: implicit CCharString conversion + same-TU GetSubDef stand-in.

## Parked for Fable 5.1 (31)
- 007dd540 CTCCreatureFlyInCircles::UpdateSwoop DIFFER(1074v1099): frame 0x40 v 0x4c (12-byte temp placement); register diffs in burn path | tried: see differ/007dd540.cpp header | next: continue from the differ source
- 0074b2b0 CTCCreatureGenerator::CreateCreatures DIFFER(1248v1208): register allocation (zero kept in ebx, this edi v esi), frame +8 | tried: see differ/0074b2b0.cpp header | next: continue from the differ source
- 0074c130 CTCCreatureGenerator::FrameUpdate DIFFER(1220v1202): def release dec [edi+4] (no null test) + frame-slot placement; structure, remove/erase, effect/sound timers match | tried: see differ/0074c130.cpp header | next: continue from the differ source
- 007c99f0 CTCCreatureGeneratorCreator::FrameUpdate DIFFER(1186v1123): frame 0x38 v 0x30 + register assignment (this edi v ebx); control flow and callees as retail | tried: see differ/007c99f0.cpp header | next: continue from the differ source
- 008651b0 CTCCreatureHitNotification::SetHitNotification DIFFER(685v1161): x87 temporary chains for the edge points not reproduced (first draft) | tried: see differ/008651b0.cpp header | next: continue from the differ source
- 006cc270 CTCCreatureNavigation::vfunc_11 DIFFER(1035v1085): label: really CTCCreatureNavigation::DrawPreparePrimitivesForRendering (vslot 11); frame/temp order | tried: see differ/006cc270.cpp header | next: continue from the differ source
- 00683c10 CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex DIFFER(1085v1085): guard block placement (retail at end) + register assignment; same size | tried: see differ/00683c10.cpp header | next: continue from the differ source
- 0077a430 CTCDExperienceOrb::FrameUpdate DIFFER(1067v1115): lane gap: inlined asm round-to-int helper at entry (fistp + 0.5/1.0 slots) | tried: see differ/0077a430.cpp header | next: lane gap
- 0080d300 CTCDExplosion::UpdateDynamicExplosion DIFFER(999v1097): x87 ring-direction sequence + pair conversion temporary (first draft) | tried: see differ/0080d300.cpp header | next: continue from the differ source
- 008101f0 CTCDExplosiveTrail::FrameUpdate DIFFER(1608v1914): x87 steering block (acos/tanh) approximated (first draft) | tried: see differ/008101f0.cpp header | next: continue from the differ source
- 007ef710 CTCDecapitation::Decapitate DIFFER(1157v1244): frame/slot sharing + x87 fling sequence (first draft) | tried: see differ/007ef710.cpp header | next: continue from the differ source
- 0078ecd0 CTCDivineWrath::CTCDivineWrath DIFFER(2000v2034): frame-slot layout (cond-temp flag, dead-param spills) + hand-effect/PlaySound ordering; structure and callees as retail | tried: see differ/0078ecd0.cpp header | next: continue from the differ source
- 0078a690 CTCHeroRangedAttacks::FrameUpdate DIFFER(1094v1097): frame 0x1c v 0x14 (lookup key words not shared; vector start not kept in ebp) + bitset byte narrowing lane gap; control flow and callees as retail | tried: see differ/0078a690.cpp header | next: lane gap
- 0071d3b0 CTCHeroMorph::UpdateTattoos DIFFER(1017v1027): register assignment (tattoo def ebx v esi, texture id kept in memory/eax v ebx); CDateAndTime needs a user ctor for the fastcall hidden-ecx return | tried: see differ/0071d3b0.cpp header | next: continue from the differ source
- 005850fd CTCInventory::GetStyleCardsListUIComponent DIFFER(1660v1678): block placement of the hair-card arm (retail out of line after the join / after the epilogue); everything else lines up | tried: see differ/005850fd.cpp header | next: continue from the differ source
- 00589402 CTCInventory::GetStyleCardsListUIComponent DIFFER(1581v1582): PC twin of 005850fd; same hair-card-arm block placement residue | tried: see differ/00589402.cpp header | next: continue from the differ source
- 00587bff CTCInventory::OpenPCSkillsMenu DIFFER(1150v1150): frame 0x20 v 0x1c (one extra reserved word in the find-loop group; advisor variants inert) | tried: see differ/00587bff.cpp header | next: continue from the differ source
- 0047e196 CTCInGameMenu::SaveGame DIFFER(1211v1211): frame-slot sharing only (first SetText key temp not sharing the -8 name slot); shape/callees/size as retail | tried: see differ/0047e196.cpp header | next: continue from the differ source
- 0047fe83 CTCInGameMenu::GetFileNameList DIFFER(1626v1627): one residue: count sum lea form (mov ecx,[first]; lea ebx,[eax+ecx]); loop matched via guard+do-while+named byte offset | tried: see differ/0047fe83.cpp header | next: continue from the differ source
- 007c3220 CTCDrainLife::DoDrain DIFFER(1172v1138): FPO register allocation (ebx zero reg v this/shot def) + frame-slot sharing; structure/callees as retail (first drafts) | tried: see differ/007c3220.cpp header | next: continue from the differ source
- 0072fb70 CTCDummyVillager::FrameUpdate DIFFER(1280v1278): register allocation: lea CSE of def+0xB8 kills edi zero reg (thingDef spill) + vector/best slot order; frame, byte slots, loops, x87 as retail | tried: see differ/0072fb70.cpp header | next: continue from the differ source
- 007b1a30 CTCFishing::CreateFish DIFFER(1207v1145): first drafts: RA (zero reg ebx v cached vector end in edi), def release forms, duplicated failure epilogues; flow/callees/x87 as retail | tried: see differ/007b1a30.cpp header | next: continue from the differ source
- 007c57c0 CTCGhostSword::AddSword DIFFER(1531v1519): frame 0x44 v 0x3c (effect slot not shared with earlier temps) + scratch-register rotation; flow/callees/temps as retail | tried: see differ/007c57c0.cpp header | next: continue from the differ source
- 007b3120 CTCGuard::FrameUpdate DIFFER(1568v1592): first drafts: RA (this/village registers), stats counter load/add/store + stack-copied fild forms, region def addref order, frame 0x20 v 0x1c | tried: see differ/007b3120.cpp header | next: continue from the differ source
- 00707440 CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw DIFFER(1239v1253): first draft: register assignment (this/appearance/zero), morph param temp order, primitive index slot; flow/callees/arg lists as retail | tried: see differ/00707440.cpp header | next: continue from the differ source
- 00563480 CTCHeroOnlineScoreboard::DownloadScoreboardForSelectedClan DIFFER(1178v1178): one eax/edx swap in the two inlined std::string copy ctors; everything else (frame 0x238, 39-vector array, reverse, map copy) as retail | tried: see differ/00563480.cpp header | next: continue from the differ source
- 00560d00 CTCHeroOnlineScoreboard::ProcessLogin DIFFER(1844v1860): first draft: this in ebp (retail edi + late ebp push), std::string temp order, frame 0x98 v 0x9c; jump table -9..-1 | tried: see differ/00560d00.cpp header | next: continue from the differ source
- 006c6990 CTCHeroOpinionDeedLog::PostDeed DIFFER(1183v1168): first draft: frame 0x40 v 0x3c, region-def addref CSE (dec eax v dec [eax+4]), faction-block flag register | tried: see differ/006c6990.cpp header | next: continue from the differ source
- 006c6e80 CTCHeroOpinionDeedLog::FrameUpdate DIFFER(1220v1251): first draft: frame 0x38 v 0x30, deed-map loop rotation and iterator registers | tried: see differ/006c6e80.cpp header | next: continue from the differ source
- 0057c4f1 CTCHeroStats::ApplyBonusItemEffects DIFFER(1169v1192): lane gap: inline fistp round-to-int for the speed duration; plus this/def register swap, fabs compare copies | tried: see differ/0057c4f1.cpp header | next: lane gap
- 0077d5b0 CTCHeroCentre::FrameUpdate DIFFER(-v1504): not drafted: lane gap /GS per-return epilogue (cookie check before pops on the early return); triage notes only | tried: see differ/0077d5b0.cpp header | next: lane gap

## Skipped
- 0074bc30 CTCCreatureGenerator::DrawDebugInfo: catalog row starts 16 bytes into DrawDebugInfo 0x0074BC20.

## Label corrections
- 00488020 is CPlayer::~CPlayer (not CTCFireheartMinigame).
- 00561980 is DownloadMyClansLogos (label propagated from twin 005621b0).
- 006cc270 is CTCCreatureNavigation::DrawPreparePrimitivesForRendering (vslot 11).
- 0x005671C0 is STLport __reverse(first, last, const random_access_iterator_tag&) for pair<float,CWideString> (manifest: WideStringPair_SwapRange); 0x00567EA0 is that vector's default ctor; 0x0056A680 is map<EOnlineScoreboardActivity, vector>::operator[]; 0x00568D40 its vector operator=.
- 0x0055F090 / 0x0055F140 are std::string __fastcall Encrypt/Decrypt helpers with a hidden result in ecx and ONE stack arg (ret 4); 0x00569690 is cdecl operator+(const string&, const string&); 0x0042A34E is _Rb_tree_rebalance_for_erase; 0x00BFEBB0 __aullrem.
