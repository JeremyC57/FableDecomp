// Behaviour test for CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~CObject (0x00BFBBF0): destroying the object destroys its
// PrimitiveHandle: the primitive is notified when this handle holds its last reference, the reference is released (the
// primitive deletes itself at zero) and the pointer cleared.
#include <cstdio>
static int g_notified = 0, g_deleted = 0;
class CEnginePrimitiveHandle;
class CPrimitive {
public:
    CPrimitive() : RefCount(0) {}
    virtual ~CPrimitive() {}
    virtual void Delete() const { ++g_deleted; }
    virtual void NotifyReleased(const CEnginePrimitiveHandle&) { ++g_notified; }
    unsigned long RefCount;
};
class CEnginePrimitiveHandle {
public:
    CEnginePrimitiveHandle() : Object(0) {}
    CPrimitive* Object;
    virtual ~CEnginePrimitiveHandle() { Clear(); Release(); }
    unsigned long GetRefCount() const { return Object ? Object->RefCount : 0; }
    void Release()
    {
        CPrimitive* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    void Clear()
    {
        if (GetRefCount() == 1)
            Object->NotifyReleased(*this);
        Release();
        Object = 0;
    }
};
struct CObject { float Matrix[12]; float Scale; float Sphere[4]; CEnginePrimitiveHandle PrimitiveHandle; };
int main()
{
    CPrimitive shared, last;
    shared.RefCount = 2; last.RefCount = 1;
    {
        CObject a; a.PrimitiveHandle.Object = &shared;
        CObject b; b.PrimitiveHandle.Object = &last;
        CObject c;
    }
    if (g_notified != 1 || g_deleted != 1 || shared.RefCount != 1 || last.RefCount != 0) {
        std::printf("00bfbbf0_TEST FAIL\n"); return 1;
    }
    std::printf("00bfbbf0_TEST PASS\n");
    return 0;
}