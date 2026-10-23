// CTCInventoryQuests::InitialiseQuestCardList() -- rebuilds the quest-card categories from the active quest cards (or the associated thing's cards)
// Grade: DIFFER(620v600) -- the inlined CVectorMap lower_bound loop keeps an extra eax copy (known lane gap: inlined lower_bound eax copy)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
struct CThing;
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C
    virtual ~CIntelligentPointer() {}
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); }
};
struct __true_type { __true_type() {} };
struct __false_type {};
struct random_access_iterator_tag {};
struct CQuestCard { unsigned char Data[0x2c]; };
CQuestCard* __fastcall __copy(CQuestCard* first, CQuestCard* last, CQuestCard* result, const random_access_iterator_tag&, int*);   // 0x00625820
void __fastcall __destroy_aux(CQuestCard* first, CQuestCard* last, const __false_type&);   // 0x005C2A8E
struct CQuestCardVector {
    CQuestCard* _M_start;
    CQuestCard* _M_finish;
    CQuestCard* _M_end_of_storage;
    void clear()
    {
        random_access_iterator_tag tag;
        CQuestCard* i = __copy(_M_finish, _M_finish, _M_start, tag, (int*)0);
        __false_type ftag;
        __destroy_aux(i, _M_finish, ftag);
        _M_finish = i;
    }
};
struct CCardVector {
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    CCardVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CCardVector()
    {
        for (CIntelligentPointer* p = _M_start; p != _M_finish; ++p)
            p->~CIntelligentPointer();
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(CIntelligentPointer* pos, const CIntelligentPointer& x, const __true_type&, unsigned long n, bool atend);   // 0x004AC940
    void push_back(const CIntelligentPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CIntelligentPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CCardListNode { CCardListNode* _M_next; CCardListNode* _M_prev; CIntelligentPointer _M_data; };
struct CQuestManager { unsigned char _pad[0x5c]; CCardListNode* ActiveCards; };   // +0x5c list header
extern CQuestManager* g_pQuestManager;                 // 0x013B89FC
struct CTCQuestCards { void GetQuestCards(CCardVector& out); };   // 0x0077DBC0
struct CQuestCardDef { virtual void v0(); virtual void Destroy(); long m_refCount; unsigned char _pad[0x40 - 0x0c]; long Title; };
struct CTCQuestCard { unsigned char _pad[0x0c]; CQuestCardDef* Def; };   // +0x0c
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    SEntry* InlineLowerBound(long type)
    {
        SEntry* f = first;
        int len = last - f;
        while (len > 0) {
            int half = len / 2;
            if (f[half].type < type) {
                f += half + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return f;
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCInline(long type)
    {
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->InlineLowerBound(type);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
namespace NHeroInformationScreens {
struct CBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool IsActive();                         // slot 3 (+0x0C)
};
}
struct CCategory { void SetAsSelected(bool b);       // 0x005BC934
};
struct CTCInventoryQuests;
struct CInventoryOwner {};
struct CTCInventoryQuests {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74)
#undef V
    virtual bool ShowAssociatedCards();              // slot 75 (+0x12C)
    CThing*          ParentThing;                    // +0x04
    unsigned char    _pad08[0x20 - 0x08];
    CQuestCardVector QuestCards;                     // +0x20
    unsigned char    _pad2c[0x130 - 0x2c];
    NHeroInformationScreens::CBase Screen;           // +0x130
    unsigned char    _pad134[0x150 - 0x134];
    long             NumberOfCards;                  // +0x150
    CThing* GetAssociatedThing();                    // 0x005BC4C4
    void AddCategory(CTCInventoryQuests* owner, long title, CThing* association);   // 0x005C024A
    CCategory* GetCategoryWithIndex(long index);     // 0x005BCE3C
    void InitialiseQuestCardList();
};
void CTCInventoryQuests::InitialiseQuestCardList()
{
    QuestCards.clear();
    CCardVector cards;
    if (Screen.IsActive()) {
        CCardListNode* head = g_pQuestManager->ActiveCards;
        for (CCardListNode* node = head->_M_next; node != g_pQuestManager->ActiveCards; node = node->_M_next) {
            CIntelligentPointer card(node->_M_data);
            cards.push_back(card);
        }
    } else if (ShowAssociatedCards()) {
        CThing* thing = GetAssociatedThing();
        if (thing) {
            CTCQuestCards* holder = 0;
            if (thing->HasTC(0x6a))
                holder = thing->GetTC<CTCQuestCards>(0x6a);
            holder->GetQuestCards(cards);
        }
    }
    NumberOfCards = cards.size();
    for (long i = 0; i < NumberOfCards; ++i) {
        CThing* card = cards._M_start[i].GetPItem();
        if (card->HasTC(0x6c)) {
            CTCQuestCard* questCard = card->GetTCInline<CTCQuestCard>(0x6c);
            CQuestCardDef* def = questCard->Def;
            if (def)
                ++def->m_refCount;
            long title = def->Title;
            if (--def->m_refCount == 0)
                def->Destroy();
            AddCategory(this, title, card);
        }
    }
    if (NumberOfCards > 0)
        GetCategoryWithIndex(0)->SetAsSelected(true);
}
