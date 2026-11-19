# Re-land hand3c b5 (2026-11-19): AlignedPreScale (0x00A9D480), AlignedPreScaleTransposed (0x00A9D580) onto the shared text; 0x00C1C3F0 / 0x00C1C330 on the shared C3DVector

One text per member: both landed rows now carry no body of their own; they define their member out of line from the shared
FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES text in engine/CMatrix4x4AlignedSSE.h through new per-member opt-ins
FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_OUTOFLINE / FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALETRANSPOSED_OUTOFLINE (plain, not inline; each TU
defines only its own member). Default path unchanged; tests unchanged. Recheck after the merge: all 12 landed users of FABLE_RETAIL_CMATRIX4X4
opt-ins RELOCATION_MATCH + PASS (`logs/recheck_cmatrix4x4_users_good.txt`; the agent's strict-header regrade `logs/regrade_12_users.log`).

Coordinator decision (names): 0x013D2880 is `?GEnableSIMD@@3_NA` (Ego_r and FableWin publics); `g_SSEAvailable` is made-up and is queued
for the naming agent (`NAME_FIXES.tsv`).

Also re-landed (second commit): 0x00C1C3F0 TransformHelperPoint on the shared engine/C3DVector.h (+ LIBPK_IMPLICIT_COPY_PASS23; the local
`class C3DVector` strict mode rejects is gone), suffix-free descriptive name, MATCH + PASS; 0x00C1C330 re-authored to call the new sibling
name (body unchanged), RELOCATION_MATCH + PASS (`logs/recheck_c1c3_good.txt`). Catalog / file slugs queued for the naming agent.
