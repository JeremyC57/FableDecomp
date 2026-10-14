#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_node = 0;
struct C2DVector { float X, Y; }; static long g_layer = -1; static const C2DVector* g_pos = 0; struct CTree { void* GetNodeAt(const C2DVector* p, long layer) { g_pos = p; g_layer = layer; return g_node; } }; struct SNode { SNode* next; SNode* prev; CTree* tree; long c, id, minX, minY, maxX, maxY; };
struct CMgr { unsigned char _pad[0x24]; SNode* maps; void* Get(const C2DVector& p, bool layer) { for (SNode* n = maps->next; n != maps; n = n->next) { long x = (long)p.X, y = (long)p.Y; if (x >= n->minX && x < n->maxX && y >= n->minY && y < n->maxY) return n->tree->GetNodeAt(&p, layer); } return 0; } };

int main() {
    SNode head, n1; CTree t; int node; g_node = &node; head.next = &n1; n1.next = &head; n1.tree = &t; n1.minX = 0; n1.minY = 0; n1.maxX = 10; n1.maxY = 10; CMgr* m = new CMgr; m->maps = &head; C2DVector p = {3.7f, 9.9f};
    if (m->Get(p, true) != &node || g_pos != &p || g_layer != 1) { std::printf("00a14f70_TEST FAIL\n"); return 1; } p.X = 10.0f; if (m->Get(p, false) != 0) { std::printf("00a14f70_TEST FAIL\n"); return 1; } p.X = -0.5f; p.Y = 2; if (m->Get(p, false) != &node) { std::printf("00a14f70_TEST FAIL\n"); return 1; } delete m;
    std::printf("00a14f70_TEST PASS\n"); return 0;
}
