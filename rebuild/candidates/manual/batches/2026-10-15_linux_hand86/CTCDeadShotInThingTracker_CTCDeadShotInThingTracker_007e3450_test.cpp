// Behaviour test for CTCDeadShotInThingTracker::CTCDeadShotInThingTracker(CThing*) @ 0x007E3450: the pointer member is
// built with SetPItem(0) and HasThingStuckIn is false.
#include <cstdio>
#include <new>
struct CThing { int id; };
struct CBaseObject { int id; };
static CThing* g_baseParent = 0;
static int g_setCalls = 0;
static CBaseObject* g_setArg = (CBaseObject*)1;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    CBaseIntelligentPointer() : PData(0) { }
    void SetPItem(CBaseObject* item) { ++g_setCalls; g_setArg = item; PData = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer(CBaseObject* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CTCDeadShotInThingTracker : CTCBase_Calls {
    CIntelligentPointer PThingStuckIn;
    bool                HasThingStuckIn;
    virtual void v00() { }
    CTCDeadShotInThingTracker(CThing* parent);
};
CTCDeadShotInThingTracker::CTCDeadShotInThingTracker(CThing* parent) : CTCBase_Calls(parent), PThingStuckIn(), HasThingStuckIn(false) { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCDeadShotInThingTracker)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCDeadShotInThingTracker* t = new (u.raw) CTCDeadShotInThingTracker(&thing);
    if (g_baseParent != &thing || g_setCalls != 1 || g_setArg != 0 || t->PThingStuckIn.PData != 0 || t->HasThingStuckIn)
        { std::printf("007e3450_TEST FAIL\n"); return 1; }
    std::printf("007e3450_TEST PASS\n");
    return 0;
}
