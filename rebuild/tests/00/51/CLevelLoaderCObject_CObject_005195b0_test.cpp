// behaviour test for 0x005195B0 ~CLevelLoader::CObject: level files destroyed then freed, set cleared and its header freed
#include <cstdio>
#include <cstdlib>
static int g_levelFilesDestroyed = 0, g_erased = 0;
struct CLevelFiles { int LevelID; ~CLevelFiles() { ++g_levelFilesDestroyed; } };
struct Node { char color; Node* parent; Node* left; Node* right; };
struct CSet {
    Node* header; unsigned int count;
    void erase_subtree(Node* x) { if (x) ++g_erased; }
    void clear() { if (count != 0) { erase_subtree(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~CSet() { clear(); if (header) std::free(header); }
};
struct CVec {
    CLevelFiles* s; CLevelFiles* f;
    ~CVec() { for (CLevelFiles* p = s; p != f; ++p) p->~CLevelFiles(); if (s) std::free(s); }
};
struct CObject { CSet LoadableSections; void* PLevelLoader; CVec RegionLevelFiles; int RegionIndex; };
int main()
{
    {
        CObject o;
        o.LoadableSections.header = (Node*)std::malloc(sizeof(Node));
        o.LoadableSections.header->parent = o.LoadableSections.header; o.LoadableSections.count = 1;
        o.RegionLevelFiles.s = (CLevelFiles*)std::malloc(3 * sizeof(CLevelFiles)); o.RegionLevelFiles.f = o.RegionLevelFiles.s + 3;
    }
    if (g_levelFilesDestroyed != 3 || g_erased != 1) { std::printf("005195b0_TEST FAIL\n"); return 1; }
    std::printf("005195b0_TEST PASS\n");
    return 0;
}