// Behaviour test for CWorldMap::GetRegion (0x004fb490): Regions[index] with the retail 0x58-byte CRegion.
#include <stdio.h>
class CRegion { public: unsigned char _pad_0x00[0x58]; };
class CWorldMap {
public:
    unsigned char _pad_0x00[0x2C];
    CRegion* RegionsStart; CRegion* RegionsFinish; CRegion* RegionsEndOfStorage;
    CRegion& GetRegion(long index) { return RegionsStart[index]; }
};
int main()
{
    static CRegion regions[4];
    static CWorldMap m; m.RegionsStart = regions; m.RegionsFinish = regions + 4; m.RegionsEndOfStorage = regions + 4;
    const CWorldMap& cm = m;
    int ok = &m.GetRegion(0) == &regions[0] && (const unsigned char*)&m.GetRegion(3) == (const unsigned char*)regions + 3 * 0x58
        && sizeof(CRegion) == 0x58;
    printf(ok ? "004fb490_TEST PASS\n" : "004fb490_TEST FAIL\n");
    return ok ? 0 : 1;
}
