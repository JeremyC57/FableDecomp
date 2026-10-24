#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int g_baseCtors = 0;
static long g_allocBytes = -1;
#include <new>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C3DBoundingSphere {                                   // 0x10
    C3DVector Centre;
    float     Radius;
    C3DBoundingSphere() : Centre(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
};
struct CMatrix3x4 { float E[12]; };                          // 0x30, no constructor

class CBaseClass {
public:
    virtual void v000();
    CBaseClass();                                            // 0x0099A2D0
};
class CEnginePrimitiveHandle : public CBaseClass {           // vtable 0x0122F598, 8 bytes
public:
    unsigned long Handle;                                    // +0x04
    virtual void v000();
    CEnginePrimitiveHandle() : Handle(0) { }
};

struct CLocalDetailPrimitiveMeshZSpriteBatch {
    struct CObject {                                         // 0x4c
        CMatrix3x4             Matrix;                       // +0x00
        float                  Scale;                        // +0x30
        C3DBoundingSphere      BoundingSphere;               // +0x34
        CEnginePrimitiveHandle PrimitiveHandle;              // +0x44
    };
};

struct CEngineLandscapeMemoryManager {
    void* AllocVirtual(long bytes, void* owner);             // 0x00BD9AF0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    template<class T> T* AllocArrayOfObjects(long count, void* owner);
};

template<class T> T* CEngineLandscapeMemoryManager::AllocArrayOfObjects(long count, void* owner)
{
    __int64 bytes = (__int64)count * sizeof(T);
    if (IsValidPosition(bytes)) {
        T* objects = (T*)AllocVirtual(count * sizeof(T), owner);
        if (objects) {
            for (long i = 0; i < count; ++i)
                new (&objects[i]) T;
        }
        return objects;
    }
    return 0;
}

template CLocalDetailPrimitiveMeshZSpriteBatch::CObject*
CEngineLandscapeMemoryManager::AllocArrayOfObjects<CLocalDetailPrimitiveMeshZSpriteBatch::CObject>(long count, void* owner);

void CBaseClass::v000() { }
CBaseClass::CBaseClass() { ++g_baseCtors; }
void CEnginePrimitiveHandle::v000() { }
void* CEngineLandscapeMemoryManager::AllocVirtual(long bytes, void* owner) { g_allocBytes = bytes; return std::malloc(bytes); }
int main()
{
    typedef CLocalDetailPrimitiveMeshZSpriteBatch::CObject CObject;
    CEngineLandscapeMemoryManager m;
    CObject* objects = m.AllocArrayOfObjects<CObject>(3, &m);
    if (objects == 0 || g_allocBytes != (long)(3 * sizeof(CObject)) || g_baseCtors != 3) { std::printf("00bfbc80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (objects[i].BoundingSphere.Radius != 0.0f || objects[i].BoundingSphere.Centre.X != 0.0f ||
            objects[i].BoundingSphere.Centre.Z != 0.0f || objects[i].PrimitiveHandle.Handle != 0) { std::printf("00bfbc80_TEST FAIL\n"); return 1; }
    g_allocBytes = -1;
    if (m.AllocArrayOfObjects<CObject>(-1, &m) != 0 || g_allocBytes != -1) { std::printf("00bfbc80_TEST FAIL\n"); return 1; }
    if (m.AllocArrayOfObjects<CObject>(0x7fffffff, &m) != 0 || g_allocBytes != -1) { std::printf("00bfbc80_TEST FAIL\n"); return 1; }
    std::printf("00bfbc80_TEST PASS\n");
    return 0;
}