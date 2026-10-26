#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00415cf0.cpp) ----
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

// ---- stubs for the external callees ----
void NParticleEngine::CAEnvironmentInteraction::v00() {}

static void run()
{

    int nameStorage = 0;
    const CCharString& name = *reinterpret_cast<const CCharString*>(&nameStorage);
    NParticleEngine::CAEnvironmentInteraction interaction;
    C3DVector v = interaction.GetVectorParam(name);
    CHECK(v.X == 0.0f && v.Y == 0.0f && v.Z == 0.0f);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00415cf0_TEST FAIL\n"); return 1; }
    std::printf("00415cf0_TEST PASS\n");
    return 0;
}