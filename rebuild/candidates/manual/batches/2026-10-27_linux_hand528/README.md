# Chunk 528 (agent528, 2026-10-28): 17 of 20 re-queued single functions landed (241-502 bytes)

Re-queue chunk of 20 rows of 241-502 bytes (0x00A2EEF0..0x00A44420): STLport vector / heap / sort instantiations for 3D
object, helper-point, animation-state and class-type-info containers, the CSoundManagerLH NamePairMap sort family,
CSoundManagerLH::AddBank and C3DMeshStats::PackNames. One Opus 5.5 agent at high effort.

- **Landed:** 17 rows with `vland.py --land`, all RELOCATION_MATCH, every test PASS under g++ and VC7.1; no RELOC_IDENTITY,
  CONFLICT or DUPLICATE. Every vector row closed on the first or second grade once each same-TU helper COMDAT had a body
  under `auto_inline(off)` + `noinline`.
- **Parked (3, differ/):** 0x00A3BF10 C3DAnimationBlendState::CreateInterpolate (one `fstp st(0)` one instruction late),
  0x00A43CD0 `vector<CLipSyncDataBankEntry>::_M_fill_insert` (retail decrements the refcount in memory), 0x00A44420
  GFIsPointInTriangle (dot-product component order).
- **Identities:** all proven except 0x00A3F140 CSoundManagerLH::AddBank (class from RTTI slot 37, method descriptive).

Name fixes (NAME_FIXES.tsv, 37 rows, deferred): the CLipSyncDataBankEntry helper chain 0x00A43500..0x00A43B20 is landed on
NGameText::CDataBankTextEntry; 0x00A3FDC0 is swap of the NamePairMap entry, not `swap<CSymbolMap::CSymbolInfo>` (the
landed STL_swap_00a3fdc0.cpp needs respelling); `_M_insert_overflow` is IAE at 0x00A41480, 0x00A41390, 0x00A3D060;
0x00A41E20 / 0x00A42060 are std::sort; 0x00A442E0 / 0x00A443A0 GFGetShortestDistance[2D]SquaredBetweenPointAndLine;
0x00994B40 CMemoryDataOutputStream::GetLength. All 20 catalog labels were propagated or descriptive.
