# Landing 2026-11-07t: authoring chunk 843 (0 of 6 landed, 4 parked, 2 not attempted)

| Row | Grade | Note |
|---|---|---|
| 0x00B2FD60 CEngineCamera::SetupGamut (was placeholder `CPlane_InitializeFromCorners`) | DIFFER(1802v1812), 213 | x87 register / store order; label corrected (`labels_landing_2026-11-07t.tsv`) |
| 0x00A86110 CIPhysicsFacesSegmentBase::CalculateDividingPlane | DIFFER(1967v1967), 26 | axis-table `lea`, Y/Z scale operand order, one align pad |
| 0x00D22F30 CQ_BreakSiegeScript::CKG_Sheriff::Main | DIFFER(3335v3399), 200 | caller-size inline budget (RELOCATION_MATCH only under the diagnostic /d2inlS15; not used) |
| 0x00DE9690 CQ_RansomVictimScript::CRansomHelper::Main | DIFFER(3428v3412), 128 | CIsThingAlive copy-inline blocker |
| 0x005253F0 CThingManager::LoadGameState | not attempted | skeleton in `pending/2026-11-07_linux_hand843/skel/` |
| 0x00DD4AD0 CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main | not attempted | per-exit ~CTimer pattern |

- **Names:** all six proven. The 0x00A14440 AccurateNormalise and 0x008AC970 vector<CScriptThing> dtor fixes were already queued.
- **Headers:** the CEngineCamera / CMatrix3x4 / C3DVector opt-ins and new CPlane.h / CEngineCameraDesc.h serve only parked rows; they stay in
  `pending/2026-11-07_linux_hand843/hdr/`.
- The brief wrongly said the three Mains had Codex pass-13 prior art; those folders hold only an UNSTARTED ROW.json.
