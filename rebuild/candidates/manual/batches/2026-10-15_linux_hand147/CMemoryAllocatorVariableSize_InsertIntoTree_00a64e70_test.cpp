#include <cstdio>
static unsigned int g_align = 8, g_shift = 3;
#define ALIGNED_SIZE(size) ((((size) + g_align - 1) >> g_shift) << g_shift)
struct Chunk;
struct Area { Area* next; Area* right; Area* prev; Area* parent; Area* left; Chunk* chunk; long size; unsigned char colour; };
struct Alloc;
struct Chunk { long total, nFree, nAlloc, freeSize, largest, smallest; Alloc* owner; bool permanent, demand; Chunk* nextC; Chunk* prevC; Area* firstFree; Area* lastFree; Area* firstAlloc; };
static int g_calls = 0; static Area* g_lastArea = 0;
struct Alloc { Area* root;
    void InsertFixup(Area* a) { ++g_calls; g_lastArea = a; }
    void Insert(Area* rootArg, Area* area) {
        Area* parent = 0; bool left = false; Area* node = rootArg;
        while (node) { parent = node;
            if (area->size == node->size) { if (area < node) { node = node->left; left = true; } else { node = node->right; left = false; } }
            else if (area->size < node->size) { node = node->left; left = true; } else { node = node->right; left = false; } }
        area->parent = parent; if (parent == 0) root = area; else if (left) parent->left = area; else parent->right = area;
        area->colour = 0; InsertFixup(area); } };
int main()
{
    Area n[4]; for (int i = 0; i < 4; ++i) { n[i] = Area(); n[i].colour = 1; }
    n[0].size = 50; n[1].size = 20; n[2].size = 80; n[3].size = 50;
    Alloc al; al.root = 0;
    al.Insert(al.root, &n[0]); al.Insert(al.root, &n[1]); al.Insert(al.root, &n[2]); al.Insert(al.root, &n[3]);
    if (al.root != &n[0] || n[0].left != &n[1] || n[0].right != &n[2] || n[2].left != &n[3] || n[3].parent != &n[2] || n[3].colour != 0 || g_calls != 4) { std::printf("00a64e70_TEST FAIL\n"); return 1; }
    std::printf("00a64e70_TEST PASS\n");
    return 0;
}
