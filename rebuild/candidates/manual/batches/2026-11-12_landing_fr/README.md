# 2026-11-13 landing fr -- author workflow wf20 (Opus only): seven of ten rows landed, three in repair

Source: `pending/2026-11-12_linux_handWF20/` (workflow `lane_tools/workflows/author_rows.js`, 70 agents, all Opus; no row needed
Fable). The run was cut by a container restart with all ten rows matched and was resumed from its journal. Landed (all
RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x00956060 CAIStateGroup_MazeTutorialBattleTeleport::Update,
0x00F39730 CSurfaceCompositor::StartComposition, 0x00523CF0 CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps,
0x00918B90 CAIStateGroup_Gaze::Update, 0x00972990 CAIStateGroup_JackOfBladesStage1::OnActivate, 0x006C8E90
CThingFilter_IsEligibleWitness::operator() (two author passes), 0x0049BE90 CParticleEnvironmentInteraction::IsParticleCollidingAt.

**Not landed yet (matched; each in a repair + two-verifier workflow):**
- 0x00839100 CThingGameObject::GetWeaponTrailGraphicIndex: a member read through a `long[2]` stand-in where Ego_r types a
  CWeaponTrailGraphicSet {Attack, Knockdown}.
- 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks (three passes, flag /Gr): dead attempt opt-ins left in the overlay, two missing
  name fixes (the landed 0x00ADB570 is list::size, not SortTreeRecursively), an uncommented carried inline body.
- 0x008C12D0 CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise: a raw cast where TCI_BOSS exists; a test case that
  cannot fail.

**Coordinator fix at landing (both rows regraded: match, test PASS):** 0x00956060 and 0x00523CF0 each added a block at the same
place in stlport/fable_containers.h (merge conflict in front of the random-access __copy); both blocks kept
(VECTOR_RANGE_INSERT, then COPY_NOINLINE around the original line). Default and every existing opt-in path unchanged (hdrcheck2).
**Mechanical checks:** reserved_check 0 (0x0074A5D0, 0x00A059E0 are declared and called only), purity clean, no local model of a
header class. `auto_inline(off)` in 0x00523CF0 surrounds real same-TU callee bodies.
**Headers** (21; additions under new opt-ins, unused elsewhere, defaults unchanged): stlport/fable_containers.h,
CAIStateGroup_MazeTutorialBattleTeleport.h, _quarantine/CAIStateGroup_MazeBase.h, _quarantine/CMazeBattleDef.h, CDisplayManager.h,
CRenderStateCaptureBlock.h, CCompressorZlib.h, CClassFactory.h, CWorldMap.h, CMemoryDataOutputStream.h,
NThingManager_CEntityRuntimePersistence.h, CThingManager.h, CThing.h, fable_string.h, CAIStateGroup_Gaze.h,
CCreatureNavigationDef.h, C3DVector.h, CTCBoss.h, CAIStateGroup_JackOfBladesStage1.h, CThingSearchTools.h, CTCHitLocations.h,
CTCPerceiveThing.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fr): the rows' name fixes and the label warnings (0x00A10E20 DetachZBuffer, 0x00A0AAA0 GetCurrentAWindow,
0x00A0AA80 SetAWindow, 0x0099EF90 CCharString::Reserve, 0x00815820 GetPHitLocationWithDefName).
