#include <cstdio>
static int g_addref = 0, g_reset = 0;
struct CViewBase { virtual void v00() { } virtual void AddRef() { ++g_addref; } };
struct CDisplayViewManager { CViewBase* PCurrentView_Data; void* PCurrentView_Info; void ResetCurrentView(CViewBase* v) { ++g_reset; PCurrentView_Data = v; } void SetView(CViewBase* v) { v->AddRef(); ResetCurrentView(v); } };
int main() { CViewBase v; CDisplayViewManager m; m.PCurrentView_Data = 0; m.SetView(&v); if (g_addref != 1 || g_reset != 1 || m.PCurrentView_Data != &v) { std::printf("00640050_TEST FAIL\n"); return 1; } std::printf("00640050_TEST PASS\n"); return 0; }