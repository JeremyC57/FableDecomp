// Behaviour test for CTCInventoryMap::CTCInventoryMap (0x00606D20): both bases constructed once, the two maps and the
// list get self-linked empty header nodes, the vectors are empty and the scalar members take their start values.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct CRetailVector {
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <int ValueSize> struct SRbNode {
    bool _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; unsigned char _M_value[ValueSize];
};
template <int ValueSize> struct CRetailMap {
    SRbNode<ValueSize>* _M_header; unsigned long _M_node_count; unsigned char _M_key_compare;
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
struct CRegionPairList {
    SListNode* _M_node;
    CRegionPairList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node; node->_M_prev = node; _M_node = node;
    }
};
struct CCountedPointerUIDs { void* Data; void* Info; CCountedPointerUIDs() : Data(0), Info(0) {} };
struct C2DBoxI { int X0, Y0, X1, Y1; };
enum EScreenType { SCREEN_TYPE_NONE = -1 };
struct CThing { int id; };
static int g_base1 = 0, g_base2 = 0; static const CThing* g_thing = 0;
struct CTCInventoryBase { virtual ~CTCInventoryBase() {} CTCInventoryBase(CThing& t) { g_base1++; g_thing = &t; } };
struct CInventoryMapListener { virtual ~CInventoryMapListener() {} CInventoryMapListener() { g_base2++; } };
struct CTCInventoryMap : CTCInventoryBase, CInventoryMapListener {
    CRetailVector VisitedRegions; float AccumFadeTime; CRetailMap<8> CategoriesList;
    CRetailVector LogBookNameTag, LogBookAbbreviatedNameTag, LogBookTextTag, LogBookCategory;
    int GenerateDataTimer; bool C1, C2, HN, HS; void* pmap_real; CRetailVector MiniMapMarkers, UIRegionsInfo;
    int QuestToBeDropped; bool FirstTimeCheckingCurrentRegion; CRetailVector P[7]; CCountedPointerUIDs Uids;
    CRetailVector TeleportRegions; void* MapList; CRetailMap<0x5c> MapRegions; C2DBoxI MapBox; bool WorldMapActive;
    int ActiveRegionIndex, NumRegions; bool InventoryScreen, OpenedViaGuildSeal; EScreenType PreviousScreenType;
    bool PanMode; CRegionPairList RegionsCorrespondances; int LastEventFrame; unsigned int RegionEventLife;
    CTCInventoryMap(CThing& thing)
        : CTCInventoryBase(thing), AccumFadeTime(0.0f), GenerateDataTimer(-1), C1(false), C2(false), HN(false), HS(false),
          pmap_real(0), FirstTimeCheckingCurrentRegion(true), MapList(0), WorldMapActive(true), ActiveRegionIndex(1),
          NumRegions(0), InventoryScreen(true), OpenedViaGuildSeal(false), PreviousScreenType(SCREEN_TYPE_NONE),
          PanMode(false), LastEventFrame(0), RegionEventLife(1)
    { QuestToBeDropped = 0; }
};
int main()
{
    int fails = 0;
    static unsigned char storage[sizeof(CTCInventoryMap)];
    memset(storage, 0xCD, sizeof(storage));
    CThing t; t.id = 7;
    CTCInventoryMap* m = new (storage) CTCInventoryMap(t);
    if (g_base1 != 1 || g_base2 != 1 || g_thing != &t) fails++;
    if (m->CategoriesList._M_node_count != 0 || m->CategoriesList._M_header->_M_left != m->CategoriesList._M_header) fails++;
    if (m->CategoriesList._M_header->_M_color || m->CategoriesList._M_header->_M_parent) fails++;
    if (m->MapRegions._M_header->_M_right != m->MapRegions._M_header) fails++;
    if (m->RegionsCorrespondances._M_node->_M_next != m->RegionsCorrespondances._M_node) fails++;
    if (m->VisitedRegions._M_start || m->P[6]._M_end_of_storage || m->Uids.Info || m->TeleportRegions._M_finish) fails++;
    if (m->GenerateDataTimer != -1 || m->PreviousScreenType != SCREEN_TYPE_NONE || m->QuestToBeDropped != 0) fails++;
    if (!m->FirstTimeCheckingCurrentRegion || !m->WorldMapActive || !m->InventoryScreen || m->OpenedViaGuildSeal || m->PanMode) fails++;
    if (m->ActiveRegionIndex != 1 || m->NumRegions != 0 || m->RegionEventLife != 1 || m->LastEventFrame != 0) fails++;
    if (m->MapBox.X0 != (int)0xCDCDCDCD) fails++;    // the box is left for the first update
    if (fails == 0) printf("00606d20_TEST PASS\n"); else printf("00606d20_TEST FAIL %d\n", fails);
    return fails;
}
