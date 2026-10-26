//   random_access_iterator_tag parameter UNNAMED lets the empty tag temporary share the live `reason` argument slot as in retail
//   (a template body is not needed). Kept from the parked source: the same-TU auto_inline(off) stand-in for ActiveQuests.remove.
// CQuestManager::DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason) -- stop an active quest: drop its quest-info elements and persistence, shut its script down (as the running quest), take it off the between-screens and active lists, retire its quest card; unless it is still active or reason bit 0 is set, delete its section's things (plus the day/night variants) and rebuild the nav maps for the remaining quests when it had loaded resources; finally unload its code section
// Leaf: DeactivateQuest
// Retail 0x004B3A20, 704 bytes, `ret 0xc` (Ego_r.pdb ?DeactivateQuest@CQuestManager@@AAEXV?$CCountedPointer@VCActiveQuest@CQuestManager@@@@K@Z).
// Callees: CCharString copy 0x0099EC30 / dtor 0x0099EAE0, NPlayerGui::CDrawQuestInfo::RemoveElementsFromScript(long)
// 0x00647403 (g_pPlayerGui 0x013B8790 +0x1C8), CQuestManager::ClearQuestPersistScript 0x004B39B0, the script's shutdown
// 0x00CB7F60 (name unconfirmed), STLport __find(first, last, val, random_access_iterator_tag) 0x004B8E40 (`__fastcall`),
// vector<CCharString>::erase 0x004B8770, list<CCountedPointer<CActiveQuest>>::remove 0x004BA390 (manifest label is a
// propagated remove_if), operator delete 0x00BFE9BC, RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString&,
// bool) 0x004B0B90, IsQuestActive 0x004AF610, CWorld::GetThingManager (slot 3) / GetWorldMap (slot 12),
// CThingManager::DeleteThingsWithSectionName 0x0051FC20, operator+ 0x0099F600 / 0x0099F570 (`__fastcall`), "%"
// 0x01239434, g_DayOnlySuffix 0x013BAC18 / g_NightOnlySuffix 0x013BAC1C, set<CCharString> ctor 0x004AD460 / insert
// 0x004AC380 / dtor 0x004AD4A0, CWorldMap::UpdateNavMaps 0x00501990, the static counted manager pointer 0x013CAA68
// (operator new 0x00BFEA1A, ctor 0x009F0320, Reset 0x00413590), its Contains(CCharString) 0x009F0570 / section handle
// lookup (CCharString) 0x009F0500 (name unconfirmed) / UnloadSection(unsigned long) 0x009F02F0.
// Layout: retail STLport CQuestManager (ActiveQuests +0x38, BetweenStartAndEndScreenQuests +0x4c, World +0x7c,
// PQuestCurrentlyRunning +0x88, DayActive +0x8c, NightActive +0x8d, NumberOfQuestsBetweenStartAndFinishScreens +0x98);
// CActiveQuest (landed ctor 0x004B0310): ID +0, PScript +8, LoadedResources +0x25, QuestName +0x30.
#include <new>
void __cdecl operator delete(void* p);              // 0x00BFE9BC

struct random_access_iterator_tag { random_access_iterator_tag() {} };
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
extern CCharString g_DayOnlySuffix;                 // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;               // 0x013BAC1C "NightOnly"
__declspec(noinline) CCharString* __fastcall FindString(CCharString* first, CCharString* last, const CCharString& val,
                                   const random_access_iterator_tag&)   // 0x004B8E40
{
    for (; first != last; ++first)
        if (first->PStringData == val.PStringData)
            return first;
    return last;
}
inline CCharString* find(CCharString* first, CCharString* last, const CCharString& val)
{
    return FindString(first, last, val, random_access_iterator_tag());
}
struct CStringVector {                              // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
    CCharString* erase(CCharString* position);      // 0x004B8770
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* Info;
    T* operator->() const { return Data; }
    void Release()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    void Reset()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    ~CCountedPointer() { Release(); }
};
class CScriptBase { public: void Shutdown(); };            // 0x00CB7F60
struct CActiveQuest {
    long ID;                                        // +0x00
    void* PScriptInfo;                              // +0x04
    CScriptBase* PScript;                           // +0x08 (CCountedPointer Data)
    void* PScriptInfoBlock;                         // +0x0c
    unsigned char _pad10[0x24 - 0x10];
    bool Persistent;                                // +0x24
    bool LoadedResources;                           // +0x25
    unsigned char _pad26[0x30 - 0x26];
    CCharString QuestName;                          // +0x30
};
struct CQuestNode { CQuestNode* next; CQuestNode* prev; CCountedPointer<CActiveQuest> value; };
struct CActiveQuestList {
    CQuestNode* m_node;
    void remove(const CCountedPointer<CActiveQuest>& value);   // 0x004BA390
};
struct CSectionInsertResult { void* Position; bool Inserted; };
struct CSectionSet {                                // set<CCharString>, out-of-line ctor / dtor
    void* Head;
    unsigned long Count;
    unsigned long Compare;
    CSectionSet();                                  // 0x004AD460
    ~CSectionSet();                                 // 0x004AD4A0
    CSectionInsertResult insert(const CCharString& value);   // 0x004AC380
};
class CThingManager { public: void DeleteThingsWithSectionName(const CCharString& section); };   // 0x0051FC20
class CWorldMap { public: void UpdateNavMaps(const CSectionSet& sections, bool a2); };          // 0x00501990
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();       // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();               // slot 12 (+0x30)
};
struct CDrawQuestInfo { void RemoveElementsFromScript(long scriptID); };   // 0x00647403
class CPlayerGui { public:
    unsigned char _pad[0x1c8];
    CDrawQuestInfo* PDrawQuestInfo;                 // +0x1c8
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
class CCodeSectionManager { public:
    unsigned char _body[0x2c];
    CCodeSectionManager();                          // 0x009F0320
    bool Contains(CCharString name);                // 0x009F0570
    unsigned long GetSectionHandle(CCharString name);   // 0x009F0500
    void UnloadSection(unsigned long handle);       // 0x009F02F0
};
struct CCodeSectionManagerPointer {                 // static CCountedPointer<CCodeSectionManager>
    CCodeSectionManager* m_pObject;
    void* m_pCount;
    void Reset(CCodeSectionManager* object);        // 0x00413590
};
extern CCodeSectionManagerPointer g_pCodeSectionManager;   // 0x013CAA68
__forceinline CCodeSectionManager* GetCodeSectionManager()
{
    if (!g_pCodeSectionManager.m_pObject)
        g_pCodeSectionManager.Reset(new(operator new(0x2c)) CCodeSectionManager());
    return g_pCodeSectionManager.m_pObject;
}

class CQuestManager { public:
    unsigned char    _pad00[0x38];
    CActiveQuestList ActiveQuests;                  // +0x38
    unsigned char    _pad3c[0x4c - 0x3c];
    CStringVector    BetweenStartAndEndScreenQuests;   // +0x4c
    unsigned char    _pad58[0x7c - 0x58];
    CWorld*          World;                         // +0x7c
    unsigned char    _pad80[0x88 - 0x80];
    CActiveQuest*    PQuestCurrentlyRunning;        // +0x88
    bool             DayActive;                     // +0x8c
    bool             NightActive;                   // +0x8d
    unsigned char    _pad8e[0x98 - 0x8e];
    long             NumberOfQuestsBetweenStartAndFinishScreens;   // +0x98

