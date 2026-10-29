// CQuestManager::~CQuestManager() -- unless the world was already destructed, deactivate every quest, clear the boast lists and stamp the boast frame; then destroy the members in reverse order
// Leaf: ??1CQuestManager
// Retail 0x004B4770, 624 bytes, __fastcall (no EH frame; `push ecx` frame word holds the __false_type tag byte).
// Callees: CQuestManager::DeactivateAllQuests 0x004B4450, CWorld::GetWorldFrame 0x0049D870 (manifest
// "GFGetPreMainMemoryUsed" is propagated; landed CThingSwitch::WorldUpdate names it GFGetCurrentFrame),
// CCharString::~CCharString 0x0099EAE0, CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10,
// set<CCharString>::_M_erase 0x004ABA70, the per-element-type list clear() instantiations 0x004B8C50 (Objectives),
// 0x004B8C00 (boast lists; manifest "SortTreeRecursively"/"FrameUpdate" labels are propagated), 0x004B8050 (quest
// cards), 0x004B8BB0 (card types), 0x004B8B70 (pending deactivations), 0x004B9DA0 (active quests), the persist-pair
// __destroy_aux 0x004B93E0 (`__fastcall`), free 0x00BFEA14, operator delete 0x00BFE9BC.
// Vtables 0x01239464 (CQuestManager) and 0x01239370 (CTBaseSingletonNoDefaultAlloc<CQuestManager>).
// Layout: the retail STLport layout of the landed ctor 0x004B4590 (lists 4 bytes, vectors 12, maps 16).
// Shape: the body's guarded block, then implicit member destruction: each list's dtor is its out-of-line clear()
// plus the inline sentinel free, each vector<CCharString> an inline destroy loop plus free, each CVectorMap an
// out-of-line __destroy_aux plus free, the set its inline clear plus header free, and the counted pointer an inline
// release; the intelligent pointer's derived dtor is implicit (no vptr store).
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
void __cdecl operator delete(void* p);                // 0x00BFE9BC

struct __false_type { __false_type() {} };

struct CListNodeBase { CListNodeBase* _M_next; CListNodeBase* _M_prev; };
template <class T> struct CSTLList {                 // STLport list<T>: one node pointer (sentinel)
    CListNodeBase* _M_node;
    void clear();                                    // out of line, one instantiation per element type
    ~CSTLList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
class CCharString { public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
inline void DestroyStrings(CCharString* first, CCharString* last)   // STLport _Destroy(first, last)
{
    for (; first != last; ++first)
        first->~CCharString();
}
struct CStringVector {                               // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    ~CStringVector()
    {
        DestroyStrings(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
struct CPersistPair {                                // pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>
    CCharString first;
    CCharString second[2];
};
void __fastcall DestroyPairs(CPersistPair* first, CPersistPair* last, const __false_type& tag);   // 0x004B93E0
struct CVectorMap {                                  // CVectorMap<CCharString, CPersistInfo>: vector + Compare + Dirty
    CPersistPair* _M_start;
    CPersistPair* _M_finish;
    CPersistPair* _M_end_of_storage;
    unsigned char Compare;
    bool Dirty;
    ~CVectorMap()
    {
        DestroyPairs(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
};
struct CRuntimeQuestPersistence {
    CVectorMap PersistScripts;                       // +0x00
    CVectorMap MasterDataPersistScripts;             // +0x10
};
struct CTreeNode { unsigned char Color; CTreeNode* Parent; CTreeNode* Left; CTreeNode* Right; void* Value; };
struct CStringSet {                                  // STLport set<CCharString> (12 bytes)
    CTreeNode* Head;
    unsigned long Count;
    unsigned long Compare;
    void _M_erase(CTreeNode* x);                     // 0x004ABA70
    ~CStringSet()
    {
        if (Count != 0) {
            _M_erase(Head->Parent);
            Head->Left = Head;
            Head->Parent = 0;
            Head->Right = Head;
            Count = 0;
        }
        if (Head)
            free(Head);
    }
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();              // 0x00A01C10
    void* m_pItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};
template <class T> struct CTBaseSingletonNoDefaultAlloc {
    virtual ~CTBaseSingletonNoDefaultAlloc() {}
};
class CThing;
class CWorld { public: static long __fastcall GetFrame(); };       // 0x0049D870
class CGameScriptThingManager;
class CScriptInfoManager;
class CGameScriptInterface;
struct CActiveQuest;
struct CActiveQuestEntry;
struct CQuestCardType;
struct CQuestCard;
struct CBoast;
struct CObjective;
struct CPendingQuest;

class CQuestManager : public CTBaseSingletonNoDefaultAlloc<CQuestManager> { public:
    CCountedPointer<CGameScriptThingManager> PGameScriptThingManager;   // +0x04
    CRuntimeQuestPersistence RuntimeQuestPersistence;                   // +0x0c
    CStringVector            RegisteredQuestNames;                      // +0x2c
    CSTLList<CActiveQuestEntry> ActiveQuests;                           // +0x38
    CSTLList<CPendingQuest>  PendingQuestsToDeactivate;                 // +0x3c
    CStringVector            CreatureGenerationDisabledQuests;          // +0x40
    CStringVector            BetweenStartAndEndScreenQuests;            // +0x4c
    CSTLList<CQuestCardType> AvailableQuestCardTypes;                   // +0x58
    CSTLList<CQuestCard>     ActiveQuestCards;                          // +0x5c
    CSTLList<CQuestCard>     CompletedQuestCards;                       // +0x60
    unsigned long            FrameBoastsLastUpdated;                    // +0x64
    CSTLList<CBoast>         Boasts;                                    // +0x68
    CSTLList<CBoast>         AcceptedBoasts;                            // +0x6c
    CSTLList<CBoast>         TempBoasts;                                // +0x70
    CSTLList<CObjective>     Objectives;                                // +0x74
    CScriptInfoManager*      ScriptInfoManager;                         // +0x78
    CWorld*                  World;                                     // +0x7c
    CGameScriptInterface*    GameInterface;                             // +0x80
    long                     CurrentQuestID;                            // +0x84
    CActiveQuest*            PQuestCurrentlyRunning;                    // +0x88
    bool                     DayActive;                                 // +0x8c
    bool                     NightActive;                               // +0x8d
    bool                     WorldDestructed;                           // +0x8e
    bool                     QuestCardsNeedUpdating;                    // +0x8f
    long                     HeroMapNumber;                             // +0x90
    long                     LastAutoSaveFrame;                         // +0x94
    long                     NumberOfQuestsBetweenStartAndFinishScreens; // +0x98
    CStringSet               QuestsWithResourcesLoadedThatAreNotActivated; // +0x9c
    CIntelligentPointer<CThing> PMostRecentValidUsedTarget;             // +0xa8
    CCharString              PMostRecentValidUsedTargetName;            // +0xb0

    void DeactivateAllQuests();                      // 0x004B4450
    virtual ~CQuestManager();
};

CQuestManager::~CQuestManager()
{
    if (!WorldDestructed) {
        DeactivateAllQuests();
        Boasts.clear();
        AcceptedBoasts.clear();
        FrameBoastsLastUpdated = CWorld::GetFrame();
    }
}