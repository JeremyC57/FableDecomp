// Behaviour test for 0x00770DE0 CTCSolidToThing::IsSolidToThing(thing) -- empty list or thing absent => solid
#include <cstdio>

struct Thing { int Id; };
struct Pointer { const Thing* Item; };

struct SolidToThing {
    Pointer* First;
    Pointer* Last;
    bool IsSolidToThing(const Thing* thing) const
    {
        if (First == Last)
            return true;
        const Pointer* it = First;
        for (; it != Last; ++it)
            if (it->Item == thing)
                break;
        return it == Last;
    }
};

int main()
{
    Thing a = { 1 }, b = { 2 }, c = { 3 };
    Pointer list[2] = { { &a }, { &b } };
    SolidToThing empty = { list, list };
    if (!empty.IsSolidToThing(&a)) { std::printf("00770de0_TEST FAIL\n"); return 1; }
    SolidToThing two = { list, list + 2 };
    if (two.IsSolidToThing(&a) || two.IsSolidToThing(&b)) { std::printf("00770de0_TEST FAIL\n"); return 1; }
    if (!two.IsSolidToThing(&c) || !two.IsSolidToThing(0)) { std::printf("00770de0_TEST FAIL\n"); return 1; }
    std::printf("00770de0_TEST PASS\n");
    return 0;
}