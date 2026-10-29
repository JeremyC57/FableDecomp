#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; }; struct IComponentB { virtual void vb00() { } }; struct CComponentRoot { virtual void v000() { } };
static long g_owner = 0; static IComponentA* g_registered = 0;
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB { unsigned char _body[0x15c - 0x1c]; CChangingStateComponent(long owner) { std::strcat(g_seq, "C"); g_owner = owner; } };
namespace NUISystem { class CManager { public: virtual void v000() { } virtual void v001() { } virtual void RegisterHoverable(IComponentA* c) { std::strcat(g_seq, "R"); g_registered = c; } static CManager* __fastcall GetInstance() { static CManager m; return &m; } }; }
struct CHoverable : CChangingStateComponent { virtual void v00() { } virtual void va00() { } virtual void vb00() { } long m_15c; bool m_160; long m_164; long m_168;
    CHoverable(long owner) : CChangingStateComponent(owner), m_15c(0), m_160(false), m_164(0), m_168(0) { NUISystem::CManager::GetInstance()->RegisterHoverable(this); } };

int main() {
    CHoverable* h = new CHoverable(42); if (std::strcmp(g_seq, "CR") || g_owner != 42 || g_registered != (IComponentA*)h || h->m_15c || h->m_160 || h->m_164 || h->m_168) { std::printf("0055ba20_TEST FAIL\n"); return 1; }
    std::printf("0055ba20_TEST PASS\n"); return 0;
}