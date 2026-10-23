// Behaviour test for CPlayerManager::DrawGetEnvironmentInfo (0x0044A2A0): the text is cleared; the main player or a
// player without a hero gives false; a co-op player gives "P<n+1>:<spirit score>  " and true. (Narrow characters stand
// in for the CWideString text; the operations and their order are the retail ones.)
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CWideString {
    char Text[64];
    CWideString() { Text[0] = 0; ++g_live; }
    CWideString(const CWideString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const char* t) { std::strcpy(Text, t); return *this; }
    const CWideString& operator+=(const CWideString& o) { std::strcat(Text, o.Text); return *this; }
    const CWideString& operator+=(const char* t) { std::strcat(Text, t); return *this; }
};
static CWideString IntToWideString(long v) { CWideString s; std::sprintf(s.Text, "%ld", v); return s; }
static CWideString operator+(const char* l, const CWideString& r) { CWideString s; std::strcpy(s.Text, l); std::strcat(s.Text, r.Text); return s; }
struct CTCCoopSpirit { long Score; long GetScore() const { return Score; } };
struct CThing { bool hasSpirit; CTCCoopSpirit* spirit;
    bool HasTC(long t) const { return t == 0xc7 && hasSpirit; } CTCCoopSpirit* GetTC(long) const { return spirit; } };
struct CPlayer { CThing* hero; bool main; long number;
    CThing* GetHeroThing() { return hero; } bool IsMainPlayer() const { return main; } long GetJoystickDeviceNumber() const { return number; } };
static CPlayer g_players[2];
struct CPlayerManager {
    CPlayer* GetPlayer_00449910(long n) { return &g_players[n]; }
    bool DrawGetEnvironmentInfo(long playerNumber, CWideString& text);
};
bool CPlayerManager::DrawGetEnvironmentInfo(long playerNumber, CWideString& text)
{
    text = "";
    CPlayer* player = GetPlayer_00449910(playerNumber);
    CThing* hero = player->GetHeroThing();
    if (player->IsMainPlayer() || hero == 0)
        return false;
    text += "P" + IntToWideString(player->GetJoystickDeviceNumber() + 1);
    text += ":";
    CTCCoopSpirit* spirit = 0;
    if (hero->HasTC(0xc7))
        spirit = (CTCCoopSpirit*)hero->GetTC(0xc7);
    text += IntToWideString(spirit->GetScore());
    text += "  ";
    return true;
}
int main()
{
    int bad = 0;
    CTCCoopSpirit sp; sp.Score = 42;
    CThing h0 = { false, 0 }, h1 = { true, &sp };
    g_players[0].hero = &h0; g_players[0].main = true; g_players[0].number = 0;
    g_players[1].hero = &h1; g_players[1].main = false; g_players[1].number = 1;
    CPlayerManager pm; CWideString text; text = "old";
    int base = g_live;
    if (pm.DrawGetEnvironmentInfo(0, text) || text.Text[0]) bad = 1;
    if (!pm.DrawGetEnvironmentInfo(1, text) || std::strcmp(text.Text, "P2:42  ") != 0) bad = 1;
    g_players[1].hero = 0;
    if (pm.DrawGetEnvironmentInfo(1, text) || text.Text[0]) bad = 1;
    if (g_live != base) bad = 1;
    if (bad) { std::printf("0044a2a0_TEST FAIL\n"); return 1; }
    std::printf("0044a2a0_TEST PASS\n");
    return 0;
}