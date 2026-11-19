# Landing hand2b heaps (2026-11-19): two STLport __adjust_heap instantiations

vland --land, strict headers: RELOCATION_MATCH + PASS, /Gr, no new header opt-ins.
- 0x005E1CB0 `std::__adjust_heap<pair<unsigned long,CAnimationEntry>*,..>`: shared fable_containers.h DECAL_HEAP body + the CIVCountedPointer
  opt-ins of landed same-TU 0x005E1210 (they close the parked ebx/edi swap as there). Test 702,036 cases.
- 0x00A82BE0 `std::__adjust_heap<pair<unsigned __int64,list<C2DLineF> >*,..>`: same body + STLP_LIST with the TU's out-of-line list callees
  (LIST_ASSIGN_POPBACK_DECL 0x00517720, LIST_INSERT_DISPATCH_NOINLINE 0x005152C0, LIST_DTOR_OUTOFLINE 0x005127A0). Test 221,436 cases.
  vland RELOC_NAME on __push_heap 0x00A820E0 is a label spelling difference only.
Landed slugs carry wrong / truncated catalog spellings (payload module fields); respell queued in `NAME_FIXES.tsv` (row Leaf symbols are right).
