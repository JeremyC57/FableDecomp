# Chunk 507 (agent507, 2026-10-27): 29 of 30 single functions byte-exact (128-256 bytes)

Queued chunk of single functions of 128-256 bytes (30 rows, address order, 0x00CABAE0..0x00DD6950): DirectShow strmbase,
one STLport insertion step, and NScript quest, level and entity scripts. One Opus 5.5 agent at high effort.

- **Landed:** 29 rows with `vland.py --land` (all RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY): the two
  CBaseAllocator ctors (wide and char object name), CBaseAllocator::ReleaseBuffer, CBasePin::EnumMediaTypes,
  CBaseDispatch::GetIDsOfNames, std::__unguarded_linear_insert<CBindingPair*,CBindingPair,CBindingLess>, 17 entity-script
  methods, five level-script OnPersist / Init / MonitorBanditRegion bodies, and the CQ_OrchardFarmRaidScript ctor.
- **Parked (1, pending/2026-10-27_linux_hand507/differ/):** 0x00D6BAC0 CHTE_AmbushPoint::Main (174v187: retail opens with
  `push ecx` and a dead store to an unread local, and pads the loop head with a 3-byte `lea ecx,[ecx]`).
- **Identities:** 28 proven (vtable slot, Alloc/ctor and both PDB name sets; strmbase by SDK body, slot and callee
  bindings; 0x00D6CCC0 by the CSpawnedFunc "MonitorBanditRegion" its caller builds), 1 probable (0x00DCDED0
  CGuardTeamMember::OnPredicateFail: no PDB has the class; members follow the Ego_r twin CCrateTeamMember). Proven landed
  rows in ghidra_out/labels_chunk507_2026-10-27.tsv.

Label corrections (NOTES.md): 0x00CBA9B0 `SpeechFrame_InsertSortedWithComparison` is std::__unguarded_linear_insert;
0x00DCC040 `CHeroMorphDef_CopyOrInit` is the CQ_OrchardFarmRaidScript ctor; 0x00704580 `CCountedPointer_Set_00704580` is
CCountedPointer<CScriptThing>::operator=. Review: landed CUnknown_00cacc80.cpp line 1 says WCHAR but the bindings make
0x00CACC80 the char overload (PBD). SHARED_HEADER_AVAILABLE CBaseObject on the strmbase rows is the known false positive.
No engine headers exist for the NScript classes, so each script row carries a local model with `// Layout:` lines.
