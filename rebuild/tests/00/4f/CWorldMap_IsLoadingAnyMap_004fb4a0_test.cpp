// Test for 0x004FB4A0 CWorldMap::IsLoadingAnyMap: the source object must forward to the level loader held at +0xBC and
// return its IsBusy() result (stubbed here).
#include <stdio.h>
#include <string.h>
static const void* g_this;
static bool g_busy;
class CLevelLoader {
public:
    bool IsBusy() const;
};
bool CLevelLoader::IsBusy() const { g_this = this; return g_busy; }
class CWorldMap {
public:
    bool IsLoadingAnyMap() const;
};
int main()
{
    unsigned char map[0xc4];
    unsigned char loader[4];
    memset(map, 0, sizeof(map));
    void* p = loader;
    memcpy(map + 0xbc, &p, 4);
    const CWorldMap* m = (const CWorldMap*)map;
    g_busy = true;
    bool ok = m->IsLoadingAnyMap() && g_this == loader;
    g_busy = false; g_this = 0;
    ok = ok && !m->IsLoadingAnyMap() && g_this == loader;
    if (!ok) { puts("004fb4a0_TEST FAIL"); return 1; }
    puts("004fb4a0_TEST PASS");
    return 0;
}
