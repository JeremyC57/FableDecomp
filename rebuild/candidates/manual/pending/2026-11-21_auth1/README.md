# AUTH1 (Account A authoring agent, 2026-11-21): work on the ten rows released from Account B

Owner (2026-11-21): no parking, keep working until rows match; stalls go to the advisor. AUTH1 returned early (context). **0 of 10 matched.**
Rows stay `reserved (acctA AUTH1, ...)` in UNMATCHED_ROWS.tsv. Brief: `BRIEF.md`. Per-row state: `STATUS.tsv`; per-row history: `<addr>/LOG.md`;
stall packets: `<addr>/ESCALATE.md`. Header overlays are per row, stored as `<addr>/hdr.diff` against the repo header at 2026-11-21
(add-only `#ifdef` opt-ins; the 0x00A4EFC0 C3DVector.h hunk inserts new `#if/#elif` branches ahead of the existing
`FABLE_RETAIL_C3DVECTOR_MAGNITUDE_X_THEN_YZ` test, default path unchanged). Grader outputs, objects and retail-derived listings are left out.

| row | best | state |
|---|---|---|
| 0x00A4EFC0 | DIFFER(3491v3491), 1 residue (e1.cpp, /Gr) | escalated early; new opt-in FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD |
| 0x00C73050 | DIFFER(672v672), 1 register (b0.cpp, /d2inlT150) | escalated early; c2 pressure-counter dump x0.json / x0.txt / x4.txt |
| 0x0093D270 | DIFFER(305v295) (e1.cpp) | escalated early; float getter return-value home, minis in mini/ |
| 0x007602F0 | DIFFER(1126v1126) bracket class; Ego_d text 1181v1126 | escalated (needs FableWin / Ego_r line records or an owner ruling) |
| 0x00763060 | DIFFER(1132v1146) | escalated (same) |
| 0x008E18D0 | DIFFER(2045v2073) with the re-made IMPLICIT_DTOR overlay (hdr.diff) | escalated |
| 0x00CF4EC0 / 0x00B4D370 / 0x00BD3070 / 0x00502E90 | baselines only | not reached |

Corrections to earlier records:
- 0x007602F0: the "rule-clean best" `pending/2026-11-20_p25_open/007602f0/007602f0.cpp` builds the four edges with the three-float
  constructor too, so it is in the bracket class, not Ego_d-conformant.
- 0x008E18D0: `pending/2026-11-20_p25_open/008e18d0/` lacks the IMPLICIT_DTOR overlay; without it the best grades 1903v2073. The overlay
  is re-made here (`008e18d0/hdr.diff`).

Tools AUTH1 wrote (scratch-lane helpers, no retail data): `nd.py` (relocation-masked diff with residue count), `g.sh` (batch grader),
`egl.py` (Ego_d line table by function name), `odl.py` (VC7.1 /Od bytes per source line), `mini.py` (test-program listings),
`c2x/` (c2prio_x probe kit).

Advisor (2026-11-21, owner delegated): next agent works 0x00A4EFC0, 0x0093D270, 0x00C73050 only (measurements first: helper calibration
against Ego_d; c2path.py on the m2 mini plus an Ego_r spelling check of the getter; force #32 before #9 with the hook). An owner decision
is pending for 0x007602F0 / 0x00763060 (accept an Inferred TLC-revision text, or hold for Windows line records).
