# Retry 10, second landing (2026-10-20)

The resumed Fable 5.1 retry pass reached 229 rows of the parked list (111 matched, 118 still DIFFER with one NOTES.md
line each); the 77 rows not landed in `_retry10a` landed here with `verify_and_land.py --land` (verify.log). New
families this segment: x87 sum-of-squares / dot-product term order follows STATEMENT order (`s = a*a; s += b*b; ...`
in retail's fmul order; 9 rows), the HasTC + ternary-GetTC entry pick for every out-pointer GetInterface (two mechanical
sweeps, 18 rows), member records with inline ctors as store anchors (CDefIndex -1, CDefPointer, CCountedPointer), named
bool for an `a || b` guard, `? true : false` byte narrowing, named local for `cmp <call>,<member>`, and one row
(00639930) whose parked condition was simply inverted. Open lane gaps are listed at the end of NOTES.md. The pass
resumes from STATUS.tsv in `$S/w22/retry10`.
