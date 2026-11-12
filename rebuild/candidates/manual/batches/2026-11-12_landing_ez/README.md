# 2026-11-12 landing ez -- 0x00918640 CAIStateGroup_Gaze::FindFreeParticularGazePos (wf13, after repair)

Source: `pending/2026-11-12_linux_handWF13/`. The row matched in workflow wf13 but carried a local model of CAIStateGroup_Gaze. An
Opus repair (`lane_tools/workflows/repair_and_verify_row.js`) ported it onto a new opt-in of the shared header
(`engine/CAIStateGroup_Gaze.h`, FABLE_RETAIL_CAISTATEGROUP_GAZE: real CAIStateGroupBase base, protected virtual GetOwner in slot 14,
Ego_r members, sizeof 0x30); the first graded variant matched. Mechanical verifier: ok. Evidence verifier: one finding (the
`std::unary_function` base of the locally declared filter class had no comment); fixed by the coordinator with the retail
instructions (0x009186EA `lea ecx,[esp+17h]`, 0x009187DA `lea ecx,[esp+34h]`, no store to the temporary) and regraded.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. hdrcheck2: default path unchanged, the new macro has no other user.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
