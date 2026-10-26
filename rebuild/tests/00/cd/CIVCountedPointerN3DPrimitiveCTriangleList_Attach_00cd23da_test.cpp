// Behaviour test for CIVCountedPointer<T>::Attach (0x00CD23DA): release the old object (delete at zero), take the new one without a reference.
#include <cstdio>
static int g_deleted = 0;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() {}
    virtual void DeleteThis() { ++g_deleted; }
    unsigned long IVRefCount;
    void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
class CTriangleList : public CIVCountedPointeeBase { };
template <class T> class CIVCountedPointer {
public:
    T* Object;
    void Attach(T* p) { if (Object) { Object->Release(); Object = 0; } Object = p; }
};
int main()
{
    CTriangleList a, b; a.IVRefCount = 2; b.IVRefCount = 1;
    CIVCountedPointer<CTriangleList> p; p.Object = 0;
    p.Attach(&a);
    if (p.Object != &a || a.IVRefCount != 2 || g_deleted) { std::printf("00cd23da_TEST FAIL\n"); return 1; }
    p.Attach(&b);
    if (p.Object != &b || a.IVRefCount != 1 || g_deleted || b.IVRefCount != 1) { std::printf("00cd23da_TEST FAIL\n"); return 1; }
    p.Attach(0);
    if (p.Object != 0 || b.IVRefCount != 0 || g_deleted != 1) { std::printf("00cd23da_TEST FAIL\n"); return 1; }
    std::printf("00cd23da_TEST PASS\n");
    return 0;
}