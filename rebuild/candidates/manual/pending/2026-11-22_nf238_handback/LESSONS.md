# NF238 lessons (one line each)
- A shared header can hang the lane cl.exe when included without its prelude (CDefendingCombatantInfo.h alone hung; the CThing / fable_string prelude fixed it): bisect with an includes-only probe (`int probe(){return 0;}` under q.py, timeout 120-150) before regrading the full row.
- A stand-in functor name is not automatically a folded alias: grep the PDBs for the stand-in name first (CRotateThing is a real, different functor), then match the body's record stride and functor size to the PDB instantiation.
- A vtable slot is the strongest second source for a getter with a propagated label: read the same slot in every sibling vtable of the hierarchy (7 tavern vtables put GetWinPhrase between Farewell and GetBestScore).
- Python heredocs with an unquoted EOF turn `\\n` into a real newline inside C string literals: use a quoted heredoc ('EOF') for test text.
- vland and equiv_check read repo-relative paths: run them from the repo cwd with LANDVERIFY_WORK in scratch, and diff the repo root listing before/after.
- A test for a function that calls an anonymous-namespace member cannot link the row (per-TU ?A0x name): make it self-contained on the same headers.
