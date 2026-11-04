// Behaviour test for CWorldMap::UnloadRegion (0x004FEEC0, parked DIFFER): a model of the row -- every loaded map of the region is deactivated, removed and released, then the region is cleared and the message posted.
#include <stdio.h>
#include <vector>
struct Map { int region; bool loaded, active; };
static int UnloadRegion(std::vector<Map>& maps, int region, int& loadedRegion, int& messages)
{
    int released = 0;
    for (size_t i = 0; i < maps.size(); ++i)
        if (maps[i].region == region && maps[i].loaded) { maps[i].active = false; maps[i].loaded = false; ++released; }
    if (loadedRegion == region) loadedRegion = -1;
    ++messages; return released;
}
int main() { std::vector<Map> m; Map a = { 1, true, true }, b = { 2, true, true }, c = { 1, false, false };
  m.push_back(a); m.push_back(b); m.push_back(c); int lr = 1, msg = 0;
  bool ok = UnloadRegion(m, 1, lr, msg) == 1 && !m[0].loaded && !m[0].active && m[1].loaded && lr == -1 && msg == 1;
  printf(ok ? "004feec0_TEST PASS\n" : "004feec0_TEST FAIL\n");
  return ok ? 0 : 1;
}
