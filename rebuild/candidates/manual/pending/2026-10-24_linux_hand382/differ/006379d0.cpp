// NThingMap::InitLayerInfo() -- set up the thing-map mapwho layer globals: six layers, and the largest layer cell size found in the static layer table
// Leaf: InitLayerInfo
// Grade: DIFFER(52v53) -- register swap only: retail keeps the running maximum in ecx and the table pointer in eax, the lane (RTM and QFE) the reverse (and so `mov [max],eax` is the 5-byte a3 form, one byte shorter); every instruction otherwise identical under optimize("s")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006379D0, 53 bytes (+ int3 padding), plain `ret` (no arguments, no `this`).
// Identity: the only caller is CThingMapManager::Init 0x00638540 (its first call). The globals are read back by
// CThingMapManager::GetThingMapwhoLevel 0x006378E0: a size at or above the maximum maps to the last layer (count - 1),
// anything smaller to the first table entry whose size exceeds it. No Ego_r.pdb symbol names this helper; the catalog
// label "InitLayerInfo" is kept.
// Data: the layer table @ 0x0137DAD8 (six 16-byte records: cell size 2, 4, 8, 32, 64, -1; an index; two debug colours),
// the layer count @ 0x013BA848 and the maximum cell size @ 0x013BA84C.
// Pragma: #pragma optimize("s", on) -- retail's loop head is unaligned (a speed TU pads it) and the speed build fully
// unrolls the six-pass loop (DIFFER 189v53); the size build keeps it rolled like retail.
// Shape: `count = 6; max = 0; for (i = 0; i < 6; ++i) max = std::max(table[i].Size, max);` -- STLport's
// reference-returning max (`a < b ? b : a`) selects an ADDRESS (`mov ecx,offset max ; jl ; mov ecx,eax ; mov ecx,[ecx]`)
// and the global is stored every pass; the int index is strength-reduced to a pointer walk that keeps the signed `jl`.
#pragma optimize("s", on)
namespace std {
template <class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
}

namespace NThingMap {

struct SMapwhoLayerInfo {                       // 16 bytes
    long          Size;                         // +0x00 cell size of the layer
    long          Index;                        // +0x04
    unsigned long Colour;                       // +0x08 debug colours
    unsigned long DebugColour;                  // +0x0c
};

extern SMapwhoLayerInfo g_MapwhoLayerInfo[6];   // 0x0137DAD8
extern long g_NoMapwhoLayers;                   // 0x013BA848
extern long g_MaxMapwhoLayerSize;               // 0x013BA84C

void InitLayerInfo()
{
    g_NoMapwhoLayers = 6;
    g_MaxMapwhoLayerSize = 0;
    for (int i = 0; i < 6; ++i)
        g_MaxMapwhoLayerSize = std::max(g_MapwhoLayerInfo[i].Size, g_MaxMapwhoLayerSize);
}

}
