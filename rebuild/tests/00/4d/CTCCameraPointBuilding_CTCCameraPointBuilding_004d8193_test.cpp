// Behaviour test for CTCCameraPointBuilding::CTCCameraPointBuilding(CThing*) @ 0x004D8193: the base gets the parent and the
// three building flags are cleared.
#include <cstdio>
#include <new>
class CThing { public: int id; };
static CThing* g_baseParent = 0;
struct CTCCameraPointDefinitionBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x40 - 4];
    CTCCameraPointDefinitionBase_Calls(CThing* parent) { g_baseParent = parent; }
};
struct CTCCameraPointBuilding : CTCCameraPointDefinitionBase_Calls {
    bool InMyBuilding;
    bool ExitPoint;
    bool StartPoint;
    virtual void v00() { }
    CTCCameraPointBuilding(CThing* parent);
};
CTCCameraPointBuilding::CTCCameraPointBuilding(CThing* parent)
    : CTCCameraPointDefinitionBase_Calls(parent), InMyBuilding(false), ExitPoint(false), StartPoint(false) { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCCameraPointBuilding)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCCameraPointBuilding* c = new (u.raw) CTCCameraPointBuilding(&thing);
    if (g_baseParent != &thing || c->InMyBuilding || c->ExitPoint || c->StartPoint) { std::printf("004d8193_TEST FAIL\n"); return 1; }
    std::printf("004d8193_TEST PASS\n");
    return 0;
}