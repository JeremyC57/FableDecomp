#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_event = 0;
struct IP { virtual void v00() { } }; struct CObserver { virtual void a00() { } unsigned char _pad[0x10]; void ObserveEvent(long e) { g_event = e; std::strcat(g_seq, "O"); } }; struct IB { virtual void b00() { } };
static long g_sprites = 0; static void* g_def = 0; struct CSprite : IP, CObserver, IB { CSprite(void* d) { ++g_sprites; g_def = d; } ~CSprite() { --g_sprites; } };
namespace NUISystem { struct CMouseCursor; } static CObserver* g_observed = 0;
struct CFrontEndManager { virtual void v000() { } virtual void v001() { } virtual void AddObserver(CObserver* o) { g_observed = o; std::strcat(g_seq, "A"); } virtual void v003() { } virtual void v004() { } virtual void RemoveObserver(CObserver* o) { g_observed = o; std::strcat(g_seq, "R"); }
    unsigned char _pad[0xb4]; NUISystem::CMouseCursor* m_pMouseCursor; static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; } };
namespace NUISystem { struct CMouseCursor : CSprite { virtual void v00() { } virtual void a00() { } virtual void b00() { } CMouseCursor(void* d) : CSprite(d) { CFrontEndManager::GetInstance()->AddObserver(this); ObserveEvent(0x19); CFrontEndManager::GetInstance()->m_pMouseCursor = this; } }; }

int main() {
    int def; NUISystem::CMouseCursor* c = new NUISystem::CMouseCursor(&def); if (g_sprites != 1 || g_def != &def || g_observed != (CObserver*)c || g_event != 0x19 || CFrontEndManager::GetInstance()->m_pMouseCursor != c || std::strcmp(g_seq, "AO")) { std::printf("0055c650_TEST FAIL\n"); return 1; }
    std::printf("0055c650_TEST PASS\n"); return 0;
}