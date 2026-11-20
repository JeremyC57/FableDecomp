# NF181 lessons (one line each)
- A ??_G stand-in row becomes nf86 scaffolding in one grade when the shared header has an inline constructor: placement-new it in an anonymous-namespace helper; the vtable and ??_G come out pick-any (0x009568E0, 0x00485420). Without an inline constructor there is no row-level fix (TU-emitter).
- `pgrep -f <script>` inside a `bash -c '...<script>...'` wait loop matches the loop itself: poll the output file or `ps` instead.
- An access-only callee fix on a row-local base (QAE -> IAE) is a safe transformer (61 rows, 61 matches), but standalone tests that define the same local class with a public ctor fail to link: grep the tests for the class before the batch.
- A row-local base with a manual `void* __vftable` can take `virtual ~Base()` in place of it (same layout) so a derived dtor can become virtual (UAE).
- Check RTTI (ghidra_out/rtti_bases.tsv) before renaming a vendor class: Fable's strmbase calls combase.h's CBaseObject CMyBaseObject.
- A catalog `what` label next to a 2-slot no-COL vtable identifies VC7.1 <stdexcept> code; a Fable PDB class name on such a body is a propagation error.
