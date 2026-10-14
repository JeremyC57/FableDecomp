#include <cstdio>
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags) { }
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
    static int g_deleted;
    CIVCountedPointeeBase() : IVRefCount(0) { }
};
int CIVCountedPointeeBase::g_deleted = 0;

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
static int g_cleared = 0;
struct CIVCountedHandle { virtual void v00() { } CIVCountedPointer Data;
    void ClearHandle() { ++g_cleared; }
    CIVCountedHandle& operator=(const CIVCountedHandle& o) { if (Data.Object != o.Data.Object) { ClearHandle(); Data = o.Data; } return *this; } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1;
    CIVCountedHandle x, y; x.Data.Object = &a; y.Data.Object = &b;
    x = x;  if (g_cleared != 0) { std::printf("00a69b20_TEST FAIL\n"); return 1; }
    CIVCountedHandle& r = (x = y);
    if (&r != &x || g_cleared != 1 || x.Data.Object != &b || b.IVRefCount != 2 || a.IVRefCount != 0 || CIVCountedPointeeBase::g_deleted != 1) { std::printf("00a69b20_TEST FAIL\n"); return 1; }
    std::printf("00a69b20_TEST PASS\n"); return 0;
}
