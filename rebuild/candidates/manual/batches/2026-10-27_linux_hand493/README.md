# Chunk 493 (agent493, 2026-10-27): 35 of 40 single functions byte-exact

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00B80780..0x00C19970: engine renderers,
zlib/LZO, libpk). One Opus 5.5 agent at high effort. Its STATUS.tsv has `flags=` in the status column (an agent
script slip); verify.log is the record of what landed.

- **Landed:** 35 rows with `vland.py --land` (verify.log: 18 MATCH, 17 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY).
- **Parked (2, pending/2026-10-27_linux_hand493/differ/):** 0x00BF2980 CEngineTreeFilerBase::GetTreeMemSize (113v96: VC7.1
  rewrites the child-table loop to a walked pointer and spills `this`; retail indexes `[edi+esi*4+1Ch]`), 0x00C039F0
  zlib 1.2.1 `syncsearch` (92v82: retail's register argument convention comes from LTCG, which the lane cannot link).
- **Skipped (3):** 0x00BFEEA9 (CRT exception-filter fragment on the parent frame), 0x00C088B0 (two packed functions),
  0x00C0BF9B (mid-routine fragment).
- **Identities:** 11 proven (9 landed, in ghidra_out/labels_chunk493_2026-10-27.tsv), 8 probable, 18 descriptive
  (libpk classes with no PDB or RTTI, named by vtable or address).

Label corrections (NOTES.md): 0x00BDD6B0 is the CLocalDetailObjectCollectionType ctor (RTTI vtable 0x012A7D94), not
CXMVPlayerBuffers; 0x00C08880 is CString::CString() (stores g_pchNil); 0x00C16750 is CDebugBreak::CDebugBreak();
0x00C109A0 is std::logic_error::what; callee 0x00C0C270 is CString::ConcatInPlace and 0x00C08940 the landed
CEventQueue_00c08940::Process. Review: the libpk red-black tree is split over several suffixed class names and its +0x24
member is a node pointer, not Count; CRampedParam and CInterpolatedFloat_00c3a950 are one 0x34-byte class; landed
0x00C0B380 binds 0x00C0B570 with a (void*, long, void*) signature the other callers spell (float, float, float).
