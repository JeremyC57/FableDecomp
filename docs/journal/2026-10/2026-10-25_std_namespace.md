# 2026-10-25 STLport namespace: `_STL` -> `std` (owner decision)

Evidence (retail Fable.exe): 0 occurrences of `@_STL@@`, 0 of `_STLP`; STLport RTTI type names are in `std`
(`.?AV?$CPersistContext_TransferableVector@U?$pair@VCCharString@@J@std@@`, `.?AU?$unary_function@VCThing@@_N@std@@`,
`.?AVlength_error@std@@`). FableWin.pdb names agree (`...@std@@`). Retail's STLport was built without its own namespace,
so the earlier `_STL` modelling put mangled names away from retail. Bytes were unaffected either way.

Converted (whole-word `_STL` -> `std`): 1,684 landed sources, 875 tests, 1,292 catalog names, the ps1 Module fields,
10 ghidra_out label files, 161 parked sources under candidates/manual/pending; one file renamed for its module
(009f77b0 `STLSTLPallocproxy` -> `stdSTLPallocproxy`).

Regrade (`vland --recheck-landed`, 1,691 rows): 1,682 PASS first time; 9 tests (not sources) failed to compile because
they model STLport in a nested `Ns::std` (or define `std::swap`/`pair` themselves) and became ambiguous with the host
library's `::std`. Fixed by qualifying main()'s uses (`Ns::std::`, CRT calls `::`) or including `<algorithm>`; all 9
regraded RELOCATION_MATCH PASS. Rows still modelled in a nested `Ns::std` (e.g. 0x462CB0 FileVectorAssignment) mangle
`@std@Ns@@`, not retail's `@std@@`: follow-up for the landed-errors sweep.

Landing (`land5.sh`) now converts any leftover `_STL` in a chunk's payload/sources before grading; agent rules say `std`.
