# NF211 header overlay (2026-11-21)

Every overlay header is a copy of the CURRENT repo working-tree header (base copy in `orig/`, byte-equal to the repo at the start of
this pass), with one add-only opt-in each under a member-specific macro. Without the macros each header is token-identical to its base
(`diff orig/<h> hdr/<h>` shows only the added block). pin_asserts.py after each edit: `stlport/fable_containers.h pinned 0`,
`engine/CThingCreatureBase.h pinned 0` (no assert line moved). No other agent is running (single-agent round), so no other overlay was read.

## stlport/fable_containers.h
- Added `FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY`: an `#elif` branch of the existing `FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE` block in
  `vector` that declares `vector(const vector& x);` only. The out-of-line body stays under COPY_OUTOFLINE alone, so with DECL_ONLY the TU
  defines no copy-constructor COMDAT (or its __uninitialized_copy / _Construct / allocate / size / get_allocator helpers) and calls the owner
  out of line. Same function, declaration only (the FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY pattern). Default path unchanged.
- User: row 0x006EAC50 (q4389): the vector<C3DVector> copy constructor 0x006F0810 is now external in its object (objdump UNDEF), so the
  CCombatDebugInfo copy-constructor copy (retail 0x006F3000) reaches 0x006F0810 as an external symbol, as the owner row does.
- Landed includers (`grep -rlE 'stlport/fable_containers\.h' rebuild/src/compiled`): 2179 rows (plus the 1013 fable_map.h includers through it);
  no landed row or test defines the macro (grep of rebuild/src and rebuild/tests), so the opt-in changes none of them.

## engine/CThingCreatureBase.h
- NF210's held opt-in (naming/2026-11-20_agentNF210/held/hdr/engine/CThingCreatureBase.h), verbatim; its base equals today's repo header.
  Added `FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED` (right after the `IsInMelee / IsInMeleeAsAttacker` line): a declaration of
  `bool IsInMeleeAsAttacked()const;` (Ego_r.pdb 0x0053D2C0 + FableWin.pdb 0x01DA3370 `?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ`,
  out of line at 0x00662430, the landed self row's Leaf). Declaration only: no layout change.
- User: row 0x006EAC50.
- Landed includers (`grep -rlE 'engine/CThingCreatureBase\.h' rebuild/src/compiled`): 615 rows; none defines the macro (grep), so the
  opt-in changes none of them.
