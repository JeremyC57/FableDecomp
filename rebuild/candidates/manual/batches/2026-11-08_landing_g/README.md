# Landing 2026-11-08g: chunk 857's two matches

Chunk 857 (expert agent; eight never-attempted 1.6-3.5 KB engine / UI rows) matched 2 of 8; both landed RELOCATION_MATCH + PASS (read-only vland
grade with a trimmed overlay, then `--land`). Hand-back: `pending/2026-11-08_linux_hand857/`; its six parked rows went on to chunk 860.

| Row | Grade | Note |
|---|---|---|
| 0x00AC8C80 QuadricOptimiserInternals::COptimisedPrimitive::ComputeVertexCost | **RELOCATION_MATCH + PASS, landed** | was catalogued `COptimisedPrimitive_ComputeBoundingBox` |
| 0x00A1C010 CNavigatorAStar::CullWayPoints | **RELOCATION_MATCH + PASS, landed** | was catalogued `AStar_process_node_queue` |

- **Headers landed** (all additive opt-ins, 0 lines removed; `coordinator/hdrcheck2.py` printed `default=same` for all six, and no landed file
  defines the new macros): `stlport/fable_containers.h` FABLE_RETAIL_STLP_VECTOR_RESERVE / STLP_LIST_ERASE_RANGE, `engine/C3DVector.h`
  C3DVECTOR_NORMALISE_UNPAREN, `engine/C2DVector.h` C2DVECTOR_NORMALISE / C2DVECTOR_SQUAREDMAGNITUDE_YX, `engine/CANavQuadTreeNode.h`
  CANAVQUADTREENODE_GETSIZE_OUTOFLINE, `engine/CNavigatorManager.h` CNAVIGATORMANAGER_GETNODEAT, `engine/CNavQuadTree.h`
  CNAVQUADTREE_SMALLESTNODESIZE. Not landed (parked rows only): CRenderStateManager.h STENCILOP_INLINE, C3DMeshFileBoneChunk.h GETBONE,
  C3DMeshFileSubMeshChunk.h FINDBONE.
- **Names:** `ghidra_out/labels_landing_2026-11-08g.tsv` (12 lines: the two rows, their callees MakeOrthogonal 0x00AC7200 / GetFaceNormal
  0x00AC7CB0 / IsRouteClear 0x00A1A260, and the six parked rows; 0x005AECC0 is `SellItem`, `SellItemPC` is 0x005B01FE). Queue `land0811g`: five
  landed placeholders to respell (0x00AD0A70 set-iterator `operator++(int)` PRIORITY, now also bound by 0x00AC8C80; 0x00437160 `GFGetSign`;
  0x00A8CB70 `CCountedPointer<C3DBone2>::DeleteData`; 0x00553E60 `map<ulong,ulong>::operator[]`; 0x00A8DB30 `__destroy_aux<CCountedPointer<C3DBone2>*>`).
  vland WARN: 0x0129BA3C is labelled `EPSILON` where the shared header binds `?DELTA_NOTIONAL_ZERO@@3MB` (one source only; not relabelled).
- Catalog sync restricted to this landing's addresses (backlog unchanged). `bindings_multi_check.py` prints 1, the pre-existing NullCoord.
