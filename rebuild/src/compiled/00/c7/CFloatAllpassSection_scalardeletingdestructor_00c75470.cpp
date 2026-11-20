// CFloatAllpassSection::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor expanded (vptr, buffer pointer and length zeroed), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCFloatAllpassSection@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75470, 39 bytes, thiscall, `ret 4`. Callee: operator delete 0x00BFE9BC.
// Name (NF109, q11607 family; was the descriptive ??_GCDelayLine over a local model): GameCODA 2.10 CodeView class CFloatAllpassSection.
//   Two sources: (1) this body is byte-identical (39 bytes, relocations masked) to the CODAApi.lib release ??_GCFloatAllpassSection@@UAEPAXI@Z
//   (work/tools/sdkfind.py, one hit); (2) it is the only slot of the vtable 0x012BF074 that the byte-identical SDK
//   ??0CFloatAllpassSection 0x00C74F60 / ??1CFloatAllpassSection 0x00C75400 store (relocation ??_7CFloatAllpassSection@@6B@).
// Shape: the dtor is defined in this TU (the CODAApiDbg.lib body: Release), so `??_G` expands it; the vtable comes with the dtor.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE
#include "engine/CFloatAllpassSection.h"

CFloatAllpassSection::~CFloatAllpassSection()
{
    Release();
}
