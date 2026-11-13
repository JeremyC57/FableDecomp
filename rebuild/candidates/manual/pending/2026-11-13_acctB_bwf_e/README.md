# 2026-11-13 acctB hand-back: bwf_e (4 rows, all RELOCATION_MATCH, tests PASS under VC7.1)

Coordinator re-verification: read-only `vland.py` with `LANDVERIFY_STRICT_HEADERS=1` and each row's own header overlay
(`hdr/<addr>/`); logs in `coordverify/`. Nothing here is landed. No oracle bytes are in this folder.

| address | bytes | function | state |
|---|---|---|---|
| 0x00B5FAB0 | 5302 | CEngineScreenEffectGlowRenderer::CaptureBackBuffer | new match; **open review item below** |
| 0x005A04F7 | 9469 | CTCInventoryTrade::ConstructNormalShopList | repaired after the verifiers; supersedes the copy in `2026-11-12_acctB_bwf_b` |
| 0x0073B130 | 6673 | CTCCreatureModeManager::RegisterModes | repaired after the verifiers; supersedes the copy in `2026-11-12_acctB_bwf_b` |
| 0x00D6D670 | 5786 | NScript::CQ_HangingTreeEvilScript::Conclusion | repaired after the verifiers; supersedes the copy in `2026-11-12_acctB_bwf_b` |

## Open review item: `volatile` in 0x00B5FAB0
The source declares two function statics `static volatile float BlurOffsetScale` (0x0139A6B0) and `static volatile long
NoBlurPasses` (0x0139A6B4). The author's evidence is instruction ORDER only (retail schedules the second `fld BlurOffsetScale`
before `mov ecx,[NoBlurPasses]` at the loop end; see `notes/00b5fab0/NOTES.md`). That is not one of the three cases
`docs/pipeline/ASM_RULES.md` allows (`volatile` section): no kept read is cited, there is no vendor source, and no PDB
`LF_MODIFIER` record was checked (the lane has no record for these statics, and both names are descriptive, not PDB names).
After the verifiers the author added a `// Retail-volatile:` line (retail 0x00B60C79 and 0x00B60C90 both read 0x0139A6B0 around the
`i` increment; without `volatile` on both statics VC7.1 moves the `NoBlurPasses` load and the `i` store: 3 hunks). That is still
an ordering argument, so the decision stays with the owner. Until someone with the PDBs confirms a volatile modifier on these two
statics, treat this row as a matched draft, not as landable.

## Verifier findings
`VERIFIER_FINDINGS.md` holds every finding of the two verifier lenses for all bwf2 rows matched so far (also the rows staged in
`2026-11-12_acctB_bwf2a`, `_bwf_b`, `_bwf_c`, `_bwf_d`), verbatim from the run record. Rows 0x005AAF28, 0x005AD571, 0x007CACB0
and 0x00BBF4A0 passed both lenses after their repair. The re-verification of 0x00B72180 is running; the repaired rows
0x005A04F7, 0x0073B130, 0x00D6D670, 0x00B9ECE0 and 0x00BCFF10 are re-verified next.

Update 01:55 UTC: 0x005A04F7, 0x00B5FAB0 and 0x00D6D670 were repaired once more by the workflow and re-staged here after a fresh coordinator re-verification.
