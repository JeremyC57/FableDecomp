// CQuestManager::ActivateQuests(const CArray<CActivateQuestInfo>& quests) -- first pass: for every quest neither active nor already resource-loaded, queue its section for resource loading (and remember it as loaded-but-not-activated) when asked, and load its code section; load the queued resources; second pass: for every quest not yet active, create its CActiveQuest (with its script, master data and persisted state when it has a game script and scripts are enabled; else a bare record), append it to the active list, drop it from the loaded-not-activated set, activate the script, restore its persist script and post the quest-activated message
// Grade: DIFFER(1266v1285) -- shape close; callee-saved register roles swapped (retail this=ebp, quests=ebx), the
//   persist-lookup cached `last` kept in edx with a redundant compare in retail, zero-register use and the counted-
//   pointer branch layout differ. ~10 variants tried.
// Leaf: ActivateQuests
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B3CE0, 1283 bytes (int3 padded to 1296), `ret 4`.
// Callees: IsQuestActive 0x004AF610, set<CCharString> find 0x004B9370 / insert 0x004AC380 / erase(key) 0x004BB270 /
//   _Ubound 0x004B9D50 / _Lbound 0x004B9D00 / erase(first, last) 0x004B9C10, vector<CCharString> _M_insert_overflow 0x00433530,
//   CCharString copy 0x0099EC30 / ~ 0x0099EAE0, code section manager (static counted pointer 0x013CAA68, operator new
//   0x00BFEA1A, ctor 0x009F0320, Reset 0x00413590, Contains 0x009F0570, GetSectionHandle 0x009F0500, LoadSection 0x009F02C0),
//   LoadResourcesForQuests 0x004B2510, persistence LowerBound 0x004B97D0 + CBasicString<char>::operator< 0x00429950,
//   LoadActiveQuest 0x004B3760, GetMasterScriptDataForScript 0x004AFA10, CActiveQuest ctor 0x004B0310 (and inline),
//   count-block deleter 0x004BAEF0, malloc 0x00BFEA0E (list node), CScriptBase::Activate 0x00CB7900 / LoadGameState
//   0x00CB8690, GetIndexFromRegisteredQuest 0x004AF740, CMessageEventManager::AddMessage 0x00687540, operator delete
//   0x00BFE9BC, free 0x00BFEA14; g_bScriptsEnabled 0x01375454.
// Layout: retail STLport CQuestManager (RuntimeQuestPersistence +0xc, ActiveQuests +0x38, World +0x7c, CurrentQuestID +0x84,
//   QuestsWithResourcesLoadedThatAreNotActivated +0x9c); CActiveQuestInfo 12 bytes {QuestName, PScriptInfo, GameScript,
//   LoadResources}; CScriptInfo {AllocFunc +0, MasterScriptDataAlloc +4, MasterScript +0x10}; CWorld message manager +0x60.
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
void* __cdecl operator_new(unsigned int size);      // 0x00BFEA1A
void __cdecl operator delete(void* p);              // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }
struct CCharStringData {
    bool operator<(const CCharStringData& other) const;   // 0x00429950
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};
struct __true_type { __true_type() { } };
struct CStringVector {                              // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CStringVector()
    {
        for (CCharString* p = _M_start; p != _M_finish; ++p)
            p->~CCharString();
        if (_M_start)
            free(_M_start);
    }
    void _M_insert_overflow(CCharString* position, const CCharString& value, const __true_type&, unsigned int count, bool atEnd);   // 0x00433530
    void push_back(const CCharString& value)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) new (_M_finish) CCharString(value);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
    bool empty() const { return _M_start == _M_finish; }
};
struct CSetNode;
struct CSetIterator {
    CSetNode* Node;
    CSetIterator(CSetNode* node) : Node(node) {}
};
inline bool operator==(const CSetIterator& a, const CSetIterator& b) { return a.Node == b.Node; }
struct CSectionInsertResult { CSetNode* Position; bool Inserted; };
struct CStringSet {                                 // set<CCharString>
    CSetNode* Head;
    unsigned long Count;
    unsigned long Compare;
    CSetIterator end() { return CSetIterator(Head); }
    CSetNode* _M_find(const CCharString& key);      // 0x004B9370
    CSetIterator find(const CCharString& key) { return CSetIterator(_M_find(key)); }
    CSectionInsertResult insert(const CCharString& value);   // 0x004AC380
    unsigned int erase(const CCharString& key);     // 0x004BB270
    CSetNode* _Ubound(const CCharString& key);      // 0x004B9D50
    CSetNode* _Lbound(const CCharString& key);      // 0x004B9D00
    void erase(CSetIterator first, CSetIterator last);   // 0x004B9C10
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CScriptDataBase {
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2 (+0x08)
};
struct CScriptBase {
    void Activate();                                // 0x00CB7900
    void LoadGameState(const CCharString& state);   // 0x00CB8690
};
struct CScriptInfo {
    CScriptBase* (__fastcall* AllocFunc)(CScriptDataBase* data, void* gameInterface);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                               // +0x04
    CCharString Name;                               // +0x08
    long ID;                                        // +0x0c
    bool MasterScript;                              // +0x10
};
struct CActivateQuestInfo {                         // 12 bytes
    CCharString  QuestName;                         // +0x00
    CScriptInfo* PScriptInfo;                       // +0x04
    bool         GameScript;                        // +0x08
    bool         LoadResources;                     // +0x09
};
struct CActivateQuestInfoArray {                    // CArray<CActivateQuestInfo> (STLport vector)
    CActivateQuestInfo* _M_start;
    CActivateQuestInfo* _M_finish;
    CActivateQuestInfo* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const CActivateQuestInfo& operator[](unsigned int i) const { return _M_start[i]; }
};
template <class T> struct CCountedPointer {
    T* Data;
    SCountBlock* Info;
    static void __fastcall Destroy(void* object);   // 0x004BAEF0
    CCountedPointer(T* object) : Data(object)
    {
        if (object) {
            Info = new(operator_new(sizeof(SCountBlock))) SCountBlock;
            Info->m_references = 1;
            Info->m_pfnDestroy = Destroy;
            Info->m_pObject = object;
        } else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
};
struct C2DCoordI { long X, Y; };
struct CRegion;
template <class T> struct CRetailVector {
    T* m_start; T* m_finish; T* m_endOfStorage;
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};
struct CPlainCountedPointer {
    void* Data; SCountBlock* Info;
    CPlainCountedPointer(void* object);
};
struct CActiveQuest {                               // 0x34 bytes
    long                          ID;               // +0x00
    CScriptInfo*                  PScriptInfo;      // +0x04
    CScriptBase*                  PScript;          // +0x08 (CCountedPointer)
    SCountBlock*                  PScriptBlock;     // +0x0c
    CRetailVector<const CRegion*> Regions;          // +0x10
    C2DCoordI                     MarkerPosition;   // +0x1c
    bool                          Persistent;       // +0x24
    bool                          LoadedResources;  // +0x25
    CScriptDataBase*              PScriptData;      // +0x28 (CCountedPointer)
    SCountBlock*                  PScriptDataBlock; // +0x2c
    CCharString                   QuestName;        // +0x30
    CActiveQuest(long id, const CCharString& questName, CScriptInfo* scriptInfo, CScriptBase* script,
                 CScriptDataBase* scriptData, bool loadedResources);   // 0x004B0310
    CActiveQuest(long id, const CCharString& questName, bool loadedResources)
        : ID(id), PScriptInfo(0), PScript(0), PScriptBlock(0), Persistent(true), LoadedResources(loadedResources),
          PScriptData(0), PScriptDataBlock(0), QuestName(questName) {}
};
struct CQuestNode {
    CQuestNode* _M_next;
    CQuestNode* _M_prev;
    CCountedPointer<CActiveQuest> _M_data;
};
struct CActiveQuestList {                           // STLport list<CCountedPointer<CActiveQuest>>
    CQuestNode* _M_node;
    void push_back(const CCountedPointer<CActiveQuest>& value)
    {
        CQuestNode* position = _M_node;
        CQuestNode* node = (CQuestNode*)malloc(sizeof(CQuestNode));
        new (&node->_M_data) CCountedPointer<CActiveQuest>(value);
        node->_M_next = position;
        node->_M_prev = position->_M_prev;
        position->_M_prev->_M_next = node;
        position->_M_prev = node;
    }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CThing;
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector& position, CThing* thing1, CThing* thing2,
                              float a, float b, long data);   // 0x00687540
};
struct CWorld {
    unsigned char _pad00[0x38];
    void* GameInterface;                            // +0x38
    unsigned char _pad3c[0x60 - 0x3c];
    CMessageEventManager* PMessageEventManager;     // +0x60
};
struct CCodeSectionManager {
    unsigned char _body[0x2c];
    CCodeSectionManager();                          // 0x009F0320
    bool Contains(CCharString name);                // 0x009F0570
    unsigned long GetSectionHandle(CCharString name);   // 0x009F0500
    void LoadSection(unsigned long handle);         // 0x009F02C0
};
struct CCodeSectionManagerPointer {
    CCodeSectionManager* m_pObject;
    void* m_pCount;
    void Reset(CCodeSectionManager* object);        // 0x00413590
};
extern CCodeSectionManagerPointer g_pCodeSectionManager;   // 0x013CAA68
__forceinline CCodeSectionManager* GetCodeSectionManager()
{
    if (!g_pCodeSectionManager.m_pObject)
        g_pCodeSectionManager.Reset(new(operator_new(0x2c)) CCodeSectionManager());
    return g_pCodeSectionManager.m_pObject;
}
extern bool g_bScriptsEnabled;                      // 0x01375454
namespace NQuestManager {
struct CRuntimeQuestPersistence {
    struct CPersistInfo { CCharString QuestName; CCharString PersistScript; };
    struct SPair { CCharString first; CPersistInfo second; };
    struct CPersistMap {
        SPair* m_pFirst;
        SPair* m_pLast;
        SPair* m_pEnd;
        long   m_sorted;
        SPair* LowerBound_004b97d0(const CCharString& key);   // 0x004B97D0
        SPair* Find(const CCharString& key)
        {
            SPair* it = LowerBound_004b97d0(key);
            SPair* last = m_pLast;
            return (it == last || key < it->first) ? last : it;
        }
    };
    CPersistMap PersistScripts;                     // +0x00
    CPersistMap MasterDataPersistScripts;           // +0x10
    const CCharString* GetQuestScriptPersistScript(const CCharString& quest)
    {
        SPair* it = PersistScripts.LowerBound_004b97d0(quest);
        SPair* last = PersistScripts.m_pLast;
        SPair* found = (it == last || quest < it->first) ? last : it;
        return found != last ? &found->second.PersistScript : 0;
    }
};
}
struct CQuestManager {
    unsigned char    _pad00[0x0c];
    NQuestManager::CRuntimeQuestPersistence RuntimeQuestPersistence;   // +0x0c
    unsigned char    _pad2c[0x38 - 0x2c];
    CActiveQuestList ActiveQuests;                  // +0x38
    unsigned char    _pad3c[0x7c - 0x3c];
    CWorld*          World;                         // +0x7c
    unsigned char    _pad80[0x84 - 0x80];
    long             CurrentQuestID;                // +0x84
    unsigned char    _pad88[0x9c - 0x88];
    CStringSet       QuestsWithResourcesLoadedThatAreNotActivated;   // +0x9c
    bool IsQuestActive(const CCharString& name) const;                            // 0x004AF610
    void LoadResourcesForQuests(const CStringVector& questSections);             // 0x004B2510
    void LoadActiveQuest(CScriptDataBase* data, const CActivateQuestInfo& info);  // 0x004B3760
    CScriptDataBase* GetMasterScriptDataForScript(CScriptInfo* info);            // 0x004AFA10
    long GetIndexFromRegisteredQuest(const CCharString& name);                   // 0x004AF740
    void ActivateQuests(const CActivateQuestInfoArray& quests);
};

void CQuestManager::ActivateQuests(const CActivateQuestInfoArray& quests)
{
    CStringVector questsToLoad;
    for (unsigned int i = 0; i < quests.size(); ++i) {
        const CActivateQuestInfo& info = quests[i];
        if (!IsQuestActive(info.QuestName) && QuestsWithResourcesLoadedThatAreNotActivated.find(info.QuestName) == QuestsWithResourcesLoadedThatAreNotActivated.end()) {
            if (info.LoadResources) {
                questsToLoad.push_back(info.QuestName);
                QuestsWithResourcesLoadedThatAreNotActivated.insert(info.QuestName);
            }
            CCodeSectionManager* manager = GetCodeSectionManager();
            if (manager->Contains(info.QuestName)) {
                CCharString name = info.QuestName;
                manager->LoadSection(manager->GetSectionHandle(name));
            }
        }
    }
    if (!questsToLoad.empty())
        LoadResourcesForQuests(questsToLoad);
    for (unsigned int j = 0; j < quests.size(); ++j) {
        const CActivateQuestInfo& info = quests[j];
        if (IsQuestActive(info.QuestName))
            continue;
        const CCharString* persistScript = RuntimeQuestPersistence.GetQuestScriptPersistScript(info.QuestName);
        if (info.PScriptInfo && g_bScriptsEnabled) {
            long id = CurrentQuestID++;
            CScriptDataBase* scriptData = 0;
            CScriptDataBase* masterData;
            if (info.PScriptInfo->MasterScript) {
                scriptData = info.PScriptInfo->MasterScriptDataAlloc();
                scriptData->Init();
                LoadActiveQuest(scriptData, info);
                masterData = scriptData;
            } else
                masterData = GetMasterScriptDataForScript(info.PScriptInfo);
            CScriptBase* script = info.PScriptInfo->AllocFunc(masterData, World->GameInterface);
            CCountedPointer<CActiveQuest> quest(new(operator_new(sizeof(CActiveQuest))) CActiveQuest(id, info.QuestName, info.PScriptInfo, script, scriptData, info.LoadResources));
            ActiveQuests.push_back(quest);
            QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
            script->Activate();
            if (persistScript && quest.Data->PScript)
                quest.Data->PScript->LoadGameState(*persistScript);
            World->PMessageEventManager->AddMessage(0x37, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, GetIndexFromRegisteredQuest(info.QuestName));
        } else {
            long id = CurrentQuestID++;
            CCountedPointer<CActiveQuest> quest(new(operator_new(sizeof(CActiveQuest))) CActiveQuest(id, info.QuestName, info.LoadResources));
            ActiveQuests.push_back(quest);
            CStringSet& loaded = QuestsWithResourcesLoadedThatAreNotActivated;
            loaded.erase(CSetIterator(loaded._Lbound(info.QuestName)), CSetIterator(loaded._Ubound(info.QuestName)));
            if (persistScript && quest.Data->PScript)
                quest.Data->PScript->LoadGameState(*persistScript);
        }
    }
}
