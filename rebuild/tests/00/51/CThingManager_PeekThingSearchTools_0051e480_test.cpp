// Test for 0x0051E480 CThingManager::PeekThingSearchTools() const: the search tools of the manager's world.
#include <stdio.h>
#include <string.h>
class CThingSearchTools { public: int id; };
class CWorld {
public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;
};
class CThingManager {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;
    const CThingSearchTools& PeekThingSearchTools() const;
};
const CThingSearchTools& CThingManager::PeekThingSearchTools() const
{
    return *World->PThingSearchTools;
}
int main()
{
    CThingSearchTools a, b; CWorld w; memset(&w, 0, sizeof(w)); w.PThingSearchTools = &a;
    CThingManager tm; memset(&tm, 0, sizeof(tm)); tm.World = &w;
    bool ok = (char*)&tm.World - (char*)&tm == 0x24 && (char*)&w.PThingSearchTools - (char*)&w == 0x20;
    ok = ok && &tm.PeekThingSearchTools() == &a;
    w.PThingSearchTools = &b;
    ok = ok && &tm.PeekThingSearchTools() == &b;
    if (!ok) { printf("0051e480_TEST FAIL\n"); return 1; }
    printf("0051e480_TEST PASS\n");
    return 0;
}
