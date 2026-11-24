# hdr overlay, row 0x00975BB0 (B24A6 / B24A7, 2026-11-24). Full copies of the CURRENT repo headers plus one opt-in each.
- engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_ISNONZERO_YZX_BRACKET (B24A6). IsNonZero as `Y*Y+(Z*Z)+X*X > DELTA^2` (bracket rule 2026-11-19).
  FableWin ?IsNonZero 31i/91b, fwsize SAME. Retail 0x0097614F..0x0097618B. Placed in the IsNonZero chain before NONZERO_SQUARED_XYZ.
  The probe sections were removed (old file kept in hdr_probe_B24A6/).
- engine/CThing.h: FABLE_RETAIL_CTHING_ISDEAD_DIRECT (B24A7). IsDead as `return Dead;`. FableWin ?IsDead@CThing@@QBE_NXZ is 10i/22b, and this
  text matches it exactly (fwsize SAME); the default `Dead!=0` is 14i/32b. Same value.
- engine/CThing.h: FABLE_RETAIL_CTHING_GETTCDRIVER_PHYSOBSTRUCTION_SIZE_OWNER (B24A7). This is pick-any scaffolding, not retail: an explicit
  specialisation of GetTCDriver<CTCDPhysicalObstruction>, noinline, under optimize("s"), at the end of the header. It includes
  engine/CTCDPhysicalObstruction.h. The body is the landed owner 0x007890EA's text; the owner sets optimize("s", on). The copy is now EQ to
  0x007890EA, and the row is unchanged.
