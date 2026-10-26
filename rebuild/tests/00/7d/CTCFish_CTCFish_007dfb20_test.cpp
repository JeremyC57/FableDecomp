// Behaviour test for CTCFish::CTCFish(CThing&) (retail 0x007DFB20): model copy + stub callees.
#include <cstdio>

static unsigned long GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CThing;
class CFishDef { public: float MinLength, MaxLength, MinWeight, MaxWeight, MinSpeed, MaxSpeed; };
template <class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) { } T* operator->() const { return Object; } };
static CFishDef g_def = { 10.0f, 5.0f, 2.0f, 0.0f, 1.5f, 3.0f };
class CDefClassBase { public: bool GetSubDef(CDefPointer<const CFishDef>* out) const { out->Object = &g_def; return true; } };
class CThing { public: const CDefClassBase* PBaseDef; };
static unsigned long g_seed = 12345;
class CTCBase {
public:
    CTCBase(CThing* parent) : ParentThing(parent) {}
    CThing* ParentThing;
    unsigned long* GetPRandomSeed() { return &g_seed; }
};
class CTCFish : public CTCBase {
public:
    CDefPointer<const CFishDef> PDef;
    float Length, Weight, Speed;
    CTCFish(CThing& thing);
};
CTCFish::CTCFish(CThing& thing)
    : CTCBase(&thing), PDef()
{
    ParentThing->PBaseDef->GetSubDef(&PDef);
    Length = PDef->MinLength + (float)GFRandom((unsigned long)PDef->MaxLength, GetPRandomSeed());
    Weight = PDef->MinWeight + (float)GFRandom((unsigned long)PDef->MaxWeight, GetPRandomSeed());
    Speed = PDef->MinSpeed + (float)GFRandom((unsigned long)PDef->MaxSpeed, GetPRandomSeed());
}
int main()
{
    int fail = 0;
    CDefClassBase base; CThing thing; thing.PBaseDef = &base;
    unsigned long s = 12345;
    unsigned long r1 = GFRandom(5, &s), r2 = GFRandom(0, &s), r3 = GFRandom(3, &s);
    CTCFish fish(thing);
    if (fish.PDef.Object != &g_def || fish.ParentThing != &thing) fail = 1;
    if (fish.Length != 10.0f + (float)r1 || fish.Weight != 2.0f || fish.Speed != 1.5f + (float)r3 || r2 != 0) fail = 2;
    if (fish.Length < 10.0f || fish.Length >= 15.0f || fish.Speed >= 4.5f || g_seed != s) fail = 3;
    if (fail) { std::printf("007dfb20_TEST FAIL\n"); return 1; }
    std::printf("007dfb20_TEST PASS\n");
    return 0;
}