    void ClearQuestPersistScript(const CCharString& questName);                          // 0x004B39B0
    void RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString& name, bool b);    // 0x004B0B90
    void DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason);
public:
    bool IsQuestActive(const CCharString&) const;   // 0x004AF610
};

// Same-TU stand-in for list<CCountedPointer<CActiveQuest>>::remove 0x004BA390 (an instantiation in this TU): only its
// visibility matters -- VC7.1 then knows the parameter's address does not escape, so after the parameter is reset the
// zero stores die and the parameter's end-of-function destructor folds away, as in retail. Not graded.
extern "C" void __cdecl free(void* p);              // 0x00BFEA14
#pragma auto_inline(off)
void CActiveQuestList::remove(const CCountedPointer<CActiveQuest>& value)
{
    CQuestNode* first = m_node->next;
    while (first != m_node) {
        CQuestNode* next = first->next;
        if (first->value.Data == value.Data) {
            first->prev->next = next;
            next->prev = first->prev;
            first->value.Release();
            free(first);
        }
        first = next;
    }
}
#pragma auto_inline(on)

void CQuestManager::DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason)
{
    CActiveQuest* activeQuest = quest.Data;
    CCharString questName = activeQuest->QuestName;
    bool loadedResources = activeQuest->LoadedResources;
    g_pPlayerGui->PDrawQuestInfo->RemoveElementsFromScript(activeQuest->ID);
    ClearQuestPersistScript(questName);
    if (activeQuest->PScript) {
        CActiveQuest* previous = PQuestCurrentlyRunning;
        PQuestCurrentlyRunning = activeQuest;
        activeQuest->PScript->Shutdown();
        PQuestCurrentlyRunning = previous;
    }
    CCharString sectionName = activeQuest->QuestName;
    CCharString* last = BetweenStartAndEndScreenQuests._M_finish;
    CStringVector* quests = &BetweenStartAndEndScreenQuests;
    CCharString* it = FindString(quests->_M_start, last, questName, random_access_iterator_tag());
    if (it != last) {
        if (NumberOfQuestsBetweenStartAndFinishScreens > 0)
            --NumberOfQuestsBetweenStartAndFinishScreens;
        quests->erase(it);
    }
    ActiveQuests.remove(quest);
    quest.Reset();
    bool makeAvailable = true;
    if (reason & 2)
        makeAvailable = false;
    RemoveActiveQuestCardAndMakeAvailableAgain(questName, makeAvailable);
    if (!IsQuestActive(questName) && !(reason & 1)) {
        World->GetThingManager()->DeleteThingsWithSectionName(sectionName);
        if (DayActive)
            World->GetThingManager()->DeleteThingsWithSectionName(sectionName + "%" + g_DayOnlySuffix);
        if (NightActive)
            World->GetThingManager()->DeleteThingsWithSectionName(sectionName + "%" + g_NightOnlySuffix);
        if (loadedResources) {
            CSectionSet sections;
            for (CQuestNode* node = ActiveQuests.m_node->next; node != ActiveQuests.m_node; node = node->next)
                sections.insert(node->value->QuestName);
            World->GetWorldMap()->UpdateNavMaps(sections, false);
        }
    }
    CCodeSectionManager* manager = GetCodeSectionManager();
    if (manager->Contains(questName)) {
        CCharString name = questName;
        manager->UnloadSection(manager->GetSectionHandle(name));
    }
}