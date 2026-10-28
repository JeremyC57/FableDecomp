# Chunk 527 (agent527, 2026-10-28): 13 of 20 re-queued single functions landed (248-467 bytes)

Re-queue chunk of 20 rows of 248-467 bytes (0x009C2800..0x00A2D960): bank files, the console, file installer, C3DVector
math, navigation, surfaces and STLport vector / tree / find instantiations for mesh and animation infos. One Opus 5.5
agent at high effort.

- **Landed:** 12 rows with `vland.py --land`, all RELOCATION_MATCH (0x00A169B0 `std::__find<C2DVector*,C2DVector>` needs
  /Gr), and 0x00A145C0 C3DVector::GetRandomUnitVector as grade `retail_asm` (payload_retail_asm.json): retail runs two bare
  `fistp dword` (0x00A14617, 0x00A14668) from the engine's inline-asm float-to-long helper inlined through
  GFFastCos / GFFastSin. Every test PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Parked (7, differ/):** 0x009C5170 ~CFileInstaller (retail keeps zero in ebx; likely a `volatile` member, which purity
  forbids), 0x009CE050 CBankFile::InitEntry (temporary lifetime; two probes not yet graded together), 0x009F39A0
  CSurface::CopyToMemory, 0x00A12D20 GetNavigationLineList, 0x00A146C0, 0x00A1AD40, 0x00A22CA0 (scheduling / x87 order).
- **Identities:** all 20 row names proven except the C2DVector element type of 0x00A169B0 (probable, 8-byte stride).

Name fixes (NAME_FIXES.tsv, 29 rows, deferred): 0x009EC890 CConsole::RunScript (bound ExecuteConfigFile in 7 callers),
0x00A14260 `C3DVector::Rotate(float,float)`, invented CRetailVector_*::InsertGrow / CMeshInfoArray spellings are STLport
`vector<T>::_M_insert_overflow` (IAE), `__uninitialized_copy` PAV/PAV forms, 0x009C1F70 `_Rb_tree_iterator::operator--`,
0x0099ED10 GetAsLowerCase, 0x00A62960 CCriticalSectionLock::Release. 16 catalog labels corrected (NOTES.md).
