// CTCInventoryMap::FindQuestsForActiveRegion() -- rebuilds ActiveRegionQuestPositions: the marker position of every active quest that contains the active region (zero markers skipped)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FD0F0, length 224 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CTCBase::GetWorld() const @ 0x00686A80 (manifest "CViewBase::GetDisplayEngine" is propagated), CWorld virtual slot 13 (+0x34,
// PeekWorldMap), CWorldMap::GetRegion(long) @ 0x004FC180 (CRegion&), CQuestManager::CActiveQuest::ContainsRegion(const CRegion&) const
// @ 0x004AFD80 (manifest "CHitParameters::IsDivineWrathStrike" is propagated), vector<C2DCoordI>::_M_insert_overflow(pos, x, __false_type,
// n, bool) @ 0x00618550 (manifest "CVector_InsertRange"); the active-quest list is g_pQuestManager(0x013B89FC)->ActiveQuests (+0x38).
// Layout: retail ActiveRegionQuestPositions +0x1DC (12-byte vector of 8-byte C2DCoordI; PDB +0x224), ActiveRegionIndex +0x238; CQuestManager::
// ActiveQuests at +0x38 (4-byte STLport list of CCountedPointer<CActiveQuest> nodes, the quest pointer at node +8); CActiveQuest::
// MarkerPosition +0x1C (PDB +0x20).
// Shape: the active-quest list is held by reference (ebp = its address; end() re-reads the sentinel each pass) and the loop node is
// declared before the quest pointer so that node takes edi and quest ebx; the marker is a reference (its Y is read through the lea'd pointer); the inline STLport clear() is erase(begin, end): the copy loop over (finish - end)/8 = 0 elements survives and the finish store
// follows; the region reference is a frame local re-read for every ContainsRegion push; the inline push_back is the landed
// PushBackConstant form (placement copy when there is room, else _M_insert_overflow with the empty tag homed in the frame byte).
#include <new>
struct __false_type { __false_type() { } };
struct C2DCoordI {
    long X;
    long Y;
};
struct CCoordVector {                              // std::vector<C2DCoordI> (12 bytes)
    C2DCoordI* _M_start;
    C2DCoordI* _M_finish;
    C2DCoordI* _M_end_of_storage;
    C2DCoordI* begin() { return _M_start; }
    C2DCoordI* end() { return _M_finish; }
    static C2DCoordI* copy(C2DCoordI* first, C2DCoordI* last, C2DCoordI* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    C2DCoordI* erase(C2DCoordI* first, C2DCoordI* last)
    {
        C2DCoordI* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(C2DCoordI* position, const C2DCoordI& x, const __false_type& tag, unsigned long n, bool atEnd);   // 0x00618550
    void push_back(const C2DCoordI& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C2DCoordI(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CRegion;
struct CWorldMap {
    CRegion& GetRegion(long index);                // 0x004FC180
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();              // slot 12
    virtual CWorldMap* PeekWorldMap();             // slot 13 (+0x34)
};
struct CActiveQuest {                              // CQuestManager::CActiveQuest
    unsigned char _pad00[0x1c];
    C2DCoordI MarkerPosition;                      // +0x1c (PDB +0x20)
    bool ContainsRegion(const CRegion& region) const;    // 0x004AFD80
};
struct CActiveQuestNode {                          // STLport list<CCountedPointer<CActiveQuest>> node
    CActiveQuestNode* _M_next;
    CActiveQuestNode* _M_prev;
    CActiveQuest*     Data;                        // +0x08 CCountedPointer::Data
    void*             Info;
};
struct CActiveQuestList {                         // STLport list<CCountedPointer<CActiveQuest>> (4 bytes: the sentinel)
    CActiveQuestNode* _M_node;
    CActiveQuestNode* begin() const { return _M_node->_M_next; }
    CActiveQuestNode* end() const { return _M_node; }
};
struct CQuestManager {
    unsigned char _pad00[0x38];
    CActiveQuestList ActiveQuests;                 // +0x38
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC
struct CTCInventoryMap {
    unsigned char _pad00[0x1dc];
    CCoordVector  ActiveRegionQuestPositions;      // +0x1dc
    unsigned char _pad1e8[0x238 - 0x1e8];
    long          ActiveRegionIndex;               // +0x238
    CWorld* GetWorld() const;                      // 0x00686A80 (CTCBase)
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