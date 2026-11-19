# Re-land hand3c b5 (2026-11-19): AlignedPreScale (0x00A9D480) and AlignedPreScaleTransposed (0x00A9D580) onto the shared text

One text per member: both landed rows now carry no body of their own; they define their member out of line from the shared
FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES text in engine/CMatrix4x4AlignedSSE.h through new per-member opt-ins
FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_OUTOFLINE / FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALETRANSPOSED_OUTOFLINE (plain, not inline; each TU
defines only its own member). Default path unchanged; tests unchanged. Recheck after the merge: all 12 landed users of FABLE_RETAIL_CMATRIX4X4
opt-ins RELOCATION_MATCH + PASS (`logs/recheck_cmatrix4x4_users_good.txt`; the agent's strict-header regrade `logs/regrade_12_users.log`).

Coordinator decision (names): 0x013D2880 is `?GEnableSIMD@@3_NA` (Ego_r and FableWin publics); `g_SSEAvailable` is made-up and is queued
for the naming agent (`NAME_FIXES.tsv`).
