# Hand chunk 309 (CThingTrackNode .. CV_ChapelOfEvilScript): 25 of 40 byte-exact

## What matched (25)
All 25 are RELOCATION_MATCH with no extra flags; every one has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).
- **CTools**
  - 008751d0 GetIntersectionWithWorldMap marches from start to end in halving steps and refines the ground hit (at most 4 hits, 100 steps).
    - Shape: `return hits > 0 ? true : false;`. The plain compare puts `xor eax,eax` before `setg`.
    - Declare `remaining` before `initial`. The 0x0129BA3C epsilon must be an extern variable, not a literal.
- **CUserProfileManager**
  - 00407370 ApplyValues: every profile read is an inline Get* accessor that picks the new-profile copy.
  - 00408a70 SetAllTutorialsAsGiven: CCharString::EndsWith is an inline member (length difference, then the strcmp intrinsic on the tail).
- **Script entity Mains**
  - 00e05360 CTraderComment::Main: the trader list is a function-scope STLport vector. VC expands its dtor at three exits and calls the 0x008AC970 copy at two exits under `#pragma inline_depth(0)`.
  - 00e0d210 CUndeadRisingBoss::Main: the effect is reassigned through the inline counted-pointer operator=.
  - 00e0d850 CUndeadRisingCentre::Main: a CTimer RAII local, and a switch over three lines.
  - 00e0e2e0 CUndeadRisingZombieAttackerBoss::Main: `ParentClass->Add(new CSpawnedFunc1(...), "")` with conditional name temps.
  - 00e9a5c0 CThunder::Main: health-halving duel stages; the handle is released both before and after scripting.
  - 00ef25e0 CTriggerCrate::Main: an inline C3DVector operator+, where VC folds the `Z + 0.0f` term.
- **CQ_WaspBossScript / CV_AmbushScamScript / CV_ArcheryCompetitionScript / CV_BanditTollScript**
  - 00e0f1d0 EndMission: script WAIT loops. The empty CWideString is built before the pushes, and the "PicnicArea" wait is written `!= false`.
  - 00e2e520 WatchForHeroAttack and 00e331f0 WatchForOwnerKilled:
    - `for (;;)` with the terminate return first;
    - `__forceinline ~CScriptThing`, with `#pragma inline_depth(0)` around the returns that call the out-of-line copy;
    - IsAlive in the `Data && Data->IsAlive()` form.
  - 00e480d0 ResetToll: conditional string temps keep a frame flag word, and the second wait is written `!= false`.
- **CV_BodyGuardScript**
  - 00e5d4f0 Main: the explicit `CCharString("")` argument is built after the new-expression.
  - 00e5db30 FisticuffsWatch: `"ObservePosBG" + GFIntToCharString(i + 1)` marker temps are destroyed inline after each call.
- **CV_BookCollectingScript**
  - 00e566f0 BookReaction and 00e569d0 DoConversation: straight script sequences; `handle` is picked per speaker. The terminate returns use inline_depth(0).
- **CV_BordelloScript / CV_BeardyBaldyScript**
  - 00e3a350 CreateCustomers: an inline CTimer, with the three name strings as implicit conversions.
  - 00e3e720 PlayCutscene: the landed CutsceneScreamers recipe (cast map plus handle dtors).
  - 00e3e320 IsHeroDressedAsLady: four `if (a||b||c) ++count;` tests, plus a named bool for `count == 4`.
  - 00e44a40 and 00e53670 IsHeroWearingBeard: an eight-way `||` chain of conditional temporaries, with ebx bit flags 1, 3, 7 .. 0xff.
- **CV_ChapelOfEvilScript**
  - 00e6a0b0 WatchForInsideChapel: the inline virtuals IsNull and GetDefName on a local are devirtualised. The loop is `for (;;)` with the terminate check first.
  - 00e6af20 WatchForAcolyteHit: `__forceinline` dtor plus inline_depth(0) on the two in-loop returns.
- **CTonyTestScriptsScript**
  - 00eff1c0 AddQuestsToGuild: seven ReplaceQuestCards statements, with implicit temps built right to left.

