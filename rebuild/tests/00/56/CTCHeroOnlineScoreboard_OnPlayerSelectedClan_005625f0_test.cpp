// Behaviour test for CTCHeroOnlineScoreboard::OnPlayerSelectedClan (retail 0x005625F0): model copy + stub callees.
#include <cstdio>

struct CTCBase;
struct CWideString {
    int id;
    CWideString(int i) : id(i) {}
    CWideString(const CWideString& o) : id(o.id) {}
    ~CWideString() {}
    CWideString& operator=(const CWideString& o) { id = o.id; return *this; }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NUISystem {
struct CComponent {
    bool active; int stopped; long lastStopped; unsigned char fadeR, fadeA; float fadeTime;
    CComponent* first; CComponent* second;
    void StopObservingEvent(long e) { ++stopped; lastStopped = e; }
    void FadeColour(const CRGBColour& c, float t, long) { fadeR = c.R; fadeA = c.A; fadeTime = t; }
    void SetActive_slot48(bool a) { active = a; }
    CComponent* GetFirstButton() { return first; }
    CComponent* GetSecondButton() { return second; }
};
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long wanted) { return (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) != 0; }
    template<class T> T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CTCInventory { NUISystem::CComponent* PMouse_Pointer; };
static int g_erased = 0;
struct SScoreboardNode;
struct CClanScoreboardMap {
    struct SHeader { long _M_color; SScoreboardNode* _M_parent; SHeader* _M_left; SHeader* _M_right; };
    SHeader* _M_header; unsigned long _M_node_count;
    void _M_erase(SScoreboardNode*) { ++g_erased; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
struct CTCHeroOnlineScoreboard {
    CThing* ParentThing;
    CWideString SelectedClan;
    CClanScoreboardMap DownloadedClanScoreboard;
    long ScoreboardState;
    NUISystem::CComponent* PScoreboardScreen;
    NUISystem::CComponent* PClanMenuEntry;
    NUISystem::CComponent* PConnectingMessage1;
    CTCHeroOnlineScoreboard() : SelectedClan(0) {}
    void OnPlayerSelectedClan(CWideString clanName, NUISystem::CComponent* clanMenuEntry);
};
void CTCHeroOnlineScoreboard::OnPlayerSelectedClan(CWideString clanName, NUISystem::CComponent* clanMenuEntry)
{
    if (PClanMenuEntry == 0) {
        SelectedClan = clanName;
        PConnectingMessage1->SetActive_slot48(true);
        ScoreboardState = 6;
        DownloadedClanScoreboard.clear();
        PClanMenuEntry = clanMenuEntry;
        NUISystem::CComponent* screen = PScoreboardScreen;
        screen->GetFirstButton()->StopObservingEvent(0x19);
        screen->GetSecondButton()->StopObservingEvent(0x19);
        CThing* thing = ParentThing;
        if (thing->HasTC(0x11)) {
            CTCInventory* inventory = thing->GetTC<CTCInventory>(0x11);
            inventory->PMouse_Pointer->StopObservingEvent(0x19);
            inventory->PMouse_Pointer->FadeColour(CRGBColour(255, 255, 255, 0), 0.1f, 0);
        }
    }
}
int main()
{
    int fail = 0;
    NUISystem::CComponent b1 = { false, 0, 0, 0, 0, 0, 0, 0 }, b2 = b1, pointer = b1, msg = b1, entry = b1;
    NUISystem::CComponent screen = b1; screen.first = &b1; screen.second = &b2;
    CTCInventory inv = { &pointer };
    CInterfaceMap::SEntry entries[1] = { { 0x11, (CTCBase*)&inv } };
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0; hero.m_presentBits[0] = 1UL << 0x11;
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1;
    CClanScoreboardMap::SHeader header = { 0, (SScoreboardNode*)0x10, 0, 0 };
    CTCHeroOnlineScoreboard sb; sb.ParentThing = &hero; sb.DownloadedClanScoreboard._M_header = &header; sb.DownloadedClanScoreboard._M_node_count = 3;
    sb.ScoreboardState = 0; sb.PScoreboardScreen = &screen; sb.PClanMenuEntry = 0; sb.PConnectingMessage1 = &msg;
    sb.OnPlayerSelectedClan(CWideString(42), &entry);
    if (sb.SelectedClan.id != 42 || !msg.active || sb.ScoreboardState != 6 || sb.PClanMenuEntry != &entry) fail = 1;
    if (g_erased != 1 || sb.DownloadedClanScoreboard._M_node_count != 0 || header._M_left != &header || header._M_parent != 0) fail = 2;
    if (b1.stopped != 1 || b2.lastStopped != 0x19 || pointer.stopped != 1 || pointer.fadeR != 255 || pointer.fadeA != 0 || pointer.fadeTime != 0.1f) fail = 3;
    sb.OnPlayerSelectedClan(CWideString(7), 0);   // already pending: ignored
    if (sb.SelectedClan.id != 42 || b1.stopped != 1) fail = 4;
    if (fail) { std::printf("005625f0_TEST FAIL\n"); return 1; }
    std::printf("005625f0_TEST PASS\n");
    return 0;
}