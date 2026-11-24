# Landing nf391 b1: the 0x0051B720 map unit (T3 nf389, rulings NF389 r1/r2/r5 and R1)

Three rows of `map<CThingBuilding*, list<CThingAICreature*>>` drop their stand-ins for STLport names:
- 0x00519970 is `_Rb_tree<...>::_M_erase`; it was `?_M_erase@CBuildingCreatureMapTree@@`.
- 0x0051AF20 is `~_Rb_tree<...>`.
- 0x0051B720 is `~map<...>`.

The last two were both `??1CRetailTree_erase00519970`.
- **Identity:** FableWin's CWorldMap::PopulateRegionWithVillagers destroys two locals of this map type at every exit. Retail 0x00502E90 calls 0x0051B720 at 0x005048C6 / 0x005048D2. 0x00502E90 has two name sources: both PDB name sets, and the same three RTTI-slot callers in retail and FableWin. A full raw scan finds exactly two 66 B destructor bodies that reach 0x00519970. With ~map pinned and two compiler-required names, elimination places ~_Rb_tree at 0x0051AF20; this is not the R9 case.
- **Ruling R1 (advisor decision, owner delegated):** every chain COMDAT is tier 1 or tier 2. Tier 1: identical by `comdat_cmp` to the landed 005029a0 / 0051a2f0. Tier 2: a retail home by a reloc-masked function-start scan. The 51 B tree clear has a unique home at 0x0051A560; that fixes the address, not the name (T3 nf391 0x0051A560). The 14 B `_Rb_tree_base` dtor matches many starts, so it is accepted with no address. The 1 B allocator dtors are reloc-free. The holder dtors are the nf387 per-TU helper. NF391's evidence is in pending/2026-11-24_nf391_handback (scan_b1.tsv, comdat_bins.tsv).
- **Table (all four present after landing):**
  - replace the old `_M_erase` self line with the STLport spelling;
  - delete `?Erase@CRetailTree_erase00519970 -> 00519970`, whose only binders were 0051af20 / 0051b720 (VC7.1 census; no source, header or table use of `CRetailTree_erase00519970` or `CBuildingCreatureMapTree` remains, apart from a comment in 0051aa00);
  - add self lines for ~map and ~_Rb_tree.
- **Coordinator checks:**
  - landed 3 of 3;
  - each source equals the batch row;
  - each self equals its `// Leaf:` line and `self_symbols.tsv` (aligned by hand, unit id 0051b720);
  - no includers;
  - pk EQ at 0x00519970 / 0x0051AF20 / 0x0051B720 / 0x004AB4F0 with no new NEQ, reach flips 0, link duplicates 1 -> 0 (NF391);
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- **T3 nf389 0x0051B720:** narrowed and moved to 0x00513420 (the find form and the two binder ports). NF391's residuals are queued as T3 nf391 lines.
