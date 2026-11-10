// CArray<CTVertexSeaBackground>::push_back(const CTVertexSeaBackground& x) -- append a 12-byte sea vertex: placement-copy at the finish pointer when there is room, else the out-of-line _M_insert_overflow @ 0x00B80400
// Leaf: ?push_back@?$CArray@VCTVertexSeaBackground@@@@QAEXABVCTVertexSeaBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE5950, 72 bytes, `__fastcall`, `ret 4`. Callers: four sites in
// CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge 0x00BE4FE0 (the BridgeVertexBuffer CArray, Ego_r.pdb
// CWaterPatchTesselatorBase<CTVertexSeaBackground>::BridgeVertexBuffer). The manifest label "Vector_PushBack_3Floats" is descriptive.
// Layout: the shared engine/CArray.h (CArray<T> : std::vector<T>, the 12-byte STLport vector) and engine/CTVertexSeaBackground.h
// (Ego_r.pdb CGenericVertex<CTVertexSeaBackgroundBase>, 12 bytes: X, Y, Z); the overflow is the protected
// std::vector<CTVertexSeaBackground>::_M_insert_overflow 0x00B80400 (Codex pass 15 name fix: no longer a CArray member).
// Shape: STLport `push_back`: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- `this` stays in ecx across the call; the vertex copy is three
// dword moves through one register (a memberwise copy over the three words: the floats move as integers, a float copy
// constructor would go through the FPU); the empty tag temporary has no user constructor, so VC7.1 zero-stores its byte in
// the `push ecx` frame word (`mov byte ptr [esp+17h],0` after the pushes).
#define FABLE_RETAIL_CTVERTEXSEABACKGROUND_HIERARCHY
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "engine/CTVertexSeaBackground.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"

template void CArray<CTVertexSeaBackground>::push_back(const CTVertexSeaBackground&);
