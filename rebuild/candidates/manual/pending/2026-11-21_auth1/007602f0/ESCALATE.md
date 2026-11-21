# ESCALATE 0x007602F0 NShape::IsShapeConvex (1126 B) -- AUTH1, 2026-11-21 (stall clause 2: remaining ideas contradict Ego_d)
P = /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-20_p25_open

## Best sources (repo, unchanged)
- The repo's "rule-clean best" P/007602f0/007602f0.cpp regrades DIFFER(1126v1126), 6 lines (AUTH1 r0.cpp, /Gr), BUT it builds all four
  edges with the three-float ctor (its lines 43/51/58/64), so it is the bracket class too, not Ego_d-conformant.
- The Ego_d-conformant text (literal `v = second - first;` at the four sites, AUTH1 r1.cpp): DIFFER(1181v1126), 116 lines.
- Bracket (RELOCATION_MATCH, not landable): P/007602f0/bracket_p25Q_paren_threefloat.cpp + hdr_p25Q_gfangles_paren.diff.

## Remaining diff
The edge construction. Retail never reads first.Z / second.Z and normalises with a two-square sum (Z = scale * 0.0f), which the
three-float ctor `C3DVector v(second.X - first.X, second.Y - first.Y, 0.0f)` gives; Ego_d lines 309/317 (24 B) and 328/338 (44 B)
decode as `v = second - first; v.Normalise();` and cannot hold the three-float ctor (41/58 B, p25Q odl2 calibration).

## Tried (all p25O/p25Q, repo notes)
literal op- 1181v1126; op- + `v.Z = 0.0f` at all four sites 1114v1126, one site 1130v1126; helper search in Ego_d TPI (no 2D normalise,
no fastcall C3DVector& helper); closing-2 iterator forms; GFArcCos v raw acos. AUTH1 spent no grade: no new Ego_d-consistent idea.

## Hypotheses
(a) TLC retail's edge text differs from the Ego_d / FableWin revision at these four lines (size-neutral at /Od, as FableWin = Ego_d).
(b) An inline 2D helper existed in TLC that Ego_d's revision lacks (contradicted: Ego_d has no such proc, and VC10 /Od emits every inline).

## Question for the advisor
Is there any admissible route left besides the Windows request (Ego_r / FableWin line records for lines 309..338)? If the standing
"different revision" criterion can never pass here (no call-level difference to test), should the row be held until those records exist?
