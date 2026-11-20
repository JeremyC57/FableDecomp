# NF184 lessons (one line each)
- Declaration-only is not a fix when a row depends on seeing the callee's body. Three times here (_M_fill_insert, less<CCharString>, the CAnimInit ctor) hiding the body broke the row (frame byte, cached list head). Make the pick-any copy equal the owner instead.
- When an emitted STLport copy differs from its owner, try the header's existing STLport-faithful opt-ins first. FILL_CHAR + ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22 turned the 266-byte _M_fill_insert<char> into the owner's 289 bytes, with no header edit.
- A copy whose owner sits in a size-optimised TU matches when the header defines it under the owner's pragmas (an explicit specialization, or an out-of-class inline definition). Callers still expand it, and the COMDAT gets the owner's bytes.
- `extern template` (C4231) on a member suppresses the copy but also the body's analysis: the same DIFFER as declaration-only.
- Grade the source at each owner address with q.py's leaf argument (q.py <owner> <row.cpp> <copy symbol>). It is the quickest pick-any check, and it catches DIFFERs that cmpbody on a local-model owner object cannot explain.
- A root-level `#pragma once` header (fable_string.h) cannot be overlaid when repo headers include it by relative path: it gets defined twice. Put the opt-in in an include-guarded header the row includes first (stlport/*.h).
- STLP_MAP makes insert_unique's body visible. For a key type with no operator< in its header (CWideString) the emitted copy silently compares converted pointers. Check every newly defined COMDAT, not only the ones the brief names.
- Before porting a CCountedPointer "destructor" claim, look for a 5-byte `jmp` thunk to the 44-byte body (bytes around the jmp site): the thunk is the destructor and the body is ReleaseData.
- probe.py compiles every REFS row. Check locks on that list first, even for a read-only census.
