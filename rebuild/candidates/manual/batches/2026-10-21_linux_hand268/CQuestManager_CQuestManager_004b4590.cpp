// CQuestManager::CQuestManager(CWorld* world, CScriptInfoManager* scriptInfoManager, CGameScriptInterface* gameInterface) -- construct the quest manager: empty persistence maps, quest/card/boast/objective lists and the loaded-resources set, then create its CGameScriptThingManager
// Leaf: ??0CQuestManager
// Retail 0x004B4590, 477 bytes, __thiscall ret 0xC (no EH frame). Callees: malloc 0x00BFEA0E (ten list sentinels and the
// set header), operator new 0x00BFEA1A, CBaseIntelligentPointer() 0x00A01B10, CCharString(const char*, long = -1)
// 0x0099EBF0 on "" 0x0122D70E, CGameScriptThingManager() 0x00704A80, CCountedPointer::Reset(T*) 0x004BB9D0 (manifest
// label CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset, propagated). Vtables 0x01239464 (CQuestManager),
// 0x01238C6C (CIntelligentPointer<CThing>).
// Layout: engine/CQuestManager.h is the Ego_r.pdb (Dinkumware) layout: 16-byte vectors and 12-byte lists put World at
// +0xD8. Retail STLport lists are one sentinel pointer and vectors 12 bytes, so every member from RegisteredQuestNames
// on sits lower (ActiveQuests +0x38, Objectives +0x74, World +0x7C, DayActive +0x8C, set +0x9C, name +0xB0); the
// list sentinel sizes (0x10 / 0x18 / 0x28 / 0x14) are each element type's size + 8. Local model in PDB order/names.
// Shape: init-list members in declaration order; the STLport list base ctor (node member zeroed, sentinel
// allocated and self-linked in a register, then stored), the set ctor of the landed CSectionSet model, the
// intelligent pointer's inline derived ctor storing its vptr after the out-of-line base ctor; the body resets the
// counted pointer to `new CGameScriptThingManager` (null-checked new, so the Reset call and epilogue are duplicated).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CListNodeBase { CListNodeBase* _M_next; CListNodeBase* _M_prev; };
template <int NodeSize> struct CSTLList {             // STLport list<T>: one node pointer (sentinel)
    CListNodeBase* _M_node;
    CSTLList() : _M_node(0)
    {
        CListNodeBase* n = (CListNodeBase*)malloc(NodeSize);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
};
struct CSTLVector {                                  // STLport vector<T> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CSTLVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CVectorMap {                                  // CVectorMap<K,V,Cmp>: vector + Compare + Dirty
    CSTLVector Items;
    unsigned char Compare;
    bool Dirty;
    CVectorMap() : Dirty(false) {}
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
    CStringSet()
    {
        Head = 0;
        Head = (CTreeNode*)malloc(20);
        Count = 0;
        Head->Color = 0;
        Head->Parent = 0;
        Head->Left = Head;
        Head->Right = Head;
    }
};
struct CGameScriptThingManager {
    unsigned char _pad[0x10];
    CGameScriptThingManager();                       // 0x00704A80
};
template <class T> struct CCountedPointer {
    T* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* p);                                // 0x004BB9D0
};
struct CBaseIntelligentPointer {
    virtual void v000();
    void* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                       // 0x00A01B10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() {}
    virtual void v000();
};
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
};
struct CWorld;
struct CScriptInfoManager;
struct CGameScriptInterface;
struct CActiveQuest;

struct CQuestManager {
    CCountedPointer<CGameScriptThingManager> PGameScriptThingManager;   // +0x04
    CRuntimeQuestPersistence RuntimeQuestPersistence;                   // +0x0c
    CSTLVector               RegisteredQuestNames;                      // +0x2c
    CSTLList<0x10>           ActiveQuests;                              // +0x38
    CSTLList<0x10>           PendingQuestsToDeactivate;                 // +0x3c
    CSTLVector               CreatureGenerationDisabledQuests;          // +0x40
    CSTLVector               BetweenStartAndEndScreenQuests;            // +0x4c
    CSTLList<0x18>           AvailableQuestCardTypes;                   // +0x58
    CSTLList<0x10>           ActiveQuestCards;                          // +0x5c
    CSTLList<0x10>           CompletedQuestCards;                       // +0x60
    unsigned long            FrameBoastsLastUpdated;                    // +0x64
    CSTLList<0x28>           Boasts;                                    // +0x68
    CSTLList<0x28>           AcceptedBoasts;                            // +0x6c
    CSTLList<0x28>           TempBoasts;                                // +0x70
    CSTLList<0x14>           Objectives;                                // +0x74
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

    CQuestManager(CWorld* world, CScriptInfoManager* scriptInfoManager, CGameScriptInterface* gameInterface);
    virtual ~CQuestManager();
};

CQuestManager::CQuestManager(CWorld* world, CScriptInfoManager* scriptInfoManager, CGameScriptInterface* gameInterface)
    : FrameBoastsLastUpdated(0),
      ScriptInfoManager(scriptInfoManager),
      World(world),
      GameInterface(gameInterface),
      CurrentQuestID(1),
      PQuestCurrentlyRunning(0),
      DayActive(false),
      NightActive(false),
      WorldDestructed(false),
      QuestCardsNeedUpdating(false),
      HeroMapNumber(-1),
      LastAutoSaveFrame(0),
      NumberOfQuestsBetweenStartAndFinishScreens(0),
      PMostRecentValidUsedTargetName("")
{
    PGameScriptThingManager.Reset(new CGameScriptThingManager);
}
