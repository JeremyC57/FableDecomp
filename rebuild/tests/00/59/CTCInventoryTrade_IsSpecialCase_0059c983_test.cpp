// Behaviour test for 0059c983: src/0059c983.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCInventoryTrade::IsSpecialCase(long itemType)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryTrade::IsSpecialCase(long itemType) -- an item is a special trade case when it is a hair card or a tattoo card, or when its definition carries an ability sub-def
// Leaf: ?IsSpecialCase@CTCInventoryTrade@@QAE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C983, length 274 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?IsSpecialCase@CTCInventoryTrade@@QAE_NJ@Z.
// Callees: GetHairCardDef(long) @ 0x005D9A10 and GetTattooCardDef(long) @ 0x005D9AA0 (`__fastcall`, a counted def pointer by
// value through the hidden pointer; the catalog labels both GetHairCardDef), CTCBase::GetDefinitions() @ 0x006869D0,
// CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer*) @
// 0x005B1E5A, the defs' virtual slot 1 on release.
// Shape: the landed ProcessTradeAction (0x005AEBFE) card-def temporaries (long-form release through the def's
// ReleaseTemporary; the tattoo pointer's dtor also clears its word); the item def and its ability sub-def are owning out
// slots (zeroed from the esi zero register, short-form release) filled by same-TU out-only stand-ins of GetDef and
// GetSubDef, so VC7.1 keeps the fetched defs in edi/ecx; one `&&` test, and every dtor forced inline so the true and false
// exits each get their own release chain.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRefCounted {                                 // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:              // a counted def reference returned by value
    T* m_p;
    __forceinline ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};
template <class T> struct CClearingDefPointer {      // the same, clearing its word after the release
    T* m_p;
    __forceinline ~CClearingDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
template <class T> struct CDefSlot {                 // an owning out slot for GetDef / GetSubDef
    T* m_p;
    CDefSlot() : m_p(0) { }
    __forceinline ~CDefSlot()
    {
        if (m_p && --m_p->m_count == 0)
            m_p->Destroy();
    }
};
struct CTattooCardDef : CDefBase { };
class CHairCardDef : public CDefBase { public: };
class CAbilityDef : public CDefBase { public: };
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefSlot<CAbilityDef>* out);      // 0x005B1E5A
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefSlot<CDefClassBase>* out);   // 0x0042AF3C
};
CDefClassBase* FindRef_CDefinitionManager_GetDef(CDefinitionManager* manager, long index);   // stand-in callee
CAbilityDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase* defClass);                       // stand-in callee
#pragma auto_inline(off)
// Same-TU stand-ins for GetDef 0x0042AF3C and GetSubDef<CAbilityDef> 0x005B1E5A: they only write the out slot, so VC7.1
// knows the slots do not escape (retail's link-time knowledge) and keeps the fetched defs in registers.
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefSlot<CDefClassBase>* out)
{
    CDefClassBase* def = FindRef_CDefinitionManager_GetDef(this, index);
    out->m_p = def;
    return def != 0;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefSlot<CAbilityDef>* out)
{
    CAbilityDef* def = FindRef_CDefClassBase_GetSubDef(this);
    out->m_p = def;
    return def != 0;
}
#pragma auto_inline(on)
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType);              // 0x005D9A10
CClearingDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType);  // 0x005D9AA0

class CTCInventoryTrade { public:
    CDefinitionManager* GetDefinitions() const;      // 0x006869D0 (CTCBase)
    bool IsSpecialCase(long itemType);
};

bool CTCInventoryTrade::IsSpecialCase(long itemType)
{
    CDefPointer<CHairCardDef> hair = GetHairCardDef(itemType);
    if (hair.m_p)
        return true;
    CClearingDefPointer<CTattooCardDef> tattoo = GetTattooCardDef(itemType);
    if (tattoo.m_p)
        return true;
    CDefSlot<CDefClassBase> itemDef;
    CDefSlot<CAbilityDef> ability;
    if (GetDefinitions()->GetDef(itemType, &itemDef) && itemDef.m_p->GetSubDef(&ability))
        return true;
    return false;
}

