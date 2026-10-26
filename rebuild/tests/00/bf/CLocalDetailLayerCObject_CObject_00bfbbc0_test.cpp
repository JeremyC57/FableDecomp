// Behaviour test for CLocalDetailLayer::CObject::CObject (0x00BFBBC0): zeroed bounding sphere, empty primitive handle.
#include <cstdio>

static int g_baseCtors = 0;

class CBaseClass { public:
    CBaseClass() { ++g_baseCtors; }
    virtual ~CBaseClass() { }
};

class CEnginePersistentPrimitiveBase;
struct CIVCountedPointer {
    CEnginePersistentPrimitiveBase* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass { public:
    CIVCountedPointer Data;
    CEnginePrimitiveHandle() { }
};

class C3DVector { public: float X, Y, Z; };

class C3DBoundingSphere { public:
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() { Center.X = 0; Center.Y = 0; Center.Z = 0; Radius = 0; }
};

namespace CLocalDetailLayer {
struct CObject {
    float                  Matrix[12];
    float                  Scale;
    C3DBoundingSphere      BoundingSphere;
    CEnginePrimitiveHandle PrimitiveHandle;
    CObject();
};
CObject::CObject() { }
}

int main()
{
    CLocalDetailLayer::CObject o;
    if (g_baseCtors != 1 || o.BoundingSphere.Center.X != 0.0f || o.BoundingSphere.Center.Y != 0.0f || o.BoundingSphere.Center.Z != 0.0f
        || o.BoundingSphere.Radius != 0.0f || o.PrimitiveHandle.Data.m_pObject != 0) {
        std::printf("00bfbbc0_TEST FAIL\n"); return 1;
    }
    std::printf("00bfbbc0_TEST PASS\n");
    return 0;
}