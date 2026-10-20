// CQuestManager::AttachExistingScriptedEntitiesToScript(CScriptBase&) -- collect every thing that has a script name and hand each one the script binds to it as an activated entity (UID, CScriptThing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B26B0, length 232 bytes, `ret 4`
// (PDB ?AttachExistingScriptedEntitiesToScript@CQuestManager@@QAEXAAVCScriptBase@@@Z).
// Called when a quest script starts, so that entities already in the world get the same OnActivateEntity treatment
// (landed 0x004AFA60) they would have had if the script had been running when they appeared.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport list<CThing*> sentinel and nodes),
// CThingSearchTools::GetAllThings<CThing, list<CThing*>, CThingFilter_HasAnyScriptName> 0x004BAA50 (filter
// operator() 0x006A74F0; the manifest names a vector/HasTCOfLeafType twin), the script's name-binding test 0x00CB7D60
// (PDB IsEntityScriptBound), CThing::GetScriptThing 0x004C7E50 (hidden-pointer result), the script's entity
// activation 0x00CB8960 (PDB OnScriptedEntityActivated), operator delete 0x00BFE9BC (the counted block), CBaseClass::
// ~CBaseClass 0x0099A2E0. The callee names follow the landed OnActivateEntity.
// Layout: retail World +0x7c (PDB +0xd8), CWorld::PThingSearchTools +0x20; CThing 64-bit UID +0x18, ScriptName +0x74.
// Shape: the list is a local STLport list (sentinel malloc'd in the inline ctor, cleared node by node and freed in the
// inline dtor); the filter is an empty temporary passed by const reference (its byte sits in the frame word below the
// list); CScriptThing is a named local of the hit block whose inline dtor stores the vtable, releases the counted PImp
// and calls the out-of-line ~CBaseClass.
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E (MSVCR71)
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct CCharString { void* PStringData; };

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {                                      // polymorphic root of the script handles
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}
};

struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                                // +0x18
    unsigned char    _pad20[0x74 - 0x20];
    CCharString      ScriptName;                         // +0x74
    unsigned char    _pad78[0x91 - 0x78];
    unsigned char    Dead : 1;                           // +0x91 bit 0
    unsigned char    CreatedByOwnerThingCreation : 1;
    unsigned char    IncludeInThingScans : 1;            // bit 2
    unsigned char    Activated : 1;                      // bit 3
    unsigned char    GloballyLoaded : 1;
    unsigned char    InLimbo : 1;                        // bit 5
    unsigned char    KillLocked : 1;
    unsigned char    CreatedBySerialisation : 1;
    CScriptThing GetScriptThing();                       // 0x004C7E50
};

struct SThingNode {                                      // STLport _List_node<CThing*>
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CThing*     _M_data;
};
struct CThingList {                                      // STLport list<CThing*>: one sentinel pointer
    SThingNode* _M_node;
    CThingList()
    {
        SThingNode* head = (SThingNode*)malloc(sizeof(SThingNode));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
    ~CThingList()
    {
        SThingNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
    void push_back(CThing* thing)                        // STLport insert(end(), thing)
    {
        SThingNode* pos = _M_node;
        SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
        node->_M_data = thing;
        node->_M_next = pos;
        node->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = node;
        pos->_M_prev = node;
    }
};

struct CThingFilter_HasAnyScriptName {                   // empty predicate
    CThingFilter_HasAnyScriptName() {}
    bool operator()(const CThing* thing) const;          // 0x006A74F0
};
struct CThingSearchTools {
    unsigned char _pad00[0x0c];
    CThingList& GetThingList_0049c770();                 // 0x0049C770 (`return &[this+8]->+0x18`)
    void GetAllThings(const CThingFilter_HasAnyScriptName& filter, CThingList& things);   // 0x004BAA50
};

// The GetAllThings instantiation lives in this translation unit (0x004BAA50, compiled before the caller): with its
// body in view VC7.1 knows the walk only appends nodes, so the caller keeps the list sentinel in a register across it.
#pragma auto_inline(off)
void CThingSearchTools::GetAllThings(const CThingFilter_HasAnyScriptName& filter, CThingList& things)
{
    CThingList& all = GetThingList_0049c770();
    for (SThingNode* node = all._M_node->_M_next; node != all._M_node; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (thing->IncludeInThingScans && thing->Activated && !thing->Dead && !thing->InLimbo && filter(thing))
            things.push_back(thing);
    }
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;                // +0x20 scoped_ptr<CThingSearchTools>
};
struct CScriptBase {
    bool BindsEntityScript(const CCharString& name);                              // 0x00CB7D60
    void ActivateEntity(unsigned __int64 uid, const CScriptThing& thing);         // 0x00CB8960
};

struct CQuestManager {
    unsigned char _pad00[0x7c];
    CWorld*       World;                                 // +0x7c (PDB +0xd8)
    void AttachExistingScriptedEntitiesToScript(CScriptBase& script);
};

void CQuestManager::AttachExistingScriptedEntitiesToScript(CScriptBase& script)
{
    CThingList things;
    World->PThingSearchTools->GetAllThings(CThingFilter_HasAnyScriptName(), things);
    for (SThingNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (script.BindsEntityScript(thing->ScriptName)) {
            CScriptThing scriptThing = thing->GetScriptThing();
            script.ActivateEntity(thing->UID, scriptThing);
        }
    }
}