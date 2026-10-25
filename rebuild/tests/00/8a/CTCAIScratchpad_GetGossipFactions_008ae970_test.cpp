// Behaviour test for 0x008AE970 CTCAIScratchpad gossip lookup: null for an unknown category, else the category's vector.
#include <cstdio>
#include <cstring>
struct Entry { const char* key; int lines[3]; int n; };
static Entry g_map[4];
static int g_count = 0;
static Entry* Find(const char* k) { for (int i = 0; i < g_count; ++i) if (!std::strcmp(g_map[i].key, k)) return &g_map[i]; return 0; }
static Entry& Subscript(const char* k) { Entry* e = Find(k); if (e) return *e; g_map[g_count].key = k; g_map[g_count].n = 0; return g_map[g_count++]; }
static const int* GetLines(const char* category)
{
    Entry* it = Find(category);
    if (it == 0)
        return 0;
    return Subscript(category).lines;
}
int main()
{
    Entry& e = Subscript("Bowerstone"); e.lines[0] = 7; e.n = 1;
    if (GetLines("Oakvale") != 0 || g_count != 1) { std::printf("008ae970_TEST FAIL\n"); return 1; }
    const int* l = GetLines("Bowerstone");
    if (l == 0 || l[0] != 7 || g_count != 1) { std::printf("008ae970_TEST FAIL\n"); return 1; }
    std::printf("008ae970_TEST PASS\n");
    return 0;
}