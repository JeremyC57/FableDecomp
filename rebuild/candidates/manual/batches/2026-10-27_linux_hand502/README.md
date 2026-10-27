# Chunk 502 (agent502, 2026-10-27): 30 of 33 single functions byte-exact

The last queued chunk of single functions under 128 bytes (33 rows, 0x00EC5420..0x01224CA5: quest/entity scripts,
CPredicate_And clones, def transfers, and compiler dynamic initializers in .text$yc). One Opus 5.5 agent at high effort.

- **Landed:** 30 rows with `vland.py --land` (verify.log: 8 MATCH, 22 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nine are dynamic initializers (Leaf `_$E1`, object defined in the TU with its real inline ctor):
  NullString, g_PendingLoadedGlyphs, NProfileTimer LogFile, CMatrix4x4 Identity, DefaultUncompressedPFormat,
  CNavQuadTree BlockedNode, LOCAL_HEAP_CParticle, CpuDetect, GlobalConsoleCutsceneName. The 6 RELOC_CONFLICT warnings are
  the TU-local `_$E2` atexit names (one per TU by design).
- **Parked (2, pending/2026-10-27_linux_hand502/differ/):** 0x01223F00 / 0x01223F60, the two
  StreamingFontCacheBufferData_static initializers: VC7.1 RTM folds both records into .data and emits no `_$E1`.
- **Skipped (1):** 0x00F29F60 is a 5-byte `jmp` to 0x00F29F70 plus a second body (a copy-assign); re-cut as two rows.
- **Identities:** 29 proven (21 carried into ghidra_out/labels_chunk502_2026-10-27.tsv from the agent's notes), 2
  probable, 1 descriptive (the libpk CPU-detect static).

Label corrections (NOTES.md): the atexit thunks 0x0122A5E0 (NullString) and 0x0122AAD0 (BlockedNode) carry a
shape-copied `std::_Cons_val<...>` label, 0x0122AC90 `CTCInventoryItem::ClearCachedDefs`; landed 0x0040E1E0 is
CPersistContext::GetDefaultVal<CCharString>; 0x00C52E40 is labelled InitCPUCapabilities but bound DetectCpu (pick one);
landed 0x00A768C0 models CANavQuadTreeNode against the PDB ctor signature.
