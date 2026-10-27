# Chunk 504 (agent504, 2026-10-27): 24 of 30 single functions byte-exact (128-256 bytes)

Queued chunk of single functions of 128-256 bytes (30 rows, address order, 0x00A5AAA0..0x00C1DD70: STLport sorts and
vector growth, engine methods, libpk queues). One Opus 5.5 agent at high effort.

- **Landed:** 24 rows with `vland.py --land` (verify.log: 24 RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY):
  four `__introsort_loop` and one `__partial_sort`, nine `vector::_M_insert_overflow`, two `vector::operator=`,
  COptimisedVertex::InterpolateGroups, CEngineSceneGrid::GetTotalCellCount, ~CEngineLandscapeMemoryManager,
  std::swap<pair<long,CCompositeSurfaceInfo>>, and four libpk /EHsc rows (PKCStaticQueue<SFileRequest>::Push / Pop,
  PKCOneShotQueue<SFileReply>::Pop, a CMorphableNode slot).
- **Parked (6, pending/2026-10-27_linux_hand504/differ/):** 0x00A60410 GFRemoveComments (262v245, strstr loop inverted),
  0x00AC7200 MakeOrthogonal (255v255, x87 term order), 0x00B67E30 RegisterSubPatchForZSorting (128v129, folded bitfield
  word not kept in a register), 0x00C0FFE0 ~PKCProtectionInstance (256v253), 0x00C11230 CSoundEventManager::CanStartEvent
  (242v242, epilogue block placement), 0x00C1CA20 CMatrix3x4::RotateC (140v140, x87 product order).
- **Identities:** 17 proven (14 landed rows in ghidra_out/labels_chunk504_2026-10-27.tsv), 7 probable, 6 descriptive.

Label corrections (NOTES.md): `Quicksort_*` labels are std::__introsort_loop, `Vector_*` labels `_M_insert_overflow`,
`Vector_AssignWithRealloc` vector::operator=; `Vector_ProjectOntoPlane` is MakeOrthogonal;
`QuadricOptimiser_UpdateQuadricCosts` InterpolateGroups; `ParticleSpriteGroup_Destroy` ~CEngineLandscapeMemoryManager
(its callee `TexturePool_Clear` is FreeDisposalList); `Std_Make_Heap_ProcessPairs` __partial_sort; 0x00BF90A0 is
CEngineTexturePalette code, not CParticleMorphs; `EventQueue_ProcessWithTimeout` is ~PKCProtectionInstance; 0x00C1B460
is PKCStaticQueue::GetTailIndex; the `operator&&<NScript...>` labels on 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10 are
CString::operator<<. Review: landed libpk queue models declare `void Pop` (slot 0 returns its argument: `T* Pop(T*)`) and
signed count/capacity (retail uses unsigned); landed 0x00BDB750 is typed C2DLineF from a BSim label; landed sort_heap
0x00BEF350 uses an invented SDecalEntry; 0x00A6B420 / 0x00A6BBB0 declare pair and less as class (PDB: struct); landed
0x00C25CB0 `~CParticleHandle` is a Release.
