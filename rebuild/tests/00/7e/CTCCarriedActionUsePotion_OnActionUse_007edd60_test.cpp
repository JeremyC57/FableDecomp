// Behaviour test for 007edd60: src/007edd60.cpp copied verbatim; OnActionUse runs the base use hook, applies the potion's
// bonus-item sub-def to the user's hero stats (interface 4) when both exist, kills the potion thing, releases the sub-def.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCarriedActionUsePotion::OnActionUse(CThing* user) -- drink the carried potion: run the base use hook, apply the potion's bonus-item def to the user's hero stats (when it has them), then kill the potion thing
// Leaf: vfunc_28
// (catalogued slot +0x28; it is OnActionUse: the qualified base call is CTCActionUseBase::OnActionUse 0x007EAB90,
// as in the landed CTCCarriedActionUseAbility::OnActionUse 0x007F4D50)
//
// Retail 0x007EDD60, 133 bytes (`ret 4`). Vtable 0x0123FDE4 (CTCCarriedActionUsePotion) slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4` in another TU), CDefClassBase::GetSubDef
// <CBonusItemDef>(CDefPointer<const CBonusItemDef>*) @ 0x007E1D30 (landed), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef*)
// @ 0x0057C4F1, CThing::Kill(bool) @ 0x004C9B80, the def's virtual slot 1 (release at zero).
// Layout: CThing PDef_Object +0x70 (the parent thing's def), the interface bitset +0x20 and map +0x44.
// Shape: GetSubDef has a same-TU out-slot-only stand-in body, so the CDefPointer local (starting null) is homed in the
// dead `user` argument slot (user rides in esi) and that slot is reused for the interface key; the def class is read into
// a local first (`mov eax,[edi+4] ; mov ecx,[eax+70h]`); the hero stats come from the out-parameter GetInterface
// (bitset test, LowerBound, end/hit choice, `*out = component`: the component goes through eax into ecx); the pointer's
// release has a null test and no null store (the object is in ebx).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

template <class T>
struct CDefPointer {                            // GetSubDef's out pointer (released by hand below)
    CDefPointeeBase* Object;                    // +0x00
};

struct CBonusItemDef;

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CBonusItemDef>* out);  // 0x007E1D30 GetSubDef<CBonusItemDef>
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CBonusItemDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CDefPointeeBase*)def;
    return true;
}
#pragma auto_inline(on)

struct CTCHeroStats {
    void ApplyBonusItemEffects(const CBonusItemDef* def);   // 0x0057C4F1
};

struct CThing {
    unsigned char    _pad0[0x20];
    unsigned long    m_presentBits[9];                      // +0x20 interface-present bitset
    CInterfaceMap    m_interfaces;                          // +0x44
    unsigned char    _pad4c[0x70 - 0x4c];
    CDefPointeeBase* PDef_Object;                           // +0x70 CDefPointer<const CThingDef>::Object
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    void Kill(bool removeNow);                              // 0x004C9B80
};

struct CTCCarriedActionUseBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThing* user);                           // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUsePotion : CTCCarriedActionUseBase {
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUsePotion::vfunc_28(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CDefPointer<const CBonusItemDef> out;
    out.Object = 0;
    CDefClassBase* defClass = (CDefClassBase*)ParentThing->PDef_Object;
    bool found = defClass->GetSubDef(&out);
    CDefPointeeBase* bonus = out.Object;
    if (found) {
        CTCHeroStats* stats;
        if (user->GetInterface(&stats, 4))
            stats->ApplyBonusItemEffects((const CBonusItemDef*)bonus);
    }
    ParentThing->Kill(true);
    if (bonus && --bonus->m_refCount == 0)
        bonus->Delete();
}

// ---- harness ----
static char g_log[16]; static int g_n = 0;
static void L(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }
static int g_deleted = 0; static void* g_sub = 0; static const void* g_owner = 0;
static CThing* g_baseUser = 0; static const void* g_applyThis = 0; static const CBonusItemDef* g_applyDef = 0;
static const void* g_killThis = 0; static bool g_killNow = false;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CDefPointeeBase::Destroy() { }
void CDefPointeeBase::Delete() { ++g_deleted; L('D'); }
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_owner = owner; if (g_sub) ++((CDefPointeeBase*)g_sub)->m_refCount; return g_sub; }
void CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef* def) { g_applyThis = this; g_applyDef = def; L('A'); }
void CThing::Kill(bool removeNow) { g_killThis = this; g_killNow = removeNow; L('K'); }
void CTCCarriedActionUseBase::v00() { }
void CTCCarriedActionUseBase::OnUseBase(CThing* user) { g_baseUser = user; L('B'); }
static void reset() { g_n = 0; g_log[0] = 0; g_deleted = 0; g_owner = 0; g_baseUser = 0; g_applyThis = 0; g_applyDef = 0;
  g_killThis = 0; g_killNow = false; }
int main()
{
    CDefClassBase defClass; defClass.m_refCount = 1;
    CDefPointeeBase bonus; bonus.m_refCount = 1;
    CTCHeroStats stats3, stats4, statsEnd;
    CInterfaceMap::SEntry entries[3] = { { 3, (CTCBase*)&stats3 }, { 4, (CTCBase*)&stats4 }, { 99, (CTCBase*)&statsEnd } };
    CThing potion; memset(&potion, 0, sizeof potion); potion.PDef_Object = &defClass;
    CThing user; memset(&user, 0, sizeof user);
    user.m_presentBits[0] = (1UL << 3) | (1UL << 4); user.m_interfaces.first = entries; user.m_interfaces.last = entries + 2;
    CTCCarriedActionUsePotion tc; tc.ParentThing = &potion;

    reset(); g_sub = &bonus;
    tc.vfunc_28(&user);
    CHECK(strcmp(g_log, "BAK") == 0 && g_baseUser == &user && g_owner == &defClass);
    CHECK(g_applyThis == &stats4 && g_applyDef == (const CBonusItemDef*)&bonus);
    CHECK(g_killThis == &potion && g_killNow && bonus.m_refCount == 1 && g_deleted == 0);

    // the user has no hero stats: no apply
    reset(); user.m_presentBits[0] = 1UL << 3;
    tc.vfunc_28(&user);
    CHECK(strcmp(g_log, "BK") == 0 && bonus.m_refCount == 1);
    user.m_presentBits[0] = (1UL << 3) | (1UL << 4);

    // no bonus sub-def: no apply, nothing released
    reset(); g_sub = 0;
    tc.vfunc_28(&user);
    CHECK(strcmp(g_log, "BK") == 0 && g_deleted == 0);

    // last reference: deleted after the kill
    reset(); g_sub = &bonus; bonus.m_refCount = 0;
    tc.vfunc_28(&user);
    CHECK(strcmp(g_log, "BAKD") == 0 && bonus.m_refCount == 0 && g_deleted == 1);

    if (fails == 0) printf("007edd60_TEST PASS\n");
    return fails;
}