// CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA6A0, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T = NCreatureMode::CModeSitBase: caller binding (008ae2a0) + PDB symbol ??0?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@ABV0@@Z.
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeSitBase; }
template CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(const CCountedPointer<NCreatureMode::CModeSitBase>&);
