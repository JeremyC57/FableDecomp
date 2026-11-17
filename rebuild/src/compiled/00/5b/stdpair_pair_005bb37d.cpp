// std::pair<long const, CEnginePrimitiveHandle>::pair(const pair&) -- STLport 4.6 _pair.h copy constructor
// Leaf: ??0?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB37D, 29 bytes (speed TU): the first member copied inline, then CEnginePrimitiveHandle(const CEnginePrimitiveHandle&) 0x0043F450 (engine/CEnginePrimitiveHandle.h COPY_DECL); caller 0x005BBBA5 (NF61 _M_create_node).
// NF64 (round 7, q10679): an explicit instantiation of the stlport/fable_containers.h pair constructor (STLport 4.6 _pair.h); the landed
//   caller binds this spelling (see the entry). Was the generated stand-in `SPairLongCSecond_0043f450::SPairLongCSecond_0043f450`. Descriptive-by-library (retail is STLport 4.6; both
//   PDBs are Dinkumware builds).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_COPY_DECL
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitiveHandle.h"
template std::pair<long const, CEnginePrimitiveHandle>::pair(const std::pair<long const, CEnginePrimitiveHandle>&);
