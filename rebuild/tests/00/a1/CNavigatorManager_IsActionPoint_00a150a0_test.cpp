#include <cstdio>
#include <cstdlib>
#include <cstring>
static bool g_ap = false;
class C2DVector { public: float X, Y; }; static long g_layer = -1; static const C2DVector* g_pos = 0; struct CTree { bool IsActionPoint(const C2DVector* p) { g_pos = p; return g_ap; } }; struct SNode { SNode* next; SNode* prev; CTree* tree; long c, id, minX, minY, maxX, maxY; };
struct CMgr { unsigned char _pad[0x24]; SNode* maps; bool Get(const C2DVector& p, int) { for (SNode* n = maps->next; n != maps; n = n->next) { long x = (long)p.X, y = (long)p.Y; if (x >= n->minX && x < n->maxX && y >= n->minY && y < n->maxY) return n->tree->IsActionPoint(&p); } return false; } };

int main() {
    SNode head, n1; CTree t; head.next = &n1; n1.next = &head; n1.tree = &t; n1.minX = 0; n1.minY = 0; n1.maxX = 10; n1.maxY = 10; CMgr* m = new CMgr; m->maps = &head; C2DVector p = {3.7f, 9.9f}; g_ap = true;
    if (!m->Get(p, 0) || g_pos != &p) { std::printf("00a150a0_TEST FAIL\n"); return 1; } g_ap = false; if (m->Get(p, 0)) { std::printf("00a150a0_TEST FAIL\n"); return 1; } g_ap = true; p.Y = 10.0f; if (m->Get(p, 0)) { std::printf("00a150a0_TEST FAIL\n"); return 1; } delete m;
    std::printf("00a150a0_TEST PASS\n"); return 0;
}