# 00ec54e0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 5. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00ec54e0/NAMING_EVIDENCE.md

# 00ec54e0 identity and validation

`NScript::CV_SickChildScript::Main`, exact `?Main@CV_SickChildScript@NScript@@UAEXXZ`, occurs in both original PDB name sets. Native vtable 012f17dc slot 2 points at this row; its constructor and registration factory prove the script identity independently. Ego_r TPI 0x109e9 provides the complete owner model. Shared CLevelScriptBase removes the donor debug-container growth; native Init stores FinishedQuest at +0x68 and the other original state fields at their reconstructed offsets. All owner and nested-class fields are retained.

The row registers eleven literal-named original entities, waits for QS_GuardianSisterInfo, activates two quests, waits for FinishedQuest, marks the active quest complete, waits for BowerstoneSlums to unload, then schedules three deactivations. Native vtable 01260f0c and both PDBs prove every virtual receiver method and slot. Its assigned extent is complete, ending at 00ec5ba2 ret.

The self-contained test pastes the exact selected Main body and instruments its dependencies. It checks registration and string lifetimes, all allocation-failure positions, each WAIT's frame counts, and termination at every guard. An independent arithmetic oracle predicts which stages may execute and the exact number of termination checks. 2457 scenarios pass GNU C++03 and native VC7.1 /GR-. The fixture tests behavior, while production objects independently validate actual class ABI and byte identity.
