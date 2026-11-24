# hdr overlay, row 0x00975BB0 (B24A6, 2026-11-24)
engine/C3DVector.h = repo header of 2026-11-24 plus:
- FABLE_RETAIL_C3DVECTOR_ISNONZERO_YZX_BRACKET (new nested opt-in, used by differ/00975bb0.cpp): IsNonZero as `Y*Y+(Z*Z)+X*X > DELTA^2`
  (bracket rule 2026-11-19). FableWin ?IsNonZero@C3DVector@@QBE_NXZ 0x01B4F100 is 31i/91b = a one-expression text (fwsize SAME for the
  flat forms; the bracket emits no code). Retail 0x0097614F..0x0097618B: Z,Y,X squared once, sum (YY+ZZ)+XX twice (not CSE'd), and the
  Normalise factor fstp+reload. Measured: flat (any order) CSEs the whole sum with Normalise's; (Y*Y+Z*Z)+X*X CSEs the pair; (Y*Y)+.. and
  ..+(X*X) fail; Y*Y+(Z*Z+X*X) wrong order.
- FABLE_PROBE_* sections (NORMALISE_E1, GFSQR_NAMED, ISNONZERO_P1..P3/G1..G3/H1/H3, MAG_B1..B9) are measurement probes only: not used by
  the best source; delete them before any hand-back.
