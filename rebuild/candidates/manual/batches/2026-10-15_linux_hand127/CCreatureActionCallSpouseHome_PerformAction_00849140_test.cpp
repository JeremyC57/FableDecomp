#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct C3DVector { float X, Y, Z; };
struct CMessageEvent { int id; };
struct CCharString { const char* m_text; CCharString(const char* text, long length) : m_text(text) { (void)length; } };
static int g_destroyed = 0;
struct CMessageEventDef { virtual ~CMessageEventDef() { } virtual void Destroy() { ++g_destroyed; } long m_count; CMessageEventDef() : m_count(0) { } };
struct CDefPointer { CMessageEventDef* m_p; CDefPointer() : m_p(0) { } };
static CMessageEventDef g_def; static bool g_found = true; static const char* g_lookedUp = 0;
struct CDefinitionManager { bool GetDef(const CCharString& name, CDefPointer& def) { g_lookedUp = name.m_text; if (g_found) { def.m_p = &g_def; ++g_def.m_count; } return g_found; } };
struct CThing { C3DVector m_pos; const C3DVector& GetPosition() const { return m_pos; } };
static CMessageEventDef* g_msgDef = 0; static const C3DVector* g_msgPos = 0; static CThing* g_msgThing1 = 0; static CThing* g_msgThing2 = 0; static long g_msgPriority = -1; static int g_messages = 0;
struct CMessageEventManager {
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority)
    { g_msgDef = def; g_msgPos = position; g_msgThing1 = thing1; g_msgThing2 = thing2; g_msgPriority = priority; ++g_messages; return 0; }
};
struct CWorld { CMessageEventManager* m_pMessageEventManager; };
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { } CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CThing* GetPItem() const { return m_pItem; } };
static CDefinitionManager g_manager; static CMessageEventManager g_messageManager; static CWorld g_world;
struct CCreatureActionBase {
    virtual ~CCreatureActionBase() { }
    CBaseIntelligentPointer PCreature;
    CDefinitionManager* GetPDefinitionManager() const { return &g_manager; }
    CWorld* GetPWorld() const { return &g_world; }
};
struct CCreatureAction_CallSpouseHome : CCreatureActionBase { virtual void PerformAction(); };
void CCreatureAction_CallSpouseHome::PerformAction()
{
    CDefPointer def;
    CMessageEventDef* p;
    bool found;
    {
        CCharString name("STANDARD_MESSAGE_EVENT_CALL_SPOUSE_HOME", -1);
        found = GetPDefinitionManager()->GetDef(name, def);
    }
    p = def.m_p;
    if (found) {
        CThing* pCreatureThing = PCreature.GetPItem();
        CThing* pCreature = PCreature.GetPItem();
        CMessageEventManager* pManager = GetPWorld()->m_pMessageEventManager;
        pManager->AddMessage(p, &pCreature->GetPosition(), PCreature.GetPItem(), 0, 0);
        (void)pCreatureThing;
    }
    if (p) {
        if (--p->m_count == 0)
            p->Destroy();
    }
}
int main()
{
    CThing creature; g_world.m_pMessageEventManager = &g_messageManager;
    CCreatureAction_CallSpouseHome action; action.PCreature.m_pItem = &creature;
    action.PerformAction();
    bool ok = std::strcmp(g_lookedUp, "STANDARD_MESSAGE_EVENT_CALL_SPOUSE_HOME") == 0 && g_messages == 1 && g_msgDef == &g_def && g_msgPos == &creature.m_pos
        && g_msgThing1 == &creature && g_msgThing2 == 0 && g_msgPriority == 0 && g_def.m_count == 0 && g_destroyed == 1;
    g_def.m_count = 1; action.PerformAction();
    ok = ok && g_messages == 2 && g_def.m_count == 1 && g_destroyed == 1;
    g_found = false; action.PerformAction();
    ok = ok && g_messages == 2 && g_destroyed == 1;
    if (!ok) { std::printf("00849140_TEST FAIL\n"); return 1; }
    std::printf("00849140_TEST PASS\n");
    return 0;
}
