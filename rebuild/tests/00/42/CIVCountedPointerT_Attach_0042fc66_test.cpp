// behaviour test for 0x0042FC66
#include <stdio.h>
static int g_deleted = 0;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() {}
    virtual void DeleteThis() { ++g_deleted; }
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
namespace N3DPrimitive { class CTriangleList : public CIVCountedPointeeBase { }; }
template <class T> class CIVCountedPointer {
public:
    T* Object;
    void Attach(T* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
    }
};
template class CIVCountedPointer<N3DPrimitive::CTriangleList>;

int main()
{
    N3DPrimitive::CTriangleList a, b; a.IVRefCount = 1; b.IVRefCount = 3;
    CIVCountedPointer<N3DPrimitive::CTriangleList> p; p.Object = &a;
    p.Attach(&b);
    int ok = p.Object == &b && a.IVRefCount == 0 && g_deleted == 1 && b.IVRefCount == 3;
    p.Object = 0; p.Attach(&a); ok &= p.Object == &a && g_deleted == 1;
    printf(ok ? "0042fc66_TEST PASS\n" : "0042fc66_TEST FAIL\n");
    return ok ? 0 : 1;
}