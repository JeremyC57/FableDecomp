// CQuestManager::LoadResourcesForQuests(const vector<CCharString>& questSections) -- gather every quest section name (plus its "%DayOnly" / "%NightOnly" variants while day / night is active) into a set, then have the world map load the things in those sections and update its nav maps
// Leaf: LoadResourcesForQuests
// Shape: the three set inserts drop their pair<iterator,bool> results; a same-TU noinline stand-in body for the insert
//   0x004AC380 (as the landed UpdateHiddenMaterials 0x00706510) lets VC7.1 pack the three results into one 8-byte frame
//   slot like retail's link-time code generation (declaration only: three slots, 0x34 frame).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
struct CCharString {
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
extern CCharString g_DayOnlySuffix;                  // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;                // 0x013BAC1C "NightOnly"

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
};
struct CCharStringVector {                           // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const CCharString& operator[](unsigned int i) const { return _M_start[i]; }
};
struct CWorldMap {
    void LoadThingsWithSectionNames(const CSectionSet& sections, bool a2);   // 0x00500EB0
    void UpdateNavMaps(const CSectionSet& sections, bool a2);                // 0x00501990
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
    void LoadResourcesForQuests(const CCharStringVector& questSections);
};

CSectionNode* InsertNode_004ac380(CSectionSet* set, void* key, bool* inserted);
#pragma auto_inline(off)
// Same-TU stand-in for set<CCharString>::insert 0x004AC380 (pair returned by hidden pointer), as the landed
// UpdateHiddenMaterials 0x00706510: with a body VC7.1 knows the result pointer does not escape and packs the three
// dropped pairs into one frame slot as retail's link-time code generation did. Only LoadResourcesForQuests is graded.
__declspec(noinline) CSectionInsertResult CSectionSet::Insert(const CCharString& value)
{
    CSectionInsertResult result;
    bool inserted;
    result.Position = InsertNode_004ac380(this, value.PStringData, &inserted);
    result.Inserted = inserted;
    return result;
}
#pragma auto_inline(on)
void CQuestManager::LoadResourcesForQuests(const CCharStringVector& questSections)
{
    CSectionSet sections;
    for (unsigned int i = 0; i < questSections.size(); ++i) {
        const CCharString& section = questSections[i];
        sections.Insert(section);
        if (DayActive) {
            CCharString dayName = section + "%" + g_DayOnlySuffix;
            sections.Insert(dayName);
        }
        if (NightActive) {
            CCharString nightName = section + "%" + g_NightOnlySuffix;
            sections.Insert(nightName);
        }
    }
    World->GetWorldMap()->LoadThingsWithSectionNames(sections, false);
    World->GetWorldMap()->UpdateNavMaps(sections, false);
}