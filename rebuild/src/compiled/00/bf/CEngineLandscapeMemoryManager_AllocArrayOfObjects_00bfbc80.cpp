// CEngineLandscapeMemoryManager::AllocArrayOfObjects(long count, void* owner) -- [T = CLocalDetailPrimitiveMeshZSpriteBatch::CObject] allocate a count-element array of z-sprite batch objects from the landscape virtual memory (refusing a byte size outside 0..0x7fffffff) and default-construct every element
// Leaf: ?AllocArrayOfObjects@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFBC80, length 145 bytes (+ int3 padding), `ret 8`; called out of line by the landed
// CLocalDetailPrimitiveMeshZSpriteBatch::Load 0x00BFB210 (which models it as this member template).
// Callees: __allmul @ 0x00BFEB50 (the 64-bit `count * 0x4c` size check), CEngineLandscapeMemoryManager::AllocVirtual(long, void*)
// @ 0x00BD9AF0, CBaseClass::CBaseClass @ 0x0099A2D0 (the primitive handle's base; manifest "CBase").
// Layout: Ego_r.pdb CLocalDetailPrimitiveMeshZSpriteBatch::CObject (0x4c): Matrix +0, Scale +0x30, BoundingSphere +0x34 (C3DVector
// Centre + Radius, zeroed by its inline constructor), PrimitiveHandle +0x44 (CEnginePrimitiveHandle: vtable 0x0122F598 + handle).
// Shape: the inline IsValidPosition test (`p >= 0 && p <= 0x7fffffff` on the __int64 product), the allocation result homed in
// the dead `count` argument slot, then a placement-new loop (`new (p) T` keeps its null test) counting down in ebp with the
// element pointer advanced by 0x4c; members are built in declaration order (bounding-sphere zeroes, handle base call, vtable, handle 0).
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