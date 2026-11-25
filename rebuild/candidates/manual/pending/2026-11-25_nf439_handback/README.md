# NF439 hand-back (2026-11-25): T1 caller respells, list B

**Landed: 71 caller rows** in `batches/2026-11-24_landing_nf439_{p1,q1}`, landed p1 then q1.
- They make 77 callee respells over 43 targets: 27 close fully and 15 partly.
- No header changes. Each row's own self is unchanged.
- The generator is `tools/gen439.py`, with two shapes: LOCAL (the row's own model class) and NSFUNC (a free stand-in becomes the PDB namespace function).

**Gates:** the full NF423 set (vland, objgate ALL_EQ, name_gate no worse, pk, /FORCE, xrcheck, apply_table, reserved). Rewitness lines were checked against compiled HEAD objects.

## Coordinator fixes at landing
- **Cross-batch table lines.** The lander's same-batch witness filter dropped 32 of the sequenced lines: rewitnesses to rows outside the batch that still bind the stand-in, and deletes witnessed by a target or another batch's row. 31 were applied by hand: `batches/2026-11-24_landing_nf439_q1/TABLE_CHANGES_CROSSBATCH.tsv`.
- **One superseded line.** p1's rewitness of `?Geometry_DistancePointToLineSegment` was superseded by q1's delete, which the lander had already applied.
- **Result:** the multi-target count stays 1.

## Ruling r1 (coordinator, 2026-11-25): declined
An empty local `enum E {};` when a shared engine header defines E is a local model of a header type, which the shared-header rule forbids. Include the header instead. The 5 rows (0x00407040, 0x00687DB0, 0x00691680, 0x00757F90, 0x004A2D70) stay held.

## Held
14 rows (`p1_hold`, `q1_hold`):
- 3 on target 0x0070E8B0, whose newest claim is Account B's;
- 5 under r1;
- 1 on the catalog label `GetStrafeAnimationName`: both PDBs say `NCreatureMode::GetStrafingAnimationName`;
- 5 with objgate, DIFFER or extra copies.

## Residue (`RESIDUE.tsv`, 837 targets)
| Reason | Targets |
|---|---:|
| SHAPE | 281 |
| special members | 195 |
| T3 col 2/3 | 140 |
| virtual | 24 |
| SIG | 21 |
| partial | 15 |
| family header ports | 12 |
| held | 9 |
