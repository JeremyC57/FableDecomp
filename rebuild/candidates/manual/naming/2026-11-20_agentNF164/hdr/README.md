# NF164 header overlay

Every opt-in is `#ifdef`-guarded with a member/class-specific macro; with the macro undefined each header preprocesses token-identically
to its base in orig/ (checked with cpp -P over rebuild/include, work/h_orig.i vs work/h_new.i).

## engine/CCreatureAction_ReleasePhysicalShieldSpell.h (base: repo working tree, copy in orig/engine/)
- Added `FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL` (q5102, row 0x008B93D0): the retail class on engine/CCombatActionBase.h
  (RTTI .?AVCCreatureAction_ReleasePhysicalShieldSpell@@ base CCombatActionBase, ghidra_out/rtti_bases.tsv; vtable 0x0127C09C slot 4 =
  0x008B93D0), declaring only `virtual void FinishAction();` (Ego_r.pdb + FableWin.pdb UAE). The generated opaque model moved into the `#else`.
- pin_asserts.py pinned 2 asserts (the generated sizeof / __vftable asserts now FABLE_STATIC_ASSERT_AT(30 / 31, ...), their original lines).
- Landed includers (`grep -rlE 'CCreatureAction_ReleasePhysicalShieldSpell.h' rebuild/src/compiled`): none. The opt-in changes no landed row.
- Read first: the NF159 / NF160 / NF161 / NF163 hdr/engine overlays (NF162 has none); none of them carries this header.
