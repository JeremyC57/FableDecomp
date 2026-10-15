#include <cstdio>
#include <cstdlib>
// Behaviour test for CBaseObject's deleting destructor @ 0x0099A4D0: the intelligent-pointer record is detached, and freed only when unreferenced
class CBaseObject;
static int g_recordsFreed = 0;
struct CBaseObjectPointer {
    CBaseObject* object;
    unsigned long refcount;
    static void operator delete(void* p) { ++g_recordsFreed; std::free(p); }
    static void* operator new(size_t n) { return std::malloc(n); }
};
class CBaseClass { public: virtual ~CBaseClass() {} };
class CBaseObject : public CBaseClass {
public:
    CBaseObjectPointer* IntelligentPointer;
    CBaseObject() : IntelligentPointer(0) {}
    virtual ~CBaseObject()
    {
        if (IntelligentPointer) {
            IntelligentPointer->object = 0;
            if (IntelligentPointer->refcount == 0)
                delete IntelligentPointer;
            IntelligentPointer = 0;
        }
    }
};
int main()
{
    CBaseObjectPointer* shared = new CBaseObjectPointer; shared->refcount = 2;
    CBaseObject* a = new CBaseObject; a->IntelligentPointer = shared; shared->object = a;
    delete a;
    if (shared->object != 0 || g_recordsFreed != 0) { std::printf("0099a4d0_TEST FAIL\n"); return 1; }
    CBaseObjectPointer* lone = new CBaseObjectPointer; lone->refcount = 0;
    CBaseObject* b = new CBaseObject; b->IntelligentPointer = lone; lone->object = b;
    delete b;
    CBaseObject* c = new CBaseObject; delete c;
    if (g_recordsFreed != 1) { std::printf("0099a4d0_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("0099a4d0_TEST PASS\n");
    return 0;
}
