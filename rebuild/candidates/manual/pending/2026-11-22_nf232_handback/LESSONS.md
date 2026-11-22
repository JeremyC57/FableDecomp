# NF232 lessons (one line each)
- A stand-in self row's size pragma is not evidence of the TU's mode: 0x00411B00 under `optimize("s", on)` grew a frame pointer on the header body; /O2 matched.
- Porting the second binder of a split (0x00412B70) is often a one-line explicit instantiation whose pick-any inner copy equals another landed owner (resize(n, x) = 0x00412B20): try it before calling the split "left".
- VC7.1 stores no vptr in an implicitly-defined destructor, so a probe TU that only calls it emits no ??_7: declare a ctor to read a header-built vtable.
- The cpp precheck (`cpp -D_MSC_VER=1310 -I rebuild/include`) catches header #error guards (CDefPointer.h needs FABLE_RETAIL_DEF_CLASS_BASE) that otherwise spin cl under Wine for the whole timeout.
- A virtual self-call in a header test can go through a test vtable: copy a same-signature member's pointer (`void (S::*)(...)`, 4 bytes in MSVC single inheritance) into the slot and point the raw object's first word at the table.
- A protected static of a shared class is reached in a test through a never-constructed subclass with a static accessor, no `#define protected public`.
- A test's failure line must not contain the row's PassPattern (`PASS_004336b0 FAIL` would match it).
- lnk.py needs the lane env (`source $S/wine/env.sh`), or V.VC resolves to the Windows path.
