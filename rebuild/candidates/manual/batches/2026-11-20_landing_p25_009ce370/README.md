# Landing p25 009ce370: CBankFile::WriteFileInformation (Account B, 2026-11-20)

A replacement parked row (1-2 bytes off) that Account B reserved for p25Q. Written by B's Opus author p25Q and checked by B's coordinator: purity,
labels, add-only overlay, hdrcheck2 default=same, then strict vland `--land`. Grade: RELOCATION_MATCH, test PASS.

Author's notes: author_README.md (measurements, `// Inferred:` / `// Bracket:` lines, header opt-ins, mutants).

Correction (2026-11-21, author p25Q): the top-of-function declaration block is a fitted choice, now labelled `// Inferred:` in the landed source. Ego_d frame order does not prove declaration order (Ego_d 0x00521CF0 places reference locals out of order). Bytes unchanged; regrade RELOCATION_MATCH strict.
