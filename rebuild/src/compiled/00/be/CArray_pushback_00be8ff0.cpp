// CArray<CEngineWaterBackgroundSubPatch*>::push_back(CEngineWaterBackgroundSubPatch* const&) -- Lionhead's CArray push_back: store at `finish` when there is room, else the STLport growth insert for one element
// Leaf: ?push_back@?$CArray@PAVCEngineWaterBackgroundSubPatch@@@@QAEXABQAVCEngineWaterBackgroundSubPatch@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE8FF0, 50 bytes, `ret 4`.
// Flags: none
// Name: FableWin.pdb ?push_back@?$CArray@PAVCEngineWaterBackgroundSubPatch@@@@QAEXABQAVCEngineWaterBackgroundSubPatch@@@Z, and the
//   retail binding: CLandscapeBackgroundPatch::Render 0x00BE7BE0 calls 0x00BE8FF0 for the CEngineWaterRenderer CArray render lists
//   (engine/CEngineWaterRenderer.h; the landed 0x00BE7BE0 binds this symbol). Name-fix pass (2026-11-12): the row was the
//   stand-in CRetailVector_dword::PushBack_dword_00be8ff0; it is now the explicit instantiation of the engine/CArray.h push_back, the
//   same spelling as the landed CArray<CTVertexWaterBackground>::push_back 0x00BE4F90.
// Callees: std::vector<CEngineWaterBackgroundSubPatch*>::_M_insert_overflow (POD overload) 0x00BE8F10.
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
class CEngineWaterBackgroundSubPatch;

template void CArray<CEngineWaterBackgroundSubPatch*>::push_back(CEngineWaterBackgroundSubPatch* const&);
