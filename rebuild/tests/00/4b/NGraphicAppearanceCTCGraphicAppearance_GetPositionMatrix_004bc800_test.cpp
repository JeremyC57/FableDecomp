// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix 0x004BC800: a default mesh object is placed
// at the owner's render position (at the interpolation) with the physics orientation at the interpolation and the scale
// RenderSizeX * Scale * 0.01, and its position matrix is returned by value; the local mesh object is destroyed.
#include <cstdio>
#include <cmath>

struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CMatrix3x4 { float E[12]; };

static int gLive = 0;
static float gLastInterpolationPos = -1.0f, gLastInterpolationRH = -1.0f;

struct C3DMeshObject {
    CMatrix3x4 PositionMatrix;
    bool InversePositionMatrixSetup;
    C3DMeshObject() : InversePositionMatrixSetup(false) { ++gLive; for (int i = 0; i < 12; ++i) PositionMatrix.E[i] = -99.0f; }
    ~C3DMeshObject() { --gLive; }
    void SetPositionMatrix(const C3DVector& p, const CRightHandedSet& o, float s)
    {   // retail 0x00A357B0: right = Forward ^ Up; rows scaled; translation = position
        C3DVector r(o.Forward.Y * o.Up.Z - o.Forward.Z * o.Up.Y, o.Forward.Z * o.Up.X - o.Forward.X * o.Up.Z,
                    o.Forward.X * o.Up.Y - o.Forward.Y * o.Up.X);
        float* e = PositionMatrix.E;
        e[0] = r.X * s; e[1] = r.Y * s; e[2] = r.Z * s;
        e[3] = o.Forward.X * s; e[4] = o.Forward.Y * s; e[5] = o.Forward.Z * s;
        e[6] = o.Up.X * s; e[7] = o.Up.Y * s; e[8] = o.Up.Z * s;
        e[9] = p.X; e[10] = p.Y; e[11] = p.Z;
        InversePositionMatrixSetup = false;
    }
};

struct CTCPhysicsBase {
    CRightHandedSet Set;
    virtual CRightHandedSet GetRHSet(float t) const { gLastInterpolationRH = t; return Set; }
    virtual ~CTCPhysicsBase() {}
};
struct CThing { C3DVector Pos; CTCPhysicsBase* PhysicsTC; const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; } };
static C3DVector GetRenderPositionForThing(const CThing& thing, float t) { gLastInterpolationPos = t; return C3DVector(thing.Pos.X, thing.Pos.Y, thing.Pos.Z + t); }
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; };

struct CTCGraphicAppearance {
    CThing* ParentThing;
    float Scale;
    CEngineGraphic MainGraphic;
    CMatrix3x4 GetPositionMatrix(float interpolation) const
    {
        const CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
        C3DMeshObject meshObject;
        const CThing& thing = *ParentThing;
        meshObject.SetPositionMatrix(GetRenderPositionForThing(thing, interpolation),
                                     physics->GetRHSet(interpolation),
                                     (MainGraphic.RenderSizeX * Scale) * 0.01f);
        return meshObject.PositionMatrix;
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    bool ok = true;
    CTCPhysicsBase physics;
    physics.Set.Up = C3DVector(0.0f, 0.0f, 1.0f);
    physics.Set.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    CThing thing; thing.Pos = C3DVector(10.0f, 20.0f, 30.0f); thing.PhysicsTC = &physics;
    CTCGraphicAppearance app; app.ParentThing = &thing; app.Scale = 2.0f;
    app.MainGraphic.BankIndex = 7; app.MainGraphic.AnimStep = 0.0f; app.MainGraphic.RenderSizeX = 150.0f;
    CMatrix3x4 m = app.GetPositionMatrix(0.25f);
    // scale = 150 * 2 * 0.01 = 3; right = Forward ^ Up = (1,0,0)
    const float want[12] = { 3, 0, 0,  0, 3, 0,  0, 0, 3,  10, 20, 30.25f };
    for (int i = 0; i < 12; ++i) if (!Near(m.E[i], want[i])) ok = false;
    if (!Near(gLastInterpolationPos, 0.25f) || !Near(gLastInterpolationRH, 0.25f)) ok = false;
    if (gLive != 0) ok = false;   // the local mesh object is destroyed
    // a different interpolation / scale
    app.Scale = 0.5f; app.MainGraphic.RenderSizeX = 100.0f;
    m = app.GetPositionMatrix(1.0f);
    if (!Near(m.E[0], 0.5f) || !Near(m.E[4], 0.5f) || !Near(m.E[8], 0.5f) || !Near(m.E[11], 31.0f)) ok = false;
    if (!ok) { std::printf("004bc800_TEST FAIL\n"); return 1; }
    std::printf("004bc800_TEST PASS\n");
    return 0;
}