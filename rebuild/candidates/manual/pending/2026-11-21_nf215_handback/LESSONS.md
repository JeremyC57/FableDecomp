# NF215 lessons (one line each)
- Census a COMDAT family by compiling every candidate row once (cl only, 0.3 s a row) and listing DEF / REF of the symbol with objdump: callers.py alone overstated nothing but "about 20 binders" was 3 binders + 13 dead definers.
- An out-of-line inline copy takes the owner's body only if the TU sees the owner's whole spelling: NOINLINE + COMPARE_BODY gave 76 bytes (`? false : true`), the ==0 spelling gave the owner's 88.
- Inside a `#pragma auto_inline(off)` region VC7.1 calls the compiler-generated ??_G for `delete p` (and for an explicit dtor call) instead of expanding the inline destructor chain; keep only the one function out of line (guarded __declspec(noinline)).
- Check whether a "make it inline" header hypothesis has a retail out-of-line copy first (0x0099E960 operator!= calls 0x004115A0; the TU that uses it would call it).
- A same-TU copy that matched by bytes at HEAD can still bind stand-in symbols (NOT-BOUND relocations); the header port fixed 0x00859040's counter and 0x006238A0's NullChar.
