// CTCInventoryBase::GetMiniMapGraphic(const CRegion* region) -- the mini-map graphic index for `region` from the GUI def's graphic table (0 when the region's graphic is not in the table); when it is the prison courtyard and the living hero stands above the courtyard's upper floor height, the second courtyard graphic is returned instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BEB57, length 233 bytes (`ret 4`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`; MiniMapGraphics -- map<CCharString,long> -- at retail +0x94C), map::find
// @ 0x0042B467 (STLport node: key +0x10, value +0x14; end() is the header at +0), CCharString(const char*, long) @ 0x0099EBF0 and
// ~CCharString @ 0x0099EAE0 ("MINIMAP_PRISONCOURTYARD1" 0x01253D5C, "MINIMAP_EXECUTIONTREE" 0x01253D44, "MINIMAP_PRISONCOURTYARD2"
// 0x01253D24), CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
// CPlayer::PeekPControlledCreature @ 0x00487DD0, IS_THING_ALIVE<T> @ 0x0043DBF0 (`__fastcall`, the pointer by reference), CThing::
// GetPos @ 0x004C73D0; the float 0x01253D40 (51.0) is the landed g_mapEdgeZ. CRegion::MiniMapGraphic at retail +0x28.
// Shape: the landed CMiniMapDisplay::InitialiseOnRegionLoad idiom -- each lookup key is a block-scoped CCharString local whose
// find result is read after the block (`mov esi,eax ; dtor ; mov edi,[esi+14h]`); the first and third keys and the hero pointer share
// the dead argument slot ([ebp+8]) because their scopes are disjoint (the hero lives in its own block), the "MINIMAP_EXECUTIONTREE"
// key sits in [ebp-8]: its block builds and destroys the name without a lookup, a leftover of the execution-tree special case that
// this function no longer has. The GUI def is a pointer local so the table address is one `lea esi,[eax+94Ch]`; `this` is spilled
// to [ebp-0Ch]. The height test is `Z > 51.0f` (`fcomp ; test ah,41h ; jne` skips on below-or-equal).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
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
class CPlayerGuiDef { public:
    unsigned char      _pad00[0x94c];
    CCharStringLongMap MiniMapGraphics;         // +0x94c
};
CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570
class CRegion { public:
    unsigned char _pad00[0x28];
    CCharString   MiniMapGraphic;               // +0x28
};
class C3DVector { public: float X, Y, Z; };
class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };
class CThingPlayerCreature;
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingPointerAlive(const void* thing);
#pragma auto_inline(off)
// Same-TU stand-in for the IS_THING_ALIVE<const CThingPlayerCreature*> template 0x0043DBF0: reads the pointer through its const reference.
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && IsThingPointerAlive(thing); }
#pragma auto_inline(on)
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)
};
extern const float g_mapEdgeZ;                  // 0x01253D40 (51.0f)

class CTCBase
{
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

namespace NInventory {
class CTCInventoryBase : public CTCBase { public:
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
        const CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer()->PeekPControlledCreature();
        if (IS_THING_ALIVE(hero) && isPrisonCourtyard && hero->GetPos().Z > g_mapEdgeZ) {
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