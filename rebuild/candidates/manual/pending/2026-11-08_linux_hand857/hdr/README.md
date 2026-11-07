# Chunk 857 header overlay

Every change is an additive `#ifdef FABLE_RETAIL_*` opt-in with a `// Retail opt-in (chunk 857): ...` evidence comment.
No default-path text changed (`diff rebuild/include/<f> hdr/<f>` shows only added lines), so no landed includer can
change bytes and no includer recheck was needed. Not touched: engine/CBankFile.h, rebuild_abi.h.

| Header | Opt-in macro(s) | What it adds | Evidence | Used by |
|---|---|---|---|---|
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_RESERVE` | STLport 4.6 `vector::reserve` as a visible noinline body | retail calls `vector<C3DVector>::reserve` 0x00920FF0 out of line, and VC7.1 keeps the local vector's `_M_finish` in a register across the push_back loop only when it sees the body | 0x00AC8C80 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_LIST_ERASE_RANGE` | STLport 4.6 `list::erase(first, last)` | 0x00A1C010 CullWayPoints expands the range erase inline | 0x00A1C010 |
| engine/C3DVector.h | `FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN` | Normalise / IsNonZero summed as X*X+Z*Z+Y*Y | retail 0x00AC8DCA..0x00AC8E01 load order | 0x00AC8C80 |
| engine/C2DVector.h | `FABLE_RETAIL_C2DVECTOR_NORMALISE` | Normalise() (0x00A14540, out of line), `operator*=(float)`, IsNonZero() | Ego_r/FableWin name sets; 0x00A1C010 calls 0x00A14540 and expands the others | 0x00A1C010 |
| engine/C2DVector.h | `FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX` | GetSquaredMagnitude as `float squared = Y*Y; squared += X*X;` | VC7.1 canonicalises a one-expression X*X+Y*Y; retail loads Y first in every expansion in 0x00A1C010 | 0x00A1C010 |
| engine/CANavQuadTreeNode.h | `FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE` | GetSize() declared out of line | retail calls 0x00A75B30 | 0x00A1C010 |
| engine/CNavigatorManager.h | `FABLE_RETAIL_CNAVIGATORMANAGER_GETNODEAT` | `const CANavQuadTreeNode* GetNodeAt(const C2DVector&, unsigned char) const` | PDB name set; retail calls 0x00A14F70 | 0x00A1C010 |
| engine/CNavQuadTree.h | `FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE` | `static float __fastcall GetSmallestNodeSize()` | PDB name set; retail calls 0x00A76320 | 0x00A1C010 |
| engine/C3DMeshFileBoneChunk.h | `FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE` | `const C3DBone2 GetBone() const` (0x00AB9DD0) and inline `GetName()` | FableWin name set; 0x00A8B990 calls GetBone with a hidden result and copies Name (+0x6C) in place | 0x00A8B990 (parked) |
| engine/C3DMeshFileSubMeshChunk.h | `FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK_FINDBONE` | const FindFirstBone / FindNextBone (0x00ABA6A0 / 0x00ABA730) | PDB name sets; 0x00A8B990 calls both | 0x00A8B990 (parked) |
| engine/CRenderStateManager.h | `FABLE_RETAIL_CRENDERSTATEMANAGER_STENCILOP_INLINE` (inside the existing STENCIL_INLINE branch) | `__forceinline` SetStencilOpFail / SetStencilOpZFail / SetStencilOpPass through D3DStencilOperationModeTable | Ego_r/FableWin name sets; no out-of-line copy in Fable.exe; 0x009D91A4..0x009D92DC expands them on StencilOpPass +0x3AC / StencilOpFail +0x384 / StencilOpZFail +0x398 with table entry 0x01396F40 = 3 (D3DSTENCILOP_REPLACE; table 0x01396F38 = {1..8}) | 0x009D8CF0 (parked) |

Rows in differ/ that only need overlay opt-ins already present upstream: 0x009D8CF0 also defines the existing
`FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT`, `FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT`, `FABLE_RETAIL_C2DBOXI` +
`FABLE_RETAIL_C2DBOXI_COPY` (the memberwise copy reproduces retail's field-by-field viewport save at 0x009D93AD) and
`FABLE_RETAIL_C2DBOXI_CTORS`.
