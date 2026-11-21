# AUTH7 (Account A, 2026-11-21): 0x00A4EFC0 one-shot step -- escalated, not matched
Best source: 00a4efc0/b3_00a4efc0.cpp (/Gr, overlay 00a4efc0/hdr = AUTH4's), DIFFER(3491v3491) residue 16, object byte-identical to a2.
- VC10 --stl model of the row (00a4efc0/v10/model.h, m2.cpp): frame = Ego_d's, all 158 lines = Ego_d's (3872 = 3872).
- Corrections applied as one set (A..E, ESCALATE.md): keyframe before matrix; six CArray declarations; keyframe.Rotation;
  GetMagnitude SQRTF arm; `if (false) {} else for` on all 8 loops. Graded once: no flip (order readout L=7a5b > R=3cea).
- Header-set probes (b3 unused define removed, b4 unrelated declaration added): fields unmoved.
- Tools: tools/ecls.py (Ego_d class dump), tools/al.py (align odall10 output with Ego_d lines), tools/codsz.py (.cod line sizes).
- NAME_FIXES.tsv: one parameter-name fix in an overlay opt-in (no code).

Coordinator (2026-11-21): escalated. b2 (a2 + five Ego_d-evidenced corrections A..E, all 158 Ego_d lines exact under the VC10 model)
grades DIFFER(3491v3491), residue 16, byte-identical to a2; the header-set test (b3, b4) moves neither tie field, so the
fitted-dead-temporary route's condition is not met. Overlay: `00a4efc0/hdr.diff` (against the repo headers at landing nf220). Questions
for the advisor / owner in `00a4efc0/ESCALATE.md`. The winbind install AUTH7 asked for (VC10 /Zi under wine) was refused by the
permission system and is not needed for /FAsc listings.
