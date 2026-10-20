// Behaviour test for 00729410: src/00729410.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCCreatureExpression::SetExpression(long expressionType, float maxWeight)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureExpression::SetExpression(long expressionType, float maxWeight) -- pick a random animation for the expression type from the creature's expression def and force it as the primary expression; type 0 first restores or demotes the front state
//
// Retail 0x00729410, 134 bytes (`ret 8`). Callees: CDefClassBase::GetSubDef<CCreatureExpressionDef>
// (CDefPointer<const CCreatureExpressionDef>*) @ 0x00485815 (label "GetSubDef<CAbilityDef>",
// propagated), CTCBase::GetWorld @ 0x00686A70, CWorld::GetPRandomSeed @ 0x0049D810 (label
// "CVertexBufferWin32::DoSizeof", propagated), the def's expression map FindRandomElement(long,
// unsigned long*) @ 0x0086F510, CTCCreatureExpression::ForcePrimaryExpression(long, float) @ 0x007293A0,
// the def's virtual slot 1 (release at zero).
// Layout: ExpressionStates (one sentinel pointer) at +0x0c, CExpressionState as in 0x007293A0; the
// parent thing's def +0x70; the expression def's map at +0x38.
// Shape: the type-0 block uses the front node as a pointer local (AnimIndex == 0 -> MaxWeight = 1.0f,
// else Primary = false); the sub-def out pointer starts null; its object is read once (ebp) and released
// at the end without a null test.
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CExpressionState {
    long          AnimIndex;                        // +0x00
    float         MaxWeight;                        // +0x04
    float         Weight;                           // +0x08
    float         OldWeight;                        // +0x0c
    bool          Primary;                          // +0x10
    unsigned char Stage;                            // +0x11
};

struct CExpressionStateList {                       // STLport list<CExpressionState>: one sentinel pointer
    struct node { node* next; node* prev; CExpressionState value; };
    node* head;
    bool empty() const { return head->next == head; }
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

struct CExpressionMap {
    long FindRandomElement(long expressionType, unsigned long* seed);   // 0x0086F510
};

struct CCreatureExpressionDef : CDefPointeeBase {
    unsigned char   _pad08[0x38 - 0x08];
    CExpressionMap* Expressions;                    // +0x38
};

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (DecRefToZero(Object->m_refCount))
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureExpressionDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureExpressionDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef_Object;                     // +0x70
};

struct CWorld {
    unsigned long* GetPRandomSeed();                // 0x0049D810
};

struct CTCCreatureExpression {
    virtual void v00();
    CThing*              ParentThing;               // +0x04 (CTCBase)
    unsigned char        _pad08[4];
    CExpressionStateList ExpressionStates;          // +0x0c
    bool                 Blinking;                  // +0x10
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void ForcePrimaryExpression(long animIndex, float maxWeight);   // 0x007293A0
    void SetExpression(long expressionType, float maxWeight);
};

void CTCCreatureExpression::SetExpression(long expressionType, float maxWeight)
{
    if (expressionType == 0 && !ExpressionStates.empty())
    {
        CExpressionStateList::node* front = ExpressionStates.head->next;
        if (front->value.AnimIndex == 0)
            front->value.MaxWeight = 1.0f;
        else
            front->value.Primary = false;
    }
    CDefPointer<CCreatureExpressionDef> def;
    ParentThing->PDef_Object->GetSubDef(&def);
    CExpressionMap* expressions = def->Expressions;
    long anim = expressions->FindRandomElement(expressionType, GetWorld()->GetPRandomSeed());
    if (anim)
        ForcePrimaryExpression(anim, maxWeight);
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static long g_type = -1; static unsigned long* g_seedArg = 0; static long g_result = 0;
static long g_forced = -1; static float g_forcedWeight = 0;
static unsigned long g_seed = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
long CExpressionMap::FindRandomElement(long expressionType, unsigned long* seed) { g_type = expressionType; g_seedArg = seed; return g_result; }
unsigned long* CWorld::GetPRandomSeed() { return &g_seed; }
static CWorld g_world;
CWorld* CTCCreatureExpression::GetWorld() { return &g_world; }
void CTCCreatureExpression::ForcePrimaryExpression(long animIndex, float maxWeight) { g_forced = animIndex; g_forcedWeight = maxWeight; }
void CTCCreatureExpression::v00() {}
int main()
{
    CExpressionMap map; CCreatureExpressionDef def; def.m_refCount = 3; def.Expressions = &map; g_def = &def;
    CDefClassBase base; base.m_refCount = 1;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.PDef_Object = &base;
    CExpressionStateList::node head, n1; head.next = &head; head.prev = &head;
    CTCCreatureExpression e; e.ParentThing = &thing; e.ExpressionStates.head = &head; e.Blinking = false;
    // empty list, a found expression is forced
    g_result = 9; e.SetExpression(0, 0.5f);
    CHECK(g_type == 0); CHECK(g_seedArg == &g_seed); CHECK(g_forced == 9); CHECK(g_forcedWeight == 0.5f); CHECK(def.m_refCount == 2);
    // front state with anim 0 gets max weight 1; nothing found -> not forced
    head.next = &n1; n1.next = &head; n1.value.AnimIndex = 0; n1.value.MaxWeight = 0.25f; n1.value.Primary = true;
    g_forced = -1; g_result = 0; e.SetExpression(0, 0.75f);
    CHECK(n1.value.MaxWeight == 1.0f); CHECK(n1.value.Primary); CHECK(g_forced == -1); CHECK(def.m_refCount == 1);
    // front state with an anim loses primary
    n1.value.AnimIndex = 5; n1.value.MaxWeight = 0.25f; g_result = 3; e.SetExpression(0, 0.1f);
    CHECK(!n1.value.Primary); CHECK(n1.value.MaxWeight == 0.25f); CHECK(g_forced == 3); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 1);
    // a non-zero type leaves the front state alone
    def.m_refCount = 5; n1.value.Primary = true; n1.value.AnimIndex = 0; n1.value.MaxWeight = 0.25f; e.SetExpression(4, 0.2f);
    CHECK(n1.value.Primary); CHECK(n1.value.MaxWeight == 0.25f); CHECK(g_type == 4); CHECK(def.m_refCount == 4);
    if (fails == 0) printf("00729410_TEST PASS\n"); else printf("00729410_TEST FAIL\n");
    return fails;
}