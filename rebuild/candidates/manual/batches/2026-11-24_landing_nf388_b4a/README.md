# Landing nf388 b4a: 4 rows push retail's string literal (NF388 literal audit)

Each row passed the wrong text to a string-literal relocation: copied literals ("DoCreatureAction" GetClassTypeName family), made-up names where retail has the definition name (CCombatSequence GetName returns `COMBAT_SEQUENCE_*`; chunk descriptions such as "Movement Vector"), truncated text, or renderer console names ("EnableSky"). Each now spells the retail bytes the instruction names. Code bytes are unchanged; only the literal COMDAT and its relocation name change.
- NF388 gates: vland HEAD and fixed (patched grader) MATCH / RELOCATION_MATCH with LITERAL_DIFF > 0 -> 0; COMDAT-by-COMDAT code bytes equal; both pick-any checkers identical, 0 reach flips; /FORCE link with no new duplicates or unresolved names. Each test links the row object and asserts the string, and fails against the HEAD text.
- Coordinator checks:
  - this run dir is the ACCEPT rows only (the held rows' tests were left out);
  - landed 4 of 4, zero LITERAL_DIFF in the landing log (grader 7f84efd57a);
  - no includers, no T3 holds, no `??_C@` table lines witnessed by these rows;
  - `self_symbols.tsv` unchanged;
  - retail bytes read independently for 5 rows (00905160, 008b3b00, 008b42e0, 00aaee90, 00b66dc0): narrow, null-terminated, standalone strings equal to the fixes;
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- Audit table: pending/2026-11-25_nf388_handback/LITERAL_AUDIT.tsv.
