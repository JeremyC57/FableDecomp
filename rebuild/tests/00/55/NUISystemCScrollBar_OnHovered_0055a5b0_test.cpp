#include <cstdio>
static int g_clickHover = 0, g_hand = 0;
struct CMouseCursor { void TurnToHand() { ++g_hand; } };
struct CFrontEndManager {
    CMouseCursor* PMouseCursor;
    static CFrontEndManager* GetInstance();
    CMouseCursor* GetMouseCursor() const { return PMouseCursor; }
};
static CFrontEndManager g_mgr;
CFrontEndManager* CFrontEndManager::GetInstance() { return &g_mgr; }
struct CClickable { void OnHovered() { ++g_clickHover; } };
struct CScrollBar : CClickable {
    void OnHovered()
    {
        CClickable::OnHovered();
        CMouseCursor* cursor = CFrontEndManager::GetInstance()->GetMouseCursor();
        if (cursor) cursor->TurnToHand();
    }
};
int main()
{
    CMouseCursor c; CScrollBar b;
    g_mgr.PMouseCursor = 0; b.OnHovered();
    if (g_clickHover != 1 || g_hand != 0) { std::printf("0055a5b0_TEST FAIL\n"); return 1; }
    g_mgr.PMouseCursor = &c; b.OnHovered();
    if (g_clickHover != 2 || g_hand != 1) { std::printf("0055a5b0_TEST FAIL\n"); return 1; }
    std::printf("0055a5b0_TEST PASS\n");
    return 0;
}