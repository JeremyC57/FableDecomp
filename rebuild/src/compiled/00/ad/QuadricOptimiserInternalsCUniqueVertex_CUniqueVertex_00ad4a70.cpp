// QuadricOptimiserInternals::CUniqueVertex::~CUniqueVertex() -- destroy FaceList's then VertexIndices' STLport set<unsigned long>
// Leaf: ??1CUniqueVertex@QuadricOptimiserInternals@@QAE@XZ
// Retail 0x00AD4A70, 0x7B bytes (mesh-optimiser TU, 0x00AC/0x00AD block): the member-by-member destructor (FaceList then
// VertexIndices: each STLport set<unsigned long> tree destructor is clear() -- _M_erase(root) out of line at 0x004CF810 when the
// count is non-zero, then the header reset -- followed by free (0x00BFEA14) of the header node).
// Name (NF197, 2026-11-20): Ego_r.pdb 0x00C6A7D0 + FableWin.pdb 0x034C94F0 ??1CUniqueVertex@QuadricOptimiserInternals@@QAE@XZ;
//   engine/QuadricOptimiserTypes.h (FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX) declares it out of line here; the landed callers
//   (ComputeContraction 0x00AC92C0, ~COptimisedPrimitive 0x00AD84D0, ...) bind this spelling. Was the descriptive
//   CTwoLongSets::~CTwoLongSets over local CLongSet / CRbNode models (bound by no landed row). FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
//   keeps _M_erase declared only, so it is called (retail 0x004CF810, the STLport spelling 0x00AE67D0 binds) and not defined here.
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "engine/QuadricOptimiserTypes.h"

QuadricOptimiserInternals::CUniqueVertex::~CUniqueVertex()
{
}
