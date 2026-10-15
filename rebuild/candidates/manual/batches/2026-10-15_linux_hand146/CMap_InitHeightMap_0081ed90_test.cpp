// Behaviour test for 0x0081ED90 CMap::InitHeightMap: the new height map gets the map's size and seed, the old one is destroyed after the swap.
#include <cstdio>
static int g_destroyed = 0;
struct CHeightMapInit { int SizeX, SizeY; unsigned int* WorldSeed; };
struct CHeightMap {
    void* World; void* Map; CHeightMapInit init;
    CHeightMap(void* w, void* m, const CHeightMapInit& i) : World(w), Map(m), init(i) { }
    virtual ~CHeightMap() { ++g_destroyed; }
};
template <class T> struct scoped_ptr { T* ptr; void reset(T* p) { T* old = ptr; ptr = p; delete old; } };
struct CMap {
    void* World; unsigned int* WorldSeed; scoped_ptr<CHeightMap> HeightMap; int HeightMapSizeX, HeightMapSizeY;
    void InitHeightMap()
    {
        CHeightMapInit init; init.SizeX = HeightMapSizeX; init.SizeY = HeightMapSizeY; init.WorldSeed = WorldSeed;
        HeightMap.reset(new CHeightMap(World, this, init));
    }
};
int main()
{
    unsigned int seed = 5; int world = 0;
    CMap m; m.World = &world; m.WorldSeed = &seed; m.HeightMap.ptr = 0; m.HeightMapSizeX = 64; m.HeightMapSizeY = 32;
    m.InitHeightMap();
    bool ok = m.HeightMap.ptr && m.HeightMap.ptr->init.SizeX == 64 && m.HeightMap.ptr->init.SizeY == 32 && m.HeightMap.ptr->init.WorldSeed == &seed &&
              m.HeightMap.ptr->Map == &m && m.HeightMap.ptr->World == &world && g_destroyed == 0;
    CHeightMap* first = m.HeightMap.ptr;
    m.InitHeightMap();
    ok = ok && g_destroyed == 1 && m.HeightMap.ptr != 0 && m.HeightMap.ptr != first;
    delete m.HeightMap.ptr;
    if (!ok) { std::printf("0081ed90_TEST FAIL\n"); return 1; }
    std::printf("0081ed90_TEST PASS\n");
    return 0;
}
