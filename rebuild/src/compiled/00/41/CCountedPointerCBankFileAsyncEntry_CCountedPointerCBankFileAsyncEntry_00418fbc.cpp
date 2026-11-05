// CCountedPointer<NParticleEngine::CAEnvironmentInteraction>::CCountedPointer(const CCountedPointer&) -- copy a counted pointer
// Leaf: ??0?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00418FBC, 25 bytes. Callees: none.
//
// Copies Data and Info from the source and, when there is a count block, takes a reference on it; returns this
// (engine/CCountedPointer.h copy constructor). The catalog's CopyCtor placeholder describes the same body.
// T = NParticleEngine::CAEnvironmentInteraction: caller binding (004174f1) + PDB symbol ??0?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@QAE@ABV0@@Z.
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CAEnvironmentInteraction; }
template CCountedPointer<NParticleEngine::CAEnvironmentInteraction>::CCountedPointer(const CCountedPointer<NParticleEngine::CAEnvironmentInteraction>&);
