// Behaviour test for 00907010: src/00907010.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCombatSequence_ScorpionLungeAttack::OnCreate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatSequence_ScorpionLungeAttack::OnCreate() -- stamp the sequence with the current frame plus a world-random 0..(10 s of frames) offset, queue the lunge action, then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00907010, 200 bytes (208 with padding), virtual OnCreate.
// Callees: CWorld::GetPRandomSeed 0x0049D810 on the world at [0x013B86A0]+0x24 (manifest "CVertexBufferWin32::DoSizeof"
//   is propagated), GFROR13 0x00497890 (__fastcall, one step of the world random generator), GFGetCurrentFrame 0x0049D870
//   (__fastcall, no argument), operator new 0x00BFEA1A (0x114 bytes), the action ctor 0x008CCC70 (owner, target;
//   manifest "CCombatAction_GenericResponseToDrain", may be propagated), CCountedPointer::Reset 0x00905FD0, AddAction
//   0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Globals: the main game component 0x013B86A0 (world pointer +0x24), the logic frame rate 0x01375550 (15).
// Layout: owner +0x1c, target +0x20, m_frameOfNextUse +0x28 (retail offsets as the landed BattleCharge IsValid 0x00908DA0).
// Shape: the frame rate is read into a named local before the seed call; the range helper tests `range != 0` first
//   (retail places the `div` on the fall-through, unlike the landed 0x006E8730).
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
// Leaf: OnCreate
extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern long __fastcall GFGetCurrentFrame();                     // 0x0049D870

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}

struct CWorld_Calls {
    unsigned long* GetPRandomSeed();                            // 0x0049D810
};

struct CMainGameComponent_Retail {
    unsigned char _pad[0x24];
    CWorld_Calls* World;                                        // +0x24
};

extern CMainGameComponent_Retail* g_pMainGameComponent;        // 0x013B86A0
extern long g_logicFramesPerSecond;                             // 0x01375550 (15)
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

struct CCombatAction_008ccc70 : CCombatActionBase {
    unsigned char _body[0x110];                                 // sizeof = 0x114
    CCombatAction_008ccc70(CThingCreatureBase* owner, CThing* thing);   // 0x008CCC70
};

struct CCombatSequence_ScorpionLungeAttack : CCombatSequenceBase {
    unsigned char _pad24[4];                                    // +0x24
    long m_frameOfNextUse;                                      // +0x28
    virtual void OnCreate();
};

void CCombatSequence_ScorpionLungeAttack::OnCreate()
{
    long framesPerSecond = g_logicFramesPerSecond;
    unsigned long* seed = g_pMainGameComponent->World->GetPRandomSeed();
    unsigned long offset = WorldRandomRange(*seed, framesPerSecond * 10);
    m_frameOfNextUse = GFGetCurrentFrame() + offset;
    CCountedPointer action;
    action = new CCombatAction_008ccc70(GetCreature(), GetTarget());
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}

// ---- stubs and driver ----
void CCombatActionBase::v00() {}
void CCombatSequenceBase::v00() {}
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
static unsigned long g_seed = 12345;
unsigned long* CWorld_Calls::GetPRandomSeed() { Log('G'); return &g_seed; }
unsigned long __fastcall GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
long __fastcall GFGetCurrentFrame() { Log('F'); return 1000; }
static CWorld_Calls g_world;
static CMainGameComponent_Retail g_component;
CMainGameComponent_Retail* g_pMainGameComponent = &g_component;
long g_logicFramesPerSecond = 15;
static CCombatActionBase* g_act; static CThingCreatureBase* g_owner; static CThing* g_thing; static int g_actCtor = 0;
CCombatAction_008ccc70::CCombatAction_008ccc70(CThingCreatureBase* owner, CThing* thing)
{ g_act = this; g_owner = owner; g_thing = thing; ++g_actCtor; Log('C'); }
int main()
{
    g_component.World = &g_world;
    CCombatSequence_ScorpionLungeAttack seq; seq.m_pOwner = OWNER; seq.m_pTarget = TARGET; seq.m_frameOfNextUse = -7;
    unsigned long s = 12345UL * 0x24A1 + 0x24DF; s = (s >> 13) | (s << 19);
    seq.OnCreate();
    CHECK(g_seed == s); CHECK(seq.m_frameOfNextUse == 1000 + (long)(s % 150));
    CHECK(g_actCtor == 1); CHECK(g_owner == OWNER); CHECK(g_thing == TARGET);
    CHECK(g_addCount == 1); CHECK(g_added[0] == g_act); CHECK(g_addedBlock[0]->m_references == 1); CHECK(g_destroyed == 0);
    CHECK(g_baseCreate == 1);
    CHECK(strcmp(g_log, "GFCRAB") == 0);
    g_logicFramesPerSecond = 0; g_logn = 0; g_log[0] = 0;
    unsigned long s2 = g_seed * 0x24A1 + 0x24DF; s2 = (s2 >> 13) | (s2 << 19);
    seq.OnCreate();
    CHECK(g_seed == s2); CHECK(seq.m_frameOfNextUse == 1000); CHECK(g_addCount == 2); CHECK(g_baseCreate == 2);
    CHECK(g_addedBlock[0]->m_references == 1); CHECK(g_destroyed == 0);
    if (fails == 0) printf("00907010_TEST PASS\n"); else printf("00907010_TEST FAIL\n");
    return fails;
}