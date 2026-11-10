// CArray<CTVertexWaterBackground>::push_back(const CTVertexWaterBackground& x) -- append a 56-byte water vertex: block-copy it to the finish pointer when there is room, else the out-of-line _M_insert_overflow @ 0x00B7FFC0
// Leaf: ?push_back@?$CArray@VCTVertexWaterBackground@@@@QAEXABVCTVertexWaterBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE4F90, 72 bytes, `__fastcall`, `ret 4`. Callers: four sites in the water
// tesselator 0x00BE4660 (CWaterPatchTesselatorBase<CTVertexWaterBackground>, its BridgeVertexBuffer CArray in Ego_r.pdb).
// The manifest label "Vector_PushBack_14Elements" is descriptive. Sibling: the sea-vertex push_back 0x00BE5950.
// Layout: the shared engine/CArray.h (CArray<T> : std::vector<T>, the 12-byte STLport vector) and engine/CTVertexWaterBackground.h
//   (56-byte POD vertex); the overflow is the protected std::vector<CTVertexWaterBackground>::_M_insert_overflow 0x00B7FFC0 (as the
//   sea twin 0x00BE5950; no longer a CArray member).
// Shape: STLport `push_back`: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the POD's implicit copy is `mov ecx,0Eh ; rep movsd` through
// edi = _M_finish (the end pointer read first into ecx, `this` moved to eax); the empty tag temporary has no user constructor,
// so VC7.1 zero-stores its byte in the `push ecx` frame word.
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "engine/CTVertexWaterBackground.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"

template void CArray<CTVertexWaterBackground>::push_back(const CTVertexWaterBackground&);
