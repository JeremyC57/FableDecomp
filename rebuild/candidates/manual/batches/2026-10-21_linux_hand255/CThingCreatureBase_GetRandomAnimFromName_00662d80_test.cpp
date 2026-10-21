#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::GetRandomAnimFromName(const CCharString& name, long exclude) const -- a random appearance animation called `name` (not the excluded one) passing the current creature mode's filter, else a random one of any group, else the first "DEFAULT"
// Leaf: ?GetRandomAnimFromName@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662D80, length 356 bytes (+12 int3), `ret 8`.
//   Signature from Ego_r.pdb `?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z`.
// Callees: as the landed sibling GetAnimFromName 0x00662A00: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2,
//   CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 and the mode's virtual slot 4 (+0x10, its animation filter), the
//   animation set's "has a matching animation" test @ 0x005DC2E0, the random pick @ 0x005DCC00 (name, seed, filter,
//   exclude), CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) @ 0x005DC340, the game random
//   seed getter CThing::GetRandomSeed @ 0x004C7AE0, CCharString(const char*, long) @ 0x0099EBF0 ("DEFAULT" @ 0x01230554),
//   ~CCharString @ 0x0099EAE0, the def's virtual slot 1 (+4) at a zero count.
// Identity: CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout: retail CThing PBaseDef +0x70; retail CThingCreatureBase PTCModeManager +0x134 (PDB +0x158); CAppearanceDef
//   Animation +0x34; a counted def holds its count at +4.
// Shape: the sub-def is fetched before the mode's filter (the reverse of the sibling); the "DEFAULT" fallback first runs
//   a has-animation test whose result is discarded (retail calls 0x005DC2E0 and ignores al), then the lookup.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
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

struct CThingCreatureBase {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
    unsigned char  _pad74[0x134 - 0x74];
    CTCCreatureModeManager* PTCModeManager;     // +0x134
    unsigned long* GetRandomSeed() const;       // 0x004C7AE0
    const CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long exclude) const;
};

const CAnimationEntry& CThingCreatureBase::GetRandomAnimFromName(const CCharString& name, long exclude) const
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(out);
    const CAnimFilterBase& filter = PTCModeManager->GetCurrentMode()->GetAnimFilter();
    CHeldDef<CAppearanceDef> def(out.m_pObject);
    const CAnimationSet& animations = def->Animation;
    if (animations.HasAnimation(name, filter))
        return *animations.GetRandomAnimation(name, GetRandomSeed(), filter, exclude);
    if (animations.HasAnimation(name, CAnimFilterNoGroup()))
        return *animations.GetRandomAnimation(name, GetRandomSeed(), CAnimFilterNoGroup(), exclude);
    animations.HasAnimation("DEFAULT", CAnimFilterNoGroup());
    return *animations.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
}


struct CAnimationEntry { int id; };
static CAnimationEntry g_entries[4] = { {0}, {1}, {2}, {3} };
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; }
CCharString::~CCharString() {}
bool CAnimFilterBase::Accept(const CAnimationEntry* entry) const { return false; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry* entry) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static const char* g_name = "walk"; static bool g_modeHas = false, g_anyHas = false; static int g_hasCalls = 0, g_defaultHas = 0;
static long g_exclude = 0; static const unsigned long* g_seedSeen = 0; static unsigned long g_seed = 5;
static bool IsName(const CCharString& n) { return std::strcmp((const char*)n.PStringData, g_name) == 0; }
bool CAnimationSet::HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const
{ ++g_hasCalls; if (std::strcmp((const char*)name.PStringData, "DEFAULT") == 0) { ++g_defaultHas; return filter.Accept(0); }
  return IsName(name) && (filter.Accept(0) ? g_anyHas : g_modeHas); }
CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{ return std::strcmp((const char*)name.PStringData, "DEFAULT") == 0 && index == 0 && filter.Accept(0) ? &g_entries[3] : 0; }
CAnimationEntry* CAnimationSet::GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter, long exclude) const
{ g_exclude = exclude; g_seedSeen = seed; return IsName(name) ? &g_entries[filter.Accept(0) ? 2 : 1] : 0; }
static CAnimFilterBase g_modeFilter;
void CCreatureModeBase::v00() {} void CCreatureModeBase::v01() {} void CCreatureModeBase::v02() {} void CCreatureModeBase::v03() {}
const CAnimFilterBase& CCreatureModeBase::GetAnimFilter() { return g_modeFilter; }
static CCreatureModeBase g_mode;
CCreatureModeBase* CTCCreatureModeManager::GetCurrentMode() { return &g_mode; }
static int g_destroyed = 0;
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() { ++g_destroyed; }
static CAppearanceDef g_app;
void* FindDef_CDefClassBase_GetSubDef_0() { ++g_app.m_refCount; return &g_app; }
unsigned long* CThingCreatureBase::GetRandomSeed() const { return &g_seed; }
#define CHECK(c) if (!(c)) { std::printf("00662d80_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static CDefClassBase base; static CTCCreatureModeManager mm; static CThingCreatureBase c;
    c.PBaseDef = &base; c.PTCModeManager = &mm; g_app.m_refCount = 1;
    CCharString walk("walk");
    g_modeHas = true; g_anyHas = true;
    const CAnimationEntry* e = &c.GetRandomAnimFromName(walk, 7);
    CHECK(e->id == 1 && g_exclude == 7 && g_seedSeen == &g_seed && g_app.m_refCount == 1 && g_destroyed == 0);
    g_modeHas = false;
    e = &c.GetRandomAnimFromName(walk, 9);
    CHECK(e->id == 2 && g_exclude == 9 && g_app.m_refCount == 1);
    g_anyHas = false; g_hasCalls = 0;
    e = &c.GetRandomAnimFromName(walk, 3);
    CHECK(e->id == 3 && g_hasCalls == 3 && g_defaultHas == 1 && g_app.m_refCount == 1 && g_destroyed == 0);
    g_app.m_refCount = 0;
    e = &c.GetRandomAnimFromName(walk, 3);
    CHECK(e->id == 3 && g_destroyed == 1);
    std::printf("00662d80_TEST PASS\n");
    return 0;
}
