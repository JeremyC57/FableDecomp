#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

class CCharString { public: const char* s; };
struct CRetailNameMap { struct node { node* left; node* parent; node* right; long colour; const char* key; long value; }; node* head; node nodes[3];
    CRetailNameMap() { head = &nodes[0]; nodes[1].key = "a"; nodes[1].value = 1; nodes[2].key = "b"; nodes[2].value = 0; }
    node* Find(const CCharString& k) { for (int i = 1; i < 3; ++i) if (!std::strcmp(nodes[i].key, k.s)) return &nodes[i]; return head; } };
static const char* g_applied = 0; static long g_flags = -1; static int g_which = -1;
class CMap { public: int id; void ApplyScriptedMapBrush(const CCharString& n, long f) { g_applied = n.s; g_flags = f; g_which = id; } };
struct SMapEntry { unsigned char p[0x10]; CMap* m_pMap; unsigned char r[72 - 0x14]; };
struct CMapBrushOwner { unsigned char p[0x14]; CRetailNameMap m_nameToIndex; unsigned char q[0x20 - 0x18]; SMapEntry* m_pEntries;
    void ApplyScriptedMapBrushByName(const CCharString& name, long flags) { CRetailNameMap* map = &m_nameToIndex; CRetailNameMap::node* found = map->Find(name); if (found != map->head) m_pEntries[found->value].m_pMap->ApplyScriptedMapBrush(name, flags); } };
int main() { CMap m0 = {0}, m1 = {1}; SMapEntry ents[2]; ents[0].m_pMap = &m0; ents[1].m_pMap = &m1; CMapBrushOwner o; o.m_pEntries = ents; CCharString a = {"a"}, z = {"z"};
    o.ApplyScriptedMapBrushByName(z, 3); if (g_which != -1) { std::printf("004fe510_TEST FAIL\n"); return 1; }
    o.ApplyScriptedMapBrushByName(a, 3); if (g_which != 1 || g_flags != 3 || g_applied != a.s) { std::printf("004fe510_TEST FAIL\n"); return 1; }
    std::printf("004fe510_TEST PASS\n"); return 0; }