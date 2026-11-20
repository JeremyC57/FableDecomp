# NF162 lessons (round 10)

- **Class-nested enum in CPersistTraits<T>.** Porting a Transfer<T> row whose T is an enum nested in a script class onto
  engine/CPersistTraits.h fails with C2065 / C2064 in `DefaultVal()`; the existing FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT fixes it
  with the same bytes (one grade).
- **TransferOut / TransferIn pairs.** For a CPersistContext_TransferableComponent<T> generated stand-in, the landed twin 0x00E1E0F0 body on
  engine/CPersistContext_TransferableComponent.h + CPersistTraits.h (TRANSFEROUT opt-in) + the enum's script header grades
  RELOCATION_MATCH first time; only the T changes.
- **Transfer<T> and its TransferOut are a family.** Once the three-argument Transfer<T> is on the shared headers, its object defines the
  component vtable COMDAT, which names the TransferOut / TransferIn of the same T. Hand the Transfer row back with the TransferOut row.
- **Restart recovery.** Re-run the census (callers.py) on each inherited row against its exact new leaf, and do the objdump defined-symbol
  diff vs HEAD for any ported row whose STATUS does not record one. Re-grade one "SRC_FAIL at HEAD" row before reporting the break.
