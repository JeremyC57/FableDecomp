// Behaviour test for 009091d0: src/009091d0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCombatSequence_WillEnflame::OnCreate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatSequence_WillEnflame::OnCreate() -- queue a CCreatureAction_PlayCombatAnimation on "ST_BUILDUP_ENFLAME" (flag 1) and a 0x11C-byte enflame action (owner, 2), then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x009091D0, 237 bytes (192 with padding), virtual `?OnCreate@CCombatSequence_WillEnflame@@UAEXXZ`.
// Callees: operator new 0x00BFEA1A (0x118 bytes), CCharString::CCharString(const char*, long) 0x0099EBF0 ("ST_BUILDUP_ENFLAME"
//   @ 0x012801D0), CCreatureAction_PlayCombatAnimation ctor 0x008B7F80, the 0x11C-byte action ctor 0x008BB450 (owner, 2), CCountedPointer::Reset
//   0x00905FD0, ~CCharString 0x0099EAE0, AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Layout: sequence owner +0x1c, target +0x20 (landed sibling OnCreate model); engine header CCombatSequence_WillEnflame.h
//   is the PDB size only.
// Shape: the string temporary lives only on the allocated branch of the new-expression, so its destruction is guarded by
//   the construction flag (`test bl,1`) -- the same register that carries the constant 1 argument. The literal is
//   converted implicitly (default length -1), which builds the temporary before the other arguments are pushed.
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
// Leaf: OnCreate
class CThingCreatureBase;
class CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

class CSoundPair { public:                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
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

class CCombatSequenceBase { public:
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    CThingCreatureBase* GetCreature() const { return m_pOwner; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    unsigned char _body[0x114];                   // sizeof = 0x118
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long flag);   // 0x008B7F80
};

class CCreatureAction_CastWillEnflameSpell : public CCombatActionBase { public:   // 0x11C bytes; manifest label of 0x008BB450
    unsigned char _body[0x118];
    CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* owner, long mode);   // 0x008BB450
};

struct CCombatSequence_WillEnflame : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_WillEnflame::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_PlayCombatAnimation(GetCreature(), GetTarget(), "ST_BUILDUP_ENFLAME", 1);
    AddAction(action);
    action = new CCreatureAction_CastWillEnflameSpell(GetCreature(), 2);
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
static const char* g_strText[4]; static int g_strLen[4], g_strCtor = 0, g_strDtor = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; g_strText[g_strCtor & 3] = text; g_strLen[g_strCtor & 3] = length; ++g_strCtor; Log('S'); }
CCharString::~CCharString() { ++g_strDtor; Log('s'); }
static CCombatActionBase* g_act; static CThingCreatureBase* g_owner; static CThing* g_thing; static const char* g_anim; static long g_flag; static int g_actCtor = 0;
CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long flag)
{ g_act = this; g_owner = owner; g_thing = thing; g_anim = (const char*)animation.PStringData; g_flag = flag; ++g_actCtor; Log('C'); }
static CCombatActionBase* g_act2; static CThingCreatureBase* g_owner2; static long g_mode = -1; static int g_act2Ctor = 0;
CCreatureAction_CastWillEnflameSpell::CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* owner, long mode)
{ g_act2 = this; g_owner2 = owner; g_mode = mode; ++g_act2Ctor; Log('E'); }
int main()
{
    CCombatSequence_WillEnflame seq; seq.m_pOwner = OWNER; seq.m_pTarget = TARGET;
    seq.OnCreate();
    CHECK(g_actCtor == 1); CHECK(g_owner == OWNER); CHECK(g_thing == TARGET); CHECK(g_flag == 1);
    CHECK(strcmp(g_anim, "ST_BUILDUP_ENFLAME") == 0); CHECK(g_strCtor == 1); CHECK(g_strDtor == 1); CHECK(g_strLen[0] == -1);
    CHECK(g_act2Ctor == 1); CHECK(g_owner2 == OWNER); CHECK(g_mode == 2);
    CHECK(g_addCount == 2); CHECK(g_added[0] == g_act); CHECK(g_added[1] == g_act2);
    CHECK(g_addedBlock[0]->m_references == 1); CHECK(g_addedBlock[1]->m_references == 1); CHECK(g_destroyed == 0);
    CHECK(g_baseCreate == 1);
    CHECK(strcmp(g_log, "SCRsAERAB") == 0);
    if (fails == 0) printf("009091d0_TEST PASS\n"); else printf("009091d0_TEST FAIL\n");
    return fails;
}