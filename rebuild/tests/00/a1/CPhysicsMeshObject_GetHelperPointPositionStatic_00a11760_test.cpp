// Behaviour test for CPhysicsMeshObject::GetHelperPointPositionStatic (0x00A11760): the mesh-space helper point
// returned by the physics mesh (by name and index) is transformed by the object's position matrix
// (x*Right + y*Up + z*Forward + Position); the name/index pass through to the mesh untouched.
#include <cstdio>

static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class CCharString { public: void* PStringData; };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& p) : X(p.X), Y(p.Y), Z(p.Z) {}
};

static const CCharString* g_lastName = 0;
static long g_lastIndex = -99;
static int g_calls = 0;

class CPhysicsMesh {
public:
    float Base;
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const
    {
        ++g_calls; g_lastName = &name; g_lastIndex = index;
        C3DVector r; r.X = Base + (float)index; r.Y = 2.0f * (float)index; r.Z = -1.0f; return r;
    }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
};

class CMatrix3x4 {
public:
    float M[12];
};

class CPhysicsMeshObject {
public:
    void*                                 VTable;
    CIVCountedPointer<const CPhysicsMesh> Mesh;
    CMatrix3x4                            PositionMatrix;
    float                                 Scale;
    float                                 OO_Scale;
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;
};

C3DVector CPhysicsMeshObject::GetHelperPointPositionStatic(const CCharString& name, long index) const
{
    C3DVector point(Mesh->GetHelperPointPositionStatic(name, index));
    const float x = point.X;
    const float y = point.Y;
    point.X = x * PositionMatrix.M[0] + y * PositionMatrix.M[3] + point.Z * PositionMatrix.M[6] + PositionMatrix.M[9];
    point.Y = x * PositionMatrix.M[1] + y * PositionMatrix.M[4] + point.Z * PositionMatrix.M[7] + PositionMatrix.M[10];
    point.Z = x * PositionMatrix.M[2] + y * PositionMatrix.M[5] + point.Z * PositionMatrix.M[8] + PositionMatrix.M[11];
    return point;
}

int main()
{
    int ok = 1;
    if (sizeof(void*) == 4u && sizeof(CPhysicsMeshObject) != 0x40u) ok = 0;
    CPhysicsMesh mesh; mesh.Base = 1.0f;
    CPhysicsMeshObject obj; obj.VTable = 0; obj.Mesh.Object = &mesh; obj.Scale = 1.0f; obj.OO_Scale = 1.0f;
    // identity rotation, translation (10, 20, 30)
    float ident[12] = { 1, 0, 0,  0, 1, 0,  0, 0, 1,  10, 20, 30 };
    for (int i = 0; i < 12; ++i) obj.PositionMatrix.M[i] = ident[i];
    CCharString name; name.PStringData = 0;
    C3DVector p = obj.GetHelperPointPositionStatic(name, 2);   // mesh point (3, 4, -1)
    if (!(g_calls == 1 && g_lastName == &name && g_lastIndex == 2)) ok = 0;
    if (!(nearf(p.X, 13.0f) && nearf(p.Y, 24.0f) && nearf(p.Z, 29.0f))) ok = 0;
    // 90 degree turn about Z: Right=(0,1,0), Up=(-1,0,0), Forward=(0,0,2) (scaled), Position=(1,2,3)
    float rot[12] = { 0, 1, 0,  -1, 0, 0,  0, 0, 2,  1, 2, 3 };
    for (int i = 0; i < 12; ++i) obj.PositionMatrix.M[i] = rot[i];
    mesh.Base = 4.0f;
    p = obj.GetHelperPointPositionStatic(name, 1);             // mesh point (5, 2, -1)
    if (!(g_calls == 2 && g_lastIndex == 1)) ok = 0;
    // X = 5*0 + 2*-1 + -1*0 + 1 = -1 ; Y = 5*1 + 2*0 + -1*0 + 2 = 7 ; Z = 0 + 0 + -1*2 + 3 = 1
    if (!(nearf(p.X, -1.0f) && nearf(p.Y, 7.0f) && nearf(p.Z, 1.0f))) ok = 0;

    if (!ok) { std::printf("00a11760_TEST FAIL\n"); return 1; }
    std::printf("00a11760_TEST PASS\n");
    return 0;
}