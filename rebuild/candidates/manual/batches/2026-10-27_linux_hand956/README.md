# Family pass 7 (agent956, 2026-10-27): 584 of 601 rows byte-exact

199 shape families of two and three instances from `famscan.py 1024` over the 2026-10-27 catalog (FAMILIES.tsv:
M001-M199), run alongside pass 6. One Opus 5.5 agent at high effort matched each representative, generated every
instance with its own identity from its own bytes and callers, and graded every instance.

- **Landed:** 578 C++ rows (payload.json) and 6 retail_asm rows (payload_retail_asm.json: M104 CFilter_Bind2<...,
  CThingFilter_OverlappingArea>::operator() and M111 CThingSearchTools::GetAllLeafThings, both carrying the engine's
  inline-asm ceiling helper, the bare-dword fistp block of landed 0x0050EF30) with `vland.py --land` after the land5
  conversions. verify.log: 87 MATCH, 497 RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY. Families: 193
  complete, 6 parked. fixmove.py moved the GetLanguageDirectoryA/B bootstrap asm fixtures (0x0041A120, 0x0041A330).
- **Identities:** 512 proven, 67 probable, 22 descriptive; the 497 proven identities of landed rows are in
  ghidra_out/labels_familypass7_2026-10-27.tsv. M184-M199 replace propagated labels with the real ones (NOTES.md).
- **Parked (17 rows, pending/2026-10-27_linux_hand956/differ/):** M022 (3, empty allocator temporary at [ebp-1] vs
  retail's overlay on the live __x slot -- known RTM lane gap), M074 (3, a float spill slot reused), M102 (3, the
  64-bit position check's low word in edx vs eax), M115 (2, spill-slot choice across __uninitialized_copy), M130 (3,
  pair zero-store scheduling and `dec [esi] ; jne`), M180 (3, CTCVillage::IsNightTime/IsSociableTime/IsCurfewTime:
  PGameTimeManager held in ecx across two def-pointer copies; 11 spellings).

Review at landing (LESSONS.md has all 44): the RELOC_CONFLICT warnings on `_$E2` are per-TU static-destructor names
(benign); `g_MapwhoLevels` resolves to 0x0137DADC here and 0x0137DAD8 in landed 0x00637DA0 (one is likely base plus a
member offset; check); landed stand-ins to retire: CRetailTree_*, CUnprovenKey/Value_* on the COpinionReactionManagerDef
trees, UninitCopy_* / FableCopy_*, CArray<C3DVector>::_M_insert_overflow, the CVector3DList / CVector push_back
bindings on 0x0066E630, CInputProcessGameBase methods bound under derived owners, landed CThingSearchTools rows on a
derived `CThingSearchToolsMethods` (M111 uses a local CThingSearchTools); landed 0x0059D761 is FinalRefreshSubtitlesPC,
landed 0x00799FB0's container is vector<CThing*>; 173 SHARED_HEADER_AVAILABLE warnings (header debt).
