# Landing nf396 b5: STL identities traced by NF391 get their STLport spellings (006c2c00 )

Part of T3 nf391 q11. Each row was traced by NF391 (retail caller path + the matching FableWin function) and respelt by NF396 to the symbol a VC7.1 compile of the repo STLport emits (both PDBs are Dinkumware, so the vendor signature names the instance: ruling R6), in the pick-any form (explicit instantiation on fable_containers.h / fable_map.h):
- 0x004DF554: `~_Vector_base<const CSteeringAgent*>`. The ~vector thunk 0x004DF54F jumps here, which decides _Vector_base.
- 0x005793E8: `_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType, NUISystem::CComponent*>>::clear`. Its caller is the tree destructor 0x005796AB, so this is not map::clear. The table gains the `_M_erase` line -> 00578ff5 (witness 005793e8); 0x00578FF5's own self is still a stand-in (T3 nf396 0x00578FF5).
- 0x006553D0 / 0x006553FD: `list<CMultiStatBar::CStatUpdate>::erase(iterator)` and `list<CAutoPickupBar::CAutoPickup>::erase(iterator)`.
- 0x006C2C00: `_List_base<CCountedPointer<CLevelLoader::CObject>>::clear`.

(Each README lists every row; this batch lands only the rows in its title.)
- NF396 gates:
  - all nine q11 addresses are real function starts and direct call targets;
  - vland HEAD and new RELOCATION_MATCH; pk_all and pickany_vs_retail with no new NEQ, 0 reach flips;
  - extra COMDATs tier 1 or tier 2 only;
  - apply_table --dry 0 misses;
  - /FORCE link duplicates 5 -> 3 (the 3 already existed);
  - VC7.1 census of the header-reach set: no landed row binds an old name.
- Coordinator checks: landed with RELOCATION_MATCH on the new self; self = `// Leaf:` = `self_symbols.tsv`; no LITERAL_DIFF; baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- Held from q11: 0x004E1BD2 (one unit with 0x004DFB8F and the 00813460 port), 0x006CE220 (identity), 0x004E35D4 (port 0x004DD728 first), 0x0057FEFB (R1 residue). See T3 nf396.
