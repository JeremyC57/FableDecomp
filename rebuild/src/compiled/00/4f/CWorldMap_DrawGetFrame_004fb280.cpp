// Leaf: ?DrawGetFrame@CWorldMap@@UBEJXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CWorldMap.h (class-model switch FABLE_RETAIL_CWORLDMAP; was ?DrawGetFrame@CWorldMap_Methods@@QBEJXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CWorldMap.h"  // retyped onto the PDB layout; byte parity re-verified
long CWorldMap::DrawGetFrame() const { return CWorld::GetFrame(); }