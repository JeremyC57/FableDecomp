// CTCLightningOrb::OnDie() -- when the orb dies, spawns its definition's LIGHTNING_ORB_HIT special effect at the orb's position (when the def's special-effects sub-def maps that name)
// Leaf: OnDie
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F2410, length 208 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) @ 0x00669580, CCharString(const char*, long)
// @ 0x0099EBF0 ("LIGHTNING_ORB_HIT" 0x01271D48) and ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) const @ 0x00669D30 (catalog "CCRC::CalcAndLookup"), CThing::GetPosition @ 0x004C73D0, CreateEffectThing(long, const C3DVector*,
// bool) @ 0x006E0880 (`__fastcall`), the defs' virtual slot 1 on release. CThing::PDef +0x70; ParentThing +0x04.
// Shape: the thing's definition taken by value (CThing::GetPDef: the copy's addref and its release at the end of the condition) and
// the `&&` string temporary; the effects sub-def is a zeroed out slot released by hand (null-tested) after the spawn. GetSubDef and
// GetEffectFromString get same-TU stand-in bodies that write only the out slot / the `long&`, so VC7.1 knows neither address
// escapes; that orders the zero stores of the effect-type word and the out slot as in retail.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CDefPointerSlot {      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointerSlot() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointerSlot<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerSlot<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CDefPointer {          // a counted def reference held by value
    T* m_p;
    CDefPointer(const CDefPointer& other) : m_p(other.m_p)
    {
        if (m_p)
            ++m_p->m_refCount;
    }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
    T* operator->() const { return m_p; }
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;             // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
    const C3DVector& GetPosition() const;        // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CTCLightningOrb {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                   // +0x04
    void OnDie();
};
void CTCLightningOrb::OnDie()
{
    CDefPointerSlot<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = ParentThing->GetPDef()->GetSubDef(effectsSlot) && effectsSlot.m_p->GetEffectFromString("LIGHTNING_ORB_HIT", effectType);
    if (found)
        CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
    ReleaseDef(effectsSlot.m_p);
}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strLive = 0, g_destroyed = 0, g_lookups = 0, g_creates = 0;
static long g_effectType = 7, g_lastType = 0;
static bool g_haveEffects = true, g_lastFlag = true;
static const C3DVector* g_lastPos = 0;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_strLive; }
CCharString::~CCharString() { --g_strLive; }
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
static CSpecialEffectsDef g_effects;
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name)
{
    ++g_lookups;
    CHECK(def == &g_effects);
    return strcmp((const char*)name.PStringData, "LIGHTNING_ORB_HIT") == 0 ? g_effectType : -1;
}
void* Find_CDefClassBase_GetSubDef(const void*)
{
    if (!g_haveEffects)
        return 0;
    ++g_effects.m_refCount;
    return &g_effects;
}
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    ++g_creates; g_lastType = type; g_lastPos = position; g_lastFlag = flag;
    return 0;
}
int main()
{
    static CDefClassBase defClass;
    defClass.m_refCount = 1;
    static unsigned char thingMem[sizeof(CThing)];
    CThing* thing = (CThing*)thingMem;
    *(CDefClassBase**)&thing->PDef = &defClass;
    CTCLightningOrb orb;
    orb.ParentThing = thing;
    g_effects.m_refCount = 1;
    orb.OnDie();                                       // sub-def found, name mapped: spawn
    CHECK(g_creates == 1 && g_lastType == 7 && g_lastPos == &g_pos && g_lastFlag == false);
    CHECK(g_lookups == 1 && g_strLive == 0 && defClass.m_refCount == 1 && g_effects.m_refCount == 1 && g_destroyed == 0);
    g_effectType = -1;                                 // name not mapped: no spawn, sub-def still released
    orb.OnDie();
    CHECK(g_creates == 1 && g_lookups == 2 && g_strLive == 0 && g_effects.m_refCount == 1);
    g_haveEffects = false;                             // no sub-def: no lookup, nothing released
    orb.OnDie();
    CHECK(g_creates == 1 && g_lookups == 2 && defClass.m_refCount == 1 && g_destroyed == 0);
    g_haveEffects = true; g_effectType = 3; g_effects.m_refCount = 0;   // the slot owns the last reference
    orb.OnDie();
    CHECK(g_creates == 2 && g_lastType == 3 && g_destroyed == 1 && g_effects.m_refCount == 0);
    if (fails == 0) printf("007f2410_TEST PASS\n");
    return fails;
}