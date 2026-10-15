// Behaviour test for 0x005ED070 NCameraModes::CScriptModuleBase::CScriptModuleBase: the default member state.
#include <cstdio>
struct IP { void* item; IP() : item(0) {} };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(1.0f, 0.0f, 0.0f) {} };
struct CScriptModuleBase {
    IP PCamPoint; IP PThingOrigin; C3DVector CoordOrigin; CRightHandedSet CoordAxis;
    void* POwner; void* PCameraManager; bool SelfTerminate; bool ShortenOnCollision; long CutOutOverride; float FOVOverride;
    CScriptModuleBase()
        : PCamPoint(), PThingOrigin(), CoordOrigin(0.0f, 0.0f, 0.0f), CoordAxis(),
          POwner(0), PCameraManager(0), SelfTerminate(false), ShortenOnCollision(false),
          CutOutOverride(-1), FOVOverride(-1.0f) {}
};
int main()
{
    CScriptModuleBase m;
    bool ok = m.PCamPoint.item == 0 && m.PThingOrigin.item == 0 && m.CoordOrigin.X == 0.0f && m.CoordOrigin.Z == 0.0f
        && m.CoordAxis.Up.Z == 1.0f && m.CoordAxis.Up.X == 0.0f && m.CoordAxis.Forward.X == 1.0f && m.CoordAxis.Forward.Z == 0.0f
        && m.POwner == 0 && m.PCameraManager == 0 && !m.SelfTerminate && !m.ShortenOnCollision
        && m.CutOutOverride == -1 && m.FOVOverride == -1.0f;
    if (!ok) { std::printf("005ed070_TEST FAIL\n"); return 1; }
    std::printf("005ed070_TEST PASS\n");
    return 0;
}
