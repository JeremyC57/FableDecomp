// Grade: DIFFER(521v580) -- empty CAllocatorTag/filter temporaries homed at [ebp-1]/[ebp-2] where retail overlays them on [ebp+0Bh] (top byte of the LIVE region parameter slot; GOTCHAS lane gap), frame 0x44 v 0x4c (retail gives loop-2 locals their own words), RegionExits in edi v ebx, loop-2 node kept in memory with rotated entry
// Alt (retry wave 2 sec2c): DIFFER(533v580)/256 -- an inline list ctor taking the allocator tag BY VALUE homes the tag in the live region slot [ebp+0Bh] like retail, but copies it (mov al,[ebp+0Bh]) instead of passing lea [ebp+0Bh]; the const& wrapper goes back to [ebp-1]. Lead for the 24l "empty tag on a live slot" gap
#pragma optimize("y", off)
#pragma optimize("s", on)
// CDrawMiniMap::UpdateRegionExitNames(CRegion* region) -- rebuild the mini-map's region-exit labels: clear them, then for every region exit (CTCDRegionExit things) and every scripted-hook region entrance (CTCActionUseScriptedHook things) on a map of this region, add {position (+ the region's text offset for that exit), display name}
// Leaf: UpdateRegionExitNames
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064DD9A, 580 bytes, `ret 4`. Called from CDrawMiniMap::SetRegion (landed 0x0064DFDE,
//   which models it as ApplyRegion).
struct CCharString {
    void* m_pData;
    ~CCharString();                                // 0x0099EAE0
};
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b);                  // 0x0065470C for <C3DVector, CCharString>
};
typedef pair<C3DVector, CCharString> CRegionExitLabel;

struct CRegionExitLabelVector {                    // vector<pair<C3DVector, CCharString>> {first, last, end}
    CRegionExitLabel* _M_start;
    CRegionExitLabel* _M_finish;
    CRegionExitLabel* _M_end_of_storage;
    CRegionExitLabel* erase(CRegionExitLabel* first, CRegionExitLabel* last);   // 0x00656837
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const CRegionExitLabel& value); // 0x00656EF2
};

struct CRegion {
    unsigned char _pad00[0x18];
    CCharString   Name;                            // +0x18
    bool GetMiniMapRegionExitTextOffset(const CCharString& name, C2DVector* offset);   // 0x006BC350
};

struct CWorldMap {
    long GetRegionNumberFromName(const CCharString& name);   // 0x004FC210
    long GetRegionNumberMapIsIn(long mapNumber);             // 0x004FC190
};

struct CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (the const copy)
};
struct CBitset274 {
    unsigned long w[9];
    bool test(unsigned int pos) const;             // 0x00410DE0
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};

struct CTCDRegionExit {
    unsigned char _pad00[0x0C];
    bool          Active;                          // +0x0C
    const CCharString& GetNameOfRegionConnectingTo();   // 0x007791F0
    const CCharString& GetDisplayName();                // 0x00779260
};
struct CTCActionUseScriptedHook {
    unsigned char           _pad00[0x0C];
    bool                    Usable;                // +0x0C
    unsigned char           _pad0d[0x20 - 0x0D];
    CBaseIntelligentPointer PEntranceConnectedTo;  // +0x20
    const CCharString& GetRegionEntranceName();         // 0x00729C50
    const CCharString& GetRegionEntranceDisplayName();  // 0x00729CA0
};

struct CThing {
    unsigned char  _pad00[0x20];
    CBitset274     UsingComponents;                // +0x20
    CInterfaceMap  Components;                     // +0x44
    unsigned char  _pad4c[0x8E - 0x4C];
    short          MapNumber;                      // +0x8E
    const C3DVector& GetPosition();                // 0x004C73D0
    bool GetTCDriver(CTCDRegionExit** out);        // 0x00617D30
    CTCActionUseScriptedHook* GetScriptedHook()
    {
        CTCActionUseScriptedHook* tc;
        if (UsingComponents.test(0xC2)) {
            long type = 0xC2;
            const CInterfaceMap::SEntry* it = Components.LowerBound(&type);
            if (it == Components.last || 0xC2 < it->type)
                it = Components.last;
            tc = (CTCActionUseScriptedHook*)it->component;
        }
        return tc;
    }
};

