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

# NF96 part 2 lessons
- An "owner" of a one-definition item can itself be a stand-in: check that the owner row defines the PDB spelling before diffing copies
  against it (0x008F0B20 defined the non-const CDefPointer spelling on local models).
- A test can link the row's object by its old mangled name (0x006CB340: LINK_FAIL after a self respell). Grep each affected test for
  the old symbol before grading. A protected member can be reached from a test's local model with `friend int ::main();` (declare
  `int main();` before the namespace).
- When a call goes through a made-up stand-in class (CTCSeekTarget::RemoveSeeker), include the real class's shared header. Renaming the
  stand-in to the real class is a NEWLOCAL. engine/CTCStealth.h needs FABLE_RETAIL_CTCBASE for its sizeof assert.
- Pointer -> const reference and long -> enum respells change no bytes (same pushes). They are safe transformer edits when the local
  argument class derives from the parameter class.
- g.py prints no RELOCS for a row without a `// Leaf:` line. Use objdump -t on pS/wine/q/<addr>/<addr>.obj.
