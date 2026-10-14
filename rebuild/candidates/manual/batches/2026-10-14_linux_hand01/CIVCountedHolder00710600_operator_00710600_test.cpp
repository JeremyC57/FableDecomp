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
struct CIVCountedHolder { virtual void v00() { } CIVCountedPointer Data;
    CIVCountedHolder& operator=(const CIVCountedHolder& o) { Data = o.Data; return *this; } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1;
    CIVCountedHolder x, y, z; x.Data.Object = &a; y.Data.Object = &b;
    CIVCountedHolder& r = (x = y);
    if (&r != &x || x.Data.Object != &b || b.IVRefCount != 2 || a.IVRefCount != 0 || CIVCountedPointeeBase::g_deleted != 1) { std::printf("00710600_TEST FAIL\n"); return 1; }
    x = y;   // same object: no change
    if (b.IVRefCount != 2) { std::printf("00710600_TEST FAIL\n"); return 1; }
    z = x; if (z.Data.Object != &b || b.IVRefCount != 3) { std::printf("00710600_TEST FAIL\n"); return 1; }
    std::printf("00710600_TEST PASS\n"); return 0;
}
