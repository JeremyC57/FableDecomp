// Behaviour test for 005f4b30: src/005f4b30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryAbilities::GetPrioritisedAbilityDefs(vector<CDefPointer<const CHeroAbilityDef>>* defs)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryAbilities::GetPrioritisedAbilityDefs(vector<CDefPointer<const CHeroAbilityDef>>* defs) -- for an adult hero, lists the received abilities' defs in order, keeps them up to the last received one and pads that list with null defs to a whole palette row
// Retail 0x005F4B30, 277 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CThingCreatureBase::IsChild 0x00661F70 (on ParentThing), GetAbilityPDef(EHeroAbility) const 0x005EDFD9
// (CDefPointer by value), vector<CDefPointer<const CHeroAbilityDef>>::push_back(const value_type&) 0x005FA3A5,
// GetNumAbilitiesPerRow() const 0x005EDCF0 (manifest "_Dest_val<...>" is propagated), vector::operator= 0x005F9F5A,
// ~vector 0x005F9F14 (manifest "CActiveFile::OnReadFinished" is a byte twin).
// Shape: one appended def per ReceivedAbilityOrder slot (the by-value def temporary is released inside its null
// test through the ref-count base's DecRef()/GetRefCount() pair -- `dec [eax] ; cmp [eax],ebx` with the shared zero
// register -- and zeroed), remembering the last non-empty slot; a local vector copies the defs up to it and is padded
// with null-def temporaries (dtor folded away) while its size is not a multiple of the row width (the bound re-read
// every pass), then assigned back.
// Layout (retail; Ego_r.pdb names): ParentThing +0x04, ReceivedAbilityOrder vector<EHeroAbility> +0x154.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThingCreatureBase { public:
    bool IsChild();                                  // 0x00661F70
};
struct CRefCountedBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CHeroAbilityDef : public CRefCountedBase { public:};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
typedef CDefPointer<CHeroAbilityDef> CAbilityDefPointer;
struct CAbilityDefVector {                           // vector<CDefPointer<const CHeroAbilityDef>> (12 bytes)
    CAbilityDefPointer* _M_start;
    CAbilityDefPointer* _M_finish;
    CAbilityDefPointer* _M_end_of_storage;
    CAbilityDefVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CAbilityDefVector();                            // 0x005F9F14
    CAbilityDefVector& operator=(const CAbilityDefVector& other);   // 0x005F9F5A
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CAbilityDefPointer& operator[](unsigned long n) { return *(_M_start + n); }
    void push_back(const CAbilityDefPointer& value);  // 0x005FA3A5
};
void* FindRef_CAbilityDefVector_push_back_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CAbilityDefVector::push_back(const CAbilityDefPointer& value)
{
    void* p = FindRef_CAbilityDefVector_push_back_0();
    if (!p)
        return;
    if (*(void**)&value != p)
        return;
}
#pragma auto_inline(on)
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };
struct CAbilityOrderVector {                         // vector<EHeroAbility>
    EHeroAbility* _M_start;
    EHeroAbility* _M_finish;
    EHeroAbility* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    EHeroAbility& operator[](unsigned long n) { return *(_M_start + n); }
};

class CTCInventoryAbilities { public:
    void* __vftable;
    CThingCreatureBase* ParentThing;                 // +0x04
    unsigned char _pad_0x008[0x154 - 0x008];
    CAbilityOrderVector ReceivedAbilityOrder;        // +0x154
    CAbilityDefPointer GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    long GetNumAbilitiesPerRow() const;                               // 0x005EDCF0
    void GetPrioritisedAbilityDefs(CAbilityDefVector* defs);
};

void CTCInventoryAbilities::GetPrioritisedAbilityDefs(CAbilityDefVector* defs)
{
    if (ParentThing->IsChild())
        return;
    long lastReceived = -1;
    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
        defs->push_back(GetAbilityPDef(ReceivedAbilityOrder[i]));
        if (ReceivedAbilityOrder[i] != ABILITY_NULL)
            lastReceived = i;
    }
    CAbilityDefVector prioritised;
    long j;
    for (j = 0; j <= lastReceived; ++j)
        prioritised.push_back((*defs)[j]);
    for (unsigned long k = 0; k < prioritised.size() % GetNumAbilitiesPerRow(); ++k)
        prioritised.push_back(CAbilityDefPointer());
    *defs = prioritised;
}

// ---- stubs and driver ----
void CRefCountedBase::v00() {}
static int g_destroyed = 0;
void CRefCountedBase::Destroy() { ++g_destroyed; }
static bool g_child = false; static int g_childCalls = 0;
bool CThingCreatureBase::IsChild() { ++g_childCalls; return g_child; }
static CHeroAbilityDef g_defs[8]; static EHeroAbility g_asked[8]; static int g_nAsked = 0;
template<> CDefPointer<CHeroAbilityDef>::CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
CAbilityDefPointer CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    g_asked[g_nAsked++] = ability;
    CAbilityDefPointer p; p.Object = &g_defs[(int)ability]; ++p.Object->RefCount; return p;
}
static int g_rowCalls = 0;
long CTCInventoryAbilities::GetNumAbilitiesPerRow() const { ++g_rowCalls; return 4; }
static int g_dtor = 0; static int g_assign = 0; static const CAbilityDefVector* g_assignFrom = 0; static CAbilityDefVector* g_assignTo = 0;
CAbilityDefVector::~CAbilityDefVector() { ++g_dtor; }
CAbilityDefVector& CAbilityDefVector::operator=(const CAbilityDefVector& other) { ++g_assign; g_assignFrom = &other; g_assignTo = this; return *this; }
static int g_push = 0;
void* FindRef_CAbilityDefVector_push_back_0() { ++g_push; return 0; }
int main()
{
    static CThingCreatureBase creature; static unsigned char invStore[sizeof(CTCInventoryAbilities)];
    CTCInventoryAbilities& inv = *(CTCInventoryAbilities*)invStore; inv.ParentThing = &creature;
    static EHeroAbility order[4] = { (EHeroAbility)2, ABILITY_NULL, (EHeroAbility)5, ABILITY_NULL };
    inv.ReceivedAbilityOrder._M_start = order; inv.ReceivedAbilityOrder._M_finish = order + 4; inv.ReceivedAbilityOrder._M_end_of_storage = order + 4;
    static CAbilityDefPointer existing[4]; CAbilityDefVector defs; defs._M_start = existing; defs._M_finish = existing + 4; defs._M_end_of_storage = existing + 4;
    for (int i = 0; i < 8; ++i) g_defs[i].RefCount = 0;
    g_child = true; inv.GetPrioritisedAbilityDefs(&defs);
    CHECK(g_childCalls == 1); CHECK(g_nAsked == 0); CHECK(g_assign == 0); CHECK(g_dtor == 0);       // a child: nothing
    g_child = false; inv.GetPrioritisedAbilityDefs(&defs);
    CHECK(g_nAsked == 4); CHECK(g_asked[0] == 2); CHECK(g_asked[1] == 0); CHECK(g_asked[2] == 5); CHECK(g_asked[3] == 0);
    CHECK(g_destroyed == 4);                       // every by-value def temporary released (count 1 -> 0)
    CHECK(g_defs[2].RefCount == 0); CHECK(g_defs[5].RefCount == 0);
    CHECK(g_push == 4 + 3);                        // four appends to defs, slots 0..2 (last received = 2) copied
    CHECK(g_rowCalls == 1);                        // stand-in push_back keeps the local empty: no padding pass
    CHECK(g_assign == 1); CHECK(g_assignTo == &defs); CHECK(g_assignFrom != &defs); CHECK(g_dtor == 1);
    if (fails == 0) printf("005f4b30_TEST PASS\n"); else printf("005f4b30_TEST FAIL\n");
    return fails;
}