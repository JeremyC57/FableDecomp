// Behaviour test for 0099a3c0 (CBaseObject::ClearIntelligentPointers): the shared back-pointer record forgets the object; the record is deleted only when no intelligent pointer still counts it; the object's link is cleared either way.
#include <cstdio>
#include <new>
static int g_deletes = 0;
class CBaseObject;
struct CBaseObjectPointer {
    CBaseObject*  object;
    unsigned long refcount;
    static void operator delete(void* p) { ++g_deletes; ::operator delete(p); }
};
class CBaseObject {
public:
    virtual ~CBaseObject() { }
    CBaseObjectPointer* IntelligentPointer;
    CBaseObject() : IntelligentPointer(0) { }
    void ClearIntelligentPointers();
};
void CBaseObject::ClearIntelligentPointers()
{
    if (IntelligentPointer) {
        IntelligentPointer->object = 0;
        if (IntelligentPointer->refcount == 0)
            delete IntelligentPointer;
        IntelligentPointer = 0;
    }
}
int main()
{
    CBaseObject unreferenced;
    unreferenced.IntelligentPointer = new CBaseObjectPointer;
    unreferenced.IntelligentPointer->object = &unreferenced;
    unreferenced.IntelligentPointer->refcount = 0;
    unreferenced.ClearIntelligentPointers();
    if (g_deletes != 1 || unreferenced.IntelligentPointer != 0) { std::printf("0099a3c0_TEST FAIL\n"); return 1; }
    CBaseObject referenced;
    CBaseObjectPointer* shared = new CBaseObjectPointer;
    shared->object = &referenced;
    shared->refcount = 2;
    referenced.IntelligentPointer = shared;
    referenced.ClearIntelligentPointers();
    if (g_deletes != 1 || referenced.IntelligentPointer != 0 || shared->object != 0 || shared->refcount != 2) { std::printf("0099a3c0_TEST FAIL\n"); return 1; }
    delete shared;
    CBaseObject unlinked;
    unlinked.ClearIntelligentPointers();
    if (g_deletes != 2 || unlinked.IntelligentPointer != 0) { std::printf("0099a3c0_TEST FAIL\n"); return 1; }
    std::printf("0099a3c0_TEST PASS\n");
    return 0;
}