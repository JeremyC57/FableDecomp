// Behaviour test for 008b6e60: src/008b6e60.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCreatureAction_CombatBoast::Initialise()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_CombatBoast::Initialise() -- the combat dialogue manager says the dialogue def's boast speech for the creature, the action plays "SPECIAL_BOAST", then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B6E60, length 114 bytes, plain `ret`.
// Callees: CCombatActionBase::GetCombatManager @ 0x00856B10 (PCreature.GetPItem()->GetCombatManager(), out of line;
//          manifest label "_Dest_val<...>" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CombatDialogueManager::SaySpeech(CThingCreatureBase&, ulong) @ 0x008AFC40, CCharString::CCharString
//          @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0,
//          CCombatActionBase::Initialise @ 0x00857520.
// Shape: the dialogue def pointer is copied into a block-scoped CDefPointer from a named CCombatManager* local (copying
//   straight from `GetCombatManager()->...` leaves a dead `add eax,44h`: the copy ctor parameter address), null-tested
//   `inc [p+4]` add-ref, the
//   speech id (+0x5c) read into ebx, and the copy released by its destructor (count decremented, slot 1 on zero);
//   the combat manager for SaySpeech is a named local fetched before the pushes; the animation string is a scoped
//   named local; the base Initialise runs last.
// Layout: retail CCombatManager holds DialogueManager at +0x38 (PDB +0x40: the two leading maps are 12 bytes in
//   retail) and CombatDialogueManager::PDialogueDef at its +0x0c (after the 12-byte map); CCombatActionBase is 0x114
//   bytes with PCreature at +8. CCombatDialogueDef +0x5c is the boast speech id (name descriptive).
struct CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefBase {                           // counted game definition: vtable + reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
    void AddRef() const { ++((CDefBase*)this)->m_refCount; }
    void Release() const { if (--((CDefBase*)this)->m_refCount == 0) ((CDefBase*)this)->Destroy(); }
};

struct CCombatDialogueDef : CDefBase {
    unsigned char _pad08[0x5c - 0x08];
    unsigned long BoastSpeech;              // +0x5c
};

template <class T>
struct CDefPointer {
    T* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CDefPointer() { m_pDef->Release(); }
    T* operator->() const { return m_pDef; }
};

struct CombatDialogueManager {
    unsigned char SpeechGroupInfoMap[0x0c];                  // +0x00 map<ulong, TSpeechGroupInfo>
    CDefPointer<const CCombatDialogueDef> PDialogueDef;      // +0x0c
    bool SaySpeech(CThingCreatureBase& creature, unsigned long speech);   // 0x008AFC40
};

struct CCombatManager {
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;  // +0x38
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void* __vftable;                        // +0x00
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    CCombatManager* GetCombatManager();     // 0x00856B10
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_CombatBoast : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_CombatBoast::Initialise()
{
    unsigned long speech;
    {
        CCombatManager* manager = GetCombatManager();
        CDefPointer<const CCombatDialogueDef> def = manager->DialogueManager.PDialogueDef;
        speech = def->BoastSpeech;
    }
    CCombatManager* manager = GetCombatManager();
    manager->DialogueManager.SaySpeech(*PCreature.GetPItem(), speech);
    {
        CCharString name("SPECIAL_BOAST", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}

// ---- stubs and driver ----
void CDefBase::v00() {}
void CCombatActionBase::v00() {}
void CCombatActionBase::v01() {}
static char g_log[64]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 63) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
static int g_destroyed = 0;
void CDefBase::Destroy() { ++g_destroyed; Log('X'); }
static unsigned char g_managerStore[sizeof(CCombatManager)];
static CCombatManager& g_manager = *(CCombatManager*)g_managerStore;
static int g_getManager = 0;
CCombatManager* CCombatActionBase::GetCombatManager() { ++g_getManager; Log('M'); return &g_manager; }
static CThingCreatureBase* const CREATURE = (CThingCreatureBase*)0x5150;
CThingCreatureBase* CBaseIntelligentPointer::GetPItem() const { Log('P'); return m_pItem; }
static CombatDialogueManager* g_sayThis; static CThingCreatureBase* g_sayCreature; static unsigned long g_saySpeech; static long g_refAtSay;
bool CombatDialogueManager::SaySpeech(CThingCreatureBase& creature, unsigned long speech)
{ g_sayThis = this; g_sayCreature = &creature; g_saySpeech = speech; g_refAtSay = PDialogueDef.m_pDef->m_refCount; Log('S'); return true; }
static const char* g_text; static long g_len; static int g_strDtor = 0;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; g_text = text; g_len = length; Log('c'); }
CCharString::~CCharString() { ++g_strDtor; Log('d'); }
static const char* g_anim; static CCombatActionBase* g_animThis;
void CCombatActionBase::SetAnimationName(const CCharString& name) { g_anim = (const char*)name.m_pData; g_animThis = this; Log('A'); }
static int g_baseInit = 0;
void CCombatActionBase::Initialise() { ++g_baseInit; Log('I'); }
static CCombatDialogueDef g_def;
int main()
{
    g_def.m_refCount = 3; g_def.BoastSpeech = 0x1234;
    g_manager.DialogueManager.PDialogueDef.m_pDef = &g_def;
    CCreatureAction_CombatBoast action;
    action.PCreature.m_pItem = CREATURE;
    action.Initialise();
    CHECK(g_getManager == 2); CHECK(g_def.m_refCount == 3); CHECK(g_destroyed == 0);
    CHECK(g_sayThis == &g_manager.DialogueManager); CHECK(g_sayCreature == CREATURE); CHECK(g_saySpeech == 0x1234);
    CHECK(g_refAtSay == 3);                                  // the copy is released before SaySpeech
    CHECK(strcmp(g_text, "SPECIAL_BOAST") == 0); CHECK(g_len == -1); CHECK(g_anim == g_text); CHECK(g_animThis == &action);
    CHECK(g_strDtor == 1); CHECK(g_baseInit == 1);
    CHECK(strcmp(g_log, "MMPScAdI") == 0);
    // the last reference: the copy's release destroys the def through slot 1
    g_logn = 0; g_log[0] = 0;
    g_def.m_refCount = 0; g_def.BoastSpeech = 77;
    action.Initialise();
    CHECK(g_destroyed == 1); CHECK(g_def.m_refCount == 0); CHECK(g_saySpeech == 77);
    CHECK(strcmp(g_log, "MXMPScAdI") == 0);
    if (fails == 0) printf("008b6e60_TEST PASS\n"); else printf("008b6e60_TEST FAIL\n");
    return fails;
}