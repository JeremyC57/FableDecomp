// std::vector<CShopItemDef>::resize(unsigned int newSize) -- STLport's one-argument vector resize: grow or shrink to newSize, filling with a default-constructed CShopItemDef
// Leaf: ?resize@?$vector@VCShopItemDef@@V?$allocator@VCShopItemDef@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ED04D, length 48 bytes.
// Identity (proven): (1) the landed caller CPersistContext::TransferVectorOfSubComponents<CShopItemDef> 0x004ECF6E binds this address
//   under ?resize@?$vector@VCShopItemDef@@V?$allocator@VCShopItemDef@@@std@@@std@@QAEXI@Z (reloc_bindings.tsv); (2) STLport 4.6
//   stl/_vector.h `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`, matching the body (a 0x48-byte default
//   CShopItemDef temporary, vtable 0x0123B644, then the two-argument resize).
// Callees: CShopItemDef::CShopItemDef() @ 0x004D405A, std::vector<CShopItemDef>::resize(size_type, const CShopItemDef&) @ 0x004ED07D
//   (kept out of line in this TU: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), the def root destructor @ 0x009FC550.
// Shape: a frame-pointer size TU (`sub esp,48h`, `push dword ptr [ebp+8]`, `leave`): `#pragma optimize("y", off)` +
// `("s", on)`; the temporary's inline destructor chain leaves the CDefPointeeBase store and the root destructor call; `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSHOPITEMDEF
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CShopItemDef.h"

template void std::vector<CShopItemDef>::resize(std::vector<CShopItemDef>::size_type newSize);
