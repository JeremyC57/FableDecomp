# 2026-11-13 acctB hand-back: bwf_f (4 repaired rows, re-verified)

The workflow's repair agents changed these four rows after the verifier findings (see
`../2026-11-13_acctB_bwf_e/VERIFIER_FINDINGS.md`). Each was re-verified by the coordinator (read-only `vland.py`, strict
headers, the row's own overlay in `hdr/<addr>/`; logs in `coordverify/`). These copies supersede the earlier staged ones.
Nothing is landed; no oracle bytes are here.

| address | grade | function | supersedes the copy in |
|---|---|---|---|
| 0x005AD571 | RELOCATION_MATCH | CTCInventoryTrade::ConstructTradeDescription | `2026-11-12_acctB_bwf2a` |
| 0x00607330 | RELOCATION_MATCH | ConstructLogbookList | `2026-11-12_acctB_bwf_d` |
| 0x00A079B0 | MATCH | CDeviceState constructor | `2026-11-12_acctB_bwf2a` |
| 0x00BCFF10 | RELOCATION_MATCH | CEnginePrimitiveManagerAnimatedMeshesBase | `2026-11-12_acctB_bwf_b` |
