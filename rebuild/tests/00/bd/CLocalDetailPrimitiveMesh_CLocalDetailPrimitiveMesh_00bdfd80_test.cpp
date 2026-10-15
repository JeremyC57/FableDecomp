// Behaviour test for 0x00BDFD80 (CLocalDetailPrimitiveMesh::~CLocalDetailPrimitiveMesh): last-handle notification and release of the handle's counted primitive.
#include <cstdio>
struct Prim {
    long RefCount; int destroyed; int lastHandle;
    virtual ~Prim() {}
    virtual void Destroy() { ++destroyed; }
    virtual void OnLastHandleReleased(void*) { ++lastHandle; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct Counted {
    Prim* Object;
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~Counted() { if (Object) { Object->Release(); Object = 0; } }
    Counted& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
};
struct Handle { Counted Data; ~Handle() { if (Data.IsUnique()) Data.Object->OnLastHandleReleased(this); Data = 0; } };
struct Mesh { Handle m_handle; };
int main()
{
    Prim a; a.RefCount = 1; a.destroyed = 0; a.lastHandle = 0;
    { Mesh m; m.m_handle.Data.Object = &a; }
    int ok = a.lastHandle == 1 && a.destroyed == 1 && a.RefCount == 0;
    Prim b; b.RefCount = 2; b.destroyed = 0; b.lastHandle = 0;
    { Mesh m; m.m_handle.Data.Object = &b; }
    ok &= b.lastHandle == 0 && b.destroyed == 0 && b.RefCount == 1;
    { Mesh m; m.m_handle.Data.Object = 0; }
    if (!ok) { std::printf("00bdfd80_TEST FAIL\n"); return 1; }
    std::printf("00bdfd80_TEST PASS\n");
    return 0;
}