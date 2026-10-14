#include <cstdio>
struct CUIMiniMapMarker { long m_a, m_b, m_c; };
static unsigned int g_count = 0; static long g_sum = -1; static unsigned int* g_pCount = 0; static void* g_first = (void*)1;
static void FixUpMarkers(void* f, void* l, unsigned int* pc) { g_first = f; g_pCount = pc; }
struct CMarkerVector { void resize(unsigned int c, const CUIMiniMapMarker& v) { g_count = c; g_sum = v.m_a + v.m_b + v.m_c; }
    void ResizeCleared(unsigned int count) { CUIMiniMapMarker blank; blank.m_a = 0; blank.m_b = 0; blank.m_c = 0; resize(count, blank); FixUpMarkers(0, 0, &count); } };
int main() { CMarkerVector v; v.ResizeCleared(12); if (g_count != 12 || g_sum != 0 || g_first != 0 || *g_pCount != 12) { std::printf("0088aa30_TEST FAIL\n"); return 1; } std::printf("0088aa30_TEST PASS\n"); return 0; }
