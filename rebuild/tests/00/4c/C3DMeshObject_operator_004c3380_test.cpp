// Behaviour test for 0x004C3380 C3DMeshObject::operator=
#include <cstdio>
#include <cstdlib>
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeleteData(void*) { ++g_deleted; }
template<class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data; CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info != 0) {
                --Info->RefCount;
                if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); }
            }
            Data = data; Info = info;
            if (info != 0) ++info->RefCount;
        }
        return *this;
    }
};
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };
class C3DMeshObject { public:
    void* MeshInfo; CCountedPointer<int> Mesh; CMatrix3x4 PositionMatrix; bool InversePositionMatrixSetup;
    CMatrix3x4 InversePositionMatrix; float Scale;
    C3DMeshObject& operator=(const C3DMeshObject& o)
    {
        MeshInfo = o.MeshInfo; Mesh = o.Mesh; PositionMatrix = o.PositionMatrix;
        InversePositionMatrixSetup = o.InversePositionMatrixSetup; InversePositionMatrix = o.InversePositionMatrix; Scale = o.Scale;
        return *this;
    }
};
int main()
{
    int a = 1, b = 2;
    CCPPointerInfo* ia = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo)); ia->RefCount = 1; ia->DeleteFunc = DeleteData; ia->Data = &a;
    CCPPointerInfo ib = { 1, DeleteData, &b };
    C3DMeshObject x, y;
    x.MeshInfo = 0; x.Mesh.Data = &a; x.Mesh.Info = ia; x.Scale = 1.0f; x.InversePositionMatrixSetup = false;
    y.MeshInfo = (void*)&y; y.Mesh.Data = &b; y.Mesh.Info = &ib; y.Scale = 3.0f; y.InversePositionMatrixSetup = true;
    y.PositionMatrix.Position.Z = 7.0f; y.InversePositionMatrix.XAxis.X = 5.0f;
    C3DMeshObject& r = (x = y);
    bool ok = &r == &x && g_deleted == 1 && x.Mesh.Data == &b && x.Mesh.Info == &ib && ib.RefCount == 2
        && x.MeshInfo == (void*)&y && x.Scale == 3.0f && x.InversePositionMatrixSetup && x.PositionMatrix.Position.Z == 7.0f
        && x.InversePositionMatrix.XAxis.X == 5.0f;
    x = y;   // same block: no change
    ok = ok && ib.RefCount == 2 && g_deleted == 1;
    if (!ok) { std::printf("004c3380_TEST FAIL\n"); return 1; }
    std::printf("004c3380_TEST PASS\n");
    return 0;
}