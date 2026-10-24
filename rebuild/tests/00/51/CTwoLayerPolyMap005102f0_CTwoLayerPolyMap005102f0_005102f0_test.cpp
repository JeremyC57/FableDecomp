// Behaviour test for 0x005102F0 CTwoLayerPolyMap_005102f0::CTwoLayerPolyMap_005102f0(): both 256-row poly layers are cleared, the header and settings are left alone
#include <cstdio>
#include <cstring>
#include <new>
#pragma pack(push, 1)
struct CLayer {
    struct CRow {
        unsigned int Bits[32];
        unsigned int Count;
    };
    CRow Rows[256];
    CLayer()
    {
        int row;
        for (row = 0; row < 256; ++row) {
            Rows[row].Count = 0;
            std::memset(Rows[row].Bits, 0, sizeof(Rows[row].Bits));
        }
    }
};
struct CTwoLayerPolyMap_005102f0 {
    int           RefCount;
    unsigned char Type;
    int           Setting05;
    int           Setting09;
    int           Setting0D;
    int           Setting11;
    unsigned char Flag15;
    CLayer        LayerA;
    int           Setting8416;
    int           Setting841A;
    CLayer        LayerB;
    unsigned int  Cache;
    CTwoLayerPolyMap_005102f0();
};
#pragma pack(pop)
CTwoLayerPolyMap_005102f0::CTwoLayerPolyMap_005102f0()
{
}
static unsigned char g_raw[sizeof(CTwoLayerPolyMap_005102f0)];
int main()
{
    std::memset(g_raw, 0xab, sizeof(g_raw));
    CTwoLayerPolyMap_005102f0* map = new (g_raw) CTwoLayerPolyMap_005102f0();
    if (sizeof(CTwoLayerPolyMap_005102f0) != 0x10822 || (unsigned char*)&map->LayerB - g_raw != 0x841e)
        { std::printf("005102f0_TEST FAIL\n"); return 1; }
    if (map->LayerA.Rows[0].Count != 0 || map->LayerA.Rows[255].Bits[31] != 0 || map->LayerB.Rows[128].Bits[5] != 0 ||
        map->LayerB.Rows[255].Count != 0) { std::printf("005102f0_TEST FAIL\n"); return 1; }
    if (map->Type != 0xab || map->Flag15 != 0xab || g_raw[0x8416] != 0xab || g_raw[0x1081e] != 0xab)
        { std::printf("005102f0_TEST FAIL\n"); return 1; }
    std::printf("005102f0_TEST PASS\n");
    return 0;
}