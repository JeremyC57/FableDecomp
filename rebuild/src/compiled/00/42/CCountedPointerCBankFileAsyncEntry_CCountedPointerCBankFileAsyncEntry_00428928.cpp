// CCountedPointer<NUISystem::CComponent>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00428928, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T = NUISystem::CComponent: caller binding (00429f9c,0042a961,0053b321,005820e7,00582673,005b358c,005b3613,005c323a,005c3444) + PDB symbol ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@ABV0@@Z.
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
template CCountedPointer<NUISystem::CComponent>::CCountedPointer(const CCountedPointer<NUISystem::CComponent>&);