// ---- stubs and driver ----
static int destroyed = 0;
static void* lastDestroyed = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++destroyed; lastDestroyed = this; }
static CHairCardDef hairDef;
static CTattooCardDef tattooDef;
static CDefClassBase itemDefObj;
static CAbilityDef abilityDef;
static bool haveHair, haveTattoo, haveItem, haveAbility;
static int hairCalls, tattooCalls, getDefCalls, getSubDefCalls;
static long lastIndex;
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType)
{
    ++hairCalls; lastIndex = itemType;
    CDefPointer<CHairCardDef> p; p.m_p = 0;
    if (haveHair) { hairDef.m_count += 2; p.m_p = &hairDef; }   // +1 compensates the local's dtor (VC7.1 has no NRVO)
    return p;
}
CClearingDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType)
{
    ++tattooCalls; lastIndex = itemType;
    CClearingDefPointer<CTattooCardDef> p; p.m_p = 0;
    if (haveTattoo) { tattooDef.m_count += 2; p.m_p = &tattooDef; }   // +1 compensates the local's dtor (no NRVO)
    return p;
}
static CDefinitionManager manager;
CDefinitionManager* CTCInventoryTrade::GetDefinitions() const { return &manager; }
CDefClassBase* FindRef_CDefinitionManager_GetDef(CDefinitionManager*, long index)
{
    ++getDefCalls; lastIndex = index;
    if (!haveItem) return 0;
    ++itemDefObj.m_count; return &itemDefObj;
}
CAbilityDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase* defClass)
{
    ++getSubDefCalls;
    if (defClass != &itemDefObj || !haveAbility) return 0;
    ++abilityDef.m_count; return &abilityDef;
}
static void Reset(bool h, bool t, bool i, bool a)
{
    haveHair = h; haveTattoo = t; haveItem = i; haveAbility = a;
    hairCalls = tattooCalls = getDefCalls = getSubDefCalls = 0; destroyed = 0; lastDestroyed = 0;
    hairDef.m_count = 1; tattooDef.m_count = 1; itemDefObj.m_count = 1; abilityDef.m_count = 1;
}
static bool Balanced() { return hairDef.m_count == 1 && tattooDef.m_count == 1 && itemDefObj.m_count == 1 && abilityDef.m_count == 1 && destroyed == 0; }
int main()
{
    CTCInventoryTrade trade;
    Reset(true, true, true, true);
    CHECK(trade.IsSpecialCase(42) == true);
    CHECK(hairCalls == 1 && tattooCalls == 0 && getDefCalls == 0 && lastIndex == 42 && Balanced());
    Reset(false, true, true, true);
    CHECK(trade.IsSpecialCase(7) == true);
    CHECK(hairCalls == 1 && tattooCalls == 1 && getDefCalls == 0 && Balanced());
    Reset(false, false, true, true);
    CHECK(trade.IsSpecialCase(9) == true);
    CHECK(getDefCalls == 1 && getSubDefCalls == 1 && lastIndex == 9 && Balanced());
    Reset(false, false, true, false);
    CHECK(trade.IsSpecialCase(9) == false);
    CHECK(getDefCalls == 1 && getSubDefCalls == 1 && Balanced());
    Reset(false, false, false, true);
    CHECK(trade.IsSpecialCase(9) == false);
    CHECK(getDefCalls == 1 && getSubDefCalls == 0 && Balanced());
    // last reference dropped -> Destroy (slot 1)
    Reset(false, false, true, true);
    itemDefObj.m_count = 0; abilityDef.m_count = 0;
    CHECK(trade.IsSpecialCase(3) == true);
    CHECK(destroyed == 2 && itemDefObj.m_count == 0 && abilityDef.m_count == 0);
    Reset(true, false, false, false);
    hairDef.m_count = 0;    // the function's release drops the last reference
    CHECK(trade.IsSpecialCase(3) == true);
    CHECK(destroyed == 1 && lastDestroyed == (void*)(CDefVirtual*)&hairDef);
    Reset(false, true, false, false);
    tattooDef.m_count = 0;
    CHECK(trade.IsSpecialCase(3) == true);
    CHECK(destroyed == 1 && lastDestroyed == (void*)(CDefVirtual*)&tattooDef);
    char buf[64];
    _snprintf(buf, sizeof(buf), "%s", fails == 0 ? "0059c983_TEST PASS" : "0059c983_TEST FAIL");
    printf("%s\n", buf);
    return fails;
}
