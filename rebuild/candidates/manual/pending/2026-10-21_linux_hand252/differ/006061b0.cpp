// Grade: DIFFER(322v322) -- loop register swap only: retail keeps the list node in edi and the card pointer (node+8) in esi (edi pushed before the empty-list test); ours node in esi, pointer in edi
// CTCInventoryMap::FrameUpdate() -- once the region event life has passed since the last event frame, refresh the region correspondences and the visited regions and stamp the frame; then, when a quest is waiting to be dropped, find its live quest card among the quest manager's active cards and, when the card carries a quest-card component, fail the quest (no text) and deactivate it; the drop request is cleared either way
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006061B0, 336 bytes (+ int3 padding), `ret` (virtual, no arguments).
//   Ego_r.pdb: `?FrameUpdate@CTCInventoryMap@@UAEXXZ` (manifest "vfunc_9").
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static), CTCInventoryMap::UpdateRegionsCorrespondances(long, long)
//   @ 0x005FC0C0 and UpdateVisitedRegions(long, long) @ 0x00605CE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//   the component map's LowerBound @ 0x0040F020, CWideString(const wchar_t*) @ 0x0099B6B0 (L"" @ 0x0122D70C) /
//   ~CWideString @ 0x0099B510, CQuestManager::SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool)
//   @ 0x004B2280, CQuestManager::DeactivateQuest(const CCharString&, unsigned long) @ 0x004B43D0.
// Globals: the quest manager @ 0x013B89FC.
// Shape: the component lookup is the landed CInputProcessFishing 0x00690B20 inline Find (key and the wide-string
//   temporary share the one frame word); the found-card path ends in `break`, so the drop-request clear is
//   tail-duplicated by VC7.1.
// Layout: retail CTCInventoryMap (Ego_r.pdb order, smaller release containers): QuestToBeDropped +0x1A4,
//   LastEventFrame +0x250, RegionEventLife +0x254; retail CQuestManager ActiveQuestCards +0x5C (4-byte STLport list of
//   CIntelligentPointer<CThing>, vptr + item); CThing component bitset +0x20, map +0x44, def index +0x8C, dead bit
//   +0x91; the quest-card component (0x6C) quest name +0x28.
#include <stddef.h>
class CCharString { void* PStringData; };
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);              // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
};
struct CWorld {
    static long GetWorldFrame();                   // 0x0049D870
};
struct CTCQuestCard {
    unsigned char _pad00[0x28];
    CCharString QuestName;                         // +0x28
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long          m_kind;
    CTCQuestCard* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCQuestCard* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                          // +0x44
    unsigned char  _pad50[0x8c - 0x50];
    unsigned short DefIndex;                       // +0x8C
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  IsDead : 1;                     // +0x91 bit 0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCQuestCard* GetTC(long kind) { return m_tcs.Find(kind); }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct SCardNode {
    SCardNode* _M_next;
    SCardNode* _M_prev;
    CBaseIntelligentPointer _M_data;               // +0x08
};
struct CCardListIterator {
    SCardNode* _M_node;
    CCardListIterator(SCardNode* node) : _M_node(node) { }
    CBaseIntelligentPointer& operator*() const { return _M_node->_M_data; }
    CCardListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CCardListIterator& other) const { return _M_node != other._M_node; }
};
struct CCardList {                                 // STLport list<CIntelligentPointer<CThing> > (retail: node pointer)
    SCardNode* _M_node;
    CCardListIterator begin() { return CCardListIterator(_M_node->_M_next); }
    CCardListIterator end() { return CCardListIterator(_M_node); }
};
struct CQuestManager {
    unsigned char _pad00[0x5c];
    CCardList ActiveQuestCards;                    // +0x5C
    bool SetQuestAsFailed(const CCharString& name, bool failed, const CWideString& text, bool showMessage);   // 0x004B2280
    void DeactivateQuest(const CCharString& name, unsigned long flags);                                     // 0x004B43D0
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC

struct CTCInventoryMap {
    unsigned char _pad004[0x1a4 - 0x4];              // vptr at +0x00
    long          QuestToBeDropped;                // +0x1A4
    unsigned char _pad1a8[0x250 - 0x1a8];
    long          LastEventFrame;                  // +0x250
    unsigned long RegionEventLife;                 // +0x254
    void UpdateRegionsCorrespondances(long fromFrame, long toFrame);   // 0x005FC0C0
    void UpdateVisitedRegions(long fromFrame, long toFrame);           // 0x00605CE0
    virtual void FrameUpdate();
};

void CTCInventoryMap::FrameUpdate()
{
    if ((unsigned long)CWorld::GetWorldFrame() >= LastEventFrame + RegionEventLife) {
        UpdateRegionsCorrespondances(LastEventFrame, CWorld::GetWorldFrame());
        UpdateVisitedRegions(LastEventFrame, CWorld::GetWorldFrame());
        LastEventFrame = CWorld::GetWorldFrame();
    }
    if (QuestToBeDropped > 0) {
        CCardList& cards = g_pQuestManager->ActiveQuestCards;
        for (CCardListIterator it = cards.begin(); it != cards.end(); ++it) {
            const CBaseIntelligentPointer& pointer = *it;
            if (pointer.GetPItem() && !pointer.GetPItem()->IsDead && pointer.GetPItem()->DefIndex == QuestToBeDropped) {
                CThing* card = pointer.GetPItem();
                if (card->HasTC(0x6c)) {
                    CTCQuestCard* questCard = card->GetTC(0x6c);
                    {
                        CWideString noText(L"");
                        g_pQuestManager->SetQuestAsFailed(questCard->QuestName, true, noText, true);
                    }
                    g_pQuestManager->DeactivateQuest(questCard->QuestName, 0);
                }
                break;
            }
        }
        QuestToBeDropped = 0;
    }
}
