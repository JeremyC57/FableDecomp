# Static mesh helper affine transform

00a361a0 (193 bytes): main recovered the paused chunk189 row at grade9 by transforming the copied point in place while retaining original X and Y. This reproduces native x87 scheduling and duplicated scalar spills without arbitrary volatile or memcpy. Actual CCharString/CCharStringData header and PDB native mesh/helper offsets; grade10 adds a host-only adapter and explanatory comment, leaving target compilation unchanged.

Exact-source host and VC7.1 fixtures test empty/nonempty names, zero-seed CRC arguments, high unsigned index, identity/translation/nonsymmetric affine matrices, matrix changes made by the lookup callback, unchanged helper position and string ownership. A first fixture incorrectly assumed receiver/argument evaluation order across optimization levels; removing that artificial receiver mutation fixed the /Od failure without changing the matched function. Host additionally disables copy elision. All ten total source grades are preserved.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
