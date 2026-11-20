# 00eebdf0 follow-up (p25B, 2026-11-20): drop an unneeded inline device from the landed 0x00EEBDF0 (landing 2128ce873)

The advisor decisions of 2026-11-20 require device minimality. Ablating the landed row's one opt-in,
FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR, showed it is not needed:
- Without it the row is still RELOCATION_MATCH.
- The TU then emits its own COMDAT of the header's plain-inline movie dtor, and that COMDAT is byte-equal to retail 0x006E7B80 apart from
  relocations (tools/cmpcomdat.py).

Changes:
- src/00eebdf0.cpp (src.diff): the define is removed, and the header-contract comment says why.
- test/00eebdf0.cpp: the movie dtor now runs as compiled. Its tail call ~CBaseObject is bound to the receiver check that used to stand in
  for the whole dtor.

Results:
- Strict vland: RELOCATION_MATCH PASS (vland.log).
- Test: PASS (206 targeted + 20,000 random).

This is optional hygiene. The landed row is correct as it stands.
