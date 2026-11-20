# NF138 lessons (one line each)
- A free stand-in at a tiny address (`xor al,al ; ret`) is worth an Ego_r order check: the neighbour rows' PDB names pinned 0x00733290 as IsMeleeBlockCounterValid, which neither landed caller had.
- A binder that only needs its callees respelled can keep its HEAD local model of the caller class and drop just the local CDefPointer stack for engine/CDefPointer.h; the shared operator= on a known-null local gave retail's AddRef-only copy first time.
- When a callee's PDB parameter type differs from the row's local container (vector& vs CArray&), the self row's leaf changes too: exe-scan its referencers first (xr.sh) and hand the pair back together.
- engine/C3DVector.h hides operator-= behind FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN: grep the operator before the first compile.
- STLport-instance stand-ins whose callers are landed (CElement12 __copy) are families, not respells: count the callers before spending a form.
