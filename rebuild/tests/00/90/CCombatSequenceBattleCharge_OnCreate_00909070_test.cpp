// Behaviour test for 00909070: src/00909070.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCombatSequence_BattleCharge::OnCreate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatSequence_BattleCharge::OnCreate() -- stamp the use frame, read the charge value from the owner's sub-definition (0 when absent), queue a CCreatureAction_CastBattleCharge with it, then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00909070, 206 bytes (208 with padding), virtual `?OnCreate@CCombatSequence_BattleCharge@@UAEXXZ`.
// Callees: the world frame counter `mov ecx,[0x013B86A0] ; call 0x004167C8` (incremental thunk onto GFGetCurrentFrame, as in
//   the landed IsValid 0x00908DA0), CDefClassBase::GetSubDef<T>(CDefPointer<T>&) 0x008E3240 on the owner's def (+0x70),
//   operator new 0x00BFEA1A (0x130 bytes), CCreatureAction_CastBattleCharge ctor 0x008CE290 (owner, target, 0, value),
//   CCountedPointer::Reset 0x00905FD0, AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC; the def
//   is released through its vtable slot 1 when its count (+4) drops to zero.
// Layout: owner +0x1c, target +0x20, m_frameOfLastUse +0x28 (retail, as 0x00908DA0); the sub-def's value at +0x48 (T is the
//   opaque instantiation of the landed 0x008E3240, its label names CThunderBattleDef).
// Shape: the counted-pointer dtor's null stores survive because the def pointer's release follows; the def pointer is
//   released through its register-held copy (`dec [edi+4] ; jne`). The value local is declared before the def pointer
//   (zero-store order), the owner's def is read into a local before GetSubDef, owner/target reach the action through
//   inline accessors (the OnCreate family rule), and GetSubDef<T> has a same-TU no-escape stand-in body (COMDAT
//   instance): together they give retail without the parked /Oa flag.
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

struct CWorld_Retail {
    long GetCurrentFrame();                                     // 0x004167C8 -> GFGetCurrentFrame 0x0049D870
};
extern CWorld_Retail* g_pWorld;                                 // 0x013B86A0

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                     // slot 1
    long m_count;                                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

class CThunderBattleDef : public CDefBase { public:
    unsigned char _pad08[0x40];
    long m_value;                                               // +0x48
};

template<class T> class CDefPointer { public:
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
    ~CDefPointer()
    {
        if (m_pDef)
            m_pDef->Release();
    }
    T* operator->() const { return m_pDef; }
};

class CDefClassBase { public:
    template<class T> bool GetSubDef(CDefPointer<T>& out);     // 0x008E3240 (CThunderBattleDef instantiation)
};

void* FindSubDef_008e3240(CDefClassBase* def);
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    T* def = (T*)FindSubDef_008e3240(this);
    if (!def)
        return false;
    out.m_pDef = def;
    return true;
}
#pragma auto_inline(on)
class CThingCreatureBase { public:
    unsigned char _pad[0x70];
    CDefClassBase* m_pDef;                                      // +0x70
};

class CCreatureAction_CastBattleCharge : public CCombatActionBase { public:
    unsigned char _body[0x12c];                                 // sizeof = 0x130
    CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, CThing* thing, long mode, long value);   // 0x008CE290
};

struct CCombatSequence_BattleCharge : CCombatSequenceBase {
    unsigned char _pad24[4];                                    // +0x24
    long m_frameOfLastUse;                                      // +0x28
    virtual void OnCreate();
};

void CCombatSequence_BattleCharge::OnCreate()
{
    m_frameOfLastUse = g_pWorld->GetCurrentFrame();
    long value = 0;
    CDefPointer<CThunderBattleDef> def;
    CDefClassBase* ownerDef = GetCreature()->m_pDef;
    if (ownerDef->GetSubDef(def))
        value = def->m_value;
    CCountedPointer action;
    action = new CCreatureAction_CastBattleCharge(GetCreature(), GetTarget(), 0, value);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}

// ---- stubs and driver ----
void CCombatActionBase::v00() {}
void CCombatSequenceBase::v00() {}
void CDefBase::v00() {}
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
static int g_destroyed2 = 0;
void CDefBase::Destroy() { ++g_destroyed2; Log('X'); }
static CThunderBattleDef g_thunder; static bool g_haveDef = true; static CDefClassBase* g_subOf;
void* FindSubDef_008e3240(CDefClassBase* def) { g_subOf = def; return g_haveDef ? &g_thunder : 0; }
static long g_frame = 500;
long CWorld_Retail::GetCurrentFrame() { Log('F'); return g_frame; }
static CWorld_Retail g_world; CWorld_Retail* g_pWorld = &g_world;
static CCombatActionBase* g_act; static CThingCreatureBase* g_owner; static CThing* g_thing; static long g_mode = -1, g_value = -1; static int g_actCtor = 0;
CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, CThing* thing, long mode, long value)
{ g_act = this; g_owner = owner; g_thing = thing; g_mode = mode; g_value = value; ++g_actCtor; Log('C'); }
int main()
{
    static unsigned char ownerStore[0x80]; memset(ownerStore, 0, sizeof ownerStore);
    CThingCreatureBase* ownerThing = (CThingCreatureBase*)ownerStore;
    CDefClassBase* ownerDef = (CDefClassBase*)0x7777; ownerThing->m_pDef = ownerDef;
    CCombatSequence_BattleCharge seq; seq.m_pOwner = ownerThing; seq.m_pTarget = TARGET; seq.m_frameOfLastUse = 0;
    g_thunder.m_count = 1; g_thunder.m_value = 42;
    seq.OnCreate();
    CHECK(seq.m_frameOfLastUse == 500); CHECK(g_subOf == ownerDef);
    CHECK(g_actCtor == 1); CHECK(g_owner == ownerThing); CHECK(g_thing == TARGET); CHECK(g_mode == 0); CHECK(g_value == 42);
    CHECK(g_addCount == 1); CHECK(g_added[0] == g_act); CHECK(g_baseCreate == 1);
    CHECK(g_thunder.m_count == 0); CHECK(g_destroyed2 == 1);           // the stand-in hands the def over without a reference
    CHECK(strcmp(g_log, "FCRABX") == 0);
    g_haveDef = false; g_logn = 0; g_log[0] = 0; g_frame = 600;
    seq.OnCreate();
    CHECK(seq.m_frameOfLastUse == 600); CHECK(g_value == 0); CHECK(g_destroyed2 == 1); CHECK(strcmp(g_log, "FCRAB") == 0);
    if (fails == 0) printf("00909070_TEST PASS\n"); else printf("00909070_TEST FAIL\n");
    return fails;
}