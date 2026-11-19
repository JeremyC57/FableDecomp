# Stale catalog pass patterns (2026-11-19, owner: "Do this task here")

A static scan of rebuild/build_candidates.ps1 found 363 of 52,857 landed rows whose PassPattern string does not appear literally in the test source.
A real recheck (vland --recheck-landed via recheck_rows.py) of all 363:
- 329 pass: false positives (the test builds the string with printf, e.g. `PEEK_RADIUS_OK %f`, `OK_0x%08x`).
- 26 were genuinely stale (bytes MATCH / RELOCATION_MATCH, test FAIL only because the catalog expected an old string): every one of their tests prints
  `<addr>_TEST PASS` (`fixed_patterns.tsv`: address, old pattern, observed output). Patterns updated; all 26 rechecked good. With 0x00661F70 (fixed
  earlier today) that is 27.
- 8 grade DIFFER on recheck with a passing test (`byte_differ_on_recheck.tsv`) -- not a pattern problem; investigated separately (see FINDINGS_LOG).
GOTCHAS: re-landing a row with a new test must also update its catalog PassPattern.
