// NParticleEngine::CAEnvironmentInteraction::GetVectorParam(const CCharString& name) const -- vtable slot 5: the default environment knows no vector parameters and returns the zero vector
// Leaf: ?GetVectorParam@CAEnvironmentInteraction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415CF0, 19 bytes (+13 int3 padding).
// Name: FableWin.pdb ?GetVectorParam@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@ABVCCharString@@@Z;
// slot 5 of the vtable 0x0122ED10 (rtti-vtables.tsv: slot 2 IsWindActive `mov al,1`, slot 4 GetGravityVector
// 0x00415CD0 storing (0, 0, -9.82)).
// Callees: none.
// Shape: the C3DVector result goes through the hidden pointer ([esp+4], returned in eax); the value
// constructor's three 0.0f stores share one zeroed register (`xor ecx,ecx`); `name` is unused. `ret 8`.
class CCharString;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

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