// Behaviour test for 00662ef0: src/00662ef0.cpp copied verbatim; a matching animation is picked at random with the
// caller's filter and the creature's random seed, else "DEFAULT" index 0 with a no-group filter; the def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter) -- a random animation called `name` that passes the filter from the creature's appearance animation set, else the set's first "DEFAULT" animation of any group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662EF0, length 167 bytes, `ret 8`.
// Callees: CThingBaseDef::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (byte-twin label GetSubDef<CAbilityDef>; the spelling the
//   landed CTCAnimationComplex::OnCreate 0x0070B4D0 uses), the animation set's "has a matching animation" test @ 0x005DC2E0
//   (walks the name's run calling filter.Accept), its random pick @ 0x005DCC00 (name, random seed, filter, -1),
//   CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) @ 0x005DC340, the game object's
//   random source @ 0x004C7AE0 (`[[0x013B8A1C]+0x2C]`; manifest "CViewBase::GetDisplayEngine" is a byte twin),
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the def's virtual Destroy (slot 1).
// Identity: engine_api.tsv gives CThingCreatureBase::GetRandomAnimFromNameWithFilter(CCharString*, CAnimFilterBase*)
//   returning CAnimationEntry*; the fallback filter is CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the
//   CAnimFilterBase store 0x0122FFCC.
// Layout: retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the sub-def out pointer is the zeroed `push ecx` frame word; the `-1` push is shared by the random pick's last
//   argument and the "DEFAULT" string's length; the fallback string and filter temporaries land in the dead argument
//   slots; the appearance def is copied out of the holder into a scoped CHeldDef released at each return
//   (`dec [esi+4] ; jne`, def in esi, &Animation in edi, `name` re-read from its slot).
// GetSubDef gets a same-TU `auto_inline(off)` stand-in body that writes only the out slot (VC7.1 then knows the
//   holder does not escape); with the held copy as a real local the def outranks `name` for a register, as retail.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CAnimationEntry;
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};

struct CAnimationSet {
    bool HasAnimation_005dc2e0(const CCharString& name, const CAnimFilterBase& filter) const;     // 0x005DC2E0
    const CAnimationEntry* GetRandomAnimation_005dcc00(const CCharString& name, unsigned long* randomSeed,
        const CAnimFilterBase& filter, long index) const;                                       // 0x005DCC00
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CHeldDef {            // the def copied out of the escaped out pointer, released at scope end
    T* Object;
    CHeldDef(T* object) : Object(object) { }
    ~CHeldDef() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindSubDef_CThingBaseDef_GetSubDef(const CThingBaseDef* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindSubDef_CThingBaseDef_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CAppearanceDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x70];
    CThingBaseDef* PBaseDef;                    // +0x70 CDefPointer<CThingBaseDef const>::Object
    unsigned long* GetRandomSeed_004c7ae0();    // 0x004C7AE0 (the game random seed)
};

struct CThingCreatureBase : CThing {
    const CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter);
};

const CAnimationEntry* CThingCreatureBase::GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter)
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(&out);
    CHeldDef<CAppearanceDef> def(out.Object);
    CAnimationSet& animations = def.Object->Animation;
    if (animations.HasAnimation_005dc2e0(name, filter))
        return animations.GetRandomAnimation_005dcc00(name, GetRandomSeed_004c7ae0(), filter, -1);
    return animations.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
}

// ---- harness ----
static int g_strings = 0, g_stringDtors = 0, g_destroyed = 0, g_has = 0, g_random = 0, g_get = 0;
static char g_lastGetName[32];
static long g_lastGetIndex = 99, g_lastRandomIndex = 99;
static const CAnimFilterBase* g_lastFilter;
static const CCharString* g_lastName;
static unsigned long* g_lastSeed;
static bool g_hasResult = true;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_strings; }
CCharString::~CCharString() { ++g_stringDtors; }
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return false; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static const CAnimationEntry* const kRandom = (const CAnimationEntry*)0x1000;
static const CAnimationEntry* const kDefault = (const CAnimationEntry*)0x2000;
static CAppearanceDef g_app;
bool CAnimationSet::HasAnimation_005dc2e0(const CCharString& name, const CAnimFilterBase& filter) const
{ ++g_has; g_lastName = &name; g_lastFilter = &filter; CHECK(this == &g_app.Animation); return g_hasResult; }
const CAnimationEntry* CAnimationSet::GetRandomAnimation_005dcc00(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter, long index) const
{ ++g_random; CHECK(&name == g_lastName && &filter == g_lastFilter && this == &g_app.Animation); g_lastSeed = seed; g_lastRandomIndex = index; return kRandom; }
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{ ++g_get; strcpy(g_lastGetName, (const char*)name.PStringData); g_lastGetIndex = index; CHECK(filter.Accept(0)); CHECK(this == &g_app.Animation); return kDefault; }
void CDefPointeeBase::Destroy0() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void* FindSubDef_CThingBaseDef_GetSubDef(const CThingBaseDef*) { ++g_app.RefCount; return &g_app; }
static unsigned long g_seed;
unsigned long* CThing::GetRandomSeed_004c7ae0() { return &g_seed; }
int main()
{
    CThingBaseDef base;
    CThingCreatureBase c;
    c.PBaseDef = &base;
    g_app.RefCount = 1;
    CCharString name("WALK", -1);
    CAnimFilterBase filter;
    CHECK(c.GetRandomAnimFromNameWithFilter(name, filter) == kRandom);
    CHECK(g_has == 1 && g_random == 1 && g_get == 0 && g_lastName == &name && g_lastFilter == &filter);
    CHECK(g_lastSeed == &g_seed && g_lastRandomIndex == -1 && g_app.RefCount == 1 && g_destroyed == 0);
    g_hasResult = false;
    CHECK(c.GetRandomAnimFromNameWithFilter(name, filter) == kDefault);
    CHECK(g_has == 2 && g_random == 1 && g_get == 1 && strcmp(g_lastGetName, "DEFAULT") == 0 && g_lastGetIndex == 0);
    CHECK(g_strings == 2 && g_stringDtors == 1 && g_app.RefCount == 1 && g_destroyed == 0);
    g_app.RefCount = 0;                                 // the holder owns the last reference
    CHECK(c.GetRandomAnimFromNameWithFilter(name, filter) == kDefault && g_destroyed == 1);
    g_hasResult = true; g_app.RefCount = 0;
    CHECK(c.GetRandomAnimFromNameWithFilter(name, filter) == kRandom && g_destroyed == 2);
    if (fails == 0) printf("00662ef0_TEST PASS\n");
    return fails;
}