// CTCMapwho::~CTCMapwho() -- scalar deleting destructor: remove the thing from the mapwho when it is in (DestroyMapwhoNode), run ~CTCBase and free the block when bit 0 of the flags is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A810, length 58 bytes (the `??_G`; the catalog's "vector_deleting_destructor" label is the usual propagated name).
// Leaf: ??_GCTCMapwho
// Callees: the CTCBase world-map accessor @ 0x00686D30 (`this` unused), CThingMapManager::DestroyMapwhoNode @ 0x00637A10
// (manager at world map +0x10), CTCBase::~CTCBase @ 0x00686830, operator delete @ 0x00BFE9BC. Vtable: CTCMapwho @ 0x01265A3C.
// Layout: local model with the engine/CTCMapwho.h offsets (the generated header has no base, vptr or destructor).
// Shape: the destructor body is the inline RemoveFromMapwho (0x0071A7C0: bit test, accessor result in a named local, push
// after the manager load); its vtable store is scheduled between the bit test and the branch; the `??_G` is pulled into
// the TU by a `new CTCMapwho` factory with the constructor inline; the destructor and RemoveFromMapwho are `__forceinline`
// (otherwise the `??_G` collapses to `call ??1CTCMapwho ; delete`).
class CThing;
struct CThingMapCell;
class CTCMapwho;

struct CThingMapManager {
    void DestroyMapwhoNode(CTCMapwho* mapwho);     // 0x00637A10
};

class CWorldMap { public:
    unsigned char     _pad_0x00[0x10];
    CThingMapManager* m_pThingMapManager;          // +0x10
};

class CTCBase { public:
    CTCBase(CThing* owner);                        // 0x00686800
    virtual ~CTCBase();                            // 0x00686830
    CWorldMap* GetWorldMap_00686d30();                      // 0x00686D30
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

struct CMapwhoNode {
    CTCMapwho*     PMapwho;                        // +0x00
    CMapwhoNode*   PNext;                          // +0x04
    CMapwhoNode*   PPrevious;                      // +0x08
    CThingMapCell* PCell;                          // +0x0c
    CMapwhoNode() : PMapwho(0), PNext(0), PPrevious(0), PCell(0) { }
};

class CTCMapwho : public CTCBase { public:
    CMapwhoNode   TLNode;                          // +0x0c
    CMapwhoNode   TRNode;                          // +0x1c
    CMapwhoNode   BLNode;                          // +0x2c
    CMapwhoNode   BRNode;                          // +0x3c
    unsigned long SearchID;                        // +0x4c
    unsigned char InMapwho : 1;                    // +0x50 bit 0
    CTCMapwho(CThing* owner)
        : CTCBase(owner), SearchID(0)
    {
        InMapwho = 0;
        TLNode.PMapwho = this;
        BRNode.PMapwho = this;
        TRNode.PMapwho = this;
        BLNode.PMapwho = this;
    }
    __forceinline void RemoveFromMapwho()
    {
        if (InMapwho) {
            CWorldMap* map = GetWorldMap_00686d30();
            map->m_pThingMapManager->DestroyMapwhoNode(this);
        }
    }
    __forceinline virtual ~CTCMapwho()
    {
        RemoveFromMapwho();
    }
};

CTCMapwho* CreateMapwho(CThing* owner)
{
    return new CTCMapwho(owner);
}