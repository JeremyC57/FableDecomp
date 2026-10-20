# Retry 10, third landing (2026-10-20)

Segment three of the Fable 5.1 retry pass: 274 rows worked, 138 matched in total; the 27 not yet landed in `_retry10a/b`
landed here with `verify_and_land.py --land` (verify.log). New families: a third inline-destructor exit for script wait
loops, `bool found = A || B` with the terminating check inside, named call result then member chain, CIntelligentPointer
operator= as `T* item = other.GetPItem(); SetPItem(item);` (12 rows), STLport operator[] through an inline begin()
accessor, size() as an inline `_Last - _First`. Open gaps are re-confirmed at the end of NOTES.md. About 960 rows remain;
the pass resumes from STATUS.tsv.
