# 2026-11-11 landing by -- agentNF17: 112 landed rows respelt, 86 mislabelled CheckCRC binders renamed to CPersistContext::Transfer<T>

Source: `naming/2026-11-11_agentNF17/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 112 RELOCATION_MATCH.
42 tests PASS in vland; the other 70 print their catalog PassPattern (`PASS_<addr>`, not vland's default `<addr>_TEST PASS`) --
checked by running each exe, 70/70. Bindings re-derived from the recheck objects; bindings_multi_check unchanged (NullCoord baseline).
- The 94 binders of 0x00404500 CPersistContext::CheckCRC were mislabelled rows (CEngineLightingManager::UpdateShadowScene /
  _global::PersistTransfer_<addr> / CEnginePrimitiveRenderer2D::AddPrimitive): retail-only one-argument binary transfers. 86 renamed onto
  the engine/CPersistContext.h declarations -- 57 Transfer<enum>(E&), 26 TransferMap<V,K>(map&), 1 TransferVector<ENavigatorType>
  (0x006724B0) -- with T proven per row from the caller's def field offset (Ego_r member type) plus a PDB three-argument instance or a
  landed binding. The one-argument parameter list is descriptive (no PDB carries it). Transfer<bool> / Transfer<unsigned char>
  respelt in place; 6 rows left (NOTES).
- 0x0040F020 CVectorMap LowerBound: 19 more rows (incl. four test mocks respelt); 7 CDataOutputStream adapter rows onto the plain header
  (WriteSLONG / WriteEBOOL).
- 67 name fixes queued as land1011by (callers of the renamed rows, 8 GFSerialise*Binary helpers; conflicts: 0x00580371 is
  Transfer<EHeroTrainingStatus> by its wrapper vtable RTTI, 0x00412D20 is GFSerialiseVectorBinaryIn<ETutorialCategory>; landed 0x004331F9
  spells TransferVectorBinary where the header says TransferVector).
