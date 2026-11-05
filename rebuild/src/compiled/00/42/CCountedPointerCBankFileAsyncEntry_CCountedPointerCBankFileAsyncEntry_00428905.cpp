// CCountedPointer<CMeshDataBank>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCMeshDataBank@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00428905, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T = CMeshDataBank: caller binding (0042dd28,0042e98f,005958f5) + PDB symbol ??0?$CCountedPointer@VCMeshDataBank@@@@QAE@ABV0@@Z.
#include "engine/CCountedPointer.h"
class CMeshDataBank;
template CCountedPointer<CMeshDataBank>::CCountedPointer(const CCountedPointer<CMeshDataBank>&);
