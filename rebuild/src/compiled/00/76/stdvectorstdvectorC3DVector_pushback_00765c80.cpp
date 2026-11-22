// std::vector<std::vector<C3DVector> >::push_back(const std::vector<C3DVector>& x) -- append a point list: copy-construct it in place at
//   _M_finish when there is room, else grow through _M_insert_overflow (one element, at the end)
// Leaf: ?push_back@?$vector@V?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@V?$allocator@V?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@2@@std@@QAEXABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765C80, 59 bytes, thiscall `ret 4`: `cmp [esi+4],[esi+8]; je grow`; the null-tested
//   placement copy through vector<C3DVector>'s copy constructor 0x006F0810, `add [esi+4],0Ch`; grow: _M_insert_overflow 0x00765B70
//   (x, __false_type(), 1, true; the tag temporary homed in the dead argument slot).
// Names (NF223, 2026-11-22, q12548): the STLport spelling the landed callers bind: 0x00763060 NShape::FractureShape calls this function as
//   ?push_back@?$vector@V?$vector@VC3DVector@@...@std@@QAEXABV?$vector@VC3DVector@@...@2@@Z, and 0x00763060 / 0x007634E0 call 0x00765B70 as
//   ?_M_insert_overflow@?$vector@V?$vector@VC3DVector@@...@std@@IAEXPAV?$vector@VC3DVector@@...@2@ABV32@ABU__false_type@2@I_N@Z (the self leaf
//   of the landed 0x00765B70). Was the July placeholder `CRetailVector_CElement12_006f0810::PushBack_CElement12_006f0810_00765c80` with the
//   callee spelled ?InsertGrow@CRetailVector_CElement12_006f0810@@... and a local element stand-in for vector<C3DVector>.
// Shape: stlport/fable_containers.h push_back, explicitly instantiated (the landed 0x00568850 recipe). The copy constructor and
//   _M_insert_overflow are out of line in this TU: FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY / FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY.
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

template void std::vector<std::vector<C3DVector> >::push_back(const std::vector<C3DVector>&);
