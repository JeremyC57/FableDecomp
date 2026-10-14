#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; }; struct C2DVector { float X, Y; C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { } }; struct CNode { unsigned char p[0xf]; bool off; }; static CNode* g_node = 0; static float g_x, g_y; static long g_layer = -1; static long g_calls = 0;
struct CMgr { CNode* GetNodeAt(const C2DVector& p, bool layer) { g_x = p.X; g_y = p.Y; g_layer = layer; ++g_calls; return g_node; } };
struct CAStar { unsigned char _pad[0x1c]; CMgr* mgr; long IsNavigableAt(const C3DVector* p, bool layer) { CNode* n = mgr->GetNodeAt(*p, layer); if (n && !n->off) return 1; return 0; } };

int main() {
    CNode n; n.off = false; CMgr m; CAStar* s = new CAStar; s->mgr = &m; C3DVector p = {7, 8, 9}; g_node = &n; if (s->IsNavigableAt(&p, true) != 1 || g_x != 7 || g_layer != 1) { std::printf("00a1a210_TEST FAIL\n"); return 1; } n.off = true; if (s->IsNavigableAt(&p, false) != 0 || g_layer != 0) { std::printf("00a1a210_TEST FAIL\n"); return 1; } g_node = 0; if (s->IsNavigableAt(&p, false) != 0) { std::printf("00a1a210_TEST FAIL\n"); return 1; } delete s;
    std::printf("00a1a210_TEST PASS\n"); return 0;
}
