# 2026-11-07: `#pragma inline_recursion(on)` behind `#ifdef _MSC_VER` (the user's go-ahead)

The pragma is MSVC-only and changes only how far VC7.1 inlines a recursive call. Retail's bytes depend on it, and the program
behaves the same either way. Every landed use is now written as

    #ifdef _MSC_VER
    #pragma inline_recursion(on)
    #endif

so non-MSVC compilers never see it (556 landed rows already use `#ifdef _MSC_VER` guards). Changed: 7 row sources (0x00CBD700,
0x00CB7310, 0x00CBDAD0, 0x00CBCA50, 0x00CD2919, 0x00CD344A, 0x00CD29F0) and the 2 tests that carry the pragma (0x00CBCA50, 0x00CB7310).

Proof:
- `vland.py --recheck-landed` on all 7 rows: 7/7 RELOCATION_MATCH + test PASS under VC7.1 (`recheck_grades.txt`).
- Both tests also build with native g++ (x86-64), and both print `_TEST PASS`; `-Wunknown-pragmas` reports nothing.

New rows that need the pragma use the same three-line form (GOTCHAS 2026-11-07).
The other MSVC-only pragmas in landed rows (`optimize`, `auto_inline`, `intrinsic`) are still written bare. Other compilers ignore
them, at most with an unknown-pragma warning.
