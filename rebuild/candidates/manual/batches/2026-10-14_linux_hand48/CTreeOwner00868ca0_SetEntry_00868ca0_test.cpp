#include <cstdio>
#include <cstdlib>
#include <cstring>
static const float g_epsilon_0129ba3c = 1e-4f; static long g_inserts = 0, g_key = 0; static float g_value = 0; static void* g_tree = 0;
struct SPair { long m_key; float m_value; }; struct SIterator { void* m_pNode; long m_1; };
struct CTree { unsigned char _body[8]; SIterator Insert(const SPair& entry) { ++g_inserts; g_key = entry.m_key; g_value = entry.m_value; g_tree = this; SIterator it = { 0, 0 }; return it; } };
struct CTreeOwner { unsigned char _pad[0x30]; CTree m_tree; void SetEntry(long key, float value) { if (-g_epsilon_0129ba3c < value) { SPair entry; entry.m_key = key; entry.m_value = value; m_tree.Insert(entry); } } };

int main() {
    CTreeOwner o; o.SetEntry(5, 0.5f); if (g_inserts != 1 || g_key != 5 || g_value != 0.5f || g_tree != &o.m_tree) { std::printf("00868ca0_TEST FAIL\n"); return 1; } o.SetEntry(6, 0.0f); if (g_inserts != 2 || g_key != 6) { std::printf("00868ca0_TEST FAIL\n"); return 1; } o.SetEntry(7, -0.5f); if (g_inserts != 2) { std::printf("00868ca0_TEST FAIL\n"); return 1; } o.SetEntry(8, -0.00005f); if (g_inserts != 3) { std::printf("00868ca0_TEST FAIL\n"); return 1; }
    std::printf("00868ca0_TEST PASS\n"); return 0;
}
