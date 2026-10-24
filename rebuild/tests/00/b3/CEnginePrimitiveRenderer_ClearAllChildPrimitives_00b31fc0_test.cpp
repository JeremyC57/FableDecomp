#include <cstdio>
// Behaviour test for CEnginePrimitiveRenderer::ClearAllChildPrimitives(CEnginePrimitiveHandle&) @ 0x00B31FC0: forward to
// the held primitive's ClearAllChildPrimitives (vtable slot 11), nothing for an empty handle.
static int g_cleared = 0;
static int g_other = 0;
class CEnginePrimitive;
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000() { ++g_other; }
    virtual void v001() { ++g_other; }
    virtual void v002() { ++g_other; }
    virtual void v003() { ++g_other; }
    virtual void v004() { ++g_other; }
    virtual void v005() { ++g_other; }
    virtual void v006() { ++g_other; }
    virtual void SetChildPrimitive(unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { ++g_other; }
    virtual void SetChildPrimitive(unsigned long, const CEnginePrimitive&, unsigned long) { ++g_other; }
    virtual void GetChildPrimitiveHandle(unsigned long, CEnginePrimitiveHandle&) { ++g_other; }
    virtual void ClearChildPrimitive(unsigned long) { ++g_other; }
    virtual void ClearAllChildPrimitives() { ++g_cleared; }
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
class CEnginePrimitiveHandle {
public:
    virtual void v000() { }
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
class CEnginePrimitiveRenderer {
public:
    void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
    {
        if (handle.Data)
            handle.Data->ClearAllChildPrimitives();
    }
};
int main()
{
    CEnginePrimitiveRenderer renderer;
    CEnginePersistentPrimitiveBase primitive;
    CEnginePrimitiveHandle handle;
    handle.Data.Object = 0;
    renderer.ClearAllChildPrimitives(handle);
    if (g_cleared != 0 || g_other != 0) { std::printf("00b31fc0_TEST FAIL\n"); return 1; }
    handle.Data.Object = &primitive;
    renderer.ClearAllChildPrimitives(handle);
    renderer.ClearAllChildPrimitives(handle);
    if (g_cleared != 2 || g_other != 0) { std::printf("00b31fc0_TEST FAIL\n"); return 1; }
    std::printf("00b31fc0_TEST PASS\n");
    return 0;
}