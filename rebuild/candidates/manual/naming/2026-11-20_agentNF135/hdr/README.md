# NF135 header overlay (round 10)

Every overlay header is the repo working-tree header (copied unchanged into orig/) plus `#ifdef` opt-ins only. Without the new macros the
preprocessed token stream is identical to the repo header (the only edits are inside the new `#ifdef` and the `#else` / `#endif` lines that
close it). Other agents' overlays (NF132: CEnvironment, CGame, CInitBaseClass, CMovieSubtitles, CNetworkClient; NF134: CVertexShaderConstantLayout)
do not touch these headers.

## engine/CSnowTrollDef.h (base: orig/engine/CSnowTrollDef.h)
- New opt-in `FABLE_RETAIL_CSNOWTROLLDEF` (q6453, row 0x008D7630): the retail class `CSnowTrollDef : public CSubDefClassBase` (RTTI
  .?AVCSnowTrollDef@@ : CSubDefClassBase, ghidra_out/rtti_bases.tsv) with the Ego_r.pdb members at the flat branch's offsets (+0x28 .. +0x5c,
  sizeof 0x60) and the inline StatGetTypeName ("CSnowTrollDef", the literal GetSubDef<CSnowTrollDef> 0x008C7A70 builds), the same shape as
  engine/CBriarRoseDef.h's FABLE_RETAIL_BRIAR_ROSE branch. The existing FABLE_RETAIL_CSNOWTROLLDEF_ALLOC declaration is repeated inside the
  new branch. The flat branch becomes the `#else` (unchanged text).
- pin_asserts.py: `pinned 0` (the flat branch's FABLE_STATIC_ASSERT_AT lines keep their tags); it lists the three new plain
  FABLE_STATIC_ASSERT lines of the new branch as "new, not pinned" (as CBriarRoseDef.h's retail branch uses plain asserts).
- Landed includers (`grep -rlE 'CSnowTrollDef\.h' rebuild/src/compiled rebuild/tests`): 0x0044C72B, 0x0044C669 (+ its test), 0x00453014,
  0x004541DF. None defines FABLE_RETAIL_CSNOWTROLLDEF, so none changes (they keep the flat branch). The new user is 0x008D7630 only.
