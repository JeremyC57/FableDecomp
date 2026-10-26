#include <cstdio>
#include <cstdlib>
#include <cstring>
class C3DVector { public: float X, Y, Z; }; class C2DVector { public: float X, Y; C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { } }; struct CNode { unsigned char p[0xf]; bool off; }; static CNode* g_node = 0; static float g_x, g_y; static long g_layer = -1; static long g_calls = 0;
struct CMgr { CNode* GetNodeAt(const C2DVector& p, bool layer) { g_x = p.X; g_y = p.Y; g_layer = layer; ++g_calls; return g_node; } };
struct CFlyer { virtual void v0() { } unsigned char _pad[0x18]; CMgr* mgr; virtual bool Clear(const C3DVector& a, const C3DVector& b) const { CMgr* m = mgr; if (m->GetNodeAt(a, false) && m->GetNodeAt(b, false)) return true; return false; } };

int main() {
    CNode n; CMgr m; CFlyer* f = new CFlyer; f->mgr = &m; C3DVector a = {1, 2, 3}, b = {4, 5, 6}; g_node = &n; if (!f->Clear(a, b) || g_calls != 2 || g_x != 4) { std::printf("00a19a30_TEST FAIL\n"); return 1; } g_node = 0; if (f->Clear(a, b) || g_calls != 3 || g_x != 1) { std::printf("00a19a30_TEST FAIL\n"); return 1; } delete f;
    std::printf("00a19a30_TEST PASS\n"); return 0;
}