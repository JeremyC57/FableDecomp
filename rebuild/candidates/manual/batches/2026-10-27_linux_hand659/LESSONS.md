# Chunk 659 lessons (GOTCHAS one-liners)

- 2026-11-01 (chunk 659): a row parked because its callee was a declared-only stand-in can match outright once the callee's REAL body sits in the same file (0x006276F0 1244v1260 -> RELOCATION_MATCH with ConstructItemCategoryList 0x00626E40 authored above it): author retail TU neighbours together.
- 2026-11-01 (chunk 659): STLport list::insert declared-only makes VC7.1 inline _M_insert_dispatch; give list the real in-class insert/_M_create_node bodies and it is called like retail.
- 2026-11-01 (chunk 659): PeekTC as `bool present = HasTC(t); if (present) {...} return present;` gives retail's `shr/and cl,1/mov [esp+N],cl` HasTC store; the direct `if (HasTC(t))` gives `test bl,ch`.
- 2026-11-01 (chunk 659): a merge loop over a vector bound by `(long)items.size()` with a named `long key = items[i].first;` reproduces retail's signed compare and key register (0x00626E40, PC sibling 0x005907AB has the same form).
- 2026-11-01 (chunk 659): the per-site ~list inline gap (chunk 583) confirmed again: retail inlines the CDeletion copy but calls ~list<CComponent*>; VC7.1 here inlines both when any loop precedes the site. Not moved by out-of-class members, allocator layers, an earlier TU user, /GL, QFE-4035; /Ob1 calls ~list but breaks other inlines.
- 2026-11-01 (chunk 659): the PlayAnimation `xor ecx,ecx; push ecx` zero register is context-driven, not call-shape-driven: the landed 0x0091E750 keeps it with an `== 2` test added, retail CleanBench 0x0090BD40 uses immediates with the same call, and PutOutFire 0x00935E40 builds immediates under every argument form tried (pointer/reference, named zero, CSoundPair()).
- 2026-11-01 (chunk 659): first-site inlined CVectorMap::LowerBound in the Minion fire pair: retail loads _M_start into eax and copies to esi after the count test; VC7.1 here loads esi directly under every body form tried (size(), begin()/end(), first-before-count, first/last locals, FableWin CVectorMap::Find). An open allocation residue across 0x00935E40 / 0x009363D0.

## Review at landing
- NAME_FIXES.tsv: respell callers of 0x005CC772 (GetXString), 0x005D9830 (IsExpression), 0x005D96F0 (GetUIInventoryCategory), slot 62 SetDeletion/CDeletion in 0x005907AB.
- src/006276f0.cpp carries the full ConstructItemCategoryList body (0x00626E40, parked DIFFER) in the same file; landing 006276f0 lands the TU with 626e40 still graded DIFFER.
- differ/00935e40.cpp and differ/009363d0.cpp are the same file (one retail TU); land/park them together.
