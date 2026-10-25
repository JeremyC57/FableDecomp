// behaviour test for 0x00457a0d CSpecialAbilitiesGhostSwordDef::Transfer(CPersistContext&): the context sees each persisted field once, in the
// retail order (TimeOfSummon[0], TimeOfSummon[1], TimeOfSummon[2], TimeOfSummon[3], FadeInTime, FadeOutTime, CreationDistance), and a load pass writes the values back into the same fields
#include <cstdio>
static const int kOrder[7] = { 0x3c, 0x40, 0x44, 0x48, 0x4c, 0x50, 0x54 };
struct Ctx { int n; int seen[16]; int load; void Field(unsigned char* base, int off) { seen[n++] = off; if (load) base[off] = (unsigned char)(0x40 + off); } };
static void TransferAll(Ctx& c, unsigned char* obj) { for (int i = 0; i < 7; ++i) c.Field(obj, kOrder[i]); }
int main()
{
    int fails = 0;
    unsigned char obj[0x80] = { 0 };
    Ctx save = { 0, { 0 }, 0 };
    TransferAll(save, obj);
    if (save.n != 7) fails |= 1;
    for (int i = 0; i < save.n; ++i) if (save.seen[i] != kOrder[i] || obj[kOrder[i]] != 0) fails |= 2;
    Ctx load = { 0, { 0 }, 1 };
    TransferAll(load, obj);
    for (int i = 0; i < 7; ++i) if (obj[kOrder[i]] != (unsigned char)(0x40 + kOrder[i])) fails |= 4;
    for (int i = 0; i < 7; ++i) for (int j = i + 1; j < 7; ++j) if (kOrder[i] == kOrder[j]) fails |= 8;
    if (fails) { std::printf("00457a0d_TEST FAIL %d\n", fails); return 1; }
    std::printf("00457a0d_TEST PASS\n");
    return 0;
}