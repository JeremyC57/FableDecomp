#include <cstdio>
struct CMapInfo { long Area[4]; bool Used; };
static CMapInfo g_store[8];
struct CMapInfoArray
{
    CMapInfo* _M_start; CMapInfo* _M_finish;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CMapInfo& operator[](unsigned int n) { return *(_M_start + n); }
    void resize(unsigned int n) { for (unsigned int i = size(); i < n; ++i) g_store[i].Used = false; _M_finish = _M_start + n; }
};
struct CWorldMap
{
    CMapInfoArray Maps;
    long AllocateNewMap();
};
long CWorldMap::AllocateNewMap()
{
    long map = 0;
    for (unsigned int i = 1; i < Maps.size(); ++i)
    {
        if (!Maps[i].Used)
        {
            map = i;
            break;
        }
    }
    if (map == 0)
    {
        map = Maps.size();
        Maps.resize(Maps.size() + 1);
    }
    return map;
}
int main()
{
    CWorldMap w; w.Maps._M_start = g_store; w.Maps._M_finish = g_store + 1;
    for (int i = 0; i < 8; ++i) g_store[i].Used = true;
    if (w.AllocateNewMap() != 1 || w.Maps.size() != 2) { std::printf("00502460_TEST FAIL\n"); return 1; }
    g_store[1].Used = true;
    if (w.AllocateNewMap() != 2 || w.Maps.size() != 3) { std::printf("00502460_TEST FAIL\n"); return 1; }
    g_store[2].Used = true; g_store[1].Used = false;
    if (w.AllocateNewMap() != 1 || w.Maps.size() != 3) { std::printf("00502460_TEST FAIL\n"); return 1; }
    std::printf("00502460_TEST PASS\n");
    return 0;
}
