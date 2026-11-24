# NF360 lessons
- An explicit specialization (`template <> ... X<T>::f(...) { }`) of a class-template member that the class body DEFINES in-class emitted no
  symbol at all under VC7.1 (0x00428B10 probe, work/var/00428b10_v1a_inclassbody.cpp: object with .drectve / .debug$S only; pass.py grade
  OBJDUMP_ERR). Under a declaration-only member (FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY) the same specialization is an ordinary
  strong definition (sel 1). Check the object, not only the grade, when a self moves onto a specialization.
- A "strong form like 0x0042B040" self collides with every pick-any copy (LNK2005), and the different-body copies make that a NEW
  different-body duplicate. Census DEF selection AND body (copycensus360.py), not only DEF/UND, before choosing strong v pick-any.
- Eleven landed binders define GetDef<CThingBaseDef> strongly (a multiline search; a single-line grep found seven) (`template <>` without `inline`): grep binders for non-inline `template <>`
  definitions of the owner's symbol before assuming they bind UND.
- The scratch-corpus census includes stale objects; only HEAD-compiled controls are proof (0x00659164, 0x007CBDC0, 0x00744ED0, 0x004CA010,
  0x005331A0, 0x00626750, 0x005820E7 here).
