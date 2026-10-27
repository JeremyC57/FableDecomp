// behaviour test for 0x008699F0 CIVCountedPointer<CUnprovenPointee_008699f0>::`scalar deleting destructor' (model copy of the
// source): deleting a wrapper releases one reference, Delete runs only when the count reaches zero, a null wrapper is
// left alone, and the wrapper storage itself is freed by the delete expression
#include <cstdio>
#include <cstdlib>
static int g_deleted = 0, g_freed = 0;
void* operator new(size_t n) { return std::malloc(n); }
void operator delete(void* p) { if (p) { ++g_freed; std::free(p); } }
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { ++g_deleted; }
    unsigned long IVRefCount;
};
class CUnprovenPointee_008699f0;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    ~CIVCountedPointer()
    {
        CIVCountedPointeeBase* p = Object;
        if (p) {
            if (--p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
};
typedef CIVCountedPointer<CUnprovenPointee_008699f0> TPtr;
int main()
{
    int fail = 0;
    CIVCountedPointeeBase a; a.IVRefCount = 2;
    TPtr* w = new TPtr; w->Object = &a;
    delete w;                                            // 2 -> 1, no Delete
    if (a.IVRefCount != 1 || g_deleted != 0 || g_freed != 1) fail |= 1;
    w = new TPtr; w->Object = &a;
    delete w;                                            // 1 -> 0, Delete
    if (a.IVRefCount != 0 || g_deleted != 1 || g_freed != 2) fail |= 2;
    w = new TPtr; w->Object = 0;
    delete w;                                            // null slot: nothing released
    if (g_deleted != 1 || g_freed != 3) fail |= 4;
    {   TPtr local; local.Object = &a; a.IVRefCount = 3; }   // plain destructor: release only
    if (a.IVRefCount != 2 || g_freed != 3) fail |= 8;
    if (fail) { std::printf("008699f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008699f0_TEST PASS\n");
    return 0;
}