# Compiler probes

- package baseline — DIFFER, 1699 v 1705 bytes, 21 strict instruction hunks, first unequal byte at 0x008E9C28.
- third 01 shared strafe classes — COMPILE_FAIL.
- third 02 animset accessors — DIFFER, 1736 v 1705 bytes, 86 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 03 selective original animinit calls — DIFFER, 1643 v 1705 bytes, 103 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 04 donor aiming frame local — DIFFER, 1608 v 1705 bytes, 96 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 05 mode base virtual context — DIFFER, 1643 v 1705 bytes, 103 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 06 donor ability lock named — DIFFER, 1643 v 1705 bytes, 103 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 07 explicit playback frame names — DIFFER, 1643 v 1705 bytes, 103 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 08 aim call without artificial loop — DIFFER, 1643 v 1705 bytes, 103 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 09 initial check separate anim index — DIFFER, 1654 v 1705 bytes, 106 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 10 empty as lifetime guard — DIFFER, 1643 v 1705 bytes, 108 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 11 normalized intelligent pointer access — DIFFER, 1643 v 1705 bytes, 103 strict instruction hunks, first unequal byte at 0x008E9B76.
- third 12 anim set lifetime precedes aim calc — DIFFER, 1653 v 1705 bytes, 92 strict instruction hunks, first unequal byte at 0x008E9B76.

The selected result is PARKED (1736 object bytes, 1705 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
