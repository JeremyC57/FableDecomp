// Behaviour test for 0x008F6170 CHeightMap::~CHeightMap(): WorldSeed is cleared, the Cells storage is released, then the
// base destructor runs.
#include <cstdio>
#include <cstdlib>

static int g_order[8];
static int g_n = 0;
static unsigned long* g_seedSeen = (unsigned long*)1;

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 3; }
};
class CHeightMapCell { public: float Height; unsigned char Index[3]; unsigned char Blend[2]; unsigned char Flags; };
class CCellVector {
public:
    CHeightMapCell* Start;
    CHeightMapCell* Finish;
    CHeightMapCell* End;
    CCellVector() : Start(0), Finish(0), End(0) { }
    ~CCellVector();
};
class CHeightMap : public CBaseClassNonCopyable {
public:
    virtual ~CHeightMap() { WorldSeed = 0; g_order[g_n++] = 1; }
    void* World;
    void* Map;
    unsigned long* WorldSeed;
    CCellVector Cells;
};
static CHeightMap* g_map = 0;
CCellVector::~CCellVector()
{
    g_seedSeen = g_map->WorldSeed;
    g_order[g_n++] = 2;
    if (Start)
        std::free(Start);
}

int main()
{
    unsigned long seed = 1234;
    CHeightMap* map = new CHeightMap;
    g_map = map;
    map->WorldSeed = &seed;
    map->Cells.Start = (CHeightMapCell*)std::malloc(4 * sizeof(CHeightMapCell));
    map->Cells.Finish = map->Cells.End = map->Cells.Start + 4;
    CBaseClassNonCopyable* base = map;
    delete base;
    if (g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_seedSeen != 0 || seed != 1234) {
        std::printf("008f6170_TEST FAIL\n");
        return 1;
    }
    std::printf("008f6170_TEST PASS\n");
    return 0;
}