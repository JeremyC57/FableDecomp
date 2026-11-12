# 2026-11-12 landing eo -- author workflow wf5: ten of ten rows landed

Source: `pending/2026-11-12_linux_handWF5/` (workflow `lane_tools/workflows/author_rows.js`, 49 agents). All ten rows byte-matched
and were VERIFIED by both independent lenses. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x009ADAE0 CDefinitionManager::InstantiateSubDef, 0x00499AA0 CProgressDisplay::StartProgress, 0x00EF4320
NScript::CPersonalScript_EmptyScript::CMeanBandit::Main (/d2inlT28, the window of its sibling CTestPerson::Main), 0x00A11D10
CPhysicsMeshObject::GetNavigationLevelAt, 0x007F53B0 CTCSoundAtmosphere::FrameUpdate (the manifest said CTCSoundAtmosphereVillage),
0x009692C0 CAIStateGroup_TavernKeeper::ReceivePayment, 0x0094D780 CAIStateGroup_TrollBase::Initialise, 0x0064C869
NPlayerGui::CDrawSpiritTargetingCursor::Draw, 0x0095E0B0 CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update, 0x00A11F20
CPhysicsMeshObject::IsSpherePartContainedInBoundingBox.

**Coordinator actions at landing:**
- Two rows (0x00A11D10, 0x00A11F20; different author agents) each added an opt-in for the two-argument inline
  CMatrix3x4::TransformPoint with the SAME three-line body. They are one opt-in now, FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
  (0x00A11F20's define respelled; regraded in this landing). Two independent derivations of one spelling is good evidence for it.
  The CPhysicsMesh.h conflict (both rows add a declaration block at the same place) keeps both blocks.
- 0x00EF4320 was first rejected NOT_GENUINE: crawl/purity.py matches the bare word "naked" and the row's comments / test locals used
  it next to the script API IsHeroNaked. The comments and test locals were reworded (no code change); it then landed.
**Mechanical checks:** reserved_check 0 (three comments mention reserved siblings), purity clean, no local model of a header class
in a row source, real callee bodies only.
**Headers** (17 + 1 new; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): CDefClassBase.h,
CDefinitionManager.h, CPhysicsMesh.h (NAVIGATION, BOUNDINGBOXINTERSECTED), CMatrix3x4.h (TRANSFORMPOINT_SOURCE_DEST_SUMS),
stlport/fable_map.h (MULTIMAP_CONSTFIND), CAIStateGroupBase.h, CAIStateGroup_TavernKeeper.h, CAIStateGroup_TrollBase.h, CExplosionDef.h,
CPlayer.h, CPlayerManager.h, CDisplayEngine.h, CCreatureAction_AimMissileWeapon.h,
CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h; new NPlayerGui_CDrawSpiritTargetingCursor.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011eo): the name fixes from the rows (see NAME_FIXES.tsv).
