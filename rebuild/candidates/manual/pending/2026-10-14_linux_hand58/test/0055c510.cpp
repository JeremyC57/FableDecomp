#include <cstdio>
#include <cstring>
#include <cstdlib>

static char g_seq[64] = ""; static void* g_removed = 0;
struct IPrimary { virtual ~IPrimary() { } };
struct IListener { virtual void a00() { } unsigned char pad[0x10]; };
struct ISecondary { virtual void b00() { } };
struct CSprite : IPrimary, IListener, ISecondary { unsigned char pad[0x18]; ~CSprite() { std::strcat(g_seq, "S"); } };
struct CCursor;
struct CManager { virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { } virtual void v4() { } virtual void RemoveListener(IListener* l) { std::strcat(g_seq, "R"); g_removed = l; } unsigned char pad[0xb4]; CCursor* cursor;
    static CManager* GetInstance() { static CManager m; return &m; } };
struct CCursor : CSprite { virtual ~CCursor() { CManager::GetInstance()->RemoveListener(this); CManager::GetInstance()->cursor = 0; } };
int main()
{
    CCursor* c = new CCursor; CManager::GetInstance()->cursor = c; IListener* expect = c;
    delete c;
    if (std::strcmp(g_seq, "RS") || g_removed != expect || CManager::GetInstance()->cursor != 0) { std::printf("0055c510_TEST FAIL\n"); return 1; }
    std::printf("0055c510_TEST PASS\n");
    return 0;
}
