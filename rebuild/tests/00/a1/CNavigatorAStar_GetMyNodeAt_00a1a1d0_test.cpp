#include <cstdio>
#include <cstdlib>
#include <cstring>
class C3DVector { public: float X, Y, Z; }; class C2DVector { public: float X, Y; C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { } }; struct CNode { unsigned char p[0xf]; bool off; }; static CNode* g_node = 0; static float g_x, g_y; static long g_layer = -1; static long g_calls = 0;
struct CMgr { CNode* GetNodeAt(const C2DVector& p, bool layer) { g_x = p.X; g_y = p.Y; g_layer = layer; ++g_calls; return g_node; } };
static bool g_lay = false; static const C3DVector* g_owner_pos = 0; struct COwner { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual void v012() { } virtual void v013() { } virtual void v014() { } virtual void v015() { } virtual bool GetLayerAt(const C3DVector* p) { g_owner_pos = p; return g_lay; } }; struct CAStar { unsigned char _pad[0x14]; COwner* owner; long m_18; CMgr* mgr; CNode* GetMyNodeAt(const C3DVector* p) { return mgr->GetNodeAt(*p, owner->GetLayerAt(p)); } };

int main() {
    CNode n; CMgr m; COwner o; CAStar* s = new CAStar; s->owner = &o; s->mgr = &m; C3DVector p = {7, 8, 9}; g_node = &n; g_lay = true; if (s->GetMyNodeAt(&p) != &n || g_x != 7 || g_y != 8 || g_layer != 1 || g_owner_pos != &p) { std::printf("00a1a1d0_TEST FAIL\n"); return 1; } g_lay = false; if (s->GetMyNodeAt(&p) != &n || g_layer != 0) { std::printf("00a1a1d0_TEST FAIL\n"); return 1; } delete s;
    std::printf("00a1a1d0_TEST PASS\n"); return 0;
}