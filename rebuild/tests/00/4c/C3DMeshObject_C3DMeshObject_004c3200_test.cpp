// Test for 0x004C3200 C3DMeshObject::~C3DMeshObject: the Mesh counted pointer drops its reference (DeleteFunc + info freed at
// zero) and is cleared, then ~CBaseClass.
#include <cstdio>
static int g_deleted = 0, g_baseDtors = 0;
class CCPPointerInfo { public:
    unsigned int RefCount; void (*DeleteFunc)(void*); void* Data;
};
static void DeleteIt(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
class CBaseClass { public: virtual ~CBaseClass() { ++g_baseDtors; } };
class C3DMeshInfo;
class C3DMeshStats;
class C3DMeshObject : public CBaseClass { public:
    virtual ~C3DMeshObject();
    C3DMeshInfo* MeshInfo;
    CCountedPointer<const C3DMeshStats> Mesh;
    float PositionMatrix[12];
    bool InversePositionMatrixSetup;
    float InversePositionMatrix[12];
    float Scale;
};
C3DMeshObject::~C3DMeshObject() { }
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteIt; info->Data = 0;
    { C3DMeshObject a; a.Mesh.Data = 0; a.Mesh.Info = info; }
    if (g_deleted != 0 || info->RefCount != 1 || g_baseDtors != 1) { std::printf("004c3200_TEST FAIL\n"); return 1; }
    { C3DMeshObject b; b.Mesh.Data = 0; b.Mesh.Info = info; }
    if (g_deleted != 1 || g_baseDtors != 2) { std::printf("004c3200_TEST FAIL\n"); return 1; }
    { C3DMeshObject c; c.Mesh.Data = 0; c.Mesh.Info = 0; }
    if (g_deleted != 1 || g_baseDtors != 3) { std::printf("004c3200_TEST FAIL\n"); return 1; }
    std::printf("004c3200_TEST PASS\n");
    return 0;
}