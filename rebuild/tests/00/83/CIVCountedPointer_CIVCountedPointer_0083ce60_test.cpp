// Behaviour test for 0083ce60 CIVCountedPointer copy constructor: copies the pointee and adds a reference when non-null.
#include <cstdio>
class CIVCountedPointeeBase { public:
    int destroyed;
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : destroyed(0), IVRefCount(0) {}
    virtual void Delete() { ++destroyed; }
    virtual ~CIVCountedPointeeBase() {}
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(CIVCountedPointeeBase* p) : Object(p) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
};
int main()
{
    CIVCountedPointeeBase obj; obj.IVRefCount = 1;
    CIVCountedPointer a(&obj);
    CIVCountedPointer b(a);
    CIVCountedPointer c(b);
    CIVCountedPointer n(0);
    CIVCountedPointer m(n);
    if (b.Object != &obj || c.Object != &obj || obj.IVRefCount != 3 || m.Object != 0 || obj.destroyed) { std::printf("0083ce60_TEST FAIL\n"); return 1; }
    std::printf("0083ce60_TEST PASS\n");
    return 0;
}