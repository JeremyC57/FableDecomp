// NParticleEngine::CAEnvironmentInteraction::GetVectorParam(const CCharString& name) const -- vtable slot 5: the default environment knows no vector parameters and returns the zero vector
// Leaf: ?GetVectorParam@CAEnvironmentInteraction@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415CF0, 19 bytes (+13 int3 padding).
// Name: FableWin.pdb ?GetVectorParam@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@ABVCCharString@@@Z;
// slot 5 of the vtable 0x0122ED10 (rtti-vtables.tsv: slot 2 IsWindActive `mov al,1`, slot 4 GetGravityVector
// 0x00415CD0 storing (0, 0, -9.82)).
// Callees: none.
// Shape: the C3DVector result goes through the hidden pointer ([esp+4], returned in eax); the value
// constructor's three 0.0f stores share one zeroed register (`xor ecx,ecx`); `name` is unused. `ret 8`.
class CCharString;

#include "engine/C3DVector.h"

namespace NParticleEngine {

class CAEnvironmentInteraction { public:
    virtual void v00();
    C3DVector GetVectorParam(const CCharString& name) const;
};

C3DVector CAEnvironmentInteraction::GetVectorParam(const CCharString& name) const
{
    return C3DVector(0.0f, 0.0f, 0.0f);
}

} // namespace NParticleEngine