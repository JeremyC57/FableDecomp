// Behaviour test for NThingShot::CTCShotDefinitionDrainLife::CTCShotDefinitionDrainLife(CThing*) @ 0x004DB15E:
// AmountToHeal 0.0f and the soul-effect pointer built from a null thing; the other floats are left alone.
#include <cstdio>
#include <new>
class CThing { public: int id; };
static CThing* g_baseParent = 0;
static int g_ptrCtors = 0;
static CThing* g_ptrArg = (CThing*)1;
namespace NThingShot {
struct CTCShotDefinitionBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x9c - 4];
    CTCShotDefinitionBase_Calls(CThing* parent) { g_baseParent = parent; }
};
struct CIntelligentPointer_CThing {
    void* __vftable;
    void* PData;
    CIntelligentPointer_CThing(CThing* thing) : __vftable(0), PData(thing) { ++g_ptrCtors; g_ptrArg = thing; }
};
class CTCShotDefinitionDrainLife : public CTCShotDefinitionBase_Calls { public:
    float MaxCosAngleToTurn;
    float MaxAngleToTurn;
    float AmountToHeal;
    float Speed;
    CIntelligentPointer_CThing PSoulEffect;
    virtual void v00() { }
    CTCShotDefinitionDrainLife(CThing* parent);
};
CTCShotDefinitionDrainLife::CTCShotDefinitionDrainLife(CThing* parent)
    : CTCShotDefinitionBase_Calls(parent), AmountToHeal(0.0f), PSoulEffect(0) { }
}
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(NThingShot::CTCShotDefinitionDrainLife)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0;
    CThing thing;
    NThingShot::CTCShotDefinitionDrainLife* s = new (u.raw) NThingShot::CTCShotDefinitionDrainLife(&thing);
    if (g_baseParent != &thing || s->AmountToHeal != 0.0f || g_ptrCtors != 1 || g_ptrArg != 0 || s->PSoulEffect.PData != 0)
        { std::printf("004db15e_TEST FAIL\n"); return 1; }
    std::printf("004db15e_TEST PASS\n");
    return 0;
}