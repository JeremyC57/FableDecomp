// behaviour test for 0x0049d8e0 CWorld::GetEventDispatchTableEntry(type) const: address of the type'th 64-byte entry
// of the global dispatch table
#include <cstdio>
struct Dispatch { unsigned char bytes[64]; };
static Dispatch gTable[8];
struct World { const Dispatch* GetEventDispatchTableEntry(long type) const { return &gTable[type]; } };
int main()
{
    int fails = 0;
    World w;
    if (w.GetEventDispatchTableEntry(0) != gTable) fails |= 1;
    if ((const unsigned char*)w.GetEventDispatchTableEntry(5) - (const unsigned char*)gTable != 5 * 64) fails |= 2;
    if (fails) { std::printf("0049d8e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0049d8e0_TEST PASS\n");
    return 0;
}