# 2026-11-11 landing cj -- chunk Q7 (parked near misses, Opus 5.5): 0x0086BFB0 CRandomAppearanceMorph::TransferBinaryIn

vland --land: 1 win (RELOCATION_MATCH), test PASS.

**Headers**
- engine/CRandomAppearanceMorph.h gains the opt-in FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN, which adds:
  - the inline default constructors of CBodyPartMesh and CSkeletalMorph (Ego_r.pdb; retail zeroes them at 0x0086C086 and 0x0086C29C);
  - the TransferBinaryIn declaration.
- hdrcheck2 reports a line DIFF only because two class lines were split around the `#ifdef` blocks. With the opt-in blocks stripped, the default tokens are identical to HEAD.
- engine/CVectorMap.h gains FABLE_RETAIL_CVECTORMAP_SETDIRTY (SetDirty(bool), Ego_r). default=same.

**Warning**
- vland's RELOC_NAME warning: 0x0086E5C0 is vector<CBodyPartMesh>::_M_fill_insert. The catalog label `std::vector::std::vector::Resize_CSharerParserEntry_24byte` is a placeholder, and the fix is queued as land1011cj.

**Q7's other matches are not landed**
0x0047E651, 0x0047E9AE, 0x0047F30F (CTCInGameMenu) and 0x00697490 (CGameCameraManager) went back to the agent. They used local models, or stand-in classes and names, where shared headers exist.
