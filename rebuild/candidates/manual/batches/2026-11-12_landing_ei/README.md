# 2026-11-12 landing ei -- author workflow wf2: nine of ten rows landed, one back for repair

Source: `pending/2026-11-12_linux_handWF2/` (workflow `lane_tools/workflows/author_rows.js`, 50 agents). All ten rows byte-matched.
Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x00868CE0 CHitParameters::SetWeapon, 0x00E2EAC0
NScript::CV_AmbushScamScript::TricksterApproach, 0x005590C0 NUISystem::CScrollBarOutside::OnLeftClicked, 0x00541FD0
NUISystem::CIconText::GetDataFromDef, 0x008C2AD0 CCombatAction_WillHealInto::FinishAction, 0x005F11A9
CTCInventoryAbilities::UnAssignAbility, 0x006978D0 CGameCameraManager::GetInterpolatedCamera, 0x00EB3F40
NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Main, 0x008D5300 CCreatureAction_ReloadMissileWeapon::PerformAction.

**Not landed yet:** 0x009ADC90 CDefinitionManager::InstantiateDef matches, but the evidence verifier found its nested record
constructor spelled against FableWin.pdb (4th parameter is `const CDefPointer<CDefClassBase>&`, not `CDefClassBase*`) and a test that
edits the body under test. It went back to a Fable agent for a second repair; its four overlay headers are not landed.
**Inferred statements** (owner rule: fine when the row behaves exactly as retail; both marked `Inferred:` in the source header with
the retail bytes): 0x00E2EAC0 -- a folded `true ? (void)0 : (void)GetTrickster();` (the never-read destruction flag word at
0x00E2EACE) and one extra `do { } while (0)` round the dead arm's terminate check (the inline split of ~CScriptThing; the natural
form is byte-exact under /d2inlT86..100 only); 0x00EB3F40 -- `if (trust && false && ...) { }` (flag word at 0x00EB3F4F) and an
if / else on `trust` with identical arms (the dead `mov bl,al` at 0x00EB3FF4; the twin Guard::Main 0x00EB55D0 has both).
**Verification:** nine rows VERIFIED by both lenses after the workflow's repair pass (`EVIDENCE_VERDICTS.md`). Mechanical checks:
reserved_check 0 (two comments mention reserved siblings), purity clean (`#pragma optimize("s", on)` in 0x005F11A9 as its landed
sibling 0x005F1320), no local model of a header class in a row source, no stand-in bodies.
**Headers** (6, all additions; every new opt-in unused elsewhere, default paths unchanged per hdrcheck2): CHitParameters.h
(AUGMENTATIONS, SETWEAPON), CTCObjectAugmentations.h (GETAUGMENTATIONS), NUISystem_CManager.h (GETMOUSECURSOR),
NUISystem_CScrollBarOutside.h (ONLEFTCLICKED), CGameCameraManager.h, CCamera.h (FOV).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ei): ten name fixes from the rows (see NAME_FIXES.tsv).
