# 00e16e00 (landed; coordinator R1 check, AUTH30 2026-11-27; repo not edited)
head = the landed source; n1 hypothesis: the landed row without `inline` on Main (R1: neither in-class source is available) still matches.
head: RELOCATION_MATCH (landed source as is). n1 result: DIFFER(1120v1064); pick-any 12 EQ / pk_all 15 EQ. Without `inline` the landed Main does not match.
R1 tests: (a) no LF_UDT_SRC_LINE records exist in any PDB on disk (no IPI streams); (b) FableWin puts 209,304 of 209,316 code contributions in COMDATs (register_scripts.cpp controls included), so the COMDAT test is void; Ego_d readable part 63 of 64 (the other is the linker module).
