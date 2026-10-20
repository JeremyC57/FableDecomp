// Behaviour test for CV_BordelloScript::WatchForBordelloStatus (0x00E3A030): a scripted sequence of region-loaded
// answers and terminate requests drives the thread; leaving and re-entering the Bordello sets PlayerHasLeftRegion and
// clears HeroPartying/MagicianSleeping, and a terminate request at any wait point ends the thread.
#include <cstdio>

static int g_live = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
static const char* g_loaded;   // one char per IsRegionLoaded call: '1' loaded, '0' not
static const char* g_term;     // one char per IsActiveThreadTerminating call
static int g_frames = 0;
struct CGameScriptInterface {
    virtual void NewScriptFrame() { ++g_frames; }
    virtual bool IsRegionLoaded(const CCharString&) { char c = *g_loaded; if (c) ++g_loaded; return c == '1'; }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { char c = *g_term; if (c) ++g_term; return c != '0'; }
};
struct CV_BordelloScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    bool HeroPartying, MagicianSleeping, PlayerHasLeftRegion;
    void WatchForBordelloStatus();
};
void CV_BordelloScript::WatchForBordelloStatus()
{
    while (!IsActiveThreadTerminating()) {
        if (!m_pInterface->IsRegionLoaded("Bordello")) {
            if (IsActiveThreadTerminating())
                return;
            while (!m_pInterface->IsRegionLoaded("Bordello")) {
                m_pInterface->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            PlayerHasLeftRegion = true;
            HeroPartying = false;
            MagicianSleeping = false;
        }
        m_pInterface->NewScriptFrame();
    }
}
static int Run(const char* loaded, const char* term, bool left, int frames)
{
    CGameScriptInterface gi; CV_BordelloScript s; s.m_pInterface = &gi;
    s.HeroPartying = true; s.MagicianSleeping = true; s.PlayerHasLeftRegion = false;
    g_loaded = loaded; g_term = term; g_frames = 0;
    s.WatchForBordelloStatus();
    if (*g_term || *g_loaded || g_live != 0 || g_frames != frames) return 1;   // every scripted answer consumed
    if (s.PlayerHasLeftRegion != left || s.HeroPartying == left || s.MagicianSleeping == left) return 1;
    return 0;
}
int main()
{
    int bad = 0;
    bad |= Run("11", "001", false, 2);           // loaded twice, then terminate at the loop top
    bad |= Run("0001", "000001", true, 3);       // unloaded, waits two frames, reloaded: flags change, one more frame
    bad |= Run("00", "001", false, 1);           // terminate while waiting
    bad |= Run("0", "01", false, 0);             // terminate right after noticing the unload
    bad |= Run("01", "001", false, 0);           // reloaded at once but terminating before the flags
    if (bad) { std::printf("00e3a030_TEST FAIL\n"); return 1; }
    std::printf("00e3a030_TEST PASS\n");
    return 0;
}
