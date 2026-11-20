// CWaterPatchMesh::CWaterPatchMesh(const CEngineLandscapeMap* map, CDataInputStream& stream, C2DCoordI& offset) -- a water patch mesh at a landscape offset: map and offset kept, no vertex data yet, then loaded from the stream
// Leaf: ??0CWaterPatchMesh@@QAE@PBVCEngineLandscapeMap@@AAVCDataInputStream@@AAVC2DCoordI@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEB490, 57 bytes (+7 int3 padding), thiscall, `ret 0Ch`.
// Identity: stores the CWaterPatchMesh vtable 0x012A8134; called from CEngineLandscapePatch::LoadForegroundPatch
//   (0x00BF409E) and the landed CWaterPatchMesh::CreateFromStream 0x00BF5A30 (0x00BF5B04), whose header names this
//   constructor's (map, stream, const C2DCoordI*) arguments. Catalog label CWaterPatchMesh::CWaterPatchMesh.
// Callees: CWaterPatchMesh::Load(CDataInputStream*) 0x00BEAE50 (bool result dropped).
// Layout: engine/CWaterPatchMesh.h (Map +4, Offset +8, VertexBuffer +0x10, VertexHeightData +0x14) models the vptr as a
//   data word, so the constructor of the polymorphic class needs a local model with a real vptr (same offsets and names).
// Shape: init-list members (Map, the two Offset words, the xor-zeroed VertexBuffer/VertexHeightData) around the vtable
//   store, then the Load call on `this` with the stream argument. C2DCoordI needs a memberwise user copy constructor:
//   the implicit bitwise copy sinks the Offset.Y store below the Load argument push.
// NF188 (2026-11-20, q6204): the PDB signature (Ego_r.pdb / FableWin.pdb ??0CWaterPatchMesh@@QAE@PBVCEngineLandscapeMap@@AAVCDataInputStream@@
//   AAVC2DCoordI@@@Z, the only CWaterPatchMesh ctor taking a stream; the body keeps the const map pointer and hands the stream on to
//   Load without a dereference) on the shared engine/CWaterPatchMesh.h retail class (vptr from CMovableResource, Load by reference
//   ?Load@CWaterPatchMesh@@AAE_NAAVCDataInputStream@@@Z) and engine/C2DCoordI.h with its memberwise copy (FABLE_RETAIL_C2DCOORDI_COPY);
//   was a row-local model with pointer parameters.
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_C2DCOORDI_COPY
#include "engine/CWaterPatchMesh.h"

CWaterPatchMesh::CWaterPatchMesh(const CEngineLandscapeMap* map, CDataInputStream& stream, C2DCoordI& offset)
    : Map(map), Offset(offset), VertexBuffer(0), VertexHeightData(0)
{
    Load(stream);
}
