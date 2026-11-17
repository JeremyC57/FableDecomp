// std::vector<CEnginePrimitiveHandle>::resize(size_type n) -- STLport 4.6.2 `resize(n, _Tp())`: build a default engine handle, erase the excess elements or fill-insert copies at the end, then destroy the temporary
// Leaf: ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00443F90, 177 bytes, thiscall, `ret 4` (+15 int3 padding).
// Identity (NF48, round 7): the one-argument STLport vector<CEnginePrimitiveHandle>::resize, as stlport/fable_containers.h's
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL note says (Codex 2026-11-09) and as the landed caller 0x0064E1BE binds it; 66 callers pass a
//   scalar count; the element is 8 bytes (`sar 3`) with RTTI vtable 0x0122F598 = CEnginePrimitiveHandle. Name descriptive-by-library
//   (both PDBs are Dinkumware builds). Was the stand-in CEnginePrimitiveHandleVector::Resize(unsigned long) with Erase / Insert stand-ins.
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the temporary), vector<CEnginePrimitiveHandle>::erase(iterator,
//   iterator) 0x00440D70, _M_fill_insert(iterator, size_type, const T&) 0x00442350, then the temporary's inline ~CEnginePrimitiveHandle
//   (NotifyReleased slot 3 when the count is 1, the counted release, Delete slot 1 at zero).
// Shape: STLport 4.6.2 `void resize(size_type n) { resize(n, _Tp()); }` over the inline two-argument resize, emitted by an explicit
//   instantiation on the shared headers.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"

template void std::vector<CEnginePrimitiveHandle>::resize(size_t);
