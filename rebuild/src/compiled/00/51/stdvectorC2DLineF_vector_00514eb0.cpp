// std::vector<C2DLineF>::vector(const vector& x) -- STLport copy ctor: allocate x.size() lines, copy-construct them (out-of-line __uninitialized_copy), set the end
// Leaf: ??0?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@ABV01@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514EB0, 87 bytes, thiscall `ret 4`, built /Gr.
// Identity: FableWin.pdb ??0?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@ABV01@@Z; C2DLineF is 16 bytes (Ego_r
//   Point1/Point2); callers in the navigation-area code and std::vector<CArray<C2DLineF>>::_M_insert_overflow 0x00A75010.
// Callees: malloc (STLport allocator), __uninitialized_copy 0x00513BB0 (__fastcall under /Gr).
// Shape: STLport `_Vector_base(x.size())` + `_M_finish = __uninitialized_copy(x.begin(), x.end(), _M_start, __false_type())`.
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"

template class std::vector<C2DLineF>;