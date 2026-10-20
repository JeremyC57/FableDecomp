// Behaviour test for 00449f90: the model below is a copy of src/00449f90.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __thiscall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
struct C3DVector;
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
inline CCharString EmptyString()
{
    CCharString empty;
    return empty;
}

struct CPersistContext {
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);             // 0x004109A0
};

struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                          // +0x18
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CThingPlayerCreature : CThing { };

struct CRegion {
    unsigned char _pad00[0x18];
    CCharString   Name;                                 // +0x18
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& position) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;  // 0x004FC190
    CRegion& GetRegion(long region);                    // 0x004FC180
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* PeekWorldMap();                  // slot 13 (+0x34)
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                               // +0x24 scoped_ptr<CWorld>
};

struct CPlayer {
    unsigned char _pad00[0x34];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    CThingPlayerCreature* GetPlayerCharacter() const { return PlayerCharacter.GetPItem(); }
};

namespace NPlayerManager {
struct CPlayerSave {
    unsigned __int64 CharacterUID;                      // +0x00
    CCharString      RegionName;                        // +0x08
};
}

struct CPlayerManager {
    void*               __vftable;
    CMainGameComponent* Component;                      // +0x04
    void*               DefinitionManager;              // +0x08
    unsigned char       Players[0x0c];                  // +0x0c vector<CPlayer*>
    long                PlayerNeutral;                  // +0x18
    long                MainPlayer;                     // +0x1c
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    void SaveGameState(CPersistContext& context);
};

void CPlayerManager::SaveGameState(CPersistContext& context)
{
    CThing* character = GetPlayer(MainPlayer)->GetPlayerCharacter();
    NPlayerManager::CPlayerSave save;
    save.CharacterUID = character->UniqueID;
    const C3DVector& position = character->GetPosition();
    CWorldMap* worldMap = Component->PWorld->PeekWorldMap();
    save.RegionName = worldMap->GetRegion(worldMap->GetRegionNumberMapIsIn(worldMap->GetMapNumberAt(position))).Name;
    context.Transfer("PlayerCharacterUID", save.CharacterUID, 0);
    context.Transfer("PlayerRegionName", save.RegionName, EmptyString());
}

// ---- test doubles and driver ----
// CCharString doubles: PStringData points at a static C string (no allocation)
static int g_live = 0;
CCharString::CCharString() : PStringData((void*)"") { ++g_live; }
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const CCharString& o) { PStringData = o.PStringData; return *this; }
struct C3DVector { float X, Y, Z; };
static const char* g_names[2]; static unsigned __int64 g_uid = 0; static unsigned __int64 g_uidDefault = 1; static const char* g_region = 0;
static int g_regionDefaultEmpty = 0; static int g_transfers = 0;
void CPersistContext::Transfer(const char* name, unsigned __int64& v, const unsigned __int64& d) { g_names[0] = name; g_uid = v; g_uidDefault = d; ++g_transfers; }
void CPersistContext::Transfer(const char* name, CCharString& v, const CCharString& d) { g_names[1] = name; g_region = (const char*)v.PStringData; g_regionDefaultEmpty = (d.PStringData && !*(const char*)d.PStringData); ++g_transfers; }
static C3DVector g_pos = { 1, 2, 3 }; static const C3DVector* g_seenPos = 0; static long g_seenMap = -1, g_seenRegion = -1;
const C3DVector& CThing::GetPosition() const { return g_pos; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::v00() {}
static CRegion g_regionObj;
struct CMap : CWorldMap { long GetMapNumberAt(const C3DVector& p) const { g_seenPos = &p; return 7; } };
long CWorldMap::GetMapNumberAt(const C3DVector&) const { return -1; }
long CWorldMap::GetRegionNumberMapIsIn(long m) const { g_seenMap = m; return 4; }
CRegion& CWorldMap::GetRegion(long r) { g_seenRegion = r; return g_regionObj; }
static CMap g_map;
struct CW : CWorld { CWorldMap* PeekWorldMap() { return &g_map; } };
CWorldMap* CWorld::PeekWorldMap() { return 0; }
#define V(n) void CWorldMap::v##n() {} void CWorld::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12)
void CWorldMap::v13() {} void CWorldMap::v14() {} void CWorldMap::v15() {}
static CPlayer g_player; static long g_askedPlayer = -1;
CPlayer* CPlayerManager::GetPlayer(long n) { g_askedPlayer = n; return &g_player; }
int main()
{
    static CThingPlayerCreature hero; hero.UniqueID = 0x123456789ULL;
    g_player.PlayerCharacter.PItem = (CBaseObject*)&hero;
    g_regionObj.Name.PStringData = (void*)"Oakvale";
    static CW world; static CMainGameComponent mgc; mgc.PWorld = &world;
    static CPlayerManager pm; pm.Component = &mgc; pm.MainPlayer = 2;
    CPersistContext ctx;
    int live0 = g_live;
    pm.SaveGameState(ctx);
    bool ok = true;
    if (g_transfers != 2 || g_askedPlayer != 2 || g_uid != 0x123456789ULL || g_uidDefault != 0) ok = false;
    if (std::strcmp(g_names[0], "PlayerCharacterUID") || std::strcmp(g_names[1], "PlayerRegionName")) ok = false;
    if (g_seenPos != &g_pos || g_seenMap != 7 || g_seenRegion != 4 || !g_region || std::strcmp(g_region, "Oakvale")) ok = false;
    if (!g_regionDefaultEmpty || g_live != live0) ok = false;
    if (!ok) { std::printf("00449f90_TEST FAIL\n"); return 1; }
    std::printf("00449f90_TEST PASS\n");
    return 0;
}
