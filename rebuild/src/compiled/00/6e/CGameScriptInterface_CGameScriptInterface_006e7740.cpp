// CGameScriptInterface::CGameScriptInterface(CMainGameComponent&, const CGameDefinitionManager&, CWorld&,
// CPlayerManager&, CDisplayEngine&) -- the script API object: store the engine services, reset the script state, build
// the empty hero script thing and timer map, and publish itself
// Leaf: ??0CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7740, length 176 bytes, `ret 14h`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CBaseClass::CBaseClass @ 0x0099A2D0 (the hero
// CScriptThing's base; CScriptThing vtable 0x01238C8C), malloc @ 0x00BFEA0E (the timer map's 0x18-byte header node),
// the `__fastcall` global setter @ 0x00CBE0C0 (stores the interface in 0x0143E8F8, read by the quest scripts).
// Vtable 0x01260F0C. The interface is also stored in the global 0x0143E8F0.
// Layout (engine/CGameScriptInterface.h agrees up to +0x3C; the PDB map is 16 bytes, retail STLport 12, so
// CameraResetToViewBehindHeroCount is +0x48 and CreateCreatureDelayFrames +0x4C, size 0x50): World +4, Component +8,
// DisplayEngine +0xC, DefinitionManager +0x10, PlayerManager +0x14, ThingSearchTools +0x18 (the world's +0x20),
// CurrentPlayer +0x1C (-1), CurrentLevelID/CurrentScriptLevelID/CurrentScriptID +0x20..+0x28, InMovieSequence +0x2C,
// AllowScreenFadingIfAlreadyFaded +0x2D, HeroSThing +0x30, ScriptTimers +0x3C.
// The engine header is all opaque bytes for the members that need constructors, so `this` is modelled locally with
// its member names.
// Shape: init list in declaration order (VC7.1 interleaves the argument stores around the vtable store); the
// ThingSearchTools initialiser reads the world argument, not the member; the CScriptThing default ctor is inline here;
// the STLport map is the landed CRetailMap model (`_M_header` zeroed, malloc, empty-initialise).

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

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
    CBaseClass();                         // 0x0099A2D0
    virtual ~CBaseClass();                // 0x0099A2E0
};
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();              // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();     // 0x0099A300
};

struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) {}
};
struct CScriptThing : CBaseClass {        // vtable 0x01238C8C
    CCountedPointerWords PImp;
    CScriptThing() {}
    virtual ~CScriptThing();
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
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
extern CGameScriptInterface* g_pGameScriptInterfaceInstance;      // 0x0143E8F0
void __fastcall SetGameScriptInterface(CGameScriptInterface* gsi); // 0x00CBE0C0

class CGameScriptInterface : public CBaseClassNonCopyable {
public:
    CGameScriptInterface(CMainGameComponent& component, const CGameDefinitionManager& definitions, CWorld& world,
                         CPlayerManager& players, CDisplayEngine& display);
    virtual ~CGameScriptInterface();

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