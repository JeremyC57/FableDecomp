# Hand chunk 202 (CQ_GuildTrainingDepartureScript .. CReactionFrequencyTraits_ControlledCount): 34 of 40 byte-exact

All 34 sources in src/ re-graded MATCH/RELOCATION_MATCH with gall.sh (catalog leaf) and with gall2.sh (the leaf finish.py
hands the lander: `// Leaf:` line, else the line-1 name); all 34 tests print `<addr>_TEST PASS` under g++ (tall.sh) and
VC7.1/Wine (tall.py). `tools/rcheck.py` ran reloc_identity.check on every row: no HARD findings (warnings listed under
"Name conflicts"). No pragmas except 0064a568 (`optimize("y", off)` + `("s", on)`), no flags, `pack(2)` only on
NParticleEngine::CRandomAcceleration (PDB puts its C3DVector at +0x0a).

## What matched (34)
- **Quest scripts (18, previous run, re-graded and re-tested):** 00d506b0 Init (implicit marker-name temporary built
  before the pushes); 00e27ac0 CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver (two rotated wait loops, handle local
  destroyed out of line on both exits); 00ded0c0 / 00e1ad60 WatchForTermination twins (`&&` wait condition rotated into
  an entry test of both flags + bottom test of the first); 00e16a80 / 00e16c40 CWB_ScaredVillager / CWB_Villager::Main
  (CIsThingConscious predicate as a scoped frame object); 00d74f80 ReassignLeader (inline pop_back loop through a `lea`
  reference); 00d905c0 MakeBanditComment (merged `return false` yield arms); 00dd0eb0 ReplaceQuestCards and 00e18480
  CQ_WhiteBalverineWWScript::Init (implicit string temporaries built right to left); 00e80c00
  CV_FishingCompetitionScript::SuspendCompetition (`||` of two IsRegionLoaded on implicit temporaries keeps the
  construction flag in ebx); 00dd5860 / 00ddcf50 MakePrisonerComment / CQ_PrisonRaceScript::MakeRacerComment (null
  listener CScriptThing() built first, right-to-left argument evaluation); 00de0320 / 00edeba0 quest-script ctors (the
  argument pair in an inline `__declspec(novtable)` intermediate base, stored before the vtable); 00df06a0 PhaseOne
  (unused scripted-thing handle local); 00e0e820 CQ_WaspBossScript::Init (implicit string temp before the C2DCoordI
  temp); 00e1c4d0 WatchForLeaving (implicit "" temporary inside the `&&` loop condition).
- **CQuestManager (11):** 004b0aa0 RemoveActiveQuestCard, 004b0c80 GetActiveQuestCardFromScriptName, 004b0fc0
  IsQuestCompleted (previous run; tests written this run); 004b0f10 PrepareQuestsWhenFinalQuestIsActivated and 004b11a0
  IsRegionStartOfRegionLockingMode (the unchecked `GetQuestCard` helper: its unset local is seeded from the argument
  word before the walk; the counted CQuestCardDef copy of the landed IsAnyQuestActive); 004b1270
  IsActiveQuestStartedInRegion (definition copy in an inner block so the bool survives the release in bl); 004afb00
  OnDeactivateEntity (twin of the landed OnActivateEntity, PDB `CThing&`); 004afca0 ResetPositionOfAllQuestCards (two
  written-out walks; the physics TC in a named local puts it in esi); 004b26b0 AttachExistingScriptedEntitiesToScript
  (the list<CThing*> sentinel stays in edi across the GetAllThings call only when that template's body is compiled
  in the TU above the caller under `#pragma auto_inline(off)` -- it allocates, so this extends GOTCHAS 356).
- **CQuestInfoElementMiniGameBar::Update 0064a568:** size frame-pointer TU; out-parameter `__forceinline` GetInterface
  (type held in ebx), IS_THING_ALIVE fed through a converted temporary (hero stays in esi), `f("LIT") ? false : true`
  on implicit CCharString conversions (an explicit `CCharString("LIT")` reuses eax instead of `lea`).
- **CReactionFrequencyTraits_ControlledCount::CalculateWeight 006ffe10:** engine header + a `_Methods` struct;
  `count >= max` gives `test ah,1 ; jne` (`!(count < max)` gives `test ah,5 ; jnp`); Clamp(const float&...) fed the
  product as an unnamed temporary gives retail's `fst` into the dead argument slot (a named local gives `fstp ; fld`).
