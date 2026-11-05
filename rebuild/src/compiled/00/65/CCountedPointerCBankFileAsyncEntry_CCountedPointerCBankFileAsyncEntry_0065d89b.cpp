// CCountedPointer<CVillageTaskHorn>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCVillageTaskHorn@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065D89B, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T = CVillageTaskHorn: caller binding (0065f96d) + PDB symbol ??0?$CCountedPointer@VCVillageTaskHorn@@@@QAE@ABV0@@Z.
#include "engine/CCountedPointer.h"
class CVillageTaskHorn;
template CCountedPointer<CVillageTaskHorn>::CCountedPointer(const CCountedPointer<CVillageTaskHorn>&);
