# NF96 lessons
- Start every Bash call with the brief's env line (`source $S/wine/env.sh`): g.py without it fails before compiling (NO_RESULT).
- reloc_bindings.tsv lists one binder per symbol: enumerate exe call sites with nftools/xref.py (needs `S` exported) + owner.py; it found
  0x006DAEC0 as a binder of 0x004DA27F that the table misses.
- A row whose callee needs `mov edx,ecx ; call ; mov ecx,edx` register knowledge cannot use a header inline+noinline body: VC7.1 only
  assumes it for a strong same-TU definition (q11204).
- Static list initializer / atexit pairs: define the static on its shared header and let the compiler emit `_$E1` / `_$E2`; add
  FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE when the destructor tail-jumps to an out-of-line ~list.
- When an object's undefined symbols look odd, remember the header-inline COMDATs the object defines (e.g. __pop_heap calling
  __adjust_heap); vland's relocation list is the authority.
- vector::size() in a modulo: retail signed `cdq ; idiv` needs an inferred `(long)` cast.
