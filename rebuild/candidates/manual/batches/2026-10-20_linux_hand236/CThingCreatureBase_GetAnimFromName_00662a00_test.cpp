#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
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
    CAnimationEntry* GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter,
                                        long exclude) const;                                                     // 0x005DCC00
};
struct CCreatureModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CAnimFilterBase& GetAnimFilter();      // slot 4 (+0x10)
};
struct CTCCreatureModeManager {
    CCreatureModeBase* GetCurrentMode();        // 0x0073A6E0
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
    unsigned char  _pad74[0x134 - 0x74];
    CTCCreatureModeManager* PTCModeManager;     // +0x134
    unsigned long* GetRandomSeed();             // 0x004C7AE0
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);
};

CAnimationEntry* CThingCreatureBase::GetAnimFromName(const CCharString& name, long index)
{
    const CAnimFilterBase& filter = PTCModeManager->GetCurrentMode()->GetAnimFilter();
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(out);
    CHeldDef<CAppearanceDef> def(out.m_pObject);
    const CAnimationSet& animations = def->Animation;
    if (animations.HasAnimation(name, filter))
        return animations.GetAnimation(name, index, filter);
    if (animations.HasAnimation(name, CAnimFilterNoGroup()))
        return animations.GetRandomAnimation(name, GetRandomSeed(), CAnimFilterNoGroup(), -1);
    return animations.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
}
// ---- test doubles ----
void CCreatureModeBase::v00() {} void CCreatureModeBase::v01() {} void CCreatureModeBase::v02() {} void CCreatureModeBase::v03() {}
const CAnimFilterBase& CCreatureModeBase::GetAnimFilter() { return *(CAnimFilterBase*)0; }
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() {}
static CAnimFilterBase g_modeFilter;
static int g_strings = 0; static const char* g_lastName = 0;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_strings; }
CCharString::~CCharString() { --g_strings; }
static CAnimationEntry g_entries[4];
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }

static bool g_hasMode, g_hasAny; static long g_lastIndex, g_lastExclude; static unsigned long* g_lastSeed;
static const CAnimFilterBase* g_lastFilter;
bool CAnimationSet::HasAnimation(const CCharString&, const CAnimFilterBase& f) const { return &f == &g_modeFilter ? g_hasMode : g_hasAny; }
CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& f) const
{
    g_lastName = (const char*)name.PStringData; g_lastIndex = index; g_lastFilter = &f; return &g_entries[&f == &g_modeFilter ? 0 : 1];
}
CAnimationEntry* CAnimationSet::GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& f, long exclude) const
{
    g_lastName = (const char*)name.PStringData; g_lastSeed = seed; g_lastExclude = exclude; g_lastFilter = &f; return &g_entries[2];
}
struct CTestMode : CCreatureModeBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual const CAnimFilterBase& GetAnimFilter() { return g_modeFilter; }
};
static CTestMode g_mode;
CCreatureModeBase* CTCCreatureModeManager::GetCurrentMode() { return &g_mode; }
static unsigned long g_seed;
unsigned long* CThingCreatureBase::GetRandomSeed() { return &g_seed; }
struct CTestAppearance : CAppearanceDef { int destroyed; virtual void v000() {} virtual void DestroyDef() { ++destroyed; } };
static CTestAppearance* g_def;
void* FindDef_CDefClassBase_GetSubDef_0() { ++g_def->m_refCount; return g_def; }
int main()
{
    CTestAppearance def; def.m_refCount = 1; def.destroyed = 0; g_def = &def;
    CDefClassBase base; CTCCreatureModeManager modes;
    CThingCreatureBase creature; creature.PBaseDef = &base; creature.PTCModeManager = &modes;
    CCharString name("RUN");
    bool ok = true;
    g_hasMode = true; g_hasAny = true;
    ok = ok && creature.GetAnimFromName(name, 4) == &g_entries[0] && g_lastIndex == 4 && g_lastFilter == &g_modeFilter;
    g_hasMode = false;
    ok = ok && creature.GetAnimFromName(name, 4) == &g_entries[2] && g_lastSeed == &g_seed && g_lastExclude == -1
        && std::strcmp(g_lastName, "RUN") == 0;
    g_hasAny = false;
    ok = ok && creature.GetAnimFromName(name, 4) == &g_entries[1] && g_lastIndex == 0 && std::strcmp(g_lastName, "DEFAULT") == 0
        && g_strings == 1 && def.m_refCount == 1 && def.destroyed == 0;
    def.m_refCount = 0;
    creature.GetAnimFromName(name, 1);
    ok = ok && def.destroyed == 1;
    if (!ok) { std::printf("00662a00_TEST FAIL\n"); return 1; }
    std::printf("00662a00_TEST PASS\n");
    return 0;
}
