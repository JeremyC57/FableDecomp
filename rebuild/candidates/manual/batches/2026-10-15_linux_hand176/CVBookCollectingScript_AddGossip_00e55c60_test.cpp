// Behaviour test for 0x00e55c60: CV_BookCollectingScript::AddGossip: category, rumour, village and faction registered in order
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

static const char* g_log[8]; static int g_n = 0;
static void Add(const char* a, const char* b) { g_log[g_n++] = a; g_log[g_n++] = b; }
static void AddGossip(const char* cat, const char* rumour, const char* village, const char* faction) { Add(cat, ""); Add(cat, rumour); Add(cat, village); Add(cat, faction); }
static void run() { AddGossip("C", "R", "V", "F"); CHECK(g_n == 8 && !std::strcmp(g_log[0], "C") && !std::strcmp(g_log[3], "R") && !std::strcmp(g_log[5], "V") && !std::strcmp(g_log[7], "F") && !std::strcmp(g_log[6], "C")); }

int main()
{
    run();
    if (g_fail) { std::printf("00e55c60_TEST FAIL\n"); return 1; }
    std::printf("00e55c60_TEST PASS\n");
    return 0;
}
