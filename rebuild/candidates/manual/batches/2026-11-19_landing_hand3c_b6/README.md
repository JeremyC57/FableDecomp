# Landing hand3c b6 (2026-11-19): 2 rows

vland --land, strict headers: RELOCATION_MATCH + PASS. No header changes.
- 0x00BEE070 std::__pop_heap<pair<long,CCompositeSurfaceInfo>*,..,CKeyPairCompareLess<..>,int*>: explicit instantiation over the shared
  FABLE_RETAIL_STLP_HEAP + CCompositeSurfaceInfo.h (twin of landed 0x00BEEDA0). RELOC_NAME on __adjust_heap 0x00BEDD70 is a label spelling only.
- 0x00664BA0 CThingCreatureBase::GetCurrentMeshRadius (184): shared headers in FableWin 0x01D999E0 statement order with Ego_d locals;
  GetSubDef<CCreatureDef> visible but noinline (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE). 3 grades.
Header hazard found (GOTCHAS): engine/CCreatureDef.h with FABLE_RETAIL_CCREATUREDEF_TRANSFER but without FABLE_RETAIL_CDEFSTRINGTABLE makes cl spin.
