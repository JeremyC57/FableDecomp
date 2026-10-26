// CTCInventoryMap::CTCInventoryMap(CThing& thing) -- construct the inventory map screen component: both bases, empty region/log-book/primitive containers and maps, an empty region-correspondence list, the screen flags and counters at their start values
// Leaf: CTCInventoryMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00606D20, 608 bytes (`ret 4`). Label correction: the catalog calls this
//   row "CPSCOrbit::CPSCOrbit"; it stores CTCInventoryMap's RTTI vtable 0x01257A9C (the second-base vtable 0x01257A84 at
//   +0x130) and is called from CTCInventoryMap::Alloc 0x004D2F13 after operator new(0x258).
// Callees: CTCInventoryBase::CTCInventoryBase(CThing&) 0x005BFF07, the second base's ctor 0x0066E734 (at +0x130),
//   malloc 0x00BFEA0E (the two map header nodes and the list header node).
// Layout: Ego_r.pdb CTCInventoryMap (0x2C0) with the retail CTCInventoryBase 0x130 (PDB 0x148), 12-byte STLport vectors
//   and maps (header, count, compare), a 4-byte list head: VisitedRegions +0x134, AccumFadeTime +0x140, CategoriesList
//   +0x144, the four log-book vectors +0x150, GenerateDataTimer +0x180, the four mini-map flags +0x184, pmap_real +0x188,
//   MiniMapMarkers +0x18C, UIRegionsInfo +0x198, QuestToBeDropped +0x1A4, FirstTimeCheckingCurrentRegion +0x1A8, the
//   seven primitive/position vectors +0x1AC, SerialisedTeleporterUIDs +0x200, TeleportRegions +0x208, MapList +0x214,
//   MapRegions +0x218, MapBox +0x224, WorldMapActive +0x234, ActiveRegionIndex +0x238, NumRegions +0x23C,
//   InventoryScreen +0x240, OpenedViaGuildSeal +0x241, PreviousScreenType +0x244, PanMode +0x248,
//   RegionsCorrespondances +0x24C, LastEventFrame +0x250, RegionEventLife +0x254 (size 0x258). No engine header.
// Shape: the landed CTCCameraPointScriptedSpline ctor 0x0075DD10 container models (malloc'd STLport headers); every
//   member in the init list except QuestToBeDropped, a body assignment (retail stores it last).
class CThing;

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CRetailVector {                               // STLport vector (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <int ValueSize> struct SRbNode {
    bool _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; unsigned char _M_value[ValueSize];
};
template <int ValueSize> struct CRetailMap {         // STLport map: header pointer, count, empty compare
    SRbNode<ValueSize>* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
    CRetailMap() : _M_header(0)
    {
        _M_header = (SRbNode<ValueSize>*)malloc(sizeof(SRbNode<ValueSize>));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; const void* _M_first; const void* _M_second; };
struct CRegionPairList {                             // std::list<pair<const CRegion*, const CRegion*> >
    SListNode* _M_node;
    CRegionPairList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct CCountedPointerUIDs {                         // CCountedPointer<vector<unsigned __int64> >
    void* Data;
    void* Info;
    CCountedPointerUIDs() : Data(0), Info(0) {}
};
class C2DBoxI { public: long X0, Y0, X1, Y1; };
enum EScreenType { SCREEN_TYPE_NONE = -1 };
class CZoomingComponent;
class CList;

class CTCInventoryBase {                             // vtable stored by the derived class
public:
    virtual void v00();
    unsigned char _body[0x130 - 4];
    CTCInventoryBase(CThing& thing);                 // 0x005BFF07
};
class CInventoryMapListener {                        // the second base (+0x130), vtable 0x01257A84 in CTCInventoryMap
public:
    virtual void l00();
    CInventoryMapListener();                         // 0x0066E734
};

class CTCInventoryMap : public CTCInventoryBase, public CInventoryMapListener {
public:
    CRetailVector       VisitedRegions;              // +0x134
    float               AccumFadeTime;               // +0x140
    CRetailMap<8>       CategoriesList;              // +0x144
    CRetailVector       LogBookNameTag;              // +0x150
    CRetailVector       LogBookAbbreviatedNameTag;   // +0x15c
    CRetailVector       LogBookTextTag;              // +0x168
    CRetailVector       LogBookCategory;             // +0x174
    long                GenerateDataTimer;           // +0x180
    bool                MiniMapIsPrisonCourtyard1;   // +0x184
    bool                MiniMapIsPrisonCourtyard2;   // +0x185
    bool                MiniMapIsHeadsmansHillNorth; // +0x186
    bool                MiniMapIsHeadsmansHillSouth; // +0x187
    CZoomingComponent*  pmap_real;                   // +0x188
    CRetailVector       MiniMapMarkers;              // +0x18c
    CRetailVector       UIRegionsInfo;               // +0x198
    long                QuestToBeDropped;            // +0x1a4
    bool                FirstTimeCheckingCurrentRegion;   // +0x1a8
    CRetailVector       MapScreenPrimitiveHandles;   // +0x1ac
    CRetailVector       MapScreenQuestPrimitiveHandles;   // +0x1b8
    CRetailVector       MapScreenTeleportPrimitiveHandles;   // +0x1c4
    CRetailVector       MapScreenHousePrimitiveHandles;   // +0x1d0
    CRetailVector       ActiveRegionQuestPositions;  // +0x1dc
    CRetailVector       ActiveRegionTeleportPositions;   // +0x1e8
    CRetailVector       ActiveRegionHousePositions;  // +0x1f4
    CCountedPointerUIDs SerialisedTeleporterUIDs;    // +0x200
    CRetailVector       TeleportRegions;             // +0x208
    CList*              MapList;                     // +0x214
    CRetailMap<0x5c>    MapRegions;                  // +0x218
    C2DBoxI             MapBox;                      // +0x224
    bool                WorldMapActive;              // +0x234
    long                ActiveRegionIndex;           // +0x238
    long                NumRegions;                  // +0x23c
    bool                InventoryScreen;             // +0x240
    bool                OpenedViaGuildSeal;          // +0x241
    EScreenType         PreviousScreenType;          // +0x244
    bool                PanMode;                     // +0x248
    CRegionPairList     RegionsCorrespondances;      // +0x24c
    long                LastEventFrame;              // +0x250
    unsigned long       RegionEventLife;             // +0x254
    virtual void v00();
    CTCInventoryMap(CThing& thing);
};

CTCInventoryMap::CTCInventoryMap(CThing& thing)
    : CTCInventoryBase(thing),
      AccumFadeTime(0.0f),
      GenerateDataTimer(-1),
      MiniMapIsPrisonCourtyard1(false),
      MiniMapIsPrisonCourtyard2(false),
      MiniMapIsHeadsmansHillNorth(false),
      MiniMapIsHeadsmansHillSouth(false),
      pmap_real(0),
      FirstTimeCheckingCurrentRegion(true),
      MapList(0),
      WorldMapActive(true),
      ActiveRegionIndex(1),
      NumRegions(0),
      InventoryScreen(true),
      OpenedViaGuildSeal(false),
      PreviousScreenType(SCREEN_TYPE_NONE),
      PanMode(false),
      LastEventFrame(0),
      RegionEventLife(1)
{
    QuestToBeDropped = 0;
}