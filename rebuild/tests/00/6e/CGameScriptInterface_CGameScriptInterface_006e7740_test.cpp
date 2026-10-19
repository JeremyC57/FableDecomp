// Behaviour test for 0x006E7740 CGameScriptInterface::CGameScriptInterface: services stored, script state reset, the
// thing-search tools taken from the world, empty hero thing and timer map, both globals published.
#include <cstdio>
#include <cstdlib>
static int g_bases = 0;
struct CMainGameComponent;
struct CGameDefinitionManager;
struct CPlayerManager;
struct CDisplayEngine;
struct CThingSearchTools;
struct CWorld {
    unsigned char _pad00[0x20];
    CThingSearchTools* ThingSearchTools;  // +0x20
};

struct CBaseClass {
    CBaseClass() { ++g_bases; }
    virtual ~CBaseClass() {}
};
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable() {}
    virtual ~CBaseClassNonCopyable() {}
};

struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) {}
};
struct CScriptThing : CBaseClass {        // vtable 0x01238C8C
    CCountedPointerWords PImp;
    CScriptThing() {}
    virtual ~CScriptThing() {}
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)std::malloc(NodeSize < sizeof(_Rb_tree_node_base) + 8 ? sizeof(_Rb_tree_node_base) + 8 : NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map<long, long>
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};

class CGameScriptInterface;
static CGameScriptInterface* g_pGameScriptInterfaceInstance = 0;
static CGameScriptInterface* g_pGameScriptInterface = 0;
static void SetGameScriptInterface(CGameScriptInterface* gsi) { g_pGameScriptInterface = gsi; }

class CGameScriptInterface : public CBaseClassNonCopyable {
public:
    CGameScriptInterface(CMainGameComponent& component, const CGameDefinitionManager& definitions, CWorld& world,
                         CPlayerManager& players, CDisplayEngine& display);
    virtual ~CGameScriptInterface() {}

    CWorld*                       World;                            // +0x04
    CMainGameComponent*           Component;                        // +0x08
    CDisplayEngine*               DisplayEngine;                    // +0x0c
    const CGameDefinitionManager* DefinitionManager;                // +0x10
    CPlayerManager*               PlayerManager;                    // +0x14
    CThingSearchTools*            ThingSearchTools;                 // +0x18
    long                          CurrentPlayer;                    // +0x1c
    long                          CurrentLevelID;                   // +0x20
    long                          CurrentScriptLevelID;             // +0x24
    long                          CurrentScriptID;                  // +0x28
    bool                          InMovieSequence;                  // +0x2c
    bool                          AllowScreenFadingIfAlreadyFaded;  // +0x2d
    CScriptThing                  HeroSThing;                       // +0x30
    CRetailMap<0x18>              ScriptTimers;                     // +0x3c
    long                          CameraResetToViewBehindHeroCount; // +0x48
    long                          CreateCreatureDelayFrames;        // +0x4c
};

CGameScriptInterface::CGameScriptInterface(CMainGameComponent& component, const CGameDefinitionManager& definitions,
                                           CWorld& world, CPlayerManager& players, CDisplayEngine& display)
    : World(&world),
      Component(&component),
      DisplayEngine(&display),
      DefinitionManager(&definitions),
      PlayerManager(&players),
      ThingSearchTools(world.ThingSearchTools),
      CurrentPlayer(-1),
      CurrentLevelID(0),
      CurrentScriptLevelID(0),
      CurrentScriptID(0),
      InMovieSequence(false),
      AllowScreenFadingIfAlreadyFaded(false),
      CameraResetToViewBehindHeroCount(-1),
      CreateCreatureDelayFrames(0)
{
    g_pGameScriptInterfaceInstance = this;
    SetGameScriptInterface(this);
}

struct CMainGameComponent { int a; };
struct CGameDefinitionManager { int a; };
struct CPlayerManager { int a; };
struct CDisplayEngine { int a; };
struct CThingSearchTools { int a; };
int main()
{
    CMainGameComponent mgc; CGameDefinitionManager defs; CPlayerManager pm; CDisplayEngine de; CThingSearchTools tools;
    CWorld world; world.ThingSearchTools = &tools;
    CGameScriptInterface* gsi = new CGameScriptInterface(mgc, defs, world, pm, de);
    int fail = 0;
    if (gsi->World != &world || gsi->Component != &mgc || gsi->DisplayEngine != &de || gsi->DefinitionManager != &defs ||
        gsi->PlayerManager != &pm || gsi->ThingSearchTools != &tools) fail |= 1;
    if (gsi->CurrentPlayer != -1 || gsi->CurrentLevelID || gsi->CurrentScriptLevelID || gsi->CurrentScriptID ||
        gsi->InMovieSequence || gsi->AllowScreenFadingIfAlreadyFaded || gsi->CameraResetToViewBehindHeroCount != -1 ||
        gsi->CreateCreatureDelayFrames) fail |= 2;
    if (gsi->HeroSThing.PImp.Data || gsi->HeroSThing.PImp.Info || g_bases != 2) fail |= 4;
    _Rb_tree_node_base* h = gsi->ScriptTimers._M_header;
    if (!h || h->_M_color || h->_M_parent || h->_M_left != h || h->_M_right != h || gsi->ScriptTimers._M_node_count) fail |= 8;
    if (g_pGameScriptInterfaceInstance != gsi || g_pGameScriptInterface != gsi) fail |= 16;
    std::free(h);
    if (fail) { std::printf("006e7740_TEST FAIL\n"); return 1; }
    std::printf("006e7740_TEST PASS\n");
    return 0;
}