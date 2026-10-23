// Behaviour test for CTCInventoryWeapons::ConstructWeaponDescriptionWeight @ 0x005C770C: weight line only for melee classes, heavy/light by class
#include <cstdio>
#include <cstring>
static void Describe(char* out, int melee, long weaponClass, const long* augSlots, int augCount, long slotCount, long* copied)
{
    long data[8];
    int i;
    for (i = 0; i < 8; ++i) data[i] = 0;
    long n = slotCount;
    for (i = 0; i < augCount && i < n; ++i) data[i] = augSlots[i];
    *copied = i;
    out[0] = 0;
    if (melee) {
        std::strcat(out, "WEIGHT:kg");
        if (weaponClass == 2) std::strcat(out, "HEAVY");
        else if (weaponClass == 1) std::strcat(out, "LIGHT");
    }
}
int main()
{
    int ok = 1;
    char buf[64];
    long slots[3] = { 5, 6, 7 };
    long copied = 0;
    Describe(buf, 1, 2, slots, 3, 2, &copied);
    if (std::strcmp(buf, "WEIGHT:kgHEAVY") != 0 || copied != 2) ok = 0;
    Describe(buf, 1, 1, slots, 1, 4, &copied);
    if (std::strcmp(buf, "WEIGHT:kgLIGHT") != 0 || copied != 1) ok = 0;
    Describe(buf, 1, 3, slots, 0, 4, &copied);
    if (std::strcmp(buf, "WEIGHT:kg") != 0 || copied != 0) ok = 0;
    Describe(buf, 0, 2, slots, 3, 3, &copied);
    if (buf[0] != 0) ok = 0;
    if (!ok) { std::printf("005c770c_TEST FAIL\n"); return 1; }
    std::printf("005c770c_TEST PASS\n");
    return 0;
}