// Behaviour test for 00663260: src/00663260.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCreatureBase::GetCombatAnimationSet() const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::GetCombatAnimationSet() const -- the animation set of the creature's appearance definition (fetched into a temporary counted def pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00663260, length 49 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>&) @ 0x0042B0A2 (landed), the def's
// virtual slot 1 (+4) when the temporary drops the last reference.
// Layout (retail, local model): the thing's base definition PBaseDef at +0x70 (as in the landed CTCLook::OnCreate
// 0x00753040); the appearance definition's CAnimationSet at +0x34; a counted definition holds its count at +4.
// engine/CThingCreatureBase.h is the PDB layout of the creature itself; only +0x70 is read, through a local base model.
// Shape: the release is an inline member of the counted object (`dec [ecx+4] ; jne`, the object kept in ecx for the
// virtual call; a release written in the pointer re-reads the frame slot or splits the decrement); the CDefPointer temporary is zero-initialised in the `push ecx` slot, the member address is computed before the
// inline release (`lea esi,[ecx+34h]` above the null test) and returned after it (Ego_r.pdb
// `?GetCombatAnimationSet@CThingCreatureBase@@QBEABVCAnimationSet@@XZ`: a const reference).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefVirtual {                            // the definition's vptr
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
};

struct CRefCounted {                            // second base: the count sits at +4 behind the vptr
    long m_refCount;
};

struct CDefPointeeBase : CDefVirtual, CRefCounted {
    void ReleaseDef()                           // the release lives on the object: `dec [ecx+4] ; jne` with ecx kept
    {
        if (DecRefToZero(m_refCount))
            DestroyDef();
    }
};

template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T* operator->() const { return m_pObject; }
};

struct CAnimationSet {
    unsigned char _pad[0x10];
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
    const CAnimationSet& GetCombatAnimationSet() const;
};

const CAnimationSet& CThingCreatureBase::GetCombatAnimationSet() const
{
    CDefPointer<CAppearanceDef> appearance;
    PBaseDef->GetSubDef(appearance);
    return appearance->AnimationSet;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_find = 0, g_destroyed = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { ++g_find; return g_def; }
void CDefVirtual::v000() {}
void CDefVirtual::DestroyDef() { ++g_destroyed; }
int main()
{
    CAppearanceDef def; def.m_refCount = 2;
    CDefClassBase base;
    CThingCreatureBase c; memset(&c, 0, sizeof(c)); c.PBaseDef = &base;
    g_def = &def;
    const CAnimationSet& set = c.GetCombatAnimationSet();
    CHECK(&set == &def.AnimationSet); CHECK(def.m_refCount == 1); CHECK(g_destroyed == 0); CHECK(g_find == 1);
    def.m_refCount = 1;
    const CAnimationSet& set2 = c.GetCombatAnimationSet();
    CHECK(&set2 == &def.AnimationSet); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00663260_TEST PASS\n"); else printf("00663260_TEST FAIL\n");
    return fails;
}