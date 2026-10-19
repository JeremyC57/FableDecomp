# Mesh readers and activity score formatting

00abd150 ReadFromFile (289 bytes): actual shared vertices-chunk and stream headers; 32-byte vertices, grow/shrink/same/empty arrays, bulk reads, buffered/mixed/overflow stream paths and cursor checks. Two author grades including the initial missing-header failure.

00aa6960 ReadFromFile (246 bytes): actual shared triangle-strip chunk and stream headers; two-byte strip indices, high-bit values, array resizing and buffered/mixed/overflow cases. One author grade.

0055e6b0 GetScoreAsString (263 bytes): actual target fable_string header, PDB activity layout, audited formatter/helper addresses and literals. Tests cover positive/negative/zero integer truncation, decimal precision, absent dot, double conversion, localization receiver/unit and full temporary cleanup. Three author grades.

All independently RELOCATION_MATCH + PASS, with source embedded verbatim in host and canonical VC7.1 fixtures.
