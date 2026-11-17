# NF52 lessons (one line each)
- "Heavy" rows with whole local STLport / boost / string / D3D models moved onto the shared headers in 1-2 grades: run `cpp` over the target header and grep it for the row's local class names first; only the clashing models need replacing.
- Made-up RAII helpers (CShaderLayoutSaver, CShaderEnableSaver) usually have a PDB twin with a shared header (CScopedVertexShaderConstantLayout, CScopedShadersEnable); search engine/CScoped*.h before writing an overlay.
- A static data member's compiler-generated atexit destructor (??__F / `dynamic atexit destructor`) is ported by DEFINING the static in the row; for a template static VC7.1 names it the TU-local `_$E2` (initializer `_$E1`), so grade with that leaf.
- When retail loads an argument into a callee-saved register before an out-of-line call that yields the object (edi = ParentThing, then PeekWorld()), bind the argument to a reference local first; computing the object first moves the load after the call.
- The same object can need different spellings in different TUs: keep each as its own opt-in (inline ctor for the expanding TU, declaration for the COMDAT's TU) instead of changing the default path.
- EH funclet rows `#include` their parent by a relative path: stage the new parent at <dir>/../src/compiled/<path> and put <dir> on FABLE_HDR_OVERLAY, then grade the funclets with `funclet_offset` + `/EHsc` in the payload.
- vland ignores a row's `// Flags:` line; put the flags in the payload (mkmy does not), or an /EHsc row costs an extra sweep compile.
- A shared header whose layout asserts fail (CCard.h) usually needs a companion opt-in its other users define (FABLE_RETAIL_C3DVECTOR_PACKED); grep a row that includes it before the first grade.
- Never edit grades.log; guard every grade script against a missing source file before it compiles.
