// CQuestManager::InitializeDaySection() -- entering the day: collect every "DayOnly" section of the current load-section set and have the world map load the things in them, then mark the day active
// Leaf: InitializeDaySection
// Retail 0x004B3060, 499 bytes. The twin of the landed CQuestManager::UninitializeDaySection (0x004B3260 is the
// day one) and of 0x004B3060 / 0x004B33E0: malloc 0x00BFEA0E / free 0x00BFEA14 (each set's header node), the set erase
// 0x004ABA70, set insert 0x004AC380 (pair<iterator,bool> result discarded), CQuestManager::BuildLoadSections
// 0x004B2CA0, GetDayOrNightOnlyStringFromFullSectionName 0x004AF350 (on the node's value itself, no copy),
// ~CCharString 0x0099EAE0, Compare 0x00411570 (the landed body, auto_inline(off)), the world's GetWorldMap (slot 12,
// +0x30), CWorldMap::LoadThingsWithSectionNames 0x00500EB0 (its two arguments are pushed before the vcall).
// Globals: g_DayOnlySuffix 0x013BAC18 "DayOnly".
// Layout: engine/CQuestManager.h is the PDB layout (World +0xD8, DayActive +0xE8); retail sits 0x5C lower after its
// narrower STLport containers (World +0x7C, DayActive +0x8C, NightActive +0x8D), hence the landed local model.
// Shape: two local STLport set<CCharString> (the landed set model): the load sections, walked with the inline
// _M_increment, and the DayOnly subset handed to the world map; the CCharString equality is the landed nested-inline
// operator== (SameText), whose `xor al,al ; mov bl,al` keeps `this` spilled and reloaded after the loop.
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
struct CCharString {
    CCharStringData* PStringData;
    bool SameText(const CCharStringData& other) const
    {
        if (PStringData->m_length != other.m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.m_pData) ? false : true;
    }
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return SameText(*other.PStringData);
    }
};
extern CCharString g_DayOnlySuffix;                  // 0x013BAC18 "DayOnly"
struct CSectionNode { unsigned char Color; CSectionNode* Parent; CSectionNode* Left; CSectionNode* Right; CCharString Value; };
struct CSectionInsertResult { CSectionNode* Position; bool Inserted; CSectionInsertResult() {} };
struct CSectionSet {                                 // STLport set<CCharString> (12 bytes)
    CSectionNode* Head;
    unsigned long Count;
    unsigned long Compare;
    CSectionSet()
    {
        Head = 0;
        Head = (CSectionNode*)malloc(20);
        Count = 0;
        Head->Color = 0;
        Head->Parent = 0;
        Head->Left = Head;
        Head->Right = Head;
    }
    CSectionInsertResult Insert(const CCharString& value);   // 0x004AC380
    void Erase(CSectionNode* node);                  // 0x004ABA70
    ~CSectionSet()
    {
        if (Count) {
            Erase(Head->Parent);
            Head->Left = Head;
            Head->Parent = 0;
            Head->Right = Head;
            Count = 0;
        }
        if (Head)
            free(Head);
    }
    static CSectionNode* Increment(CSectionNode* node)
    {
        if (node->Right != 0) {
            node = node->Right;
            while (node->Left != 0)
                node = node->Left;
        } else {
            CSectionNode* y = node->Parent;
            while (node == y->Right) {
                node = y;
                y = y->Parent;
            }
            if (node->Right != y)
                node = y;
        }
        return node;
    }
};
struct CWorldMap {
    void LoadThingsWithSectionNames(const CSectionSet& sections, bool a2);   // 0x00500EB0
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                // slot 12 (+0x30)
};
struct CQuestManager {
    unsigned char _pad00[0x7c];
    CWorld*       World;                             // +0x7c
    unsigned char _pad80[0x0c];
    bool          DayActive;                         // +0x8c
    bool          NightActive;                       // +0x8d
    void BuildLoadSections(CSectionSet* sections);   // 0x004B2CA0
    CCharString GetDayOrNightOnlyStringFromFullSectionName(const CCharString& section);   // 0x004AF350
    void InitializeDaySection();
};

void CQuestManager::InitializeDaySection()
{
    if (DayActive)
        return;
    DayActive = true;
    CSectionSet sections;
    BuildLoadSections(&sections);
    CSectionSet daySections;
    for (CSectionNode* node = sections.Head->Left; node != sections.Head; node = CSectionSet::Increment(node)) {
        bool dayOnly = GetDayOrNightOnlyStringFromFullSectionName(node->Value) == g_DayOnlySuffix;
        if (dayOnly)
            daySections.Insert(node->Value);
    }
    World->GetWorldMap()->LoadThingsWithSectionNames(daySections, false);
}
