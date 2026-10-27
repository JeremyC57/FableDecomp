// Behaviour test for 0x00A629C0 CPackedUIntArray::CPackedUIntArray()
#include <cstdio>
#include <new>
class CPackedUIntArray {
public:
    unsigned long* PackedInts;
    unsigned long  Size;
    unsigned char  Bits;
    unsigned long  Bias;
    CPackedUIntArray() : PackedInts(0), Size(0), Bits(0), Bias(0) { }
};
int main()
{
    unsigned char raw[sizeof(CPackedUIntArray)];
    for (unsigned int i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CPackedUIntArray* a = new (raw) CPackedUIntArray();
    if (a->PackedInts != 0 || a->Size != 0 || a->Bits != 0 || a->Bias != 0) { std::printf("00a629c0_TEST FAIL\n"); return 1; }
    std::printf("00a629c0_TEST PASS\n");
    return 0;
}