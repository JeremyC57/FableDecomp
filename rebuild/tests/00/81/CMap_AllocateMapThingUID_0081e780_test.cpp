#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CMap { public: unsigned char _pad[0x1d30]; u64 m_nextThingUID; u64 AllocateMapThingUID() { return m_nextThingUID++; } };

int main()
{
    CMap* map = new CMap; map->m_nextThingUID = 0xFFFFFFFFu; u64 a = map->AllocateMapThingUID(); u64 b = map->AllocateMapThingUID();
    if (a != 0xFFFFFFFFu || (unsigned long)(b >> 32) != 1 || (unsigned long)(b & 0xFFFFFFFFu) != 0 || map->m_nextThingUID != b + 1) { std::printf("0081e780_TEST FAIL\n"); return 1; } delete map;
    std::printf("0081e780_TEST PASS\n");
    return 0;
}