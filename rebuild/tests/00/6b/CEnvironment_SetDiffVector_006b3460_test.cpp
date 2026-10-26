// Behaviour test for 006b3460: CEnvironment::SetDiffVector normalises (x, y, z) from the console args into both game-tick states (model copy).
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise() { float inv = 1.0f / std::sqrt(X * X + Y * Y + Z * Z); X *= inv; Y *= inv; Z *= inv; }   // 0x00A14440
};
class CGenericVar { public: void* vt; long Type; void* ValString; long ValSLONG; unsigned long ValULONG; bool ValBool; float ValFloat; };
struct CVarVector { CGenericVar* first; CGenericVar* last; CGenericVar* end; const CGenericVar& operator[](unsigned int n) const { return first[n]; } };
struct CState { C3DVector DiffuseVector; float other; };
class CEnvironment { public:
    CState GameTickState, PrevGameTickState;
    static void SetDiffVector(CEnvironment& env, const CVarVector& args)
    {
        C3DVector diffuse(args[0].ValFloat, args[1].ValFloat, args[2].ValFloat);
        diffuse.Normalise();
        env.GameTickState.DiffuseVector = diffuse;
        env.PrevGameTickState.DiffuseVector = diffuse;
    }
};
int main()
{
    CGenericVar v[3] = {};
    v[0].ValFloat = 3.0f; v[1].ValFloat = 0.0f; v[2].ValFloat = -4.0f;
    CVarVector args = { v, v + 3, v + 3 };
    CEnvironment env; env.GameTickState.other = 7.0f; env.PrevGameTickState.other = 8.0f;
    CEnvironment::SetDiffVector(env, args);
    int fail = 0;
    const C3DVector& a = env.GameTickState.DiffuseVector; const C3DVector& b = env.PrevGameTickState.DiffuseVector;
    if (std::fabs(a.X - 0.6f) > 1e-6f || a.Y != 0.0f || std::fabs(a.Z + 0.8f) > 1e-6f) fail = 1;
    if (b.X != a.X || b.Y != a.Y || b.Z != a.Z) fail = 1;
    if (env.GameTickState.other != 7.0f || env.PrevGameTickState.other != 8.0f) fail = 1;
    if (fail) { std::printf("006b3460_TEST FAIL\n"); return 1; }
    std::printf("006b3460_TEST PASS\n");
    return 0;
}