#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; }; struct C2DVector { float X, Y; C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { } }; struct CNode { unsigned char p[0xf]; bool off; }; static CNode* g_node = 0; static float g_x, g_y; static long g_layer = -1; static long g_calls = 0;
struct CMgr { CNode* GetNodeAt(const C2DVector& p, bool layer) { g_x = p.X; g_y = p.Y; g_layer = layer; ++g_calls; return g_node; } };
struct CFlyer { virtual void v0() { } unsigned char _pad[0x18]; CMgr* mgr; virtual bool Can(const C3DVector&, const C3DVector& to, float, bool) const { CNode* n = mgr->GetNodeAt(to, false); if (n && !n->off) return true; return false; } };

int main() {
    CNode n; n.off = false; CMgr m; CFlyer* f = new CFlyer; f->mgr = &m; C3DVector a = {1, 2, 3}, b = {4, 5, 6}; g_node = &n; if (!f->Can(a, b, 1.0f, true) || g_x != 4 || g_y != 5 || g_layer != 0) { std::printf("00a199f0_TEST FAIL\n"); return 1; } n.off = true; if (f->Can(a, b, 1.0f, true)) { std::printf("00a199f0_TEST FAIL\n"); return 1; } g_node = 0; if (f->Can(a, b, 1.0f, true)) { std::printf("00a199f0_TEST FAIL\n"); return 1; } delete f;
    std::printf("00a199f0_TEST PASS\n"); return 0;
}
