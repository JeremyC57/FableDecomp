# Chunk 495 (agent495, 2026-10-27): 36 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00C34E20..0x00C57110: libpk / GameCODA audio
middleware, no PDB or RTTI). One Opus 5.5 agent at high effort.

- **Landed:** 36 rows with `vland.py --land` (verify.log: 16 MATCH, 20 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). 0x00C3D9A0 (`??_E` of CMorphAction_00c3dcd0, /EHsc) first declared CPKObject's `operator delete[]`
  out of line at 0x00C3C370, binding one retail address under two names (RELOC_DUPLICATE); the coordinator made
  `operator delete[]` an inline forwarder to `operator delete`, which keeps RELOCATION_MATCH with one name.
- **Parked (2, pending/2026-10-27_linux_hand495/differ/):** 0x00C3D030 CAudioSystem::GetChannelCounts (92v94: retail
  hoists the out pointer and tail-duplicates the last device store), 0x00C42FB0 CAudioObject::SetMillibels (40v36: `this`
  kept in ecx across a cdecl call, an LTCG contract the lane cannot link).
- **Skipped (2):** 0x00C35460 (four packed functions) and 0x00C3C4B0 (two) need a catalog re-split.
- **Identities:** 6 proven by existing bindings or landed models (ghidra_out/labels_chunk495_2026-10-27.tsv), 9 probable,
  21 descriptive.

Label corrections (NOTES.md): six propagated catalog labels replaced (e.g. 0x00C4BFF0 CRACBuffer::GetAlignedSize,
0x00C4C000 CRACBuffer::GetPointer); landed 0x00C35170 (CBaseIntelligentPointer ctor) is the single-item table's ctor,
0x00C34DD0 the grid table's ctor, 0x00C42AB0 / 0x00C42AC0 CPtrArray<CTableEntry*>::GetAt / GetCount, landed 0x00C5A1D0
(CTCStealth::OnCreate) the software voice's slot-1 setup, 0x00C3D7B0 ~CMorphAction_00c3dcd0.
