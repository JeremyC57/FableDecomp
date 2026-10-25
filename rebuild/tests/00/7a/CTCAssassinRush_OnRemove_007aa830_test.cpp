// Behaviour test for CTCAssassinRush::OnRemove (0x007AA830): links the reconstructed object; FinalCleanup and the other
// members are stubs that record which object was cleaned up.
#include <cstdio>
static void* g_cleaned = 0;
static int g_calls = 0;
class CTCAssassinRush {
public:
    CTCAssassinRush() {}
    virtual ~CTCAssassinRush();
    virtual void OnRemove();
    virtual void OnKill();
private:
    void FinalCleanup();
};
CTCAssassinRush::~CTCAssassinRush() {}
void CTCAssassinRush::FinalCleanup() { g_cleaned = this; ++g_calls; }
void CTCAssassinRush::OnKill() {}
int main()
{
    CTCAssassinRush a, b;
    a.OnRemove();
    if (g_cleaned != &a || g_calls != 1) { std::printf("007aa830_TEST FAIL 1\n"); return 1; }
    CTCAssassinRush* p = &b;
    p->OnRemove();                                // through the vtable
    if (g_cleaned != &b || g_calls != 2) { std::printf("007aa830_TEST FAIL 2\n"); return 1; }
    std::printf("007aa830_TEST PASS\n");
    return 0;
}
