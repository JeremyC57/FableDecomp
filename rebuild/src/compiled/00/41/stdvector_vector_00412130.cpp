// std::vector<CWideString>::~vector() -- destroy every wide string, then free the vector's block
// Leaf: ??1?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412130, thiscall, plain `ret`; callees ~CWideString 0x0099B510 (per element, stride 4),
//   free 0x00BFEA14 (allocator::deallocate's null test).
// Identity: STLport 4.6 ~vector (_Destroy(_M_start, _M_finish) through the inline __destroy_aux loop, then the inline
//   ~_Vector_base); 20+ landed owners of a vector<CWideString> (0x0044C72B, 0x005CE0E6, 0x005D1F0C, 0x005958F5, ...) call it as
//   ??1?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAE@XZ. The manifest label "CCombatWheel::ResetRings" is
//   propagated (acctB 0x0044C72B evidence).
#include "stlport/fable_containers.h"
#include "fable_string.h"

template std::vector<CWideString>::~vector();