- **CRandomAppearanceMorph::AddTextureMorphToMesh 0086bd20:** sibling of the landed AddTextureMorphToBodyPart; three
  body-part slots by a counted `for`, the meshes by `size()` recomputed each pass.
- **CQuickDrawTriInfo copy ctor 009de890:** bit fields, window, string and texture in the init list; union views and
  metrics assigned in the body; colour through an inline `operator=` calling `Set(r, g, b, a)`.
- **CQuaternion::operator* 00a88b60 (MATCH):** a named local filled W, X, Y, Z, returned by the implicit copy.
- **C3DPrimitive2::~C3DPrimitive2 00a90790:** CArray dtor = `DestroyRange` overloads (no-op for PODs, the landed
  0x00A96940 for the counted cloth pointers, allocator by const reference) + `if (begin) free`; scoped_ptr `if (p)
  delete p`; inline base `~CMovableResource()` stores its vtable last.
- **NParticleEngine::CRandomAcceleration ctor 00b0a210:** the LCG step in an inline helper returning `unsigned long`
  (an `unsigned char` return adds a `mov cl,al` copy per store); `OldOffset = C3DVector(0, 0, 0)` needs a user default ctor.

## Parked for Fable 5.1 (6)
- 00e32200 CV_AmbushScamScript::WatchForTricksterComplete DIFFER(208v188): retail inlines the by-value CScriptThing dtor only on the early return in the wait loop and calls ~CScriptThing 0x004AA840 at the common exit; ours inlines both | tried: in-class inline dtor, out-of-class inline dtor (previous run) | next: in-class inline member called twice is inlined only at the first site (GOTCHAS 217) -- try a third return path or the common exit written as an explicit block so the call site order flips
- 004b0b90 CQuestManager::RemoveActiveQuestCardAndMakeAvailableAgain DIFFER(233v238): retail keeps `this` in ebp and re-reads the name argument per pass (ebx node, edi card, esi the unset TC); ours parks `this` in the frame word and caches the name in ebp | tried: continue / block / function-scope TC forms (previous run) | next: the real Compare 0x00411570 body above the caller under auto_inline(off) (GOTCHAS 233), or the inline completed-list search of the matched twin 004b0aa0
- 004b1590 CQuestManager::ActivateFeatCard DIFFER(223v223): FeatIndex for ActivateFeat loads into eax (ours ecx), the pointer temporary's address is pushed through ecx (ours edx), FeatIndex for remove_if reloads into the dead tc register ebp (ours eax) | tried: && chain, nested ifs, named and unnamed CIntelligentPointer temporary | next: an inline `GetFeatIndex()` accessor or a `long featIndex` local read before the ActivateFeat call; otherwise lane gap: register choice
- 0086b810 CRandomAppearanceMorph::GetRandomTextureMorphs DIFFER(238v230): in the push_back block retail takes the result vector into esi, loads the entry key before its texture and stores the free slot from the same registers; ours keeps the vector in edx, loads the texture first, reloads the second word from the frame, and homes the grow tag in the groupID slot instead of the live morphs slot | tried: named local, by-value and by-reference record ctors, placement copy with a memberwise copy ctor | next: lane gap: empty tag over a live argument slot (GOTCHAS 222) plus record load order
- 00a88d10 CQuaternion::SetFromAxisAngle DIFFER(144v196): retail rounds the cosine-table position with a bare dword `fistp` after a store/reload through the frame; plain C++ calls __ftol2 | tried: (long) cast in an inline table helper | next: lane gap: inline-asm FastFloatToInt (PackVertexNormal / cosine-fade family, GOTCHAS 211/225)
- 00ac70f0 QuadricOptimiserInternals::CQuadric::GetMatrixPart DIFFER(231v237): retail loads AB before BC in the c23 cofactor and BB before AA in E33 (the lane picks the other operand first for both source orders), and parks E31 through a frame float with an integer copy to E13 | tried: plain chained stores; `det +=` chain + mirror copies (fixed the determinant order) | next: lane gap: x87 operand order (as 0x852220), unless a temporary per product changes the choice

## Skipped
- none (no row holds a second body).

