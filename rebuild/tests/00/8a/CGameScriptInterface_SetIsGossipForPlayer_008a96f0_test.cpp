// behaviour test for 0x008A96F0 CGameScriptInterface::SetIsGossipForPlayer: a copy of the name keys CTCAIScratchpad's static name -> bool map; the flag is stored (inserted false first when absent)
#include <cstdio>
#include <cstring>
struct Name { char text[32]; int* live; };
static int g_live = 0;
static void Copy(Name& d, const Name& s) { std::strcpy(d.text, s.text); d.live = &g_live; ++g_live; }
static void Destroy(Name& n) { if (n.live) --*n.live; n.live = 0; }
struct Map { Name keys[8]; bool values[8]; int count; };
static Map g_map;
static bool& Subscript(const Name& key)                      // map<CCharString, bool>::operator[]
{
    for (int i = 0; i < g_map.count; ++i)
        if (std::strcmp(g_map.keys[i].text, key.text) == 0) return g_map.values[i];
    std::strcpy(g_map.keys[g_map.count].text, key.text); g_map.keys[g_map.count].live = 0;
    g_map.values[g_map.count] = false;
    return g_map.values[g_map.count++];
}
static void SetIsGossipForPlayer(const Name& name, bool isGossip)   // the inlined by-value helper
{
    Name copy; Copy(copy, name);
    Subscript(copy) = isGossip;
    Destroy(copy);
}
int main()
{
    g_map.count = 0;
    Name a; std::strcpy(a.text, "GOSSIP_A"); a.live = 0;
    Name b; std::strcpy(b.text, "GOSSIP_B"); b.live = 0;
    SetIsGossipForPlayer(a, true);
    SetIsGossipForPlayer(b, false);
    SetIsGossipForPlayer(a, false);
    SetIsGossipForPlayer(b, true);
    if (g_map.count != 2 || g_map.values[0] != false || g_map.values[1] != true || g_live != 0) {
        std::printf("008a96f0_TEST FAIL\n"); return 1;
    }
    std::printf("008a96f0_TEST PASS\n");
    return 0;
}