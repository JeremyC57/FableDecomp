// Grade: DIFFER(ours v retail) -- 42v40: retail forms the map address as `lea esi,[eax+934h]` after PeekGuiDef, ours `mov esi,eax ; add esi,934h` (reference, pointer, inline-accessor and /Os forms tried)
// CGameScriptInterface::GetTextIDFromSymbol(const CCharString& symbol) const -- looks the symbol up in the GUI definition's ScriptSprites map<CCharString,long>; returns its id, or 0 when absent
// Leaf: GetTextIDFromSymbol
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E77F0, 39 bytes (+ int3 padding), `ret 4`; `this` is never read.
// Identity: the manifest label LTextSystem::GetIDFromSymbol is propagated. 38 landed CGameScriptInterface callers
// (MiniMapAddMarker 0x00891F50, TextEntryExists 0x00892070, ...) declare it as CGameScriptInterface::GetTextIDFromSymbol,
// so this row keeps that symbol. What it reads is CPlayerGuiDef::ScriptSprites (Ego_r.pdb map<CCharString,long>, the map
// two before MiniMapGraphics; retail +0x934, MiniMapGraphics +0x94C as the landed CTCInventoryBase::GetMiniMapGraphic
// 0x005BEB57 shows), so a truer name would be "GetScriptSpriteIDFromSymbol".
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), STLport map<CCharString,long>::find @ 0x0042B467 (thiscall,
// returns the node; key +0x10, value +0x14; end() is the header word at +0 of the map).
// Layout: local models (engine/CGameScriptInterface.h is not needed: no member of `this` is touched; the PDB CPlayerGuiDef
// offsets do not fit retail here).
// Shape: the map address kept in esi across find for the end() compare.
class CCharString;
struct CPlayerGuiDef;

struct SGraphicNode {                              // STLport _Rb_tree_node<pair<const CCharString, long> >
    unsigned char _base[0x10];
    unsigned char Key[4];                          // +0x10 CCharString
    long          Value;                           // +0x14
};

struct CCharStringLongMap {                        // STLport map<CCharString, long> (12 bytes)
    SGraphicNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    SGraphicNode* find(const CCharString& key);    // 0x0042B467
    SGraphicNode* end() const { return _M_header; }
};

struct CPlayerGuiDef {
    unsigned char      _pad000[0x934];
    CCharStringLongMap ScriptSprites;              // +0x934
    CCharStringLongMap GameActionValues;           // +0x940
    CCharStringLongMap MiniMapGraphics;            // +0x94C
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570

struct CGameScriptInterface {
    long GetTextIDFromSymbol(const CCharString& symbol) const;
};

long CGameScriptInterface::GetTextIDFromSymbol(const CCharString& symbol) const
{
    CCharStringLongMap* sprites = &PeekGuiDef()->ScriptSprites;
    SGraphicNode* it = sprites->find(symbol);
    if (it != sprites->end())
        return it->Value;
    return 0;
}
