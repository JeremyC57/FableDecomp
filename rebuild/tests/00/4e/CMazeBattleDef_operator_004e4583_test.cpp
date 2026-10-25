// behaviour test for 0x004e4583 CMazeBattleDef::operator=: the base part is assigned first, then each member (ShieldUpTime, LightningDuration, MinThrowShotRange, LightningRange, EnflameRange, TutorialNumGetHits, MinTimeBetweenHeals, HealSpellLevel, TutorialTeleportTime) is
// copied bit for bit (floats included, so NaN payloads survive), the rest of the object is untouched, *this returned
#include <cstdio>
#include <cstring>
static const int kOff[9] = { 0x3c, 0x40, 0x44, 0x48, 0x4c, 0x50, 0x54, 0x58, 0x5c };
static const int kSize[9] = { 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static int gBaseCalls = 0;
static unsigned char* Assign(unsigned char* self, const unsigned char* other)
{
    ++gBaseCalls;                                   // base operator=
    for (int i = 0; i < 9; ++i) std::memcpy(self + kOff[i], other + kOff[i], kSize[i]);
    return self;
}
int main()
{
    int fails = 0;
    unsigned char a[0x60], b[0x60];
    for (int i = 0; i < 0x60; ++i) { a[i] = 0xAA; b[i] = (unsigned char)i; }
    unsigned char* r = Assign(a, b);
    if (r != a || gBaseCalls != 1) fails |= 1;
    for (int i = 0; i < 9; ++i) for (int k = 0; k < kSize[i]; ++k) if (a[kOff[i] + k] != b[kOff[i] + k]) fails |= 2;
    int copied = 0; for (int i = 0; i < 9; ++i) copied += kSize[i];
    int same = 0; for (int i = 0; i < 0x60; ++i) if (a[i] == b[i]) ++same;
    if (same != copied) fails |= 4;
    if (fails) { std::printf("004e4583_TEST FAIL %d\n", fails); return 1; }
    std::printf("004e4583_TEST PASS\n");
    return 0;
}