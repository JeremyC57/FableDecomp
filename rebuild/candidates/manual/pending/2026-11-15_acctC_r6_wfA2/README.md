# acctC hand-back 2026-11-15_acctC_r6_wfA2

Rows: 0x00439070.

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. independent audit (Opus verifier, round 5 fix-ups): no hard-rule violation; the local CDrawMiniMap is replaced by the additive engine/CDrawMiniMap.h opt-in. LANDING NOTE: vland warns RELOC_CONFLICT SetCrimeAsDisplaying 0x00437770 here vs 0x004376B0 in landed 0x00775B40; NAME_FIXES.tsv gives the two-source case that the landed side is wrong (0x004376B0 is SetMoneyAsDisplaying), respell the landed callers at landing. OWNER REVIEW: /d2inlT63 with Inline-budget line; visible 0x0043FA20 ctor body in shared header; fitted if/else-if chain for drawHud. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
