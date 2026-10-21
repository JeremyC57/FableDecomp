// Behaviour test for 005fbd90: src/005fbd90.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryMap::TeleportHeroToGuild()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryMap::TeleportHeroToGuild() -- while the hero creature is alive, find the guild teleport marker among the world's teleporters and send the hero through it
// Leaf: TeleportHeroToGuild
// Retail 0x005FBD90, 272 bytes (+ int3), esp frame (speed TU).
// Callees: CTCBase::GetPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer 0x00449960,
// CPlayer::GetControlledCreature 0x00487DD0, malloc 0x00BFEA0E (the STLport list head node), CTCBase::GetWorld
// 0x00686A70 (non-const twin of 0x00686A80), the search tools' GetAllThings<CThing, list, CThingFilter_HasTC>
// 0x00617860 (manifest "GetAllLeafThings<CThingBuilding const, vector>" is propagated: the body fills a list),
// CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0
// (__fastcall), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, TeleportHeroToTeleporter 0x005FBD20
// (landed), the out-of-line list destructor 0x006153C0 (manifest "CGuiControlTreePane::SortTreeRecursively" is a
// propagated twin), free 0x00BFEA14.
// Shape: list<CThing*> of every thing with a teleporter component (0xDD); the first whose def global index (short at
// +0x8C) equals the "HERO_GUILD_TELEPORT_MARKER" def gets the hero; the compare is taken into a bool before the
// string temporary dies (`sete bl`). The list dtor is inline at the normal exit and out of line at the early return.
// Layout: retail CThing: def index word +0x8C, state byte +0x91 bit 0 = dead.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();  // 0x0044C6B0
};

struct CThing {
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                          // +0x8c global def index
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char IsDead : 1;                         // +0x91 bit 0
};

template <class T> struct list {                      // STLport list<T>: one head-node pointer
    struct _Node { _Node* _M_next; _Node* _M_prev; T _M_data; };
    _Node* _M_node;
    struct iterator {
        _Node* _M_node;
        iterator(_Node* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    list()
    {
        _Node* head = (_Node*)malloc(sizeof(_Node));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
    void push_back(const T& x)
    {
        _Node* n = (_Node*)malloc(sizeof(_Node));
        n->_M_data = x;
        n->_M_next = _M_node;
        n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n;
        _M_node->_M_prev = n;
    }
    ~list()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        _Node* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};

struct CThingFilter_HasTC {
    long Type;                                        // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) {}
};
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasTC& filter, list<CThing*>& things);   // 0x00617860
};
CThing* FirstSearchThing();                           // stand-in callees of the stand-in body below
CThing* NextSearchThing(CThing* previous);
bool PassesFilter(const CThingFilter_HasTC& filter, CThing* thing);
// Stand-in body (the real one is 0x00617860, same TU in retail): it touches the list only inline, so VC7.1 knows the
// head pointer is not rewritten across the call (retail keeps it in esi) -- the landed 0x007FD580 idiom; review at landing.
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetAllThings(const CThingFilter_HasTC& filter, list<CThing*>& things)
{
    for (CThing* thing = FirstSearchThing(); thing; thing = NextSearchThing(thing))
        if (PassesFilter(filter, thing))
            things.push_back(thing);
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char _pad00[0x20];
    CThingSearchTools* ThingSearchTools;              // +0x20
};
struct CPlayer {
    CThing* GetControlledCreature() const;            // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                   // 0x00449960
};

struct CTCInventoryMap {
    CPlayerManager* GetPlayerManager() const;         // 0x00686D50 (CTCBase)
    CWorld* GetWorld();                               // 0x00686A70 (CTCBase)
    void TeleportHeroToTeleporter(CThing* teleporter);   // 0x005FBD20
    void TeleportHeroToGuild();
    __forceinline void GetAllThings(const CThingFilter_HasTC& filter, list<CThing*>& things)
    {
        CWorld* world = GetWorld();
        world->ThingSearchTools->GetAllThings(filter, things);
    }
};

void CTCInventoryMap::TeleportHeroToGuild()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero && !hero->IsDead) {
        list<CThing*> teleporters;
        GetAllThings(CThingFilter_HasTC(0xDD), teleporters);
        for (list<CThing*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
            CThing* teleporter = *it;
            bool isGuild = teleporter->DefIndex ==
                CGameDefinitionManager::Get().GetDefGlobalIndexFromName("HERO_GUILD_TELEPORT_MARKER");
            if (isGuild) {
                TeleportHeroToTeleporter(teleporter);
                return;
            }
        }
    }
}

// ---- stubs and driver ----

#include <string.h>
static const char* g_lastName; static int g_strings = 0, g_strDtors = 0;
CCharString::CCharString(const char* text, long) { m_pStringData = (void*)text; ++g_strings; }
CCharString::~CCharString() { ++g_strDtors; }
static long g_guildDef = 42;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_lastName = (const char*)name.m_pStringData; return g_guildDef; }
static CGameDefinitionManager g_gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
static CThing* g_things[4]; static int g_nThings = 0;
CThing* FirstSearchThing() { return g_nThings ? g_things[0] : 0; }
CThing* NextSearchThing(CThing* previous) { for (int i = 0; i < g_nThings - 1; ++i) if (g_things[i] == previous) return g_things[i + 1]; return 0; }
static long g_filterType;
bool PassesFilter(const CThingFilter_HasTC& filter, CThing* thing) { g_filterType = filter.Type; return thing->DefIndex != 7; }
static CWorld g_world; static CThingSearchTools g_tools;
CWorld* CTCInventoryMap::GetWorld() { return &g_world; }
static CThing* g_hero;
CThing* CPlayer::GetControlledCreature() const { return g_hero; }
static CPlayer g_player; static CPlayerManager g_pm;
CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
CPlayerManager* CTCInventoryMap::GetPlayerManager() const { return &g_pm; }
static CThing* g_teleportedTo; static int g_teleports = 0;
void CTCInventoryMap::TeleportHeroToTeleporter(CThing* teleporter) { g_teleportedTo = teleporter; ++g_teleports; }
int main()
{
    g_world.ThingSearchTools = &g_tools;
    static CThing hero, a, b, c; memset(&hero, 0, sizeof hero); memset(&a, 0, sizeof a); memset(&b, 0, sizeof b); memset(&c, 0, sizeof c);
    a.DefIndex = 7; b.DefIndex = 41; c.DefIndex = 42;
    g_things[0] = &a; g_things[1] = &b; g_things[2] = &c; g_nThings = 3;
    CTCInventoryMap map;
    g_hero = &hero;
    map.TeleportHeroToGuild();
    CHECK(g_teleports == 1); CHECK(g_teleportedTo == &c); CHECK(g_filterType == 0xDD);
    CHECK(g_lastName && strcmp(g_lastName, "HERO_GUILD_TELEPORT_MARKER") == 0); CHECK(g_strings == 2); CHECK(g_strDtors == 2);
    g_guildDef = 99; map.TeleportHeroToGuild(); CHECK(g_teleports == 1);           // no guild teleporter
    hero.IsDead = 1; g_guildDef = 42; map.TeleportHeroToGuild(); CHECK(g_teleports == 1);   // dead hero
    g_hero = 0; map.TeleportHeroToGuild(); CHECK(g_teleports == 1);
    if (fails == 0) printf("005fbd90_TEST PASS\n"); else printf("005fbd90_TEST FAIL\n");
    return fails;
}