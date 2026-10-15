// Behaviour test for 0x00852400 NCameraModes::CScriptModuleCameraOnAStick::Update.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { C3DVector r; r.X = X + o.X; r.Y = Y + o.Y; r.Z = Z + o.Z; return r; }
};
struct CThing { C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CCameraState { C3DVector LookAt; C3DVector Position; };
struct Module {
    CCameraState* POwner; CThing* PThing; C3DVector LocalRel; C3DVector RelVector;
    void Update()
    {
        const C3DVector& pos = PThing->GetPosition();
        C3DVector lookAt;
        lookAt.X = pos.X + LocalRel.X; lookAt.Y = pos.Y + LocalRel.Y; lookAt.Z = pos.Z + LocalRel.Z;
        POwner->Position = RelVector + lookAt;
        POwner->LookAt = lookAt;
    }
};
int main()
{
    CThing t; t.pos = C3DVector(1, 2, 3);
    CCameraState cam; Module m; m.POwner = &cam; m.PThing = &t; m.LocalRel = C3DVector(0, 0, 2); m.RelVector = C3DVector(4, 0, 1);
    m.Update();
    if (cam.LookAt.X != 1 || cam.LookAt.Y != 2 || cam.LookAt.Z != 5 || cam.Position.X != 5 || cam.Position.Y != 2 || cam.Position.Z != 6) {
        std::printf("00852400_TEST FAIL\n"); return 1;
    }
    std::printf("00852400_TEST PASS\n");
    return 0;
}
