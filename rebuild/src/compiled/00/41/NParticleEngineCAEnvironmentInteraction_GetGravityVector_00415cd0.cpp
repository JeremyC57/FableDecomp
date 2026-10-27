// NParticleEngine::CAEnvironmentInteraction::GetGravityVector() const -- returns the constant vector (0.0f, 0.0f, -9.82f)
// Leaf: ?GetGravityVector@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415CD0, 27 bytes, thiscall, hidden return pointer, `ret 4`.
// Identity (proven): slot 4 of vtable 0x0122ED10 (RTTI .?AVCAEnvironmentInteraction@NParticleEngine@@; inherited unchanged at slot 4 of CParticleEnvironmentInteraction 0x0122ED8C); returns (0, 0, -9.82) -- gravity; the class's only PDB virtual returning C3DVector with no argument is ?GetGravityVector@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@XZ (GetVectorParam takes a CCharString).
// Shape (family M018, representative 0x00415CD0): `return C3DVector(x, y, z);` -- the constructor stores the three
//   immediates through the hidden return pointer (no NRVO question: nothing is named); `this` is unused.
// Header debt: engine/CAEnvironmentInteraction.h is a generated flat layout without the virtual interface, so the class is declared here
//   with the one virtual this TU defines.
class C3DVector {                                         // Ego_r.pdb: X, Y, Z (12 bytes)
public:
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float X;
    float Y;
    float Z;
};
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