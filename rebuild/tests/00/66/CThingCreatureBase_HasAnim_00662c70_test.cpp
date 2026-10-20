// Behaviour test for 00662c70: src/00662c70.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const -- whether the animation set of the creature's appearance definition holds an animation of that name passing the filter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662C70, length 72 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>&) @ 0x0042B0A2 (landed),
// CAnimationSet::HasAnim(const CCharString&, const CAnimFilterBase&) const @ 0x005DC2E0 (manifest "CAnimationDef_FindAnimationByName"),
// the def's virtual slot 1 (+4) when the temporary drops the last reference.
// Layout (retail, local model): the thing's base definition PBaseDef at +0x70 (as in the landed CTCLook::OnCreate
// 0x00753040); the appearance definition's CAnimationSet at +0x34; a counted definition holds its count at +4.
// Shape: Ego_r.pdb `?HasAnim@CThingCreatureBase@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z`. The def pointer is
// zero-initialised in the `push ecx` slot and filled by GetSubDef; retail loads it ONCE into esi before the query
// (`lea ecx,[esi+34h]`) and releases through that register (`dec [esi+4] ; jne`), the bool result riding in bl. A
// CDefPointer destructor re-reads the address-taken slot after the call in the lane (and forms `&m_refCount`), so the
// release is written by hand on a named pointer local (same behaviour as the original scope-exit release).
struct CCharString;
struct CAnimFilterBase;

struct CDefPointeeBase {                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};

struct CAnimationSet {
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
};

struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x2c];
    CAnimationSet AnimationSet;                 // +0x34
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);       // 0x0042B0A2
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
};

struct CThingCreatureBase : CThing {
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;
};

bool CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const
{
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    CAppearanceDef* appearance = def.m_pObject;
    bool found = appearance->AnimationSet.HasAnim(name, filter);
    if (appearance)
        appearance->ReleaseDef();
    return found;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static const void* g_name = 0; static const void* g_filter = 0; static bool g_has = false;
static const CAnimationSet* g_set = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() { ++g_destroyed; }
bool CAnimationSet::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const { g_set = this; g_name = &name; g_filter = &filter; return g_has; }
int main()
{
    CAppearanceDef def; def.m_refCount = 2; g_def = &def;
    CDefClassBase base;
    CThingCreatureBase c; memset(&c, 0, sizeof(c)); c.PBaseDef = &base;
    int nameStorage = 0, filterStorage = 0;
    const CCharString& name = *(const CCharString*)&nameStorage; const CAnimFilterBase& filter = *(const CAnimFilterBase*)&filterStorage;
    g_has = true; CHECK(c.HasAnim(name, filter)); CHECK(g_set == &def.AnimationSet); CHECK(g_name == &nameStorage); CHECK(g_filter == &filterStorage);
    CHECK(def.m_refCount == 1); CHECK(g_destroyed == 0);
    g_has = false; CHECK(!c.HasAnim(name, filter)); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00662c70_TEST PASS\n"); else printf("00662c70_TEST FAIL\n");
    return fails;
}