// Behaviour test for 005beb57: src/005beb57.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryBase::GetMiniMapGraphic(const CRegion* region)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryBase::GetMiniMapGraphic(const CRegion* region) -- the mini-map graphic index for `region` from the GUI def's graphic table (0 when the region's graphic is not in the table); when it is the prison courtyard and the living hero stands above the courtyard's upper floor height, the second courtyard graphic is returned instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BEB57, length 233 bytes (`ret 4`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`; MiniMapGraphics -- map<CCharString,long> -- at retail +0x94C), map::find
// @ 0x0042B467 (STLport node: key +0x10, value +0x14; end() is the header at +0), CCharString(const char*, long) @ 0x0099EBF0 and
// ~CCharString @ 0x0099EAE0 ("MINIMAP_PRISONCOURTYARD1" 0x01253D5C, "MINIMAP_EXECUTIONTREE" 0x01253D44, "MINIMAP_PRISONCOURTYARD2"
// 0x01253D24), CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
// CPlayer::GetControlledCreature @ 0x00487DD0, IS_THING_ALIVE<T> @ 0x0043DBF0 (`__fastcall`, the pointer by reference), CThing::
// GetPosition @ 0x004C73D0; the float 0x01253D40 (51.0) is the landed g_mapEdgeZ. CRegion::MiniMapGraphic at retail +0x28.
// Shape: the landed CMiniMapDisplay::InitialiseOnRegionLoad idiom -- each lookup key is a block-scoped CCharString local whose
// find result is read after the block (`mov esi,eax ; dtor ; mov edi,[esi+14h]`); the first and third keys and the hero pointer share
// the dead argument slot ([ebp+8]) because their scopes are disjoint (the hero lives in its own block), the "MINIMAP_EXECUTIONTREE"
// key sits in [ebp-8]: its block builds and destroys the name without a lookup, a leftover of the execution-tree special case that
// this function no longer has. The GUI def is a pointer local so the table address is one `lea esi,[eax+94Ch]`; `this` is spilled
// to [ebp-0Ch]. The height test is `Z > 51.0f` (`fcomp ; test ah,41h ; jne` skips on below-or-equal).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
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
int CompareCharStrings(const CCharString& a, const CCharString& b);
// Same-TU stand-in for the STLport map<CCharString,long>::find 0x0042B467 (a template instantiation retail's TU had): reads the key.
#pragma auto_inline(off)
__declspec(noinline) SGraphicNode* CCharStringLongMap::find(const CCharString& key)
{
    SGraphicNode* y = _M_header;
    SGraphicNode* x = *(SGraphicNode**)(_M_header->_base + 4);
    while (x) {
        if (CompareCharStrings(x->Key, key) >= 0) { y = x; x = *(SGraphicNode**)(x->_base + 8); }
        else x = *(SGraphicNode**)(x->_base + 0xc);
    }
    return (y == _M_header || CompareCharStrings(key, y->Key) < 0) ? _M_header : y;
}
#pragma auto_inline(on)
struct CPlayerGuiDef {
    unsigned char      _pad00[0x94c];
    CCharStringLongMap MiniMapGraphics;         // +0x94c
};
CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570
struct CRegion {
    unsigned char _pad00[0x28];
    CCharString   MiniMapGraphic;               // +0x28
};
struct C3DVector { float X, Y, Z; };
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingPlayerCreature;
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingPointerAlive(const void* thing);
#pragma auto_inline(off)
// Same-TU stand-in for the IS_THING_ALIVE<CThing*> template 0x0043DBF0: reads the pointer through its const reference.
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && IsThingPointerAlive(thing); }
#pragma auto_inline(on)
struct CPlayer {
    CThing* GetControlledCreature();            // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)
};
extern const float g_mapEdgeZ;                  // 0x01253D40 (51.0f)

