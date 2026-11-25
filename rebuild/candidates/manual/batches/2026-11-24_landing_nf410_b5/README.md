# Landing nf410 b5: 50 rows spell retail's string literal instead of baking its address

Owner decision (2026-11-25): these rows count as grade `baked_string` until fixed, and the fix must be exact, because some strings name retail game files that a recompile must still load.

For every address, NF410 read Fable.exe to confirm the target:
- it is in .rdata and NUL-terminated, with no non-printable bytes;
- 293 targets are narrow and 2 are wide;
- three of them follow a float constant but start on a 4-byte boundary, so each is a literal start, not a tail;
- 0x0122D70E is retail's shared `""`.

It then spelled each literal from those bytes, with escapes, and decoded it back with NF388's independent decoder. Wide literals keep the row's cast: `(const unsigned short*)L"..."`. None of the 293 strings is a file or path name; the only one in the worklist, `U:\\` at 0x00406F70, is held by a T3 line. The exact spellings are in pending/2026-11-25_nf410_handback/LITERALS.tsv.

The named-constant form (11 rows, ruling r1, GOTCHAS 2026-11-25) puts the literal where it is used, because retail stores no pointer object for those strings.

Tests:
- pointer comparisons became null-safe text comparisons;
- self-contained copies carry retail's text;
- three print-only tests became linked tests.

NF410 gates:
- vland: RELOCATION_MATCH PASS on both sides, with 0 LITERAL_DIFF / LITERAL_UNCHECKED / LITERAL_CHECK_ERROR.
- Section by section, the only new sections are `??_C` literals, and every other section is byte-identical once the new relocation fields are masked.
- purity returns [] on every row and every test.
- A first-character mutant fails 202 of the tests; the other 91 are self-contained copies (ruling q1).

Coordinator: 50/50 landed, 0 LITERAL_DIFF; reserved_check found 0; baselines OK.
