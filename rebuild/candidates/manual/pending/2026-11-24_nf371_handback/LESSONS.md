# NF371 lessons
- A landed stand-in CCountedPointer Reset (local SCountBlock model) is a plain function with COMDAT selection 1 (/O2 implies /Gy). A strong
  `template <>` specialization keeps selection 1. It needs the header's Reset declaration-only (`FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL`),
  because with the in-class body VC7.1 emits nothing for the specialization. The sibling explicit instantiation turns it into selection 2 and adds
  a pick-any ReleaseData COMDAT.
- Every `new` of a count block emits an unreferenced pick-any copy of the block's inline constructor (HEAD `??0SCountBlock@@QAE@PAX@Z`, header
  `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z`, both 25 B). Any move of a stand-in Reset onto engine/CCountedPointer.h therefore swaps that copy.
  Its body changes with the signature, so objsec is never names-only for these rows.
- Where a binder's TU emits a pick-any Reset copy (it calls Reset with the header body visible), a strong owner gives an equal-body LNK2005;
  the instantiation form avoids it (GOTCHAS nf80 / nf135). Check the binders' objects for `?Reset@?$CCountedPointer@<T>` before choosing the form.
- objsec356 maps only the leaf. Rows that rename several symbols need an explicit map (tools/objsec371.py), or every renamed relocation
  reads as a difference.
- pkrun356.py needs absolute object paths (it runs the checkers from the repo root).
- pkcmp346's planted control needs the pk_all column order: verdict, address, hex address, ..., leafref, reach, symbol.
- A literal-address static (`(*(T*)0x0143e920)`) keeps a row relocation-free there. Retyping the cast is a names-only change; a real static
  would add a data relocation under a spelling the lane cannot prove.
