// CCreatureAction_ActivateQuest::Initialise() -- record the object used on the quest manager (and, when there is one, its definition's display string), activate the named quest with the LoadResources flag, mark the action initialised
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00843FC0, 207 bytes (208 with padding), virtual `?Initialise@CCreatureAction_ActivateQuest@@UAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable
//   0x01238C6C), CThing::GetDefName 0x004C7CC0 (CDefString by hidden pointer), CDefStringTable::GetString 0x009D49B0 on the
//   global table 0x013CA828, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, CQuestManager::ActivateQuest
//   0x004B4A10 (manifest "CQuestManager_ActivateQuest"); the quest manager singleton 0x013B89FC.
// Layout: retail CCreatureActionBase is 0xa8 bytes (landed FrameUpdate 0x00844090): ScriptName +0xa8, LoadResources +0xac,
//   PObjectToUse +0xad (unaligned: engine/CCreatureAction_ActivateQuest.h is pack(1), PDB +0xb4/+0xb8/+0xb9); Initialised
//   +0x63; CQuestManager LastObjectUsed pointer +0xa8, LastObjectUsedName +0xb0.
// Shape: the manager is read once into a named local for the pointer assignment (the landed "assign through a stack
//   CIntelligentPointer" shape), then re-read through the global for the later uses; the used object is a named local
//   fetched before GetDefName's hidden result pointer is pushed; the display string is a named local. ActivateQuest takes
//   its real PDB signature (const CCharString&, bool, bool) -> bool: the bool LoadResources argument is what gives the
//   `xor edx,edx ; mov dl,[edi+0ACh]` widening (an unsigned long parameter gives movzx).
class CDefString { public:
    long m_index;
    CDefString& operator=(const CDefString& other);   // user assignment: returned through the hidden pointer, copied as a dword
};

class CCharString { public:
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CDefStringTable {
    CCharString GetString(CDefString name);     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

class CThing { public:
    CDefString GetDefName();                    // 0x004C7CC0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CQuestManager { public:
    unsigned char _pad00[0xa8];
    CIntelligentPointer LastObjectUsed;         // +0xa8
    CCharString LastObjectUsedName;             // +0xb0
    bool ActivateQuest(const CCharString& name, bool a2, bool loadResources);   // 0x004B4A10
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC

#pragma pack(push, 1)                           // the real (PDB) layout: PObjectToUse is unaligned
class CCreatureAction_ActivateQuest { public:
    virtual void v00();
    unsigned char _pad04[0x63 - 4];
    bool Initialised;                           // +0x63
    unsigned char _pad64[0xa8 - 0x64];
    CCharString ScriptName;                     // +0xa8
    bool LoadResources;                         // +0xac
    CIntelligentPointer PObjectToUse;           // +0xad
    void Initialise();
};
#pragma pack(pop)

void CCreatureAction_ActivateQuest::Initialise()
{
    CQuestManager* questManager = g_pQuestManager;
    questManager->LastObjectUsed = CIntelligentPointer(PObjectToUse.GetPItem());
    if (PObjectToUse.GetPItem()) {
        CThing* object = PObjectToUse.GetPItem();
        CCharString name = g_DefStringTable.GetString(object->GetDefName());
        g_pQuestManager->LastObjectUsedName = name;
    }
    g_pQuestManager->ActivateQuest(ScriptName, false, LoadResources);
    Initialised = true;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static const char* g_strings[4] = { "zero", "one", "two", "three" };
static int g_live = 0, g_activates = 0;
static const CCharString* g_actName = 0;
static bool g_actA2 = true, g_actLoad = false;
CDefString& CDefString::operator=(const CDefString& other) { m_index = other.m_index; return *this; }
CCharString::~CCharString() { --g_live; }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
CCharString CDefStringTable::GetString(CDefString name) { CCharString s; s.PStringData = (void*)g_strings[name.m_index & 3]; ++g_live; return s; }
CDefStringTable g_DefStringTable;
struct CThingImpl : CThing { long DefIndex; };
CDefString CThing::GetDefName() { CDefString d; d.m_index = ((CThingImpl*)this)->DefIndex; return d; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointer::v00() { }
bool CQuestManager::ActivateQuest(const CCharString& name, bool a2, bool loadResources)
{
    ++g_activates; g_actName = &name; g_actA2 = a2; g_actLoad = loadResources; return true;
}
static CQuestManager* MakeManager()
{
    static unsigned char raw[sizeof(CQuestManager)];
    CQuestManager* m = (CQuestManager*)raw;
    *(void**)&m->LastObjectUsed = *(void**)&CIntelligentPointer(0);   // copy a vtable pointer into the raw object
    m->LastObjectUsed.m_pItem = 0;
    m->LastObjectUsedName.PStringData = 0;
    return m;
}
CQuestManager* g_pQuestManager = MakeManager();
void CCreatureAction_ActivateQuest::v00() { }

int main()
{
    static CThingImpl thing;
    thing.DefIndex = 2;
    static unsigned char raw[sizeof(CCreatureAction_ActivateQuest)];
    CCreatureAction_ActivateQuest* action = (CCreatureAction_ActivateQuest*)raw;
    action->ScriptName.PStringData = (void*)"QuestX";
    action->LoadResources = true;
    action->PObjectToUse.m_pItem = &thing;
    action->Initialised = false;
    action->Initialise();
    CHECK(g_pQuestManager->LastObjectUsed.m_pItem == &thing);
    CHECK(g_pQuestManager->LastObjectUsedName.PStringData == (void*)g_strings[2]);
    CHECK(g_activates == 1 && g_actName == &action->ScriptName && g_actA2 == false && g_actLoad == true);
    CHECK(action->Initialised);
    CHECK(g_live == -1);                         // the named display string destroyed once (GetString stub counts +1)
    action->PObjectToUse.m_pItem = 0;
    action->LoadResources = false;
    action->Initialised = false;
    action->Initialise();
    CHECK(g_pQuestManager->LastObjectUsed.m_pItem == 0);
    CHECK(g_pQuestManager->LastObjectUsedName.PStringData == (void*)g_strings[2]);   // unchanged without an object
    CHECK(g_activates == 2 && g_actLoad == false);
    CHECK(action->Initialised);
    if (fails == 0) printf("00843fc0_TEST PASS\n");
    return fails;
}