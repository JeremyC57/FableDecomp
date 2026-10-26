#include <cstdio>
class CPlayerGui { public: int x; }; static CPlayerGui* g_gui = 0; static long g_reserve = 0; static int g_lists = 0;
struct CRetailAllocator { CRetailAllocator() { } };
struct CRetailList { void* head; CRetailList(const CRetailAllocator&) : head(0) { ++g_lists; } };
namespace NPlayerGui {
struct CRetailHandleVector { void* m_pFirst; void* m_pLast; void* m_pEnd; void Reserve(long n) { g_reserve = n; } };
class CDrawBase { public: virtual void v00() { } unsigned char _body[0x1c - 4]; CRetailHandleVector PrimitiveHandles; CDrawBase(CPlayerGui* g, void* l) { g_gui = g; } };
class CDrawPerceivers : public CDrawBase { public: long m_counterA, m_counterB; float m_progress, m_scale; long m_state; CRetailList m_entries;
    CDrawPerceivers(CPlayerGui* g, void* l) : CDrawBase(g, l), m_counterA(0), m_counterB(0), m_progress(0.0f), m_scale(1.0f), m_state(0), m_entries(CRetailAllocator()) { PrimitiveHandles.Reserve(3); } }; }
int main() { CPlayerGui gui; int list; NPlayerGui::CDrawPerceivers d(&gui, &list); if (g_gui != &gui || d.m_counterA || d.m_counterB || d.m_progress != 0.0f || d.m_scale != 1.0f || d.m_state || g_lists != 1 || g_reserve != 3) { std::printf("0064ed06_TEST FAIL\n"); return 1; } std::printf("0064ed06_TEST PASS\n"); return 0; }