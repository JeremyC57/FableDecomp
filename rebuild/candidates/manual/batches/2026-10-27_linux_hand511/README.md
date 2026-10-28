# Chunk 511 (agent511, 2026-10-28): 14 of 20 single functions landed (256-512 bytes), 1 held

Single functions of 256-512 bytes (20 rows, address order, 0x009C75A0..0x00A72CC0): STLport instantiations and engine
maths, texture and resource code. One Opus 5.5 agent at high effort.

- **Landed:** 14 rows with `vland.py --land` (all RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY):
  GFRemoveCommentsLeavingNewlines (0x00A60200; its test expects retail's memmove-size quirk), CVolumeTexture::GetAsTextureList
  (0x00A68910), __introsort_loop over pair<CCharString,CSymbolMap::CSymbolInfo>, and vector _M_fill_insert /
  _M_insert_overflow instantiations (CBufferChunk, NGameText::CTextTag, CInputEvent, CLipSyncPhoneme, C3DVectorWord,
  CQuaternion, C3DTriangle2, pair<CCharString,C3DHelperPoint>, pair<CCharString,C3DDummyObject>, pair<long,C3DMeshVolume>).
- **Held (1, pending/2026-10-27_linux_hand511/held/):** 0x00A70CB0 vector<C3DVector>::_M_range_insert<const C3DVector*>
  matches, but binds `__copy<const C3DVector*,C3DVector*,int>` to 0x0074CA20 while landed 0x0075B8D0 binds it to
  0x0075B560, and retail 0x0066DDF0 ends up under both the PAV and PBV __uninitialized_copy<C3DVector> spellings.
- **Parked (5, pending/.../differ/):** 0x00A4B670 NBBBMaths::IsPosInFOVWithWeight (312v314, Newton result kept on x87;
  retail_asm candidate for its inline FloatToLong fistp), 0x00A55F90 CMatrix3x4::SetFromVectorAndAngle (259v259, x87
  operand order), 0x00A5C360 NVectorMath::GFNormalise3DVectors (386v421, aligned frame copying the return address: lane
  gap), 0x00A618D0 CBusyResourceManager::OnPostSwapScreens (421v421, edi/ebx swap), 0x00A6BCD0
  vector<CIVCountedPointer<CManagedTextureData>>::_M_fill_insert (285v276, reload after the inline release).
- **Identities:** 5 proven from the PDBs, 15 probable (STLport element types from resizes, bindings and same-TU helpers).

Label corrections (NOTES.md): 0x00A51C60 `UninitCopy_C3DTriangle2` is the C3DVectorWord copy; 0x00A6ED80 / 0x00A6FE70 the
C3DHelperPoint pair copy and C3DMeshVolume pair fill; 0x00A67440 / 0x00A67450 `Nop_ret4` are CVolume::Swizzle /
Unswizzle (Ego_r donor order); 0x00A13C60 binds 0x00A55F90 as a CMatrix3x4 ctor that neither PDB has
(SetFromVectorAndAngle); 0x00A737B0 / 0x00A72B90 are CVectorMap Adds; 0x00A61880 / 0x00A61A80 / 0x00A61B10 are
CBusyResourceManager's ctor, ClearAllQueries and dtor. Review: 0x00A67460 ReleaseVolume takes a bool in retail (the PDB
form has none); the stand-ins SSkeletalMorphEntry / SNamedChunk / SElement12 and allocator-less vector spellings in
landed bindings are wrong; FableWin user copy ctors for C3DVector / C3DHelperPoint / C3DMeshVolume belong in the engine
headers.
