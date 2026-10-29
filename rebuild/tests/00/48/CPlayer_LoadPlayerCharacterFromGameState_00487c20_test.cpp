// Behaviour test for CPlayer::LoadPlayerCharacterFromGameState (0x00487C20).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
class CCharString { public: const char* Text; };
struct CPlayerSave { unsigned long long_lo, long_hi; CCharString RegionName; };
class C3DVector { public: float X, Y, Z; };
class CBaseObject { public: int id; };
class CThing : public CBaseObject { public: C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
static CThing g_thing;
static i64 g_uidWanted;
static char g_log[256];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CThingManager { public: CThing* GetPThingWithUID(i64 uid) { LOG("uid"); return uid == g_uidWanted ? &g_thing : 0; } };
class CWorldMap { public:
    long region, loaded, player; const C3DVector* pos;
    long GetRegionNumberFromName(const CCharString& name) const { return std::strcmp(name.Text, "Oakvale") == 0 ? 7 : 0; }
    void LoadRegion(long r, bool keep, bool now) { LOG("load"); loaded = (r == 7 && !keep && now) ? 1 : -1; }
    void SetPlayerPos(long p, const C3DVector& position, long loadType) { LOG("pos"); player = loadType == 0 ? p : -1; pos = &position; }
};
struct CWorldCalls {
    CThingManager tm; CWorldMap wm;
    virtual CThingManager* GetThingManager() { return &tm; }
    virtual CWorldMap* GetWorldMap() { return &wm; }
    virtual ~CWorldCalls() {}
};
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    ~CBaseIntelligentPointer() {}
    void SetPItem(CBaseObject* item) { PItem = item; }
    CBaseObject* GetPItem() const { return PItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CThingPlayerCreature;
class CPlayer { public:
    CWorldCalls* World; long Number; CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;
    CWorldCalls* GetWorld() { return World; }
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPointer() { return PlayerCharacter; }
    bool LoadPlayerCharacterFromGameState(const CPlayerSave& save)
    {
        i64 uid = (i64)save.long_lo | ((i64)save.long_hi << 32);
        long region = GetWorld()->GetWorldMap()->GetRegionNumberFromName(save.RegionName);
        if (region != 0) {
            GetWorld()->GetWorldMap()->LoadRegion(region, false, true);
            CThing* thing = GetWorld()->GetThingManager()->GetPThingWithUID(uid);
            if (thing != 0) {
                GetWorld()->GetWorldMap()->SetPlayerPos(Number, thing->GetPosition(), 0);
                PlayerCharacterPointer() = CIntelligentPointer<CThingPlayerCreature>((CThingPlayerCreature*)thing);
                return true;
            }
        }
        return false;
    }
};
int main()
{
    CWorldCalls world; std::memset(&world.wm, 0, sizeof(world.wm));
    CPlayer p; p.World = &world; p.Number = 2;
    CPlayerSave s; s.long_lo = 5; s.long_hi = 1; s.RegionName.Text = "Nowhere";
    g_uidWanted = ((i64)1 << 32) | 5;
    g_log[0] = 0;
    if (p.LoadPlayerCharacterFromGameState(s) || g_log[0] != 0 || p.PlayerCharacter.PItem != 0) { std::printf("00487c20_TEST FAIL\n"); return 1; }
    s.RegionName.Text = "Oakvale"; s.long_lo = 6;
    if (p.LoadPlayerCharacterFromGameState(s) || std::strcmp(g_log, "load;uid;") != 0 || p.PlayerCharacter.PItem != 0) { std::printf("00487c20_TEST FAIL\n"); return 1; }
    s.long_lo = 5; g_log[0] = 0;
    if (!p.LoadPlayerCharacterFromGameState(s) || std::strcmp(g_log, "load;uid;pos;") != 0) { std::printf("00487c20_TEST FAIL\n"); return 1; }
    if (p.PlayerCharacter.PItem != &g_thing || world.wm.player != 2 || world.wm.pos != &g_thing.Pos || world.wm.loaded != 1) { std::printf("00487c20_TEST FAIL\n"); return 1; }
    std::printf("00487c20_TEST PASS\n");
    return 0;
}