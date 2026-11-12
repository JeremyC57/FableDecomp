# 2026-11-12 landing ex -- 0x004085F0 CUserProfileManager::ResetAssignedInputs (workflow wf12's tenth row, after repair)

Source: `pending/2026-11-12_linux_handWF12/` (row notes under "0x004085F0" plus `REPAIR3_004085f0.md`). RELOCATION_MATCH, test PASS
under VC7.1, LANDVERIFY_STRICT_HEADERS=1. All Opus.
**Repair** (`lane_tools/workflows/repair_and_verify_row.js`, four grades): the global 0x013B85F4 is `bool
g_bControlSchemeLeftClickIsKing` as landed row 0x00808E40 declares it (still matches with bool); the startup-manager presence test
is the PDB inline `CStartupDefinitionManager::IsAllocated()` (new opt-in in engine/CStartupDefinitionManager.h); GetDef is called
unqualified in both branches (the row defines the existing FABLE_RETAIL_CGAMEDEFINITIONMANAGER, which hides the non-retail overload).
**Verification after the repair:** mechanical lens ok (regrade plain and strict, test mutation fails, overlay = repo + additions,
no reserved bodies). Evidence lens: two comment findings, fixed by the coordinator and regraded -- the inlined IsAllocated is
`mov eax,[0x013B8760] ; test eax,eax ; je`, not a `cmp`; FABLE_RETAIL_STLP_VECTOR_C is REQUIRED (569v557 without it), which the
notes had called unneeded.
**Headers** (2, additions only, defaults unchanged): CStartupDefinitionManager.h (ISALLOCATED), CUserProfileManager.h
(RESETASSIGNEDINPUTS).
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ex): the global 0x013B8760 has three other landed spellings (g_value_013b8760 in 0x0043368D, g_pool_013b8760 in
0x004336B0, g_AutoTinyThunkObject_013b8760 in 0x01228AF1); the row uses the descriptive g_StartupDefinitionManagerInstance.
With this row wf12 is 10 of 10 landed.
