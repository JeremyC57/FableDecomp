// CCountedPointer<NInventory::CPerItemData>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCPerItemData@NInventory@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C05D5, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T = NInventory::CPerItemData: caller binding (005c08e6) + PDB symbol ??0?$CCountedPointer@VCPerItemData@NInventory@@@@QAE@ABV0@@Z.
#include "engine/CCountedPointer.h"
namespace NInventory { class CPerItemData; }
template CCountedPointer<NInventory::CPerItemData>::CCountedPointer(const CCountedPointer<NInventory::CPerItemData>&);
