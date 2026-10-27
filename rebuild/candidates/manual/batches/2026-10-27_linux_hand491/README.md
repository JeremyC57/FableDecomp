# Chunk 491 (agent491, 2026-10-27): 39 of 40 single functions byte-exact

Seventh queued 40-row chunk of single functions under 128 bytes (address order; first of the 491-496 cut). One Opus 5.5
agent at high effort.

- **Landed:** 39 rows with `vland.py --land` (verify.log: 17 MATCH, 22 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked.
- **Skipped (1):** 0x00A446A0 is the start of `?ThreadEntry@CSuspendableProcessMicrothread@@CAXPAX@Z`, one /Od function
  of 158 bytes (0x00A446A0..0x00A4473D, SEH catch funclet at 0x00A446F4) that the catalog splits into five rows; it needs
  re-cutting as one /Od + /EHsc row.
- **Identities:** 28 proven (ghidra_out/labels_chunk491_2026-10-27.tsv), 9 probable, 2 descriptive (NOTES.md).

Label corrections (NOTES.md): 0x00A01D40 CHandleBasedMemoryPool::GetHandleFromPointer (not CWorld::DrawGetEnvironment);
0x009D8240 NProfileTimer::StartProfile (twin 0x009D8250 EndProfile; not `$E2`); 0x00A44690
CSuspendableProcessMicrothread::InternalSuspend (the `_Cons_val`/`_Dest_val` labels in this region are wrong); 0x00A25E60
CPhysicsMeshInfo::CMeshData::OnResourceLoaded; outside the chunk 0x00A3AA10 is CCompressorZlib::Initialise and 0x00A2F120
C3DMeshStats::GetAllHelperPointNames.

Review at landing:
- 0x009F2AC0 CMemoryAllocator::InternalAllocate (five-argument, Ego_r name) keeps a dead `char filename[48]` local only so
  VC7.1 emits the /GS cookie retail has. It stands for the real CTrackingDebugRecord::Filename (char[48]); VC7.1 keeps
  the cookie for an array the optimiser removes, so the original source likely had such a buffer, but the local is a
  device until proven.
- 0x009D47E0 emits its copy ctor through the model-only helper `ThrowBankCreatorException_009d47e0`.
- 0x009E7380 carries a same-TU stand-in body for `__find_if` 0x009E6A70 (must not bind as a second definition).
- 0x009F8860 spells `_M_create_node` public (QAE, as bound) where STLport 4.6 declares it protected.
- 0x009EAAE0 uses a model-only CRGBColour::SetChannels helper.
