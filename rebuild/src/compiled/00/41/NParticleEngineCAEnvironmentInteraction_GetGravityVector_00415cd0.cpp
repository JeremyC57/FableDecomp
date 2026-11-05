// NParticleEngine::CAEnvironmentInteraction::GetGravityVector() const -- returns the constant vector (0.0f, 0.0f, -9.82f)
// Leaf: ?GetGravityVector@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415CD0, 27 bytes, thiscall, hidden return pointer, `ret 4`.
// Identity (proven): slot 4 of vtable 0x0122ED10 (RTTI .?AVCAEnvironmentInteraction@NParticleEngine@@; inherited unchanged at slot 4 of CParticleEnvironmentInteraction 0x0122ED8C); returns (0, 0, -9.82) -- gravity; the class's only PDB virtual returning C3DVector with no argument is ?GetGravityVector@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@XZ (GetVectorParam takes a CCharString).
// Shape (family M018, representative 0x00415CD0): `return C3DVector(x, y, z);` -- the constructor stores the three
//   immediates through the hidden return pointer (no NRVO question: nothing is named); `this` is unused.
// Header debt: engine/CAEnvironmentInteraction.h is a generated flat layout without the virtual interface, so the class is declared here
//   with the one virtual this TU defines.
#include "engine/C3DVector.h"
namespace NParticleEngine {
class CAEnvironmentInteraction {
public:
    virtual C3DVector GetGravityVector() const;
};
C3DVector CAEnvironmentInteraction::GetGravityVector() const
{
    return C3DVector(0.0f, 0.0f, -9.82f);
}
}