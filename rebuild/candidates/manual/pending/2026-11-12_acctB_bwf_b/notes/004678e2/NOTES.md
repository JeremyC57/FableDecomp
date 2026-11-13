## 0x004678E2 CInventoryDef::Transfer -- RELOCATION_MATCH (grade 3 of 3 real grades + 2 size probes)
- Made it match: the whole 389-statement list in OUTLINE (retail) order on the shared CInventoryDef.h extended in the overlay
  (FABLE_RETAIL_CINVENTORYDEF_TRANSFER), the duplicate TradeShadedCircleTLPos statement written literally, `#pragma optimize("s", on)`.
- Needs: FABLE_RETAIL_DEF_CLASS_BASE (without it the default CParentDefClassBase has no virtuals, so declaring the virtual
  Transfer adds a second vfptr and shifts every member by 4), FABLE_RETAIL_C2DVECTOR (packed C2DVector), FABLE_RETAIL_CDEFSTRINGTABLE +
  FABLE_RETAIL_CCHARSTRING (CDefString.h #error otherwise). No flags. NAME_FIXES candidates are the dossier's (0x004568BC/0x00456903
  UpdateShadowScene -> CPersistContext::Transfer<C2DVector>/<C3DVector>; TransferBinary_xxx -> Transfer<std::vector<...> >).
