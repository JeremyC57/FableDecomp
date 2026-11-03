# Codex pass 12 -- verified rows waiting to land (2026-11-03)

50 rows (47 RELOCATION_MATCH + 3 BYTE_MATCH_NAME_REVIEW held for the STLport `_M_find` decision) that regraded byte-exact on our lane.
Records, header needs, naming audit and landing order: `rebuild/candidates/manual/batches/2026-11-03_codex_pass12/README.md`.

Each `<addr>/` holds Codex's selected source and test, its naming evidence (NAMING_EVIDENCE.md, NAME_FIXES.tsv, RELOCATIONS.json,
SELECTED_GRADE.json, extra evidence files) and `hdr/`: the row's whole include closure as graded (`HEADER_STATES.tsv` says which files
are identical to, edit, or are new against rebuild/include as of 2026-11-03). Graded read-only with `FABLE_HDR_OVERLAY=<addr>/hdr`
over the current tree: 50 of 50 RELOCATION_MATCH PASS. The overlays are not the landing headers: most rows need the per-class
consolidation first (README "Header needs"). Payload: `batches/2026-11-03_codex_pass12/scripts/payload_from_verified_pending.py`.
