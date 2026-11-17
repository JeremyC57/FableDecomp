// Behaviour test for 0x008A9E00 CGameScriptInterface::RemoveRumourCategory(const CCharString&) const: copy the category into a local
// key, erase that key from CTCAIScratchpad::Gossip (map<CCharString, vector<CCharString> >::erase(const key&) 0x008AE420), then
// destroy the local key.
#include <cstdio>

static int g_step = 0, g_copies = 0, g_dtors = 0, g_erases = 0;
static const void* g_erasedKey = 0;
static const char* g_erasedText = 0;

struct CCharString {
    const char* Text;
    explicit CCharString(const char* t) : Text(t) { }
    CCharString(const CCharString& o) : Text(o.Text) { ++g_copies; g_step = 1; }
    ~CCharString() { ++g_dtors; if (g_step == 2) g_step = 3; }
};
struct GossipMap {
    unsigned int erase(const CCharString& key) { ++g_erases; g_erasedKey = &key; g_erasedText = key.Text; g_step = 2; return 1; }
};
struct CTCAIScratchpad { static GossipMap Gossip; };
GossipMap CTCAIScratchpad::Gossip;

struct CGameScriptInterface {
    void RemoveRumourCategory(const CCharString& category) const
    {
        CCharString localKey(category);
        CTCAIScratchpad::Gossip.erase(localKey);
    }
};

int main()
{
    CCharString category("Bandits");
    CGameScriptInterface script;
    script.RemoveRumourCategory(category);
    bool ok = g_copies == 1 && g_erases == 1 && g_erasedKey != &category && g_erasedText == category.Text && g_dtors == 1 && g_step == 3;
    std::printf(ok ? "CGameScriptInterface_008a9e00_TEST PASS\n" : "CGameScriptInterface_008a9e00_TEST FAIL\n");
    return ok ? 0 : 1;
}
