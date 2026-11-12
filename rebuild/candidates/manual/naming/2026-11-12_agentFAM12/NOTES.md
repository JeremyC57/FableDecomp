# agentFAM12 notes (2026-11-12)

## Part 1 (queue land1011dr + FAM11 gaps)
- 0x00929550 -> CTCDiceGame::SetCurrentPlayer(CThing*) (local CTCDiceGame model like the landed sibling 0x00928E20; no shared header exists).
- 0x009285D0: Leaf now the PDB MAE (protected virtual) spelling; enum from engine/EAIAction.h; callee 0x0097DB70 respelled to the PDB
  IAE / const-reference spelling. 17 other binders of 0x0097DB70 still spell it other ways (stand-in classes; reloc_bindings.tsv).
- 0x00429160 -> CCountedPointer<NUISystem::CAction>::ReleaseData (claim holds). Coordinator: the ~CCountedPointer<CAction> binders
  (0x0056F615, 0x005729D8, 0x00573792, 0x005791E5, 0x00582ACE, 0x0058315B, 0x00596CCE, 0x00597006, 0x0059CB90, 0x0059ED42, 0x0059F51E,
  0x005A45F6, 0x005A731F, 0x005B70B8, 0x005C323A, 0x005C3444, 0x005CD074 and the stand-ins 0x0041DC9D / 0x0053B7F0) call it directly:
  in those TUs the destructor `{ ReleaseData(); }` is expanded (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY is the per-TU route).
  The 5-byte dtor jmp 0x00429B72 (landed as TailJmp__Dest_val...) is ~CCountedPointer<NUISystem::CAction>.
- 0x0042ABCA: claim rejected (REJECTED.tsv); binder 0x00545B10 now binds ~list.
- 0x005BC4B2 / 0x0066E799: names verified, but both bodies are folded one-instruction accessors with callers of several classes
  (brief rule 4: no single owner). Inventory binders reach them through stand-in models: left.
- 0x005BAA41 is in BUSY.txt (FAM11 row) but the brief asks for the port: rows/005baa41.cpp is built on FAM11's version; land it after FAM11.

## Part 2 (the 718 `_unattributed::` rows)
- 145 of 718 renamed (+7 rows outside ROWS.tsv). Sub-families:
  - TransferBinaryIn: 85 of 87 -> CPersistTraits<E>::TransferBinaryIn. Sources: the landed caller binding (reloc_bindings.tsv) + the PDB
    symbol itself (79) or, for 6 enums without their own PDB instance, the PDB caller template for that E (GFSerialiseVectorBinaryIn<E>,
    GFSerialiseMapBinaryIn<..E..>, Transfer<E>). Frame-pointer /Os TUs (65+4): the shared engine/CPersistTraits.h body
    (FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY over engine/CMemoryDataInputStream.h). No-frame TUs (14+2): the shared header's Read orders
    the range-check exit tails the other way (1 grade DIFFER 129v129), so these keep the landed row's own stream model (pre-existing local
    CMemoryDataInputStream text, its IsValidPosition predicate) and specialise the shared header's CPersistTraits<E>::TransferBinaryIn.
  - STLport tree / vector / map rows (InsertUnique 14, EraseRange 6, FreeTree 3, Forward_CForwarder 13 map::insert(iterator, v),
    ReleaseFirstMember 10 vector::_M_clear, Destroy_CRetailVector 3, Insert 2): stlport/fable_map.h + the explicit instantiation of the
    member; sources = the landed caller binding + Ego_r/FableWin _Tmap_traits / _Tset_traits / vector<T> name sets for the same types.
  - AssignTarget 4 (SetCurrentPlayer, SetHero, SetSeat, RegisterGameMaster), ForwardThroughMember 5 (CPlayer::UninitMode<X>).
- Binder spelling debt (BINDER_FIXES.tsv, 42 lines): callers whose recorded binding differs from the proven name, not edited here --
  STLport iterators spelled without the traits argument, maps without the allocator, _M_erase spelled public, QBE/PAW4 TransferBinaryIn
  stand-ins, ?SetGameMaster@CTCTavernGameCoinGolf@@ (no PDB). CALLEE_FIXES.tsv lists the out-of-line callees (mostly _M_insert,
  insert_unique(pos,v), _Rebalance_for_erase, iterator --) that the respelled rows now bind under STLport names while their landed rows
  carry stand-in names.
- RENAMES section 2: the caller respellings inside the respelled rows (the stand-in helper names replaced by the STLport callee names),
  plus 0x00545B10 and 0x009285D0.
- Descriptive names: none added (every new name is a PDB or STLport 4.6 name).
- Local models: none added for classes with shared headers. Kept as landed: the no-frame TransferBinaryIn rows' CMemoryDataInputStream
  view, 0x00545B10's STLport list text (its release now sits in ~list; ~_List_base empty), 0x0053C97C/0x0053C879 a minimal STLport list
  text (the FAM11 0x0053CA3C text). CAIStateGroup_UsesSeat / CTCDiceGame have no shared header (local class text).
- Rows left: LEFT.tsv (573) with a reason each; most have no landed real-typed binder and no vtable slot (one source at most).
- Tests: the catalog tests are behaviour models independent of the source text; all unchanged and PASS.
