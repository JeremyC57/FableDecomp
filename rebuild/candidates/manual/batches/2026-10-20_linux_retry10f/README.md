# Retry 10, sixth landing (2026-10-20)

Segment seven of the retry pass (Opus 5.5 at extra-high effort): 113 more parked rows reach RELOCATION_MATCH and are landed
here with `verify_and_land.py --land` (verify.log, all 113 tests PASS). Totals for the pass: 377 matched of ~1,400 rows,
855 rows never worked individually (`$S/w22/retry10/s7/unworked.txt`). The pass was **stopped by the owner** after this
segment; the resume point is the last `RESUME` line of NOTES.md. The segment's big levers were same-TU callee bodies:
vector growth members that read the value only through its const reference and never touch the empty tag (about 27 rows),
GetSubDef/GetDef stand-ins that read the old out pointer like `CDefPointer::operator=` (about 20 rows), a stand-in's own
helpers also defined in the TU, template instantiations given real bodies at any address distance, and map::find with a
named copy-initialised iterator. Three DIFFER rows (00567d00, 00a5b260, 009a7ca0) are grader artefacts (int3 after a
noreturn call); they need hand-landing through the verifier, which grades them correctly. Body generators and sweep drivers
are in `$S/w22/retry10/s7/`.
