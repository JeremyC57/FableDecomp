# 2026-11-12 landing eg -- chunk F1: seven rows matched by one Fable rapid-fire agent and independently verified

Source: `pending/2026-11-12_linux_handF1/`. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00878BE0 NCameraModes::CProjectileWeapon::GetPosFromParams, 0x00877720 NCameraModes::CTools::GetStandardThings, 0x009E3B60
CPixelFormat::ConvertPFDToRGB, 0x00AD9770 CVertexGroupBlockList::RemoveAllSubsets, 0x00BC2480
CEngineInternalPrimitiveParticleMesh::Update, 0x00801920 CTCIdleScheduler::RegisterIdleStateGroup, 0x00407CD0
CUserProfileManager::RedefineControlPassive.

**Verification:** seven independent Opus evidence verifiers (`EVIDENCE_VERDICTS.md`): five ok; two findings fixed by the coordinator
before landing (a made-up member name now declared as such; ApplyValues declared protected as in both PDBs). Mechanical checks:
reserved_check 0, purity clean, no local model of a header class, no stand-in bodies.
**Inferred statement:** 0x00878BE0's `pos / 1.0f` (owner rule: fine when the row behaves exactly as retail). The owner suggested the
mechanism (a scale by one forces a float conversion); nine spellings were graded and three match -- the source header has the table.
**Headers** (11, three-way merged against 98297c04e): opt-ins in stlport/fable_map.h, CPixelFormat.h, CRightHandedSet.h
(RIGHT_COMPONENTS_GROUPED), CVertexGroupBlock.h, CUserProfileManager.h (PASSIVEINPUTS), NCameraModes_CTools.h,
CEngineInternalPrimitiveParticleMesh.h; new CActionInputControl.h, CEnginePrimitiveParticleMesh.h, CTCIdleScheduler.h.
**Default-path change:** campfire_types.h -- the CTCIdleScheduler class moved unchanged into engine/CTCIdleScheduler.h, which
campfire_types.h now includes. All six landed includers (`CAMPFIRE_INCLUDERS_rechecked.txt`) were rechecked in this landing:
RELOCATION_MATCH, PASS (`logs/recheck_campfire_includers.log`).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011eg): 0x00878430's manifest label is a propagated twin (it is the GetAllThingsInRoughArea<CThing, list<const CThing*>,
CFilter_Bind2<IsOfType, OverlappingSphere>> instance); landed ChooseNewIdle 0x00801A50 uses IdleWeights / DefIndex where the PDB has
Period / DefGlobalIndex and still carries stand-in bodies; landed 0x00407370 ApplyValues has a QAE Leaf (the PDB says IAE, protected).
