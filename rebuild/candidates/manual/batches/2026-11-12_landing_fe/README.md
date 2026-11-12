# 2026-11-12 landing fe -- 0x00972CC0 CAIStateGroup_JackOfBladesStage1::MeleeCombat (wf15, after repair)

Source: `pending/2026-11-12_linux_handWF15/`. The row matched in workflow wf15 with three verifier findings. An Opus repair
(`lane_tools/workflows/repair_and_verify_row.js`) fixed them without moving a byte: the local CBrainOutput / CNavigationInput
models are gone (shared `engine/campfire_types.h` through the new opt-ins FABLE_RETAIL_CBRAINOUTPUT_ONLY and _INLINE), the row calls
the PDB inline `SetAsPumpCombatActions()`, MovementType is EMovementType, and IsStillRunnable 0x00971B00 is handed back as a name
fix (protected in both PDBs; landed row says public).
The two verifiers then confirmed the bytes, test, purity and headers and found four comment / overlay-hygiene points, fixed by the
coordinator and regraded (RELOCATION_MATCH, test PASS, strict headers): SetAsPumpCombatActions and the CNavigationInput ctor are
named by FableWin.pdb only (the citation said Ego_r); the HasHitsCausedRush opt-in carries its own `protected:`; the forward
declarations under the ONLY opt-in sit before their first use; the INLINE comment names the landed out-of-line rows (0x00835620,
0x0093E760, 0x006D60C0) and the retail byte ranges where 0x00972CC0 expands them.
Reserved: 0x00975BB0 appears in a comment only. Headers (additions under new opt-ins, unused elsewhere, defaults unchanged):
campfire_types.h, CAIStateGroup_JackOfBladesStage1.h, CCombatManager.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken. Queued (land1011fe): 0x00971B00.
