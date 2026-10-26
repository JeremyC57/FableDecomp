// Behaviour test for the CHeightMap deleting destructor (0x008F6DE0): WorldSeed cleared, the cell
// storage freed when allocated, then the base destructor.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;
static void* g_freed = 0;
static unsigned long* g_seedSeen = (unsigned long*)1;

static void FreeBlock(void* p) { g_freed = p; g_events[g_count++] = 1; std::free(p); }

namespace N_0099a300 { struct CBaseClass {
    virtual ~CBaseClass() { g_events[g_count++] = 2; }
}; } using N_0099a300::CBaseClass;

class CWorld;
class CMap;
class CHeightMapCell { public: unsigned char _cell[0x10]; };

struct CHeightMapCellVector {
    CHeightMapCell* m_first; CHeightMapCell* m_last; CHeightMapCell* m_end;
    CHeightMapCellVector() : m_first(0), m_last(0), m_end(0) {}
    ~CHeightMapCellVector() { if (m_first) FreeBlock(m_first); }
};

class CHeightMap;
static CHeightMap* g_watched = 0;

class CHeightMap : public CBaseClass { public:
    CWorld*              World;
    CMap*                Map;
    unsigned long*       WorldSeed;
    CHeightMapCellVector Cells;
    CHeightMap(CWorld* world, CMap* map) : World(world), Map(map), WorldSeed(0) {}
    virtual ~CHeightMap()
    {
        WorldSeed = 0;
        g_seedSeen = WorldSeed;
    }
};

int main()
{
    static unsigned long seed = 5;
    CHeightMap* a = new CHeightMap(0, 0);
    a->WorldSeed = &seed;
    void* block = std::malloc(32);
    a->Cells.m_first = (CHeightMapCell*)block;
    CBaseClass* base = a;
    delete base;
    bool ok = g_seedSeen == 0 && g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    CHeightMap* b = new CHeightMap(0, 0);
    delete b;
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("008f6de0_TEST FAIL\n");
        return 1;
    }
    std::printf("008f6de0_TEST PASS\n");
    return 0;
}