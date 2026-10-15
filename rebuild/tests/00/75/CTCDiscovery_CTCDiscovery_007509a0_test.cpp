// Behaviour test for CTCDiscovery::CTCDiscovery(CThing*) @ 0x007509A0: Discovered is false and the Discoverer pointer
// is default-constructed (base constructor only, no SetPItem).
#include <cstdio>
#include <new>
struct CThing { int id; };
static CThing* g_baseParent = 0;
static int g_baseCtorCalls = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_baseCtorCalls; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer() { }
};
struct CTCDiscovery : CTCBase_Calls {
    bool                Discovered;
    CIntelligentPointer Discoverer;
    virtual void v00() { }
    CTCDiscovery(CThing* parent);
};
#pragma pack(pop)
CTCDiscovery::CTCDiscovery(CThing* parent) : CTCBase_Calls(parent), Discovered(false), Discoverer() { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCDiscovery)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCDiscovery* d = new (u.raw) CTCDiscovery(&thing);
    if (g_baseParent != &thing || d->Discovered || g_baseCtorCalls != 1 || d->Discoverer.PData != 0)
        { std::printf("007509a0_TEST FAIL\n"); return 1; }
    std::printf("007509a0_TEST PASS\n");
    return 0;
}