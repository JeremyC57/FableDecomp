// CMap::CreateScriptedMapBrush(const C2DBoxI& area) -- create a scripted map brush over a map area: build it in a frame temporary, append a copy to the map's brush list, register the new list entry with the world map under the map ID of this map's world position, and return it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820F80, length 142 bytes.
// Callees: CScriptedMapBrush::CScriptedMapBrush(const C2DBoxI&) @ 0x0082E600 (manifest "CScriptedMapBrush_CreateFromDefinition"; it reads the
//   area's four longs and installs vtable 0x01272518), malloc @ 0x00BFEA0E (the STLport list node, 0x5c bytes), the copy constructor
//   @ 0x00827040, ~CScriptedMapBrush @ 0x0082E780 (manifest label ~CPatchTesselationEdgeStrip is a byte-twin), the world's virtual slots
//   13 (+0x34, the map registry) and 12 (+0x30, the world map), the registry's slot 17 (+0x44, map ID at a world position),
//   CWorldMap::RegisterScriptedMapBrush(long, CScriptedMapBrush*) @ 0x004FF900.
// Layout (retail CMap): ScriptedMapBrushes (STLport list, header node pointer) +0x0c, World +0x20, WorldPos +0x1d24; retail CScriptedMapBrush 0x54 bytes.
// Shape: STLport push_back = insert(end(), x): the end node is read before the allocation, `_Construct` is a placement new with its null test,
//   the previous node is read once into a local before the links are written; the new entry is kept as its NODE pointer (`&node->_M_data`
//   computed after the map-ID calls, `add edi,8` just before the push), the map ID is a named local evaluated before the world-map call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct C2DCoordI { long X, Y; };
struct CScriptedMapBrush {
    unsigned char _body[0x54];
    explicit CScriptedMapBrush(const C2DBoxI& area);            // 0x0082E600
    CScriptedMapBrush(const CScriptedMapBrush& other);          // 0x00827040
    ~CScriptedMapBrush();                                       // 0x0082E780
};
struct SBrushNode {
    SBrushNode*       _M_next;
    SBrushNode*       _M_prev;
    CScriptedMapBrush _M_data;
};
struct CBrushList {
    SBrushNode* _M_node;
    __forceinline SBrushNode* _M_create_node(const CScriptedMapBrush& x)
    {
        SBrushNode* tmp = (SBrushNode*)malloc(sizeof(SBrushNode));
        new (&tmp->_M_data) CScriptedMapBrush(x);
        return tmp;
    }
    __forceinline void insert(SBrushNode* pos, const CScriptedMapBrush& x)
    {
        SBrushNode* tmp = _M_create_node(x);
        SBrushNode* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
    void push_back(const CScriptedMapBrush& x) { insert(_M_node, x); }
    CScriptedMapBrush& back() { return _M_node->_M_prev->_M_data; }
};
struct CWorldMap {
    void RegisterScriptedMapBrush(long id, CScriptedMapBrush* brush);   // 0x004FF900
};
struct CMapRegistry {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual long GetMapIDAt(const C2DCoordI& pos);   // slot 17 (+0x44)
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();              // slot 12 (+0x30)
    virtual CMapRegistry* GetMapRegistry();        // slot 13 (+0x34)
};
struct CMap {
    unsigned char _pad00[0xc];
    CBrushList    ScriptedMapBrushes;              // +0x0c
    unsigned char _pad10[0x10];
    CWorld*       World;                           // +0x20
    unsigned char _pad24[0x1d24 - 0x24];
    C2DCoordI     WorldPos;                        // +0x1d24
    CScriptedMapBrush* CreateScriptedMapBrush(const C2DBoxI& area);
};
CScriptedMapBrush* CMap::CreateScriptedMapBrush(const C2DBoxI& area)
{
    ScriptedMapBrushes.push_back(CScriptedMapBrush(area));
    SBrushNode* last = ScriptedMapBrushes._M_node->_M_prev;
    long mapID = World->GetMapRegistry()->GetMapIDAt(WorldPos);
    World->GetWorldMap()->RegisterScriptedMapBrush(mapID, &last->_M_data);
    return &ScriptedMapBrushes.back();
}