struct CThingNode {
    CThingNode* _M_next;
    CThingNode* _M_prev;
    CThing*     _M_data;
};
struct CAllocatorTag { CAllocatorTag() {} };
struct CThingListBase {                            // STLport _List_base<CThing*> {node}
    CThingNode* _M_node;
    CThingListBase(const CAllocatorTag& alloc);    // 0x00615520
    ~CThingListBase();                             // 0x00615540
};
struct CThingList : CThingListBase {               // list<CThing*>: inline ctor taking the allocator by value
    __forceinline CThingList(CAllocatorTag alloc = CAllocatorTag()) : CThingListBase(alloc) {}
};

struct CThingFilter_HasTCOfLeafType {              // the TC leaf type the search keeps
    long LeafType;
    CThingFilter_HasTCOfLeafType(long type) : LeafType(type) {}
};
struct CThingFilter_HasScriptedHook { CThingFilter_HasScriptedHook() {} };
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasTCOfLeafType& filter, CThingList* things);   // 0x00617CA0
    void GetAllThings(const CThingFilter_HasScriptedHook& filter, CThingList* things);   // 0x00618E60
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
    unsigned char      _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;          // +0x20
};

struct CDrawMiniMap {
    unsigned char          _pad000[0x0C];
    CWorld*                PWorld;                 // +0x00C
    unsigned char          _pad010[0x154 - 0x010];
    CRegionExitLabelVector RegionExits;            // +0x154
    void UpdateRegionExitNames(CRegion* region);
};

void CDrawMiniMap::UpdateRegionExitNames(CRegion* region)
{
    RegionExits.clear();
    CWorldMap* worldMap = PWorld->GetWorldMap();
    long regionNumber = worldMap->GetRegionNumberFromName(region->Name);
    {
        CThingList exits;
        PWorld->PThingSearchTools->GetAllThings(CThingFilter_HasTCOfLeafType(100), &exits);
        for (CThingNode* node = exits._M_node->_M_next; node != exits._M_node; node = node->_M_next) {
            CThing* thing = node->_M_data;
            if (worldMap->GetRegionNumberMapIsIn(thing->MapNumber) == regionNumber) {
                CTCDRegionExit* exit;
                thing->GetTCDriver(&exit);
                if (exit->Active) {
                    const CCharString& name = exit->GetNameOfRegionConnectingTo();
                    C3DVector pos = thing->GetPosition();
                    C2DVector offset;
                    if (region->GetMiniMapRegionExitTextOffset(name, &offset)) {
                        pos.X += offset.X;
                        pos.Y += offset.Y;
                    }
                    RegionExits.push_back(CRegionExitLabel(pos, exit->GetDisplayName()));
                }
            }
        }
        CThingList entrances;
        PWorld->PThingSearchTools->GetAllThings(CThingFilter_HasScriptedHook(), &entrances);
        for (CThingNode* node2 = entrances._M_node->_M_next; node2 != entrances._M_node; node2 = node2->_M_next) {
            CThing* thing = node2->_M_data;
            if (worldMap->GetRegionNumberMapIsIn(thing->MapNumber) == regionNumber) {
                CTCActionUseScriptedHook* hook = thing->GetScriptedHook();
                if (hook->Usable && hook->PEntranceConnectedTo.GetPItem()) {
                    const CCharString& name = hook->GetRegionEntranceName();
                    C3DVector pos = thing->GetPosition();
                    C2DVector offset;
                    if (region->GetMiniMapRegionExitTextOffset(name, &offset)) {
                        pos.X += offset.X;
                        pos.Y += offset.Y;
                    }
                    RegionExits.push_back(CRegionExitLabel(pos, hook->GetRegionEntranceDisplayName()));
                }
            }
        }
    }
}
