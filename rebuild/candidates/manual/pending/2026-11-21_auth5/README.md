# AUTH5 (Account A authoring agent, 2026-11-21): the NShape pair with the VC10 evidence lane

Owner: no parking; stalls go to the advisor. Brief: `BRIEF.md`; state: `STATUS.tsv`; per-row history `<addr>/LOG.md`. Grader outputs,
objects and retail-derived listings are left out. Overlay: `007602f0/hdr.diff` (add-only opt-ins).

- **Step 0 (VC10, odall10.py):** both rows are in FableLib `tc_geometric_shapes.obj` (full command line = the `fablelib` preset; `YI`
  mangling confirms -Gr). `--stl` reproduces that compiland's own `vector<C3DVector>` members (106 / 334 / 31, find_if 303) with no macro.
- **0x007602F0 NShape::IsShapeConvex: byte MATCH (/Gr) pending a ruling** (`MATCHES_PENDING_RULING.tsv`). VC10: 25 of 27 Ego_d lines and
  the named frame exact (Z cleared on each read line; edges `v = second - first; v.Normalise();` 24 / 44 B, the three-float ctor is
  66 / 86 B, so that rejection holds; GFArcCos at all three sites). VC7.1 MATCH needs the p25Q `FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST`
  (Inferred; the four landed GFArcCos users regraded MATCH with it). Open: Ego_d 313 (100 v 91 B) and 342 (122 v 110 B).
- **0x00763060 NShape::FractureShape: escalated** (`00763060/ESCALATE.md`). The AUTH1 / p25L / p25Q rejection of lines 199 / 204 rested on
  line attribution and does not survive: Ego_d increments after the call (207 find_if 168 B, 210 `it_start++;` 28 B). The Ego_d-exact
  text `g1.cpp` (all lines 195-241 and the named frame) grades DIFFER(1134v1146); `h1.cpp` 1136v1146 (37 lines); fitted `e5.cpp` 2 lines.
- **0x0093D270 line 55 (report only):** all 11 AUTH1 candidates measure Ego_d's 76 B; a `const float&` getter measures 78 B (agrees
  with the p25R ruling). `0093d270/REPORT.md`.

Advisor (2026-11-21): neither row lands yet. AUTH6 decodes 007602F0's 313 / 342 as object lifetime (one more checked iterator / scope),
and on 00763060 runs one C2X_FORCE readout, an Ego_r iterator-type check and the minimal-deviation grade (g1 + e5 at 195-204), then an
owner packet if it still does not match. Before 197 counts as settled, B's p25Q decode (`it_anchor = begin() + 1`, also 74 B?) is measured.

Correction (AUTH6, 2026-11-21): 007602f0/NOTES.md's "none 100" for line 313 is mis-recorded: AUTH5's own variant x4
(`it_second != pts.end() != false`) measures 100 B when re-run. AUTH6 decoded both open lines; the row landed (batches/2026-11-21_landing_auth6).
