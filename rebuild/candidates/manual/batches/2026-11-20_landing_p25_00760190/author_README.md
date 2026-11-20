# 0x00760190 NShape::IsTriangleWithPointsInside (p25L, Account B)

**Grade:** RELOCATION_MATCH (340/340), `// Flags: /Gr`, same TU as 0x00760020.
- **Strict vland:** passes (`vland.log`, overlay `hdr/`, fresh LANDVERIFY_WORK). The one RELOC_NAME warning is the DELTA_NOTIONAL_ZERO record.
- **Real-object test:** passes (`test/`, no callees). It checks 11 cases against an independent model:
  - clockwise and anticlockwise triangles, each with each circulation order;
  - 4 inside, 3 inside and 0 inside;
  - points on the edges and vertices;
  - an empty list;
  - a point just inside the tolerance (dot = -1e-5) and one just beyond it (-0.01).
- **Mutants:** 19 of 20 killed (`mutants/`). The survivor, `edge = *it - start_pos` -> `*it - end_pos`, is equivalent in exact arithmetic: perp is perpendicular to line = end_pos - start_pos, and perp . line = line.Y*line.X - line.X*line.Y is exactly 0 in IEEE. Only rounding near the tolerance could tell them apart.
- **Header:** the same `hdr/` as batch 00760020; this row uses only the new engine/NShape.h.
- **What closed it** (old draft pending/2026-10-24_linux_hand478/differ/00760190.cpp: DIFFER(324v340), integer register allocation): Ego_d names and nesting, the `if (> 3) return true; return false;` tail (Ego_d 180/182/184), and the edge statement placement below.

## Ablation table (sources in `evidence/`)
| change | grade |
|---|---|
| final (line, edge, perp, circ test, *=) | RELOCATION_MATCH |
| edge after the *= (Ego_d's 157 = perp reading) | DIFFER(314v340), 100 lines |
| edge before line | DIFFER(316v340) |
| edge after perp (before the circ test) | DIFFER(318v340) |
| `return number_of_points_inside > 3;` | DIFFER(332v340), 16 lines |
| FABLE_RETAIL_C2DVECTOR instead of FABLE_RETAIL_C3DVECTOR (either declares DELTA_NOTIONAL_ZERO) | RELOCATION_MATCH (neutral) |
| `start_pos = *it_trig; ++it_trig;` | RELOCATION_MATCH (neutral) |

**Edge placement against Ego_d.** Ego_d lines 156..166 are 16 / 23 / 12 / 6 / 18 / 29 B. The final text at VC7.1 /Od gives line 16, edge 16, perp 23, circ test 6, `*=` 13, dot test 28. Ego_d's 23 B line 157 equals perp's /Od size here. Edge is 16 B in VC7.1 /Od; estimated at about 23 B with Ego_d's class-iterator operator* call (that figure is computed, not compiled).
- Ego_d's sizes therefore do not fix the order; this is why the line is `// Inferred:`, chosen by grading.

**Inline-budget: none.**
