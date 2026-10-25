// behaviour test for 0x004de782 CShipDef::Transfer(CPersistContext&): the context sees each persisted field once, in the
// retail order (SightingFrequency, SightingOffset, DaysVisible, Swell, Pitch, Roll, WaveLength), and a load pass writes the values back into the same fields
#include <cstdio>
static const int kOrder[7] = { 0x28, 0x2c, 0x30, 0x34, 0x38, 0x3c, 0x40 };
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
    if (fails) { std::printf("004de782_TEST FAIL %d\n", fails); return 1; }
    std::printf("004de782_TEST PASS\n");
    return 0;
}