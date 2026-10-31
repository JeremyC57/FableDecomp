# retry/sec5 lessons
- Check the extent before tuning code. Three rows looked hopeless only because the catalog extent ran into an
  uncatalogued function (00a69c20 381->186, 009c5890 710->389, 0099e690 344->180). Disassemble to the first
  ret/int3 run and look at what follows the padding. q3/v.sh read oracle_extra.tsv first, so add the extent there.
- An `x * GetInverse()` temporary costs a full 0x30 return matrix in VC7.1. An in-place `__forceinline
  SetInverse(const CMatrix3x4&)` on a named matrix gets back retail's frame. Swapping commutative operands
  in the cofactors does nothing (VC canonicalises them).
- VC7.1 (RTM 3077) unrolls a counted `for` min/max loop by 4. A guarded `if (n > 0) { i = n; do {...; ++p;} while (--i); }` stays rolled.
- 8-byte all-float structs (C2DVector, CVertexUV) as address-taken temporaries force an `and esp,-8` frame. A user
  dtor or copy ctor does not remove it, and neither do by-value parameters.
- The unfolded add-ref (`mov esi,ebx; load; inc; store`) against ours `inc [ebx+4]` does not respond to ++/AddRef/
  explicit forms (00a69b70, 00a69c20). Treat it as volatile-only.
- Making SetDirtyDiscError a non-forced in-class inline makes VC7.1 call it out of line (fewer diff lines but wrong).
  Retail inlines it, so keep __forceinline.
- Defining UnassignString's body after its use gives partial inlining (0099fb60: 462 bytes between 390 and 620). This is a lead for
  retail's inline only in the final destructor.
