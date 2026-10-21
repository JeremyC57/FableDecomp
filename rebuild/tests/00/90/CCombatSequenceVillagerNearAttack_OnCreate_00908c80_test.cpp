// Behaviour test for 00908c80: src/00908c80.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCombatSequence_VillagerNearAttack::OnCreate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatSequence_VillagerNearAttack::OnCreate() -- queue an interruptable near attack (action code 150) and a CCombatAction_0097d050-constructed follow-up, then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00908C80, 211 bytes, virtual `?OnCreate@CCombatSequence_VillagerNearAttack@@UAEXXZ`.
// Callees: operator new 0x00BFEA1A (0x114-byte actions), CCombatActionBase::CCombatActionBase(owner, thing, code, sound)
//   0x00857F30 under the inlined CCreatureAction_InterruptableNearAttack ctor (vtable 0x01262D94, rtti-vtables.tsv),
//   the second action's ctor 0x0097D050 (the second 0x114-byte action, named by its constructor),
//   CCountedPointer::Reset 0x00905FD0, CCombatSequenceBase::AddAction 0x008B0B10, base OnCreate 0x008B0650,
//   operator delete 0x00BFE9BC (count block in the inlined counted-pointer dtor).
// Layout: sequence owner +0x1c, target +0x20 (as the landed sibling OnCreate rows, e.g. 0x00908780); engine header
//   CCombatSequence_VillagerNearAttack.h is the PDB size only, so the landed sibling model is used.
// Shape: one counted-pointer local reassigned for both actions; the sound pair is the zeroed temporary of the inline ctor.
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
// Leaf: OnCreate
struct CThingCreatureBase;
struct CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CSoundPair {                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCountedPointer {
    CCombatActionBase* m_pObject;           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(CCombatActionBase* object) { Reset(object); return *this; }
    void Reset(CCombatActionBase* object);  // 0x00905FD0
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CCombatSequenceBase {
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    CThingCreatureBase* GetCreature() const { return m_pOwner; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCreatureAction_InterruptableNearAttack : CCombatActionBase {   // vtable 0x01262D94
    unsigned char _body[0x110];
    virtual void v00();
    CCreatureAction_InterruptableNearAttack(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 150, CSoundPair()) { }
};

struct CCombatAction_0097d050 : CCombatActionBase {
    unsigned char _body[0x110];             // sizeof = 0x114
    CCombatAction_0097d050(CThingCreatureBase* owner, CThing* thing);   // 0x0097D050
};

struct CCombatSequence_VillagerNearAttack : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_VillagerNearAttack::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_InterruptableNearAttack(GetCreature(), GetTarget());
    AddAction(action);
    action = new CCombatAction_0097d050(GetCreature(), GetTarget());
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}

// ---- stubs and driver ----
void CCombatActionBase::v00() {}
void CCombatSequenceBase::v00() {}
void CCreatureAction_InterruptableNearAttack::v00() {}
static char g_log[256]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 255) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
static int g_destroyed = 0;
static void __fastcall DestroyAction(void*) { ++g_destroyed; Log('D'); }
void CCountedPointer::Reset(CCombatActionBase* object)
{
    Log('R');
    if (m_pCount && --m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
    m_pObject = object; m_pCount = 0;
    if (object) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyAction; m_pCount->m_pObject = object; }
}
static CCombatActionBase* g_added[4]; static SCountBlock* g_addedBlock[4]; static int g_addCount = 0;
void CCombatSequenceBase::AddAction(const CCountedPointer& action)
{
    Log('A'); g_added[g_addCount & 3] = action.m_pObject; g_addedBlock[g_addCount & 3] = action.m_pCount;
    if (action.m_pCount) ++action.m_pCount->m_references;
    ++g_addCount;
}
static int g_baseCreate = 0;
void CCombatSequenceBase::OnCreate() { ++g_baseCreate; Log('B'); }
static CThingCreatureBase* const OWNER = (CThingCreatureBase*)0x1111;
static CThing* const TARGET = (CThing*)0x2222;
static long g_baseCode = -1, g_baseSound0 = -1, g_baseSound1 = -1; static CThingCreatureBase* g_baseOwner; static CThing* g_baseThing;
CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound)
{ g_baseOwner = owner; g_baseThing = thing; g_baseCode = actionCode; g_baseSound0 = sound.m_first; g_baseSound1 = sound.m_second; Log('b'); }
static CCombatActionBase* g_act2; static CThingCreatureBase* g_owner2; static CThing* g_thing2; static int g_act2Ctor = 0;
CCombatAction_0097d050::CCombatAction_0097d050(CThingCreatureBase* owner, CThing* thing)
{ g_act2 = this; g_owner2 = owner; g_thing2 = thing; ++g_act2Ctor; Log('C'); }
int main()
{
    CCombatSequence_VillagerNearAttack seq; seq.m_pOwner = OWNER; seq.m_pTarget = TARGET;
    seq.OnCreate();
    CHECK(g_baseOwner == OWNER); CHECK(g_baseThing == TARGET); CHECK(g_baseCode == 150); CHECK(g_baseSound0 == 0); CHECK(g_baseSound1 == 0);
    CHECK(g_act2Ctor == 1); CHECK(g_owner2 == OWNER); CHECK(g_thing2 == TARGET);
    CHECK(g_addCount == 2); CHECK(g_added[1] == g_act2); CHECK(g_added[0] != 0); CHECK(g_added[0] != g_act2);
    CHECK(g_addedBlock[0]->m_references == 1); CHECK(g_addedBlock[1]->m_references == 1); CHECK(g_destroyed == 0);
    CHECK(g_baseCreate == 1);
    CHECK(strcmp(g_log, "bRACRAB") == 0);
    if (fails == 0) printf("00908c80_TEST PASS\n"); else printf("00908c80_TEST FAIL\n");
    return fails;
}