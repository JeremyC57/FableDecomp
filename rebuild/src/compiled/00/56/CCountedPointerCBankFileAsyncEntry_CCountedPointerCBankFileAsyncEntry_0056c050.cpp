// CCountedPointer<CUnprovenPointee_0056c050>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCUnprovenPointee_0056c050@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056C050, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T unresolved: no typed caller names the pointee (the earlier label was propagated), so a stand-in pointee class carries it (CUnproven<Kind>_<addr>, EXTRA_RULES 2026-10-28).
#include "engine/CCountedPointer.h"
class CUnprovenPointee_0056c050;
template CCountedPointer<CUnprovenPointee_0056c050>::CCountedPointer(const CCountedPointer<CUnprovenPointee_0056c050>&);
