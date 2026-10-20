#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#endif
struct CAnimationEntry { int id; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

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
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;                       // 0x005DC2E0
    CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
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
template <class T> struct CHeldDef {            // the def copied out of the escaped out pointer, released at scope end
    T* Object;
    CHeldDef(T* object) : Object(object) { }
    ~CHeldDef() { if (Object) Object->ReleaseDef(); }
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                    // +0x34
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);       // 0x0042B0A2
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}

struct CThingCreatureBase {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index);
};

CAnimationEntry* CThingCreatureBase::GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index)
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(out);
    CHeldDef<CAppearanceDef> def(out.m_pObject);
    const CAnimationSet& animations = def->Animation;
    if (animations.HasAnimation(name, filter))
        return animations.GetAnimation(name, index, filter);
    if (animations.HasAnimation(name, CAnimFilterNoGroup()))
        return animations.GetAnimation(name, index, CAnimFilterNoGroup());
    return animations.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
}
// ---- test doubles ----
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() {}
static const char* g_lastName = 0; static int g_strings = 0;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_strings; }
CCharString::~CCharString() { --g_strings; }

static CAnimationEntry g_entries[8];
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static bool g_hasWithFilter, g_hasAny; static const CAnimFilterBase* g_lastFilter; static long g_lastIndex;
static const CAnimFilterBase* g_user[2];
static bool IsInternal(const CAnimFilterBase* f) { return f != g_user[0] && f != g_user[1]; }
bool CAnimationSet::HasAnimation(const CCharString&, const CAnimFilterBase& f) const
{
    return IsInternal(&f) ? g_hasAny : g_hasWithFilter;
}
CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& f) const
{
    g_lastName = (const char*)name.PStringData; g_lastIndex = index; g_lastFilter = &f;
    return &g_entries[IsInternal(&f) ? 1 : 0];
}
struct CTestAppearance : CAppearanceDef { int destroyed; virtual void v000() {} virtual void DestroyDef() { ++destroyed; } };
static CTestAppearance* g_def;
void* FindDef_CDefClassBase_GetSubDef_0() { ++g_def->m_refCount; return g_def; }
int main()
{
    CTestAppearance def; def.m_refCount = 1; def.destroyed = 0; g_def = &def;
    CDefClassBase base; CThingCreatureBase creature; creature.PBaseDef = &base;
    CCharString name("WALK"); CAnimFilterNoGroup custom;
    struct CPlain : CAnimFilterBase { } plain;
    g_user[0] = &custom; g_user[1] = &plain;
    bool ok = true;
    g_hasWithFilter = true; g_hasAny = true;
    ok = ok && creature.GetAnimFromName(name, custom, 3) == &g_entries[0] && g_lastIndex == 3 && g_lastFilter == &custom
        && def.m_refCount == 1;
    ok = ok && creature.GetAnimFromName(name, plain, 2) == &g_entries[0] && g_lastFilter == &plain && g_lastIndex == 2;
    g_hasWithFilter = false; g_hasAny = true;
    ok = ok && creature.GetAnimFromName(name, plain, 5) == &g_entries[1] && g_lastIndex == 5
        && std::strcmp(g_lastName, "WALK") == 0;
    g_hasWithFilter = false; g_hasAny = false;
    ok = ok && creature.GetAnimFromName(name, plain, 5) == &g_entries[1] && g_lastIndex == 0
        && std::strcmp(g_lastName, "DEFAULT") == 0 && g_strings == 1 && def.m_refCount == 1 && def.destroyed == 0;
    def.m_refCount = 0;                       // the sub-def reference is the last one: released to zero
    creature.GetAnimFromName(name, plain, 1);
    ok = ok && def.destroyed == 1;
    if (!ok) { std::printf("00662b10_TEST FAIL\n"); return 1; }
    std::printf("00662b10_TEST PASS\n");
    return 0;
}
