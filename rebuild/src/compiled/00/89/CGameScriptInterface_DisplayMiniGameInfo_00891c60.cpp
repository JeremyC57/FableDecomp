struct Target {
    void Show(int display, int gameType, int zero);
};
struct Mgr {
    unsigned char _pad[0x1c8];
    Target* target; // +0x1c8
};
extern Mgr* g_mgr;

void __stdcall CGameScriptInterface_DisplayMiniGameInfo(int display, int gameType)
{
    g_mgr->target->Show(display, gameType, 0);
}