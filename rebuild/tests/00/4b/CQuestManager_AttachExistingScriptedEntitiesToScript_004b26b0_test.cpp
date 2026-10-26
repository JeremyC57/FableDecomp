// Behaviour test for CQuestManager::AttachExistingScriptedEntitiesToScript (0x004B26B0) and the GetAllThings walk it
// is compiled with: of the scannable (included, activated, alive, not in limbo) things that have a script name, those
// the script binds are activated with (UID, script thing); each script-thing handle is released once; the local list
// frees every node it allocated. The block delete is a spy that records only while armed and never frees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_mallocs, g_frees, g_blockDeletes, g_deleteArmed, g_pointeeDeletes;
static void* TestMalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void TestFree(void* p) { ++g_frees; std::free(p); }
static void DeleteBlockSpy(void*) { if (g_deleteArmed) ++g_blockDeletes; }
class CCharString { public: const char* Text; };
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static void DeletePointee(void*) { ++g_pointeeDeletes; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); DeleteBlockSpy(Info); }
        }
        Data = 0; Info = 0;
    }
};
class CScriptThing { public: CCountedPointer<CScriptThing> PImp; const char* Name; };
static CCPPointerInfo g_blocks[8];
static int g_nextBlock;
class CThing { public:
    u64 UID; CCharString ScriptName;
    unsigned char Dead : 1; unsigned char CreatedByOwnerThingCreation : 1; unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1; unsigned char GloballyLoaded : 1; unsigned char InLimbo : 1;
    unsigned char KillLocked : 1; unsigned char CreatedBySerialisation : 1;
    CScriptThing GetScriptThing()
    {
        CScriptThing t; CCPPointerInfo* b = &g_blocks[g_nextBlock++];
        b->RefCount = 1; b->DeleteFunc = DeletePointee; b->Data = this;
        t.PImp.Data = 0; t.PImp.Info = b; t.Name = ScriptName.Text; return t;
    }
};
struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; CThing* _M_data; };
struct CThingList {
    SThingNode* _M_node;
    CThingList()
    {
        SThingNode* head = (SThingNode*)TestMalloc(sizeof(SThingNode));
        head->_M_next = head; head->_M_prev = head; _M_node = head;
    }
    ~CThingList()
    {
        SThingNode* cur = _M_node->_M_next;
        while (cur != _M_node) { SThingNode* tmp = cur; cur = cur->_M_next; TestFree(tmp); }
        _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node;
        TestFree(_M_node);
    }
    void push_back(CThing* thing)
    {
        SThingNode* pos = _M_node;
        SThingNode* node = (SThingNode*)TestMalloc(sizeof(SThingNode));
        node->_M_data = thing; node->_M_next = pos; node->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = node; pos->_M_prev = node;
    }
};
class CThingFilter_HasAnyScriptName { public:
    bool operator()(const CThing* t) const { return std::strcmp(t->ScriptName.Text, "") != 0 && std::strcmp(t->ScriptName.Text, "NULL") != 0; }
};
static CThingList* g_all;
class CThingSearchTools { public:
    CThingList& GetThingList_0049c770() { return *g_all; }
    void GetAllThings(const CThingFilter_HasAnyScriptName& filter, CThingList& things);
};
void CThingSearchTools::GetAllThings(const CThingFilter_HasAnyScriptName& filter, CThingList& things)
{
    CThingList& all = GetThingList_0049c770();
    for (SThingNode* node = all._M_node->_M_next; node != all._M_node; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (thing->IncludeInThingScans && thing->Activated && !thing->Dead && !thing->InLimbo && filter(thing))
            things.push_back(thing);
    }
}
class CWorld { public: CThingSearchTools* PThingSearchTools; };
static char g_log[256];
class CScriptBase { public:
    bool BindsEntityScript(const CCharString& name) { return name.Text[0] == 'B'; }
    void ActivateEntity(u64 uid, const CScriptThing& thing)
    { char b[32]; std::sprintf(b, "%s:%d;", thing.Name, (int)uid); std::strcat(g_log, b); }
};
class CQuestManager { public:
    CWorld* World;
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
static CThing Make(u64 uid, const char* name, int dead, int scan, int act, int limbo)
{
    CThing t; std::memset(&t, 0, sizeof t); t.UID = uid; t.ScriptName.Text = name;
    t.Dead = dead; t.IncludeInThingScans = scan; t.Activated = act; t.InLimbo = limbo; return t;
}
int main()
{
    CThing t[7] = { Make(1, "Bob", 0, 1, 1, 0), Make(2, "Ann", 0, 1, 1, 0), Make(3, "Bill", 1, 1, 1, 0),
                    Make(4, "Ben", 0, 0, 1, 0), Make(5, "Bea", 0, 1, 0, 0), Make(6, "Bo", 0, 1, 1, 1), Make(7, "Burt", 0, 1, 1, 0) };
    int bad = 0;
    {
        CThingList all;
        for (int i = 0; i < 7; ++i) all.push_back(&t[i]);
        g_all = &all;
        CThingSearchTools tools; CWorld world = { &tools }; CQuestManager qm; qm.World = &world; CScriptBase script;
        int before = g_mallocs, freesBefore = g_frees;
        g_deleteArmed = 1; qm.AttachExistingScriptedEntitiesToScript(script); g_deleteArmed = 0;
        if (std::strcmp(g_log, "Bob:1;Burt:7;") != 0) bad |= 1;
        if (g_blockDeletes != 2 || g_pointeeDeletes != 2) bad |= 2;
        if (g_mallocs - before != 4 || g_frees - freesBefore != 4) bad |= 4;      // head + Bob, Ann, Burt
    }
    if (bad) { std::printf("004b26b0_TEST FAIL\n"); return 1; }
    std::printf("004b26b0_TEST PASS\n");
    return 0;
}