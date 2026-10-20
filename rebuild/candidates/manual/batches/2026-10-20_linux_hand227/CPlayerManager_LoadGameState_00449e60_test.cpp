// Behaviour test for CPlayerManager::LoadGameState (0x00449E60): joystick-associated players other than the main
// one are uninitialised; the save record's UID and region are read with 0 / "" defaults; the main player (found by id,
// else the neutral player) is initialised from that record and its number is given to the joystick manager.
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif
static int g_live = 0;
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCharString {
    char Text[32];
    CCharString() { Text[0] = 0; ++g_live; }
    CCharString(const char* t, long = -1) { std::strcpy(Text, t); ++g_live; }
    ~CCharString() { --g_live; }
};
struct CPlayerSave { U64 CharacterUID; CCharString RegionName; };
struct CPersistContext {
    void Transfer(const char* name, U64& value, const U64& def) { Log(name); value = def + 0x1234; }
    void Transfer(const char* name, CCharString& value, const CCharString& def) { Log(name); std::strcpy(value.Text, def.Text); std::strcat(value.Text, "Oakvale"); }
};
static U64 g_uid = 0; static char g_region[32];
struct CPlayer {
    long PlayerID; long Number;
    void UninitCharacter() { char b[16]; std::sprintf(b, "uninit%ld", PlayerID); Log(b); }
    bool InitCharacterFromSave(CPlayerSave* s) { g_uid = s->CharacterUID; std::strcpy(g_region, s->RegionName.Text); char b[16]; std::sprintf(b, "init%ld", PlayerID); Log(b); return true; }
    long GetJoystickDeviceNumber() const { return Number; }
};
static long g_device = -1;
struct CGameJoystickManager {
    static CGameJoystickManager* Get() { static CGameJoystickManager m; return &m; }
    void SetMainPlayerJoystickDeviceNumber(long d) { g_device = d; }
};
template <class T> struct vector { T* _M_start; T* _M_finish;
    long size() const { return _M_finish - _M_start; } T& operator[](long n) { return *(_M_start + n); } };
struct CPlayerManager {
    vector<CPlayer*> Players; long PlayerNeutral; long MainPlayer;
    bool IsPlayerAssociatedWithJoystickDeviceNumber(long d) { return d == 0 || d == 2; }
    CPlayer* GetPlayer(long id) { return FindPlayer(id); }
    CPlayer* FindPlayer(long id)
    {
        long count = Players.size();
        for (long i = 0; i < count; ++i)
            if (Players[i]->PlayerID == id)
                return Players[i];
        return Players[PlayerNeutral];
    }
    void LoadGameState(CPersistContext& context);
};
void CPlayerManager::LoadGameState(CPersistContext& context)
{
    for (long i = 0; i < 4; ++i) {
        if (IsPlayerAssociatedWithJoystickDeviceNumber(i) && MainPlayer != i)
            GetPlayer(i)->UninitCharacter();
    }
    CPlayerSave save;
    context.Transfer("PlayerCharacterUID", save.CharacterUID, (U64)0);
    context.Transfer("PlayerRegionName", save.RegionName, "");
    FindPlayer(MainPlayer)->InitCharacterFromSave(&save);
    CGameJoystickManager::Get()->SetMainPlayerJoystickDeviceNumber(FindPlayer(MainPlayer)->GetJoystickDeviceNumber());
}
int main()
{
    int bad = 0;
    CPlayer p0 = { 0, 10 }, p1 = { 1, 11 }, p2 = { 2, 12 };
    CPlayer* list[3] = { &p0, &p1, &p2 };
    CPlayerManager m; m.Players._M_start = list; m.Players._M_finish = list + 3; m.PlayerNeutral = 1; m.MainPlayer = 2;
    CPersistContext ctx; g_log[0] = 0;
    m.LoadGameState(ctx);
    if (std::strcmp(g_log, "uninit0;PlayerCharacterUID;PlayerRegionName;init2;") != 0) bad = 1;
    if (g_uid != 0x1234 || std::strcmp(g_region, "Oakvale") != 0 || g_device != 12 || g_live != 0) bad = 1;
    // unknown main player id: the neutral player is used
    m.MainPlayer = 7; g_log[0] = 0;
    m.LoadGameState(ctx);
    if (std::strcmp(g_log, "uninit0;uninit2;PlayerCharacterUID;PlayerRegionName;init1;") != 0 || g_device != 11) bad = 1;
    if (bad) { std::printf("00449e60_TEST FAIL\n"); return 1; }
    std::printf("00449e60_TEST PASS\n");
    return 0;
}
