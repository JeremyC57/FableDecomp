// std::vector<CMultiStaticMeshEntryDef>::resize(unsigned int newSize) -- STLport's one-argument vector resize: grow or shrink to newSize, filling with a default-constructed CMultiStaticMeshEntryDef
// Leaf: ?resize@?$vector@VCMultiStaticMeshEntryDef@@V?$allocator@VCMultiStaticMeshEntryDef@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDF0A, length 58 bytes.
// Identity (proven): (1) the landed caller CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef> binds this address under
//   ?resize@?$vector@VCMultiStaticMeshEntryDef@@V?$allocator@VCMultiStaticMeshEntryDef@@@std@@@std@@QAEXI@Z (reloc_bindings.tsv); (2) STLport 4.6 stl/_vector.h
//   `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`, matching the body (a default CMultiStaticMeshEntryDef temporary,
//   vtable 0x012438A4, then the two-argument resize).
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (inline CMultiStaticMeshEntryDef default ctor), std::vector<CMultiStaticMeshEntryDef>::resize(size_type, const CMultiStaticMeshEntryDef&) @ 0x004EDF44 (kept out of line in this TU:
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), CResource::~CResource @ 0x009FC550.
// Shape: a frame-pointer size TU (ebp frame, `leave`): `#pragma optimize("y", off)` + `("s", on)`; the temporary's inline
//   destructor chain leaves the CDefPointeeBase store and the root destructor call; `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMULTISTATICMESHENTRYDEF
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CMultiStaticMeshEntryDef.h"

template void std::vector<CMultiStaticMeshEntryDef>::resize(std::vector<CMultiStaticMeshEntryDef>::size_type newSize);
