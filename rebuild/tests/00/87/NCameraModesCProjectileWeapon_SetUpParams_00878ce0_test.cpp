#include <stdio.h>
#include <math.h>
// Behaviour test for NCameraModes::CProjectileWeapon::SetUpParams (0x00878CE0): height/phi come from the def (phi in
// turns), the six deltas are cleared, and the axes are built from the observed thing's flattened, normalised facing.
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& Normalise() { float l = (float)sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; return *this; }
    void Set(float x, float y, float z) { X = x; Y = y; Z = z; }
};
#pragma pack(pop)
struct CTCPhysicsBase { C3DVector Facing; virtual C3DVector GetFacing() { return Facing; } };
struct CThingGameObject {
    unsigned char _pad00[0x60]; CTCPhysicsBase* PhysicsTC;
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
struct CCameraModeDef { unsigned char _pad00[0xa0]; float ProjectileCameraInitialPhiOffsetForCamera; float ProjectileCameraInitialHeightParamForCamera; };
template <class T> struct CDefPointer { T* Object; };
namespace NCameraModes {
#pragma pack(push, 1)
struct CBase {
    unsigned char _pad04[0x44 - 0x04];
    CDefPointer<const CCameraModeDef> PCameraModeDef;
    const CThingGameObject* Observed;
    const CThingGameObject* GetObservedThing() const { return Observed; }
};
struct CProjectileWeapon : CBase {
    unsigned char _pad48[0xa9 - 0x4c];
    C3DVector XAxis; C3DVector YAxis; C3DVector ZAxis;
    unsigned char _padcd[0xd0 - 0xcd];
    float Phi; float HeightParam; float DeltaPhi; float DeltaHeightParam; float LockDeltaPhi; float LockDeltaHeightParam;
    float DeltaLockDeltaPhi; float DeltaLockDeltaHeightParam;
    void SetUpParams();
};
#pragma pack(pop)
}
static const float s_oneOver360 = 1.0f / 360.0f;
void NCameraModes::CProjectileWeapon::SetUpParams()
{
    const CCameraModeDef* def = PCameraModeDef.Object;
    HeightParam = def->ProjectileCameraInitialHeightParamForCamera;
    Phi = def->ProjectileCameraInitialPhiOffsetForCamera * s_oneOver360;
    DeltaHeightParam = DeltaPhi = DeltaLockDeltaHeightParam = DeltaLockDeltaPhi = LockDeltaHeightParam = LockDeltaPhi = 0.0f;
    C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacing();
    facing.Z = 0.0f;
    facing.Normalise();
    ZAxis.Set(0.0f, 0.0f, 1.0f);
    YAxis = facing;
    XAxis = C3DVector(facing.Y, -facing.X, 0.0f);
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    int ok = 1;
    CCameraModeDef def; def.ProjectileCameraInitialPhiOffsetForCamera = 90.0f; def.ProjectileCameraInitialHeightParamForCamera = 2.5f;
    CTCPhysicsBase phys; phys.Facing = C3DVector(3.0f, 4.0f, 7.0f);
    CThingGameObject thing; thing.PhysicsTC = &phys;
    NCameraModes::CProjectileWeapon cam; cam.PCameraModeDef.Object = &def; cam.Observed = &thing;
    cam.DeltaPhi = cam.DeltaHeightParam = cam.LockDeltaPhi = cam.LockDeltaHeightParam = cam.DeltaLockDeltaPhi = cam.DeltaLockDeltaHeightParam = 9.0f;
    cam.SetUpParams();
    if (!Near(cam.HeightParam, 2.5f) || !Near(cam.Phi, 0.25f)) ok = 0;
    if (cam.DeltaPhi != 0 || cam.DeltaHeightParam != 0 || cam.LockDeltaPhi != 0 || cam.LockDeltaHeightParam != 0 || cam.DeltaLockDeltaPhi != 0 || cam.DeltaLockDeltaHeightParam != 0) ok = 0;
    if (!Near(cam.YAxis.X, 0.6f) || !Near(cam.YAxis.Y, 0.8f) || cam.YAxis.Z != 0.0f) ok = 0;
    if (!Near(cam.XAxis.X, 0.8f) || !Near(cam.XAxis.Y, -0.6f) || cam.XAxis.Z != 0.0f) ok = 0;
    if (cam.ZAxis.X != 0.0f || cam.ZAxis.Y != 0.0f || cam.ZAxis.Z != 1.0f) ok = 0;
    if (ok) { printf("00878ce0_TEST PASS\n"); return 0; }
    printf("00878ce0_TEST FAIL\n"); return 1;
}