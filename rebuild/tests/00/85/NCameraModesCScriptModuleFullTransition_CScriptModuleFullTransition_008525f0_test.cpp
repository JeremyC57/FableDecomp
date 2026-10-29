#include <cstdio>
// Behaviour model of NCameraModes::CScriptModuleFullTransition ctor @ 0x008525F0 (copy of the source logic, stand-ins).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator/(float d) const { float inverse = 1.0f / d; return C3DVector(X * inverse, Y * inverse, Z * inverse); }
};
static long g_frame = 1000;
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
struct CScriptModuleBase { int baseBuilt; CScriptModuleBase() : baseBuilt(1) {} };
struct CScriptModuleFullTransition : CScriptModuleBase {
    C3DVector IncrementForLookAtPoints, Increment, LookAtPos, StartPos; long EndFrame;
    CScriptModuleFullTransition(const C3DVector& lookAt, const C3DVector& startPos, const C3DVector& endLookAt, const C3DVector& endPos, long frames)
        : CScriptModuleBase(), LookAtPos(lookAt), StartPos(startPos)
    {
        long count = frames ? frames : 15;
        Increment = (endPos - startPos) / (float)count;
        IncrementForLookAtPoints = (endLookAt - lookAt) / (float)count;
        EndFrame = CWorld::GetFrame() + frames;
    }
};
static bool Near(float a, float b) { return a > b - 1e-5f && a < b + 1e-5f; }
int main()
{
    C3DVector look(0, 0, 0), start(10, 20, 30), endLook(4, 8, 12), endPos(14, 28, 42);
    CScriptModuleFullTransition t(look, start, endLook, endPos, 4);
    int ok = t.baseBuilt && t.StartPos.Y == 20.0f && t.LookAtPos.X == 0.0f && t.EndFrame == 1004
          && Near(t.Increment.X, 1.0f) && Near(t.Increment.Y, 2.0f) && Near(t.Increment.Z, 3.0f)
          && Near(t.IncrementForLookAtPoints.X, 1.0f) && Near(t.IncrementForLookAtPoints.Z, 3.0f);
    CScriptModuleFullTransition u(look, start, endLook, endPos, 0);   // 0 frames: increments over 15, end frame now
    ok = ok && u.EndFrame == 1000 && Near(u.Increment.Z, 12.0f / 15.0f) && Near(u.IncrementForLookAtPoints.Y, 8.0f / 15.0f);
    if (!ok) { std::printf("008525f0_TEST FAIL\n"); return 1; }
    std::printf("008525f0_TEST PASS\n");
    return 0;
}