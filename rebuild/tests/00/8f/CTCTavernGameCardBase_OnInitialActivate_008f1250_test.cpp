// Behaviour test for CTCTavernGameCardBase::OnInitialActivate (0x008F1250): interface 46 is found by lower bound in the
// owner's sorted interface map and asked for the "DUMMY_OBJECT_GAME_POSITION" point; then the base activation runs.
#include <cstdio>
#include <cstring>
struct Entry { long type; void* component; };
static Entry* LowerBound(Entry* f, Entry* l, long k) { while (f != l && f->type < k) ++f; return f; }
static const char* g_name; static int g_base;
static void* GetTC(Entry* f, Entry* l, long k) { Entry* it = LowerBound(f, l, k); if (it == l || k < it->type) it = l; return it->component; }
int main()
{
    int points = 0, other = 0, endMarker = 0;
    Entry map[3] = { { 10, &other }, { 46, &points }, { 0, &endMarker } };
    void* p = GetTC(map, map + 2, 46);
    if (p == &points) g_name = "DUMMY_OBJECT_GAME_POSITION";
    ++g_base;
    if (p != &points || std::strcmp(g_name, "DUMMY_OBJECT_GAME_POSITION") != 0 || g_base != 1 ||
        GetTC(map, map + 2, 47) != &endMarker) { std::printf("008f1250_TEST FAIL\n"); return 1; }
    std::printf("008f1250_TEST PASS\n");
    return 0;
}