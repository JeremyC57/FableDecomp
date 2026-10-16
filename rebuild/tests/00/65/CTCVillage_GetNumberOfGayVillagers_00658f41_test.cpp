// Behaviour test for CTCVillage::GetNumberOfGayVillagers (0x00658F41): live villagers of kind 1 with flag bits 3 and 4.
#include <cstdio>
struct Villager { bool alive; long kind; unsigned char flags; };
int main()
{
    Villager v[5] = { { true, 1, 0x18 }, { false, 1, 0x18 }, { true, 2, 0x18 }, { true, 1, 0x08 }, { true, 1, 0xff } };
    long count = 0;
    for (int i = 0; i < 5; ++i)
        if (v[i].alive && v[i].kind == 1 && (v[i].flags & 8) && (v[i].flags & 0x10)) ++count;
    if (count != 2) { std::printf("00658f41_TEST FAIL\n"); return 1; }
    std::printf("00658f41_TEST PASS\n");
    return 0;
}