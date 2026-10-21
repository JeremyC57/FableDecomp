// Behaviour test for 0089bb30: src/0089bb30.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const -- set the amount on the valid thing's money-receiver component (kind 0x80), adding a "CTCMoneyReceiver" component first when it has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089BB30, length 142 bytes.
// Callees: the script thing's slots 75 (+0x12c) and 11 (+0x2c), the component-map LowerBound @ 0x0040F020,
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
//          CCharString::~CCharString @ 0x0099EAE0; the string literal "CTCMoneyReceiver" @ 0x0123C1A8.
// Shape: the store of the amount is tail-duplicated into both arms; the name temporary lives in the dead argument slot.
// The key and the name share that slot only when the key's address no longer escapes: a same-TU noinline LowerBound
// stand-in (plain binary search; the retail body is 0x0040F020) plus the name as a named local in the else block (the
// 008967f0 idiom; the stand-in with the temporary argument stays 19 lines).
// Leaf: ?EntitySetThingAsWantingMoney@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (not the retail body).
#pragma auto_inline(off)
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    long count = m_pLast - it;
    while (count > 0) {
        long half = count / 2;
        if ((unsigned long)it[half].m_kind < kind) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    CScriptThing();                                // 0x008CFF40 (the null script thing)
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0
    CThing_Retail* GetControlledCreature();        // 0x00487DD0
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetMainPlayer();               // 0x00449970
};
struct CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CThingSearchTools;
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> __forceinline bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
struct CCharString {                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = thing.GetThing();
    return t && !t->m_dead;
}
struct CTCParamsBase;
struct CTCMoneyReceiver_Retail {
    unsigned char _pad[0xc];
    long m_amountWanted;                           // +0x0c
};
struct CThingWithAddTC_Retail : CThing_Retail {
    CTCBase_Retail* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual void EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const;
};

void CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const
{
    if (thing.IsValid()) {
        CThingWithAddTC_Retail* t = (CThingWithAddTC_Retail*)thing.GetThing();
        CTCMoneyReceiver_Retail* receiver;
        if (t->HasTC(0x80))
            receiver = (CTCMoneyReceiver_Retail*)GetTC(t, 0x80);
        else {
            CCharString name("CTCMoneyReceiver", -1);
            receiver = (CTCMoneyReceiver_Retail*)t->AddTC(name, false, 0);
        }
        receiver->m_amountWanted = amount;
    }
}

