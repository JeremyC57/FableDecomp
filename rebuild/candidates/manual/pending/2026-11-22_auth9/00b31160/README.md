# 0x00B31160 CEngineCamera::ProcessDebugScript recheck after 0x00B30B50 becomes its own row (AUTH9)
- SetCameraState declaration-only: DIFFER(886v886) (swapped pair at 0x00B313A4). The copy stays as a TU-mate.
- P1.patch: Fidelity comment only. rg1 RELOCATION_MATCH; copy at 0x00B30B50 RELOCATION_MATCH; pick-any 1 EQ / 0 NEQ; repo test PASS.
