#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00BDED90 CLocalDetailCacheMap::Update(double): lock raised during the quad-tree update, unused
// file blocks deleted only when the manager exists, lock dropped, the quad-tree result returned.
static char g_seq[64] = "";
struct CBlocks { void DeleteUnusedBlocks() { std::strcat(g_seq, "D"); } }; struct CPalette { unsigned char Entries[0xc]; }; static CBlocks* g_list; static const CPalette* g_rec; static double g_time; static bool g_result; static bool g_busyDuringUpdate = false; struct CMap;
static const bool* g_pBusy = 0; struct CElement { unsigned char _body[0x4c]; bool Update(CBlocks* l, const CPalette& r, double t) { g_list = l; g_rec = &r; g_time = t; g_busyDuringUpdate = *g_pBusy; std::strcat(g_seq, "U"); return g_result; } };
struct CMap { bool LockedFromMemoryRelocation; CBlocks* FileBlockManager; long Map; CElement QuadTree; unsigned char AreaUpdateList[0xc]; CPalette CollectionTypePalette; CMap() : LockedFromMemoryRelocation(false) { g_pBusy = &LockedFromMemoryRelocation; } bool Update(double t) { LockedFromMemoryRelocation = true; bool u = QuadTree.Update(FileBlockManager, CollectionTypePalette, t); if (FileBlockManager) FileBlockManager->DeleteUnusedBlocks(); LockedFromMemoryRelocation = false; return u; } };

int main() {
    CMap m; CBlocks f; m.FileBlockManager = &f; g_result = true; if (!m.Update(2.5) || m.LockedFromMemoryRelocation || g_list != &f || g_rec != &m.CollectionTypePalette || g_time != 2.5 || !g_busyDuringUpdate || std::strcmp(g_seq, "UD")) { std::printf("00bded90_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; m.FileBlockManager = 0; g_result = false; if (m.Update(1.0) || std::strcmp(g_seq, "U")) { std::printf("00bded90_TEST FAIL\n"); return 1; }
    std::printf("00bded90_TEST PASS\n"); return 0;
}
