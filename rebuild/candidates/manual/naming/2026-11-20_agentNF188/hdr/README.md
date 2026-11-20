# NF188 header overlay (2026-11-20)

Every overlay header is a copy of the CURRENT repo header (base copy in ../orig/) with `#ifdef` opt-ins only. With none of the opt-in macros
defined, each header is token-identical to its base except the static asserts that pin_asserts.py pinned to their original lines
(`FABLE_STATIC_ASSERT(...)` -> `FABLE_STATIC_ASSERT_AT(<original line>, ...)`, so every assert typedef keeps its name).

## engine/CLandscapeBackgroundTreeNode.h (base: rebuild/include/engine/CLandscapeBackgroundTreeNode.h)
- `FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_GETEDGESTRIP` (inside the PASS22 block; row 0x00BF6930): declares
  GetSplitBranch() / PeekWorldX() / PeekWorldY() / GetEdgeStrip(long,long,LANDSCAPE_EDGE_STRIP_SIDE) (Ego_r.pdb / FableWin.pdb
  spellings) and the two private base overrides RelocateData / FreeableResourceFreeBlock (Ego_r EAE), so the split branch can embed two
  nodes; after the class it pulls the split branch's retail opt-in, the quarantined CEngineLandscapeMap (read through the existing
  `Map` pointer member only) and CEngineMap PEEK, and defines the three inline bodies. A forward declaration of
  CLandscapeBackgroundSubNodeSplitBranch under the same macro.
- pin_asserts.py pinned 7 asserts.
## engine/CLandscapeBackgroundSubNodeSplitBranch.h (base: rebuild/include/engine/CLandscapeBackgroundSubNodeSplitBranch.h)
- `FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_SPLIT_BRANCH` (row 0x00BF6930): the retail layout, RTTI base CLandscapeBackgroundSubNodeBase,
  LeftBranch / RightBranch as CLandscapeBackgroundTreeNode at +0x04 / +0xEC (needs the TreeNode PASS22 mode). The default path (the
  generated donor layout) is unchanged, inside `#else`.
- pin_asserts.py pinned 4 asserts.
- Landed includers of the two headers (grep): 19 rows; none defines either new macro, so the opt-ins change none of them.
## candidates/landscape_background_lod_compiled.h (base: rebuild/include/candidates/landscape_background_lod_compiled.h)
- `FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_SHARED` (row 0x00BDC180, q4251): includes engine/CMemoryDataInputStream.h (defining
  FABLE_RETAIL_CDATAINPUTSTREAM / FABLE_RETAIL_CMEMORYDATAINPUTSTREAM if unset) instead of the header's local CMemoryDataInputStream model
  (kept unchanged in the `#else`). pin_asserts.py pinned 0 (the asserts were already FABLE_STATIC_ASSERT_AT).
- Landed includers: 2 files (the row 0x00BDC180 and its test); both now define the new macro (the test via rows/00bdc180_test.cpp).
## engine/CWaterPatchMesh.h (base: rebuild/include/engine/CWaterPatchMesh.h)
- `FABLE_RETAIL_CWATERPATCHMESH_CREATEFROMSTREAM` (row 0x00BF5A30, q6204): declarations only, no layout: the class placement
  operator new (Ego_r.pdb / FableWin.pdb ??2CWaterPatchMesh@@SIPAXIPAVCMovableResource@@@Z, landed 0x00BEA7E0) and the static __stdcall
  factory CreateFromStream(CMovableResource*, CEngineLandscapeMap*, CDataInputStream*) at 0x00BF5A30 (descriptive name, no PDB).
- pin_asserts.py pinned 0 (every assert was already FABLE_STATIC_ASSERT_AT).
- Landed includers (grep): 11 files; none defines the new macro (only rows/00bf5a30.cpp does), so the opt-in changes none of them.
