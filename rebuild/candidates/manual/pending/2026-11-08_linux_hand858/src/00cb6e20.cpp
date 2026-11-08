// std::vector::~vector() -- (vector<std::pair<CCharString, CScriptInfo> >) destroy every script-info pair, then free the vector's block
// Leaf: ??1?$vector@U?$pair@VCCharString@@VCScriptInfo@@@std@@V?$allocator@U?$pair@VCCharString@@VCScriptInfo@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB6E20, 57 bytes, thiscall, plain `ret`; callees ~CCharString 0x0099EAE0 (twice per
//   element: CScriptInfo::Name at element +0x0C, then pair::first at +0x00), free 0x00BFEA14 (allocator::deallocate's null test).
// Identity: STLport 4.6 ~vector (_Destroy(_M_start, _M_finish) through the inline __destroy_aux loop, stride 0x18, then the inline
//   ~_Vector_base). Element: pair<CCharString, CScriptInfo> (24 bytes: first +0, CScriptInfo +4 with Name at +8 of it, Ego_r.pdb
//   CScriptInfo 20 bytes); the same element type as the protected _M_clear 0x00CB6E60 (identical body, called by the landed
//   _M_insert_overflow 0x00CB6FD0 at 0x00CB70B6). Reached only by tail `jmp` from the two one-instruction wrappers 0x00CB6FB0 and
//   0x00CB71F0 (the outer container layers, FableWin.pdb ??1?$CArray@U?$pair@VCCharString@@VCScriptInfo@@@std@@@@QAE@XZ and the
//   CVectorMap destructor, which call this layer out of line). FableWin.pdb lists
//   ??1?$vector@U?$pair@VCCharString@@VCScriptInfo@@@std@@V?$allocator@...@std@@@2@@std@@QAE@XZ. Probable layer (see NOTES.md).
// Model: stlport/fable_containers.h (vector, pair, allocator), engine/CScriptInfo.h.
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"

template std::vector<std::pair<CCharString, CScriptInfo> >::~vector();
