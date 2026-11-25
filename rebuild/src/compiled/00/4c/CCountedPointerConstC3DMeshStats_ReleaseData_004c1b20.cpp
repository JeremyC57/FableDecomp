// CCountedPointer<const C3DMeshStats>::ReleaseData() -- release one counted pointer
// Leaf: ?ReleaseData@?$CCountedPointer@$$CBVC3DMeshStats@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C1B20, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the header's FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA body, as the landed
// ReleaseData rows 0x00403750 / 0x004190B6). Retail emits this body under both ~CCountedPointer<T> and ReleaseData<T>.
// Name: best guess (owner rule 2026-11-25; one source, listed in rebuild/oracles/descriptive_names.tsv). T from T2: retail TU order v the FableWin.pdb module's pointee types (TU tc_graphic_appearance.obj (EQ: 2 copies v 1 FableWin types, 1 named anchors, 1 alignment(s))). Member: ReleaseData (positional in the TU alignment (two copies of T, ReleaseData first)).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class C3DMeshStats;
template void CCountedPointer<const C3DMeshStats>::ReleaseData();
