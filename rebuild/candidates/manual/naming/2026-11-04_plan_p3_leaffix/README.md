# Plan phase 3, wave w4 (2026-11-04): Leaf lines that named no symbol (8 rows)

## Result
- **8 landed rows had a `// Leaf:` line that named no symbol in their own object.** Seven were stale tails of a display name that was
  renamed on 2026-10-24 (for example `CSymbolInfo>_>`). `--recheck-landed` therefore graded the largest function in the object
  instead (a 30/48-byte `??_G`) and reported DIFFER. Among them is the drift seen on 0x00A9C960.
- **Fix:** each Leaf now names the stand-in helper or function the bytes were landed on, with a one-line comment giving the old
  value. No code changed.
- **Coordinator re-grade:**
  - vland `--recheck-landed`: 8 / 8 RELOCATION_MATCH PASS (`logs/regrade_vland.log`).
  - gate.py reports BYTES_DIFFER on all 8 by design. Its baseline hashes the old Leaf's symbol, so the index entry is refreshed with
    `symindex2.py --all` after this commit.
- **Not done in this wave:** the RELOC_NAME / RELOC_CONFLICT warnings in the log. Those are the stand-in names that phase 4 handles,
  including `??1CCharString@@QAE@XZ` bound to 00a01c10 and 0099a2e0.