namespace NInventory {
struct CTCInventoryBase {
    CPlayerManager* GetPlayerManager_00686d50() const;   // 0x00686D50 (CTCBase, const)
    long GetMiniMapGraphic(const CRegion* region);
};
long CTCInventoryBase::GetMiniMapGraphic(const CRegion* region)
{
    CPlayerGuiDef* guiDef = PeekGuiDef();
    long graphic = 0;
    bool isPrisonCourtyard = false;
    SGraphicNode* found = guiDef->MiniMapGraphics.find(region->MiniMapGraphic);
    if (found != guiDef->MiniMapGraphics.end()) {
        graphic = found->Value;
        SGraphicNode* courtyard;
        {
            CCharString name("MINIMAP_PRISONCOURTYARD1");
            courtyard = guiDef->MiniMapGraphics.find(name);
        }
        {
            CCharString name("MINIMAP_EXECUTIONTREE");
        }
        if (graphic == courtyard->Value)
            isPrisonCourtyard = true;
    }
    {
        CThing* hero = GetPlayerManager_00686d50()->GetMainPlayer()->GetControlledCreature();
        if (IS_THING_ALIVE(hero) && isPrisonCourtyard && hero->GetPosition().Z > g_mapEdgeZ) {
            SGraphicNode* upperCourtyard;
            {
                CCharString name("MINIMAP_PRISONCOURTYARD2");
                upperCourtyard = guiDef->MiniMapGraphics.find(name);
            }
            graphic = upperCourtyard->Value;
        }
    }
    return graphic;
}
}

// ---- stubs and driver ----
using namespace NInventory;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() {}
int CompareCharStrings(const CCharString& a, const CCharString& b) { return strcmp((const char*)a.PStringData, (const char*)b.PStringData); }
static bool g_alive = true;
bool IsThingPointerAlive(const void*) { return g_alive; }
static CPlayerGuiDef* g_gui = 0;
CPlayerGuiDef* __fastcall PeekGuiDef() { return g_gui; }
static C3DVector g_pos = { 0, 0, 0 };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
CThing* CPlayer::GetControlledCreature() { return g_heroPtr; }
static CPlayer g_player;
CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
static CPlayerManager g_pm;
CPlayerManager* CTCInventoryBase::GetPlayerManager_00686d50() const { return &g_pm; }
const float g_mapEdgeZ = 51.0f;
// a three-node tree: root "MINIMAP_PRISONCOURTYARD1" with left "MINIMAP_FOREST" and right "MINIMAP_PRISONCOURTYARD2"
static SGraphicNode* mk(unsigned char* raw, const char* key, long value)
{
    memset(raw, 0, sizeof(SGraphicNode)); SGraphicNode* n = (SGraphicNode*)raw; n->Key.PStringData = (void*)key; n->Value = value; return n;
}
int main()
{
    static unsigned char hraw[sizeof(SGraphicNode)], r1[sizeof(SGraphicNode)], r2[sizeof(SGraphicNode)], r3[sizeof(SGraphicNode)];
    SGraphicNode* header = mk(hraw, "", 0);
    SGraphicNode* root = mk(r1, "MINIMAP_PRISONCOURTYARD1", 7);
    SGraphicNode* left = mk(r2, "MINIMAP_FOREST", 3);
    SGraphicNode* right = mk(r3, "MINIMAP_PRISONCOURTYARD2", 8);
    *(SGraphicNode**)(header->_base + 4) = root; *(SGraphicNode**)(root->_base + 8) = left; *(SGraphicNode**)(root->_base + 0xc) = right;
    static unsigned char graw[sizeof(CPlayerGuiDef)]; memset(graw, 0, sizeof(graw)); g_gui = (CPlayerGuiDef*)graw; g_gui->MiniMapGraphics._M_header = header;
    static unsigned char regRaw[sizeof(CRegion)]; memset(regRaw, 0, sizeof(regRaw)); CRegion* region = (CRegion*)regRaw;
    CTCInventoryBase inv;
    region->MiniMapGraphic.PStringData = (void*)"MINIMAP_FOREST"; CHECK(inv.GetMiniMapGraphic(region) == 3);
    region->MiniMapGraphic.PStringData = (void*)"MINIMAP_UNKNOWN"; CHECK(inv.GetMiniMapGraphic(region) == 0);
    region->MiniMapGraphic.PStringData = (void*)"MINIMAP_PRISONCOURTYARD1";
    g_pos.Z = 10.0f; CHECK(inv.GetMiniMapGraphic(region) == 7);          // courtyard, hero low
    g_pos.Z = 60.0f; CHECK(inv.GetMiniMapGraphic(region) == 8);          // courtyard, hero above the edge: upper courtyard
    g_alive = false; CHECK(inv.GetMiniMapGraphic(region) == 7);
    g_alive = true; g_heroPtr = 0; CHECK(inv.GetMiniMapGraphic(region) == 7);
    region->MiniMapGraphic.PStringData = 0;
    if (fails == 0) printf("005beb57_TEST PASS\n"); else printf("005beb57_TEST FAIL\n");
    return fails;
}