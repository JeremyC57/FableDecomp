# Chunk 512 (agent512, 2026-10-28): 16 of 20 single functions landed (256-512 bytes), 1 held, landed 2026-10-28

Single functions of 256-512 bytes (20 rows, address order, 0x00A72DD0..0x00AA3AF0): STLport instantiations over the
navigation and mesh types, and engine navigation and keyframe code. One Opus 5.5 agent at high effort.

- **Landed:** 16 rows with `vland.py --land` (all RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY): C3DMesh2::PackBoneNames
  (0x00A8B6C0), `_M_fill_insert` over CArray<C2DLineF>, list<C2DLineF>, CNavigationLayer, C3DTriangle2 (/GS) and C3DVertex2,
  `_M_insert_overflow` over pair<CCharString,C3DMeshVolume>, list<C2DLineF> and CNavigationLayer, `operator=` over
  list<C2DLineF> and pair<u64,list<C2DLineF>>, three `__introsort_loop`, `__find<const CNavigationPosition*,C2DVector>` and
  `__uninitialized_fill_n<CNavigationLayer*>`.
- **Held (1, pending/2026-10-27_linux_hand512/held/):** 0x00A74AC0 vector<C2DLineF>::_M_range_insert matches, but binds
  `__uninitialized_copy<const C2DLineF*,C2DLineF*>` to 0x00A13490 while landed 0x00A13650 binds it to 0x00A11210 (two
  byte-identical 56-byte bodies), and 0x00A13490 would carry both the PAV and PBV spellings.
- **Parked (3, pending/.../differ/):** 0x00A74180 CPhysicsMesh::CalculateLayeredNavigationLineInfo (489v472, branch shape
  and a hoisted 0xFFFF), 0x00A77780 CNavRegion::IsConnectedToRegion (302v298, callee-saved pushes delayed past the early
  return), 0x00AA3AF0 C3DKeyframe::ToMatrix(CMatrix4x4&, bool) const (346v346, six integer stores scheduled early).
- **Identities:** 13 proven, 7 probable, 1 descriptive global (NavRegionSearchID_013d2d28).

Label corrections (NOTES.md): every catalog label in the chunk was a behavioural guess (NavMesh_ProcessLine_Indices,
NavigationGraph_BreadthFirstSearch, Resource_CompressAndSave_AnimationData, Quaternion_ToMatrix4x4, Vector_Assign_*).
Outside it: 0x00A77540 is CNavQuadTree::IsActionPoint (probable), 0x00994B40 CMemoryDataOutputStream::GetLength,
0x00996B30 ~CMemoryDataOutputStream, 0x00A73E50 CPhysicsMesh::GenerateLineListFromBaseline, 0x00A70EE0 `__destroy_aux`,
0x00A95A10 / 0x00A96B30 the C3DVertex2 helpers. Twelve landed bindings spell these STLport helpers with invented names or
the wrong access (IAE vs QAE); NOTES.md lists each. Header debt: CMemoryDataOutputStream.h / CDataOutputStream.h lack
the virtuals; CNavQuadTree.h lacks the nested CNavigationLayer; no headers for CPhysicsMesh, C3DKeyframe, CQuaternion,
CMatrix4x4.

**2026-10-28: the held row landed.** 0x00A74AC0 is the member template `_M_range_insert<C2DLineF*>`, not the
const_iterator form. Proof: retail calls one `__uninitialized_copy` target (0x00A13490) from all six placement copies and
one `copy` target (0x00A74840) from both range copies. STLport 4.6 produces that only when the range iterator is the
vector's own `C2DLineF*`. The const_iterator form instantiates two copy helpers, and retail does not fold identical
code (18 separate identical IS_THING_ALIVE bodies). The caller agrees: FableWin.pdb has CArray<C2DLineF>::DeallocateUnusedMemory, and its landed
body inserts a non-const CArray's begin()/end(). The landed copy 0x00A74840 was respelled `copy<C2DLineF*, C2DLineF*>` and
the landed caller 0x00A74EA0 now declares the member-template insert; both recheck clean (verify_twinfix.log) and the new
row landed RELOCATION_MATCH with no conflict. 0x00A11210, which landed 0x00A13650 binds as the C2DLineF copy, is called
only from C3DClothQuad code (fill<C3DClothQuad*>, the ClothQuad assign): a separate review item.