## Label corrections
- 00e32200 "CQ_GuildTrainingWoodsWillScript::EndMission" is CV_AmbushScamScript::WatchForTricksterComplete.
- 00e27ac0 "CQ_GuildTrainingScript::WatchForSparrowKilled" is CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver.
- 00ded0c0 / 00e16a80 / 00e16c40 / 00e1ad60 "CQ_HangingTreeEvilScript::BanditYells" are CQ_SecretPassageScript::WatchForTermination, CWB_ScaredVillager::Main, CWB_Villager::Main, CQ_WizardBattleScript::WatchForTermination.
- 00e80c00 "CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation" is CV_FishingCompetitionScript::SuspendCompetition.
- 00ddcf50 "CQ_PrisonEscapeRescueMotherScript::MakePrisonerComment" is CQ_PrisonRaceScript::MakeRacerComment.
- 00e18480 "CQ_SecretPassageScript::Init" is CQ_WhiteBalverineWWScript::Init; 00edeba0 "CQ_WizardBattleScript" ctor is the CV_TravellingHeroesScript ctor.
- 00a90790 "CQuadTreeElement::~CQuadTreeElement" is C3DPrimitive2::~C3DPrimitive2 (vtable 0x0129DF94, base CMovableResource 0x0129B860, caller = deleting dtor 0x00A98430 in slot 0).
- 00ac70f0 is QuadricOptimiserInternals::CQuadric::GetMatrixPart; 00b0a210 is NParticleEngine::CRandomAcceleration's ctor (`AAK` seed reference).
- Callees: 0x004B8C50 is list<CQuestManager::CObjective>::clear (not CGuiControlTreePane::SortTreeRecursively); 0x00702810 a CTCQuestCard bool query consulting the hero (IsActivatable by inference; not CViewBase_FindParticleMorph); 0x007022E0 CTCQuestCard::IsVignette; 0x004B0A30 CQuestManager::AddPendingQuestToDeactivate (not ListNode_InsertEntry); 0x004AFD80 CQuestManager::CActiveQuest::ContainsRegion (not CHitParameters::IsDivineWrathStrike); 0x004BAA50 CThingSearchTools::GetAllThings<CThing, list<CThing*>, CThingFilter_HasAnyScriptName>; 0x0049C770 the search tools' thing-list getter (not CVertexBufferWin32::DoSizeof); 0x00CB88B0 CScriptBase::OnScriptedEntityDeactivated; 0x00CB8960 OnScriptedEntityActivated (not AttachNavigator); 0x00CB7D60 IsEntityScriptBound (not CScriptManager::FindScriptBinding); 0x0049D870 CWorld::GetWorldFrame (not GFGetPreMainMemoryUsed); 0x0086BC10 CRandomAppearanceMorph::AddTextureToTextureMorph (PDB; "TextureMorph_AddToList"); 0x0043DBF0 is an IS_THING_ALIVE instantiation distinct from 0x0043DBD0 (T unproven; CThingPlayerCreature* assumed, the manifest says CThingBuilding*).

## Name conflicts (relocation identity, for the landing sweep)
- 0x0099A2E0: this chunk binds `??1CBaseClass` (canonical); landed 00418b7a binds the same symbol to 0x0099A300 (that is ~CBaseClassNonCopyable).
- 0x00449970: this chunk binds `CPlayerManager::GetMainPlayer` (canonical, non-const); landed 00435000 binds the same non-const symbol to 0x00449960 (the const overload).
- 0x00CB7D60 / 0x00CB8960: kept the landed caller spellings BindsEntityScript / ActivateEntity (PDB: IsEntityScriptBound / OnScriptedEntityActivated(_K, CScriptThing&)); 0x00CB88B0 has no landed caller and uses the PDB name.
- 0x00F35B30: landed callers spell it IsDone (37) / IsAborted / IsFinished; 00e16a80/00e16c40 keep the PDB `CEntityScriptBase::IsActiveThreadTerminating() const`; 0x00F35B10 is PDB SetPredicate(const CPredicate&) (landed: RegisterFilter), modelled here as SetPredicate(CIsThingConscious&).
- 0x004365B0 LowerBound: declared with `const unsigned long&` everywhere except 004b0c80/004b0aa0/004b0fc0's qm model (also `unsigned long`) -- all one spelling now except the landed `const long*` forms.
- 0x004AA840 ~CScriptThing: virtual in the CBaseClass-derived models, non-virtual in 004afb00 (as the landed OnActivateEntity).
- Landed 0x00895430 CGameScriptInterface::IsDPadButtonHeldForExpression declares CTCInventory::IsButtonHeldForExpression(long); the retail call passes a CCharString address (0064a568 uses `const CCharString&`).
