# Codex pass 31 (2026-11-28): 57 released rows that do not match yet

**Owner (2026-11-28):** "release any locks or reservations on these and put them together for a codex hand off after you have the fable
advisor look over them and see what their issues are"; then "if they already match you can leave them out".

## Selection (82 rows from UNMATCHED_NOW.tsv)
The groups the owner named: claimed by account B (27), locked by the paused agent AUTH30B (16), Codex pass 29 (7), Codex passes 26 / 28
(7), released by B (6), no record (5), claimed by Codex (3), claimed by A (3), skipped rows (3), and five singles (escalated, coordinator
hold, bounds skip, old A reservation, the landed naked_asm row 0x009BEF80).

- **9 are not separate functions** (each lies inside a landed function's extent): 004A1C41, 008F92EF, 00BA115C, 00BC3F3D, 00BD71CC,
  00564D5D, 00D2988B, 0074BC30, 00537E70. Left out; they inflate the known-function count (owner item: drop them from functions.tsv).
- **16 match at the base** (RELOCATION_MATCH, regraded 2026-11-28 with their own overlays): 005458A0 0064FDB4 007D75A0 009F3DA0 009F4590 00D05A60 00D06130 00D2D1A0 00D67890 00DA71E0 00DB3FA0 00E0CA50 00E70460 00EB5750 00EBE620 00F277F0 
  Left out (owner); locks A_MATCHED_HOLD and an A claim. Their blockers are gates, not bytes (pick-any copies, strong duplicates of
  landed rows, the inline-Main ruling for 00D05A60 / 00D06130: auth30b_archive/00d06130/ESCALATE.md).
- **57 go to Codex** (rows.tsv, RESERVED.tsv, ISSUES.tsv; locks CODEX31; one A claim).

## Review
The advisor reviewed the dossier (owner delegated). The selected draft per row is the holder's selected checkpoint, not the newest
file (STATUS.tsv best for AUTH30B, B's folder file, the Codex folder main, else UNMATCHED closest_version). Every draft was regraded at
the base with no flag / pragma sweep (GRADES_AT_BASE.json): 41 DIFFER, 7 no longer compile (first error in ISSUES.tsv), 7 no object
within 5 minutes, 1 unreadable object, 1 asm-only. Extents were measured to the next known start with padding trimmed.

## Release
Claims released by appended lines (B's, A's and Codex's original lines untouched); locks CODEX29, AUTH30B (16), COORD_HOLD_NF453 moved;
earlier RESERVED.tsv lines annotated; UNMATCHED_ROWS statuses updated. The paused agent's work is in auth30b_archive/ (scratch held the
only copy). The package is built by build.py (derived from pass 30) with NAME_ANSWERS from the tracker at this commit.
