#include <cstdio>
#include <cstdlib>
#include <cstring>
static const void* g_position = 0;
struct CWorldMapTable { long m_number; virtual long GetWorldMapNumber(const void* position) { g_position = position; return m_number; } };
class CWorld { public: CWorldMapTable m_table; virtual CWorldMapTable* GetWorldMapTable() { return &m_table; } };
class CMap { public: unsigned char _pad[0x20]; CWorld* m_pWorld; unsigned char _pad24[0x1d00]; unsigned char m_position[0xc];
    long GetWorldMapNumber() { return m_pWorld->GetWorldMapTable()->GetWorldMapNumber(m_position); } };

int main()
{
    CWorld world; CMap* map = new CMap; map->m_pWorld = &world; world.m_table.m_number = 42;
    if (map->GetWorldMapNumber() != 42 || g_position != map->m_position) { std::printf("0081e690_TEST FAIL\n"); return 1; } delete map;
    std::printf("0081e690_TEST PASS\n");
    return 0;
}