// CQuestManager::UninitializeNightSection() -- leaving the night: delete every thing placed by a "NightOnly" section of the current load-section set, then mark the night inactive
// Leaf: UninitializeNightSection
// Retail 0x004B35E0, 379 bytes. Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the set's header node), the set erase
// 0x004ABA70, CQuestManager::BuildLoadSections 0x004B2CA0 (as in the landed CWorldMap::GetLoadableSections
// 0x004FF490), CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0,
// GetDayOrNightOnlyStringFromFullSectionName 0x004AF350, Compare 0x00411570 (the landed body, auto_inline(off)),
// the world's GetThingManager (slot 3, +0x0C), CThingManager::DeleteThingsWithSectionName 0x0051FC20.
// Globals: g_DayOnlySuffix 0x013BAC18 "DayOnly", g_NightOnlySuffix 0x013BAC1C "NightOnly".
// Layout: engine/CQuestManager.h is the PDB layout (World +0xD8, DayActive +0xE8); retail sits 0x5C lower after its
// narrower STLport containers (World +0x7C, DayActive +0x8C, NightActive +0x8D), hence a local model.
// Shape: a local STLport set<CCharString> (the landed GetLoadableSections set model) walked with the inline
// _M_increment; the CCharString equality's length/Compare tail is a nested inline (SameText) -- written flat in
// operator== VC folds both results straight into bl instead of retail's `xor al,al ; mov bl,al` / `inc al ; mov bl,al`.
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
extern CCharString g_NightOnlySuffix;                // 0x013BAC1C "NightOnly"
struct CSectionNode { unsigned char Color; CSectionNode* Parent; CSectionNode* Left; CSectionNode* Right; CCharString Value; };
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
struct CThingManager {
    void DeleteThingsWithSectionName(const CCharString& section);   // 0x0051FC20
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();        // slot 3 (+0x0c)
};
struct CQuestManager {
    unsigned char _pad00[0x7c];
    CWorld*       World;                             // +0x7c
    unsigned char _pad80[0x0c];
    bool          DayActive;                         // +0x8c
    bool          NightActive;                       // +0x8d
    void BuildLoadSections(CSectionSet* sections);   // 0x004B2CA0
    CCharString GetDayOrNightOnlyStringFromFullSectionName(const CCharString& section);   // 0x004AF350
    void UninitializeNightSection();
};

void CQuestManager::UninitializeNightSection()
{
    if (!NightActive)
        return;
    CSectionSet sections;
    BuildLoadSections(&sections);
    for (CSectionNode* node = sections.Head->Left; node != sections.Head; node = CSectionSet::Increment(node)) {
        CCharString section(node->Value);
        bool nightOnly = GetDayOrNightOnlyStringFromFullSectionName(section) == g_NightOnlySuffix;
        if (nightOnly)
            World->GetThingManager()->DeleteThingsWithSectionName(section);
    }
    NightActive = false;
}