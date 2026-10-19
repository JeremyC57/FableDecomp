// CMiniMapDisplay::InitialiseOnRegionLoad(CRegion* region) -- pick the region's minimap graphic, flag the prison-courtyard / execution-tree special maps, size the region minimap for the world map and remember the region
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082BA00, length 233 bytes (`ret 4`).
// Callees: the game singleton's virtual slot 9 (0x013B86A0; named IsLevelPersistent after the landed CThingSoundEmitter 0x008367B0 model),
// PeekGuiDef @ 0x00438570 (`__fastcall`), map<CCharString,long>::find @ 0x0042B467 (returns the node; labelled StdMap_FindLE),
// CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CWorld::PeekWorldMap (slot 13, world at singleton +0x24),
// CRegionMinimap::Initialise @ 0x0066DBB0 and InitialiseRegionMinimapSize(CWorldMap*, CRegion*) @ 0x0066CC40.
// Layout: retail CMiniMapDisplay (PDB names; engine/CMiniMapDisplay.h has the PDB's 16-byte vectors): MiniMapGraphic +0x1C, the four
// special-map flags +0x40..+0x43, RegionMinimap +0x54, PRegion +0xB8, UpdateRequired +0xBC; CPlayerGuiDef::MiniMapGraphics
// (map<CCharString,long>) at retail +0x94C; CRegion::MiniMapGraphic at retail +0x28 (PDB +0x30); STLport map node: key +0x10, value +0x14.
// Shape: the two lookup keys are named CCharString locals in their own scopes (built in the frame, destroyed after each find); the flags are
// cleared with one byte zero; only PrisonCourtyard1 and HeadsmansHillNorth are set here.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct SGraphicNode {                           // STLport _Rb_tree_node<pair<const CCharString, long> >
    unsigned char _base[0x10];
    CCharString   Key;                          // +0x10
    long          Value;                        // +0x14
};
struct CCharStringLongMap {                     // STLport map<CCharString, long> (12 bytes)
    SGraphicNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    SGraphicNode* find(const CCharString& key); // 0x0042B467
    SGraphicNode* end() const { return _M_header; }
};
struct CPlayerGuiDef {
    unsigned char      _pad00[0x94c];
    CCharStringLongMap MiniMapGraphics;         // +0x94c
};
CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570
struct CWorldMap;
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* PeekWorldMap();          // slot 13 (+0x34)
};
struct CGame {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsLevelPersistent();           // slot 9 (+0x24)
    unsigned char _pad04[0x20];
    CWorld*       World;                        // +0x24
};
extern CGame* g_pGame;                          // 0x013B86A0
struct CRegion {
    unsigned char _pad00[0x28];
    CCharString   MiniMapGraphic;               // +0x28
};
struct CRegionMinimap {
    unsigned char _body[0x64];
    void Initialise();                                              // 0x0066DBB0
    void InitialiseRegionMinimapSize(CWorldMap* map, CRegion* region);   // 0x0066CC40
};
struct CMiniMapDisplay {
    void*          __vftable;                   // +0x00
    unsigned char  _pad04[0x18];
    long           MiniMapGraphic;              // +0x1c
    unsigned char  _pad20[0x20];
    bool           MiniMapIsPrisonCourtyard1;   // +0x40
    bool           MiniMapIsPrisonCourtyard2;   // +0x41
    bool           MiniMapIsHeadsmansHillNorth; // +0x42
    bool           MiniMapIsHeadsmansHillSouth; // +0x43
    unsigned char  _pad44[0x10];
    CRegionMinimap RegionMinimap;               // +0x54
    CRegion*       PRegion;                     // +0xb8
    bool           UpdateRequired;              // +0xbc
    void InitialiseOnRegionLoad(CRegion* region);
};

void CMiniMapDisplay::InitialiseOnRegionLoad(CRegion* region)
{
    if (g_pGame->IsLevelPersistent())
        return;
    CPlayerGuiDef* def = PeekGuiDef();
    MiniMapIsPrisonCourtyard1 = false;
    MiniMapIsPrisonCourtyard2 = false;
    MiniMapIsHeadsmansHillNorth = false;
    MiniMapIsHeadsmansHillSouth = false;
    CCharStringLongMap& graphics = def->MiniMapGraphics;
    SGraphicNode* found = graphics.find(region->MiniMapGraphic);
    if (found != graphics.end()) {
        MiniMapGraphic = found->Value;
        SGraphicNode* courtyard;
        {
            CCharString name("MINIMAP_PRISONCOURTYARD1", -1);
            courtyard = graphics.find(name);
        }
        SGraphicNode* executionTree;
        {
            CCharString name("MINIMAP_EXECUTIONTREE", -1);
            executionTree = graphics.find(name);
        }
        if (MiniMapGraphic == courtyard->Value)
            MiniMapIsPrisonCourtyard1 = true;
        else if (MiniMapGraphic == executionTree->Value)
            MiniMapIsHeadsmansHillNorth = true;
    }
    CWorldMap* map = g_pGame->World->PeekWorldMap();
    RegionMinimap.Initialise();
    RegionMinimap.InitialiseRegionMinimapSize(map, region);
    PRegion = region;
    UpdateRequired = true;
}