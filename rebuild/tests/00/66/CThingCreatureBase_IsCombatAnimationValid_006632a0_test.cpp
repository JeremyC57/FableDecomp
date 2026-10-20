// Behaviour test for 006632a0: src/006632a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCreatureBase::IsCombatAnimationValid(long animation)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::IsCombatAnimationValid(long animation) -- does the creature's appearance animation set hold this animation (no group filter)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006632A0, 154 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (twice; byte-twin label GetSubDef<CAbilityDef>),
//   CAnimationSet::HasAnimation(long, const CAnimFilter&) @ 0x005DC2E0 (auto label "CAnimationDef_FindAnimationByName").
// Identity: the frame word 0x0122FFDC is a CAnimFilterNoGroup temporary (RTTI locator at vtable-4).
// Layout: retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34; def ref count +4, virtual Destroy slot 1.
// Shape: the first GetSubDef only tests that the creature has an appearance def; the animation set is then taken from a
//   second CDefPointer in its own block, released (inline dtor) before the set is used; each return arm releases the
//   first def through its own inline dtor copy.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CAnimationEntry;
struct CAnimFilter {
    virtual bool Accept(const CAnimationEntry* entry) const = 0;
    virtual CAnimFilter* Clone() const = 0;
};
struct CAnimFilterNoGroup : CAnimFilter {      // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilter* Clone() const;                         // 0x00429A2F
};
struct CAnimationSet {
    bool HasAnimation(long animation, const CAnimFilter& filter) const;     // 0x005DC2E0
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --((CDefClassBase*)Object)->RefCount == 0)
            ((CDefClassBase*)Object)->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CThingBaseDef {
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;      // 0x0042B0A2
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    bool IsCombatAnimationValid(long animation);
};

bool CThingCreatureBase::IsCombatAnimationValid(long animation)
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        const CAnimationSet* set;
        {
            CDefPointer<const CAppearanceDef> appearance;
            PBaseDef->GetSubDef(&appearance);
            set = &appearance->Animation;
        }
        if (set->HasAnimation(animation, CAnimFilterNoGroup()))
            return true;
    }
    return false;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_find = 0, g_release = 0, g_destroy = 0, g_has = 0; static long g_anim = -1; static bool g_result = false; static const void* g_filterVtbl = 0;
void* FindDef_s7r_0() { ++g_find; return g_def; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefClassBase::v00() {}
void CDefClassBase::Destroy() { ++g_destroy; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilter* CAnimFilterNoGroup::Clone() const { return 0; }
static const CAnimationSet* g_set = 0;
bool CAnimationSet::HasAnimation(long animation, const CAnimFilter& filter) const
{ ++g_has; g_anim = animation; g_set = this; CHECK(filter.Accept(0)); return g_result; }
int main()
{
    static CAppearanceDef def; def.RefCount = 10;
    static CThingBaseDef base; CThingCreatureBase c; c.PBaseDef = &base;
    g_def = 0; CHECK(c.IsCombatAnimationValid(7) == false); CHECK(g_find == 1 && g_has == 0 && g_destroy == 0);
    g_def = &def; g_result = true;
    CHECK(c.IsCombatAnimationValid(7) == true); CHECK(g_find == 3 && g_has == 1 && g_anim == 7 && g_set == &def.Animation);
    CHECK(def.RefCount == 8);   // both pointers release (the stand-in fetch takes no reference)
    g_result = false; CHECK(c.IsCombatAnimationValid(9) == false); CHECK(g_find == 5 && g_has == 2 && g_anim == 9 && def.RefCount == 6);
    def.RefCount = 2; g_result = true; CHECK(c.IsCombatAnimationValid(1) == true); CHECK(def.RefCount == 0 && g_destroy == 1);
    CHECK(g_release == 0);
    if (fails == 0) printf("006632a0_TEST PASS\n"); else printf("006632a0_TEST FAIL\n");
    return fails;
}