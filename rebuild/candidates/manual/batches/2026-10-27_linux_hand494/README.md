# Chunk 494 (agent494, 2026-10-27): 40 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00C19E90..0x00C34C80: libpk, no PDB or
RTTI). One Opus 5.5 agent at high effort.

- **Landed:** all 40 rows with `vland.py --land` (verify.log: 17 MATCH, 23 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked or skipped.
- **Identities:** 3 proven (libpk's copy of CHandleBasedMemoryPool: ValidateLargestFreeArea 0x00C280E0, MoveResource
  0x00C28220, GetSizeFromHandle 0x00C283A0, from the Ego_r name set and the engine twins; labels in
  ghidra_out/labels_chunk494_2026-10-27.tsv), 2 probable (PKCDefaultFileManager::ThreadProc 0x00C1AC30,
  PKCTimerManager::ThreadMain 0x00C259E0), 35 descriptive (`CVtable_<vtable>::<method>` or existing landed spellings).

Label corrections (NOTES.md): 0x00C1BAB0 CChickenSign::GetByteD; 0x00C1EA20 CScriptOwnerThing::IsUnreferenced; 0x00C20EE0
CSoundEvent::GetMode; 0x00C265A0 CPooledResource::IsActive; 0x00C2D910 CAudioFormat::GetLoopEnd; all 33 `_gapscan` rows
named. Review at landing: landed 0x00C1EA40 "IncrementLifetimer" decrements (four bound spellings); 0x00C1EA30 and
0x00C20720 carry two and seven bound spellings; the libpk pool shares the engine class name, so 0x00C28220's PDB symbol
is also the engine 0x00A02120's (landed under an invented `CHandleBasedMemoryPool_Methods`); 0x00C34C40 / 0x00C34C80 type
their list as `CPtrArray<CBlendEntry*>` to bind existing Grow/Remove symbols (element unproven); landed 0x00C34A40,
0x00C26B20, 0x00C25A20 and 0x00C2A580 carry wrong names, layouts or vtable slots (LESSONS.md).