## Parked for Fable 5.1 (14)
- 004085f0 CUserProfileManager::ResetAssignedInputs DIFFER(622v557): retail loads the def pointer into edi once and keeps it, with &Controls in ebp, across both vector assignments; the scheme tails cross-jump, while ours reloads the address-taken local after every call | tried: GetDef out-param stand-in, named local def pointer | next: a `T*&` GetDef out with a value copy local before the assignments, plus a shared tail per scheme
- 004091c0 CUserProfileManager::LoadFileList DIFFER(534v534): /GS; every instruction matches, but retail packs the two push_back tag bytes into the `more` flag dword (0x250 frame) where ours gives them their own dword (0x254), so all stack offsets shift by 4 | tried: a ctor on CDateAndTime (hidden ecx result), a named path local, tag byte locals in five scope/order variants | next: put the two tag temporaries in a scope block shared with the flag, or make them default-argument temporaries
- 004093e0 CUserProfileManager::SetDefaultValuesVideo DIFFER(924v841): the def branch is out of line in retail, width and height sit in ebx/ebp with -1 compares, the clamp loops keep the index in a register, and ours peels the refresh loop | tried: four variants (null-first accessor, `*(begin()+i)`, while vs for loops, named width/height locals) | next: `const T&` def locals and a do/while refresh loop
- 0040b790 CUserProfileManager::GiveTutorialForCategory DIFFER(938v918): retail caches the category in ebx, with the tag temp in the dead param slot (0x14 frame); ours keeps it in memory (0x18 frame) | tried: three variants (by-value copy local, inline accessor, reordered tag) | next: read the category into a register local before the first call and pass the tag as a default-arg temp
- 00760e70 NShape::CTrack::GetMinDistanceToTrack DIFFER(606v603): retail keeps pos in ebx and puts best in the dead pos arg slot (0x44 frame); ours copies cur through ebx/ebp (0x48) | tried: three variants (raw float pointers, a C3DVector copy ctor, reordered best/cur) | next: lexical-scope blocks so best can take the arg slot, and `const C3DVector&` for cur
- 00837570 CThingTrackNode::Save DIFFER(697v697): stack slots only; retail's operator+ result reuses the conditional-temp flag slot 0xc, and the flag init is scheduled before the out load | tried: `#pragma pack(4)` for the UID offsets, an inline GetUID (fixed the push order), named temps | next: wrap the ScriptName line in its own `{}` block, or build the NULL temp explicitly
- 008378d0 CThingTrackNode::SetAsLinkedTo DIFFER(601v601): a register swap (this/other in esi/edi). The shared SetTrackName tail gives the retail order but swaps registers; per-branch calls fix the registers but emit `mov ecx,esi` before `push ebp` | tried: a shared tail, per-branch calls, a const-ref other | next: an inline helper taking (other, this), or lane gap: regalloc
- 00875b10 CTools::TestMeshForOccupationAndGetType DIFFER(771v838): x87 scheduling; retail spills the getter results through int copies into dead slots (b.x into the mesh arg slot, up.z into a local), where ours folds the loads | tried: inline getters, named float locals | next: `(float)(double)` wrappers and raw float pointer spellings for the probe vectors
- 00877840 CTools::IsLineOfSight DIFFER(1039v1023): frame layout only; retail homes the hit-thing local in the dead `ignore` arg slot (0x4c frame vs 0x50) and places the best/candidate vectors at other offsets | tried: one full decode with flow and calls matching | next: lexical-scope blocks for the vectors, and reuse the ignore param as the hit local
- 0098ecd0 CThreadedFileManager::ProcessObjects DIFFER(777v786): retail homes the GFGetTime double in an 8-aligned frame (and esp,-8), where ours keeps it in st0; retail also caches &CriticalSection in esi and makes the leave/wait block the fall-through of `status==2` | tried: a double local, an inline lock accessor | next: take the double's address (pass it by pointer to a noinline stand-in) plus a `CCriticalSection&` local
- 00e36750 CV_ArcheryCompetitionScript::DoCountDown DIFFER(530v536): tail merge; retail keeps the post-wait "Destroy; DeregisterTimer; ret" exit as its own block, while VC folds it into the function-end copy | tried: two variants (full exits written out, a for(;;) vs while wait) | next: inline_depth(0) around the timer-loop return, or lane gap: tail merge
- 00e5e350 CV_BodyGuardScript::HenchmanCommentary DIFFER(584v585): one missing instruction, the dead `mov [esp+0xc],0` store to the first CTimer slot before RegisterTimer | tried: CTimer RAII and explicit init | next: a CTimer ctor `Index = 0; Index = RegisterTimer();`, or an aggregate init of the timer array
- 00e6b900 CV_ChapelOfEvilScript::TeleportAcolytes DIFFER(860v841): retail keeps the merged out-of-line return block at the first terminate return (alive branch); ours keeps it at the last, and the in-loop eax/edx rotate | tried: per-arm exits written in full, inline_depth placement | next: inline_depth(0) only on the alive-branch return, and swap the if/else arm order
- 00f3ba70 CTriangleStripifier2::BuildCacheOptimizedVertexList DIFFER(684v853): first-pass decode; the look-ahead cache test and the argument roles are not settled | tried: one decode pass (CArray growth path and frees in place) | next: finish the decode against EgoCore's stripifier before iterating on the shape

## Skipped
- 00d043a0 (catalog CV_ChapelOfEvilScript::EndMission, really CQ_BanditCampScript::CheckForSecondAreaDoorHelp) SKIP(948v964):
  - The row holds a 5-entry jump table and over-captures the next function, 0x00D04760 `lea eax,[ecx+4]; ret`.
  - The code is byte-identical (differ/00d043a0.cpp). Land it with verify_land_jumptable.py on a trimmed oracle.

## Label corrections
- 0x00D043A0 is CQ_BanditCampScript::CheckForSecondAreaDoorHelp, not CV_ChapelOfEvilScript::EndMission.
- 0x00E0F1D0 is CQ_WaspBossScript::EndMission, not CV_ChapelOfEvilScript::EndMission. Evidence: the def field it reads is retail +0xE58 = PDB +0xF08 `WB_ExperienceReward`, it waits on the "PicnicArea" level, and it is referenced only from a table at 0x00E0F07C.
- 0x00E3E320 is CV_BordelloScript::IsHeroDressedAsLady, not IsHeroWearingBeard. It counts dress items; the Ego_r.pdb method exists and it has 15 Bordello callers.
- 0x00E53670 is CV_BeardyBaldyScript::IsHeroWearingBeard. It is a byte-identical copy of 0x00E44A40, called only from GoTalkToBeardyBaldy (0xE524E0, 0xE52FED). 0x00E44A40 is the CV_BordelloScript original, called from the nested CMagicman (0xE4304D).
- 0x00E05360 is NScript::CQ_TraderEscortScript::CTraderComment::Main. 0x00E9A5C0 is NScript::CV_HeroDuelScript::CThunder::Main.
- 0x00A11C20 and 0x00A11C90 are CPhysicsMeshObject getters; the ParticleLine labels are propagated. 0x0099B0F0 is CWideString::operator!=.
