# NF84 lessons (one line each)
- A queue "one-source" name deserves a fresh grep of all three name sets before searching further: FableWin.pdb had ?SetAsPerformAction@CBrainOutput@@ at 0x02833100, which NF83 had reported missing.
- reloc_bindings.tsv is not complete per spelling: 0x0057B783 binds 0x00662210 as `QAE_NPBV..` though no such line exists there. Probe the callers (callers.py + a compile) before calling an entry done.
- For an unreferenced COMDAT whose body fits several implicit destructors, compile a TU-mate (here 0x006DCFA0) and list its COMDAT sizes, then let FableWin's per-TU placement decide which copy the link kept from this object.
- A __declspec(novtable) per-TU opt-in (the CCardPositionsDef precedent) removes the dead vptr store from an out-of-line def-class destructor in one grade.
- An inline definition of a callee's constructor in the caller's row (retail's TU holds that COMDAT) reproduces "called on the temporary, inlined on the named local" with no invented helper; check the COMDAT is pick-any (comdat.py).
- fable_compiler.h does not define FABLE_FASTCALL; rebuild_abi.h does. Two SRC_FAILs went on that.
- A `??_G` row cannot be ported alone: VC7.1 emits the scalar deleting destructor only with the class's vtable (its constructor's TU). Block such rows together with their constructor rows.
- Rows flagged "mid-function fragment" may be complete tail-jump wrappers: disassemble the bytes before the address (a `ret` / `jmp` there means a function start) and match the PDB TU order.
