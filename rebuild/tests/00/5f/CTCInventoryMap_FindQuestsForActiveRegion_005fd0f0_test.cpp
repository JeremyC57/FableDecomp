// Behaviour test for CTCInventoryMap::FindQuestsForActiveRegion @ 0x005FD0F0 (model copy; stub world map, quest list, overflow insert).
#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
class C2DCoordI { public: long X; long Y; };
static long g_overflows = 0;
struct CCoordVector {
    C2DCoordI* _M_start; C2DCoordI* _M_finish; C2DCoordI* _M_end_of_storage;
    C2DCoordI* begin() { return _M_start; }
    C2DCoordI* end() { return _M_finish; }
    static C2DCoordI* copy(C2DCoordI* first, C2DCoordI* last, C2DCoordI* result)
    { for (long n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result; }
    C2DCoordI* erase(C2DCoordI* first, C2DCoordI* last) { C2DCoordI* i = copy(last, _M_finish, first); _M_finish = i; return first; }
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(C2DCoordI* position, const C2DCoordI& x, const __false_type& tag, unsigned long n, bool atEnd)
    { ++g_overflows; if (position == _M_finish && n == 1 && atEnd) { /* no room in the fixture: drop */ } }
    void push_back(const C2DCoordI& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) C2DCoordI(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CRegion { public: long id; };
class CWorldMap { public: CRegion regions[4]; CRegion& GetRegion(long index) { return regions[index]; } };
class CWorld { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual CWorldMap* GetWorldMap() { return 0; }
    virtual CWorldMap* PeekWorldMap() { return &map; }
    CWorldMap map;
};
struct CActiveQuest {
    unsigned char _pad00[0x1c]; C2DCoordI MarkerPosition; long regionId; long checks;
    bool ContainsRegion(const CRegion& region) const { const_cast<CActiveQuest*>(this)->checks++; return region.id == regionId; }
};
struct CActiveQuestNode { CActiveQuestNode* _M_next; CActiveQuestNode* _M_prev; CActiveQuest* Data; void* Info; };
struct CActiveQuestList { CActiveQuestNode* _M_node; CActiveQuestNode* begin() const { return _M_node->_M_next; } CActiveQuestNode* end() const { return _M_node; } };
class CQuestManager { public: unsigned char _pad00[0x38]; CActiveQuestList ActiveQuests; };
static CQuestManager g_questManager;
static CQuestManager* g_pQuestManager = &g_questManager;
class CTCInventoryMap { public:
    unsigned char _pad00[0x1dc]; CCoordVector ActiveRegionQuestPositions; unsigned char _pad1e8[0x238 - 0x1e8]; long ActiveRegionIndex;
    CWorld* world; CWorld* GetWorld() const { return world; }
    void FindQuestsForActiveRegion();
};
void CTCInventoryMap::FindQuestsForActiveRegion()
{
    ActiveRegionQuestPositions.clear();
    const CRegion& region = GetWorld()->PeekWorldMap()->GetRegion(ActiveRegionIndex);
    CActiveQuestNode* node;
    CActiveQuest* quest;
    const CActiveQuestList& quests = g_pQuestManager->ActiveQuests;
    for (node = quests.begin(); node != quests.end(); node = node->_M_next) {
        quest = node->Data;
        if (quest->ContainsRegion(region)) {
            const C2DCoordI& marker = quest->MarkerPosition;
            if (marker.X != 0 || marker.Y != 0)
                ActiveRegionQuestPositions.push_back(marker);
        }
    }
}
int main()
{
    CWorld world; for (int i = 0; i < 4; ++i) world.map.regions[i].id = 100 + i;
    CActiveQuest q[4];
    q[0].regionId = 102; q[0].MarkerPosition.X = 5;  q[0].MarkerPosition.Y = 6;   // in region, marker -> kept
    q[1].regionId = 101; q[1].MarkerPosition.X = 7;  q[1].MarkerPosition.Y = 8;   // other region -> skipped
    q[2].regionId = 102; q[2].MarkerPosition.X = 0;  q[2].MarkerPosition.Y = 0;   // zero marker -> skipped
    q[3].regionId = 102; q[3].MarkerPosition.X = 0;  q[3].MarkerPosition.Y = 9;   // Y only -> kept
    for (int i = 0; i < 4; ++i) q[i].checks = 0;
    CActiveQuestNode sentinel, n[4];
    sentinel._M_next = &n[0]; sentinel._M_prev = &n[3];
    for (int i = 0; i < 4; ++i) { n[i].Data = &q[i]; n[i]._M_next = i < 3 ? &n[i + 1] : &sentinel; n[i]._M_prev = i > 0 ? &n[i - 1] : &sentinel; }
    g_questManager.ActiveQuests._M_node = &sentinel;
    C2DCoordI storage[3]; storage[0].X = -1; storage[0].Y = -1; storage[1].X = -1; storage[1].Y = -1; storage[2].X = -1;
    CTCInventoryMap map; map.world = &world; map.ActiveRegionIndex = 2;
    map.ActiveRegionQuestPositions._M_start = storage; map.ActiveRegionQuestPositions._M_finish = storage + 2; map.ActiveRegionQuestPositions._M_end_of_storage = storage + 3;
    map.FindQuestsForActiveRegion();
    bool ok = true;
    if (map.ActiveRegionQuestPositions._M_finish - storage != 2) ok = false;    // cleared then two markers appended
    if (storage[0].X != 5 || storage[0].Y != 6 || storage[1].X != 0 || storage[1].Y != 9) ok = false;
    for (int i = 0; i < 4; ++i) if (q[i].checks != 1) ok = false;
    if (g_overflows != 0) ok = false;
    // a full vector hands the extra marker to the overflow insert
    map.ActiveRegionQuestPositions._M_end_of_storage = storage + 1;
    map.FindQuestsForActiveRegion();
    if (map.ActiveRegionQuestPositions._M_finish - storage != 1 || storage[0].X != 5 || g_overflows != 1) ok = false;
    // an empty list clears only
    g_questManager.ActiveQuests._M_node = &sentinel; sentinel._M_next = &sentinel;
    map.FindQuestsForActiveRegion();
    if (map.ActiveRegionQuestPositions._M_finish != storage) ok = false;
    if (!ok) { std::printf("005fd0f0_TEST FAIL\n"); return 1; }
    std::printf("005fd0f0_TEST PASS\n");
    return 0;
}