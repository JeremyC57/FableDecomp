# Naming pass wave 4, agent A (2026-10-29): wide-text names in the retail `unsigned short` form, 108 rows, 157 files

Owner decision 2026-10-29: wide text is `unsigned short` (mangled `G`); PDB `_W` names land with `_W` -> `G`.
Full recheck after applying (recheck.log): 108 of 108 (106 RELOCATION_MATCH, 2 MATCH), tests PASS.

Families (scripts/, exact-block edits from `*.reps.json`, idempotent): CWideString `operator=` (12) and `operator+=` (19)
`const T&` returns; 33 calls moved to `GetAsCharArray() const` (0x0099AF10) where retail calls it; `operator const
unsigned short*` const member (8); `CWideString::Sprintf` member instead of `Format` / free `FormatWideString` (12);
static `CAFile::StartFileScan` / `ScanNextFile` (10); STLport `__char_traits_base<G,G>::compare` 0x00403C60 (15, the row
itself now the explicit specialization); `CWideString(const unsigned short*)` with `L""` (6); singles (2).
FLAGS.tsv: `/Zc:wchar_t` dropped from the 7 rows that carried it (applied to build_candidates.ps1).
Bindings: 40 retired, 15 new spellings bound. STLport 4.6.2 declares `char_traits` / `allocator` as classes, so their
mangling uses `V`, not `U` (corrects DECISIONS.tsv for 0x00404280 and an older GOTCHAS line).
Left (DEFERRED.tsv, HEADERS.md): `fable_string.h` (1,269 includers) lacks GetAsCharArray / Sprintf / FindCharPos; a few
rows need restructuring or their own identity re-matched (0x00404280 / 0x0040F0A0 STLport operator==, 0x00993F00
CDataOutputStream::WriteNullTerminatedWString).
