# 2026-11-13 landing fv -- 0x004093E0 CUserProfileManager::SetDefaultValuesVideo (wf21, after two repair rounds)

Source: `pending/2026-11-12_linux_handWF21/`. The row matched in workflow wf21 (three author passes). Two Opus repair rounds
(`lane_tools/workflows/repair_and_verify_row.js`), each followed by two independent verifiers; the final pair passed it with no
findings. The row's code did not change after the match; what changed is everything around it:
- **Carried callee bodies are proven.** Each grades against its OWN retail address from this row's source: 0x004083E0
  GetConfigOptionsDefaultsDef RELOCATION_MATCH (it was DIFFER(203v194) until the template GetDef body was made visible through
  the shared GETDEF_GLOBAL_BODY opt-in), 0x00410890 GetDef<CConfigOptionsDefaultsDef> RELOCATION_MATCH (new opt-in
  FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD), 0x004069A0 ClampRefreshRate RELOCATION_MATCH, 0x00406D90 ClampResolution MATCH,
  0x00406880 MATCH. The source says for each whether retail calls it or expands it, and where.
- **The test uses retail data and can fail.** Its refresh table is the executable's {60, 75, 85, 100} at 0x0122DE30 (the first
  version used {60, 70, 75, 85}); three one-line mutants (pick loop stops at 3, width override deleted, cap test dropped) each fail.
- **One address, one name.** The row binds no symbol to 0x00406880 (an ICF fold landed as CModeBlock::GetAnimFilter; GetDisplayModes
  is expanded inline at 0x00409548). NAME_FIXES lists every landed spelling of the globals 0x013B86A0 (31) and 0x013B871C (8) and
  the wrong landed spellings of 0x00406D90, 0x004083E0 and 0x00410890.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1; landing-verifier dry run clean. Headers (additions under opt-ins with no
other user; defaults unchanged): fable_user_profile.h (VIDEODEFAULTS), CDisplayManager.h, CDefPointer.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fv): the twelve rows of NAME_FIXES.tsv. Workflow wf21 is ten of ten landed.