// ---- stubs and driver ----
void CScriptThing::v000() { } void CScriptThing::v001() { } void CScriptThing::v002() { } void CScriptThing::v003() { } void CScriptThing::v004() { } void CScriptThing::v005() { } void CScriptThing::v006() { } void CScriptThing::v007() { } void CScriptThing::v008() { } void CScriptThing::v009() { } void CScriptThing::v010() { } void CScriptThing::v012() { } void CScriptThing::v013() { } void CScriptThing::v014() { } void CScriptThing::v015() { } void CScriptThing::v016() { } void CScriptThing::v017() { } void CScriptThing::v018() { } void CScriptThing::v019() { } void CScriptThing::v020() { } void CScriptThing::v021() { } void CScriptThing::v022() { } void CScriptThing::v023() { } void CScriptThing::v024() { } void CScriptThing::v025() { } void CScriptThing::v026() { } void CScriptThing::v027() { } void CScriptThing::v028() { } void CScriptThing::v029() { } void CScriptThing::v030() { } void CScriptThing::v031() { } void CScriptThing::v032() { } void CScriptThing::v033() { } void CScriptThing::v034() { } void CScriptThing::v035() { } void CScriptThing::v036() { } void CScriptThing::v037() { } void CScriptThing::v038() { } void CScriptThing::v039() { } void CScriptThing::v040() { } void CScriptThing::v041() { } void CScriptThing::v042() { } void CScriptThing::v043() { } void CScriptThing::v044() { } void CScriptThing::v045() { } void CScriptThing::v046() { } void CScriptThing::v047() { } void CScriptThing::v048() { } void CScriptThing::v049() { } void CScriptThing::v050() { } void CScriptThing::v051() { } void CScriptThing::v052() { } void CScriptThing::v053() { } void CScriptThing::v054() { } void CScriptThing::v055() { } void CScriptThing::v056() { } void CScriptThing::v057() { } void CScriptThing::v058() { } void CScriptThing::v059() { } void CScriptThing::v060() { } void CScriptThing::v061() { } void CScriptThing::v062() { } void CScriptThing::v063() { } void CScriptThing::v064() { } void CScriptThing::v065() { } void CScriptThing::v066() { } void CScriptThing::v067() { } void CScriptThing::v068() { } void CScriptThing::v069() { } void CScriptThing::v070() { } void CScriptThing::v071() { } void CScriptThing::v072() { } void CScriptThing::v073() { } void CScriptThing::v074() { }
void CGameScriptInterface::v000() { }
static int g_strCtors = 0, g_strDtors = 0, g_adds = 0, g_getThings = 0;
static const char* g_lastText = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtors; g_lastText = text; CHECK(length == -1); m_pData = (void*)text; }
CCharString::~CCharString() { ++g_strDtors; }
static CTCMoneyReceiver_Retail g_added;
static bool g_addActivate = true; static CTCParamsBase* g_addParams = (CTCParamsBase*)1;
CTCBase_Retail* CThingWithAddTC_Retail::AddTC(const CCharString& name, bool activate, CTCParamsBase* params)
{ ++g_adds; CHECK(strcmp((const char*)name.m_pData, "CTCMoneyReceiver") == 0); g_addActivate = activate; g_addParams = params; return (CTCBase_Retail*)&g_added; }
static CThing_Retail* g_thing; static bool g_valid;
CThing_Retail* CScriptThing::GetThing() const { ++g_getThings; return g_thing; }
bool CScriptThing::IsValid() const { return g_valid; }
CScriptThing::CScriptThing() { }
struct CScriptThingTest : CScriptThing { };
int main()
{
    static CThing_Retail thing; static CScriptThingTest st; CGameScriptInterface gsi;
    static CTCMoneyReceiver_Retail receiver; static STCEntry_Retail entries[3];
    entries[0].m_kind = 0x10; entries[0].m_pTC = 0;
    entries[1].m_kind = 0x80; entries[1].m_pTC = (CTCBase_Retail*)&receiver;
    entries[2].m_kind = 0x90; entries[2].m_pTC = 0;
    thing.m_tcs.m_pFirst = &entries[0]; thing.m_tcs.m_pLast = &entries[3];
    g_thing = &thing;
    // 1) invalid script thing: nothing
    g_valid = false; gsi.EntitySetThingAsWantingMoney(st, 50);
    CHECK(g_getThings == 0 && receiver.m_amountWanted == 0 && g_adds == 0);
    // 2) has the money receiver (0x80 = word 4 bit 0): amount set, nothing added
    g_valid = true; thing.m_tcMask[4] = 1;
    gsi.EntitySetThingAsWantingMoney(st, 50);
    CHECK(g_getThings == 1 && receiver.m_amountWanted == 50 && g_adds == 0 && g_strCtors == 0);
    // 3) absent: a "CTCMoneyReceiver" is added (inactive, no params) and gets the amount; the name is destroyed
    thing.m_tcMask[4] = 0;
    gsi.EntitySetThingAsWantingMoney(st, 125);
    CHECK(g_adds == 1 && g_added.m_amountWanted == 125 && receiver.m_amountWanted == 50);
    CHECK(g_strCtors == 1 && g_strDtors == 1 && g_addActivate == false && g_addParams == 0);
    CHECK(g_lastText && strcmp(g_lastText, "CTCMoneyReceiver") == 0);
    // 4) the stand-in lower bound: bit set but the key absent -> `last`'s component
    {
        static STCEntry_Retail e2[2]; static CTCMoneyReceiver_Retail tail;
        e2[0].m_kind = 0x10; e2[0].m_pTC = 0; e2[1].m_kind = 0x7fffffff; e2[1].m_pTC = (CTCBase_Retail*)&tail;
        thing.m_tcs.m_pFirst = &e2[0]; thing.m_tcs.m_pLast = &e2[1]; thing.m_tcMask[4] = 1;
        gsi.EntitySetThingAsWantingMoney(st, 7);
        CHECK(tail.m_amountWanted == 7 && g_adds == 1);
    }
    if (fails == 0) printf("0089bb30_TEST PASS\n");
    return fails;
}
