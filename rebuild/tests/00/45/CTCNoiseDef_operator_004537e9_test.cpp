// behaviour test for 0x004537e9 CTCNoiseDef::operator=: the base part is assigned first, then Type, LifeTime and
// DistanceCanBeHeardFrom are copied bit for bit (the float included), the rest of the object is untouched, *this returned
#include <cstdio>
#include <cstring>
static const int kOff[3] = { 0x28, 0x2c, 0x30 };
static int gBaseCalls = 0;
static unsigned char* Assign(unsigned char* self, const unsigned char* other)
{
    ++gBaseCalls;                                   // base operator=
    for (int i = 0; i < 3; ++i) std::memcpy(self + kOff[i], other + kOff[i], 4);
    return self;
}
int main()
{
    int fails = 0;
    unsigned char a[0x40], b[0x40];
    for (int i = 0; i < 0x40; ++i) { a[i] = 0xAA; b[i] = (unsigned char)i; }
    unsigned char* r = Assign(a, b);
    if (r != a || gBaseCalls != 1) fails |= 1;
    for (int i = 0; i < 3; ++i) for (int k = 0; k < 4; ++k) if (a[kOff[i] + k] != b[kOff[i] + k]) fails |= 2;
    int same = 0; for (int i = 0; i < 0x40; ++i) if (a[i] == b[i]) ++same;
    if (same != 12) fails |= 4;
    if (fails) { std::printf("004537e9_TEST FAIL %d\n", fails); return 1; }
    std::printf("004537e9_TEST PASS\n");
    return 0;
}
