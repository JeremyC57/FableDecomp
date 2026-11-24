# Landing nf394 b2: the GetNumberOfNavigationLineLevels unit (T3 nf391 q9; rulings r1, r2)

One unit (owner 0x00A127C0):
- **0x00A73CF0** is `?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ`.
- **0x00A127C0** is `?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ`, which tail-jumps to the CPhysicsMesh one, as Ego_r 0x00B80500 -> 0x00C12630 does.
- **0x006A4F10** is `?GetNoNavCollisionLineLists@CThingPhysical@@QBEJXZ`. Ruling r1 (advisor, owner delegated) accepted it: both PDB name sets, and Ego_r 0x00683F90's body is instruction-identical. It replaces a void*-returning non-const self that needed a cast.

Both GetNumberOfNavigationLineLevels names are in both PDB sets, and FableWin's body calls `vector<CArray<C2DLineF>>::size` on NavigationLines. The wrong table line `?GetNoNavCollisionLineLists@CPhysicsMeshObject@@QBEPAXXZ -> 00a127c0` (witness 006a4f10) is replaced by the PDB name, and the CPhysicsMesh line is added (witness 00a127c0).
- **Headers:** two add-only, default-off opt-ins: `FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS` (types NavigationLines as `CArray<CArray<C2DLineF> >` and declares the function) and `FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATIONLINELEVELS` (declaration only). The one changed line pins CPhysicsMeshObject.h's static assert to its line, 113. The coordinator reran equiv_check (exact) over the 46 landed rows whose source or test includes either header: 0 differ. NF394 ran it over 1,431 rows: 0 differ. No claim names either header.
- **Rulings:**
  - r2 (advisor, owner delegated): R1 tier 1 applies to an extra COMDAT on a non-pick-any row. 00a127c0's `CIVCountedPointer<const CPhysicsMesh>::operator->` is EQ by COMDAT to landed 00a11d10's, which includes the shared headers (coordinator `comdat_cmp`).
  - 00a73cf0's `vector<CArray<C2DLineF>>::size` has only local-model landed definers, so it is tier 2: 24 B, reloc-free, 59 retail function starts, no address recorded.
- **Coordinator checks:**
  - landed 3 of 3;
  - graded symbols are the new names (landing log and NF394 logs/vland_u2n_gradelines.log);
  - self = `// Leaf:` = `self_symbols.tsv` (00a127c0's line is an add);
  - each source equals the graded source;
  - no other row defines the new 006a4f10 name, and no other table line targets 006a4f10;
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- **Follow-ups:** T3 nf394 lines (0x00DA0E40 port, 0x006A4F10 shared-header port, 0x004FB4B0 virtual respell, stale tests, the `?Run@T@@QAEXXZ` duplicate pair) and NAME_FIXES_QUEUE nf394 (catalog labels).
