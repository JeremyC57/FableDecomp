#include <stdio.h>
// Behaviour test for NCameraModes::CScriptModuleTransitionBetweenPoints::Update (0x00853580): before EndFrame the camera
// position steps by Increment and the look-at tracks the live thing's focal position (or drifts by LookAtIncrement);
// the camera then takes the look-at and the FOV.
static long g_frame = 0;
long __fastcall GFGetCurrentFrame() { return g_frame; }
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X + other.X; result.Y = Y + other.Y; result.Z = Z + other.Z;
        return result;
    }
    bool Is(float x, float y, float z) const { return X == x && Y == y && Z == z; }
};
class CTCGraphicAppearance { public: C3DVector Focal; C3DVector GetCameraFocalPos(float height) { return C3DVector(Focal.X, Focal.Y, Focal.Z + height); } };
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;
    unsigned char         _pad68[0x29];
    unsigned char         m_dead : 1;
    unsigned char         m_91_1 : 7;
    CTCGraphicAppearance* GetPTCGraphicAppearance() const { return PTCGraphicAppearance; }
};
struct CIntelligentPointerThing { void* __vftable; void* m_pItem; CThing* GetPItem() { return (CThing*)m_pItem; } };
struct CCameraState {
    unsigned char _pad00[0x10]; C3DVector LookAt; unsigned char _pad1c[0x38]; C3DVector Position; unsigned char _pad60[0x24]; float FOV;
};
namespace NCameraModes {
class CScriptModuleTransitionBetweenPoints {
public:
    void* __vftable; unsigned char _pad04[0x34]; CCameraState* POwner; unsigned char _pad3c[0x10];
    CIntelligentPointerThing PThing; C3DVector Increment; C3DVector LookAtIncrement; C3DVector LookAtPos; C3DVector StartPos;
    long EndFrame; float FOV; bool PreventSelfTermination;
    void Update();
};
void CScriptModuleTransitionBetweenPoints::Update()
{
    if (GFGetCurrentFrame() < EndFrame) {
        POwner->Position = POwner->Position + Increment;
        if (PThing.GetPItem() && !PThing.GetPItem()->m_dead) {
            C3DVector focal = PThing.GetPItem()->GetPTCGraphicAppearance()->GetCameraFocalPos(1.0f);
            LookAtPos = focal;
        }
        else {
            LookAtPos.X = LookAtIncrement.X + LookAtPos.X;
            LookAtPos.Y = LookAtIncrement.Y + LookAtPos.Y;
            LookAtPos.Z = LookAtIncrement.Z + LookAtPos.Z;
        }
    }
    POwner->LookAt = LookAtPos;
    POwner->FOV = FOV;
}
}
int main()
{
    int ok = 1;
    CCameraState cam; cam.Position = C3DVector(1, 2, 3); cam.LookAt = C3DVector(0, 0, 0); cam.FOV = 0;
    CTCGraphicAppearance app; app.Focal = C3DVector(10, 20, 30);
    CThing thing; thing.PTCGraphicAppearance = &app; thing.m_dead = 0;
    NCameraModes::CScriptModuleTransitionBetweenPoints m;
    m.POwner = &cam; m.PThing.m_pItem = 0; m.Increment = C3DVector(1, 1, 1); m.LookAtIncrement = C3DVector(0.5f, 0.25f, 2);
    m.LookAtPos = C3DVector(5, 6, 7); m.EndFrame = 10; m.FOV = 60.0f;
    g_frame = 3; m.Update();   // no thing: drift
    if (!cam.Position.Is(2, 3, 4) || !m.LookAtPos.Is(5.5f, 6.25f, 9) || !cam.LookAt.Is(5.5f, 6.25f, 9) || cam.FOV != 60.0f) ok = 0;
    m.PThing.m_pItem = &thing; thing.m_dead = 1; m.Update();   // dead thing: drift
    if (!cam.Position.Is(3, 4, 5) || !m.LookAtPos.Is(6, 6.5f, 11)) ok = 0;
    thing.m_dead = 0; m.Update();   // live thing: focal position + 1
    if (!cam.Position.Is(4, 5, 6) || !m.LookAtPos.Is(10, 20, 31) || !cam.LookAt.Is(10, 20, 31)) ok = 0;
    g_frame = 10; m.FOV = 45.0f; m.Update();   // at EndFrame: only aim + FOV
    if (!cam.Position.Is(4, 5, 6) || !m.LookAtPos.Is(10, 20, 31) || !cam.LookAt.Is(10, 20, 31) || cam.FOV != 45.0f) ok = 0;
    if (ok) { printf("00853580_TEST PASS\n"); return 0; }
    printf("00853580_TEST FAIL\n"); return 1;
}