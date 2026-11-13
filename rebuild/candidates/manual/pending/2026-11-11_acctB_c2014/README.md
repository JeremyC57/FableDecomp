# 2026-11-11_acctB_c2014 -- acctB hand-back, chunk 2014 (2 C3DAnimation2 rows: 1 matched, 1 parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00A50510 C3DAnimation2::InitialiseAnimationSequences | RELOCATION_MATCH 1748v1748, test PASS (g++ and VC7.1) | 29 grades over three passes (3 of them dead-local diagnostics, not in the final source) + about 70 c2prio traces. Defines C3DAnimationSequence::InitialiseAsConstant in the row (retail's out-of-line copy 0x00A4FE30 is in this TU). REVIEW: the last x87 sum orders were closed by spellings picked with a c2prio read-order metric (std::vector<bool>, a separate `++data_index;`, a named loop-1 bone pointer, an explicit `C3DVector(E41, E42, E43)`, a named `original_index`) |
| 0x00A4E300 C3DAnimation2::GetDeltaMovement | parked, DIFFER 1609v1619, 97 frame-masked diff lines | first wrong decision 0x00A4E4F4.. / 0x00A4E567 (retail keeps the start matrix's E11-E13 on the x87 stack); product row 2 / row 3 term order 0x00A4E766.., 0x00A4E7CE..; E42 / E43 0x00A4E90A / 0x00A4E90D; second norm 0x00A4E578; frame 0x100 vs 0x104. Only 6 grades (pass 1); source + passing test in `rows/parked/`. Relies on an unproven access change (MovementVector protected, opt-in C3DANIMATIONSTATS_PROTECTED) |

Coordinator re-verification of the matched row (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade,
both test runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean.

To land: the row needs `hdr/` (new opt-ins on copies of the r3001 overlay or HEAD; `hdr/README.md`): merge once with
`2026-11-11_acctB_r3001/hdr`. Rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 3
entries. LABELS.tsv names the inline-member COMDAT run 0x00A4BB40..0x00A4BE10; 0x00A4BB40 (GetType) and 0x00A4BD40
(GetOriginalNoChildren) are byte-identical and told apart only by position.
