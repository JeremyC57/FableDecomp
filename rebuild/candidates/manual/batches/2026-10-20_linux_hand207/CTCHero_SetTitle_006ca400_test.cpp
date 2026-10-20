#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
class CThing;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CRefCounted {                                // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                         // slot 1
};
template<class T> class CDefPointerT;
struct CDefBase : CDefVirtual, CRefCounted {        // CDefClassBase
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    template<class T> bool GetSubDef(CDefPointerT<const T>* out) const;   // 0x005B1E5A for CHeroTitleDef
};
struct CHeroTitleDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long Title;                                     // +0x28 EHeroTitle
};

template<class T> class CDefPointerT {              // CDefPointer<const T> member
public:
    T* Object;
    T* operator->() const { return Object; }
    CDefPointerT& operator=(int)
    {
        if (Object) {
            ((CDefBase*)Object)->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

struct CDefPointer {                                // CDefPointer<CDefClassBase>: the manager's return type
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};

struct CDefinitionManager {
    CDefPointer GetPDefFromGlobalIndex(long index); // 0x009AD9E0 (hidden result pointer)
};
struct CDefinitionHolder : CDefinitionManager { };

struct CTCOpinionSource;
struct COpinionReactionManager {
    bool AttachOpinionSource(CThing* thing, const CCharString& name, CTCOpinionSource** ppSource);   // 0x00700780
};
struct CWorld {
    unsigned char _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;   // +0x74
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                        // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

class CTCHero {
public:
    void* __vfptr;                                  // +0x00
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    long Title;                                     // +0x10 EHeroTitle
    long HeroTitleObjectDef;                        // +0x14
    CDefPointerT<const CHeroTitleDef> PHeroTitleSubDef;   // +0x18
    CDefinitionHolder* GetDefinitions() const;      // 0x006869D0 (CTCBase)
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void SetTitle(long titleObjectDef);
};

void CTCHero::SetTitle(long titleObjectDef)
{
    CDefPointer def = GetDefinitions()->GetPDefFromGlobalIndex(titleObjectDef);
    if (def.m_p && def.m_p->GetSubDef(&PHeroTitleSubDef)) {
        Title = PHeroTitleSubDef->Title;
        HeroTitleObjectDef = titleObjectDef;
        if (Title != 0) {
            if (ParentThing->ThingType != 2 && !ParentThing->HasTC(0x50)) {
                CTCOpinionSource* source;
                CCharString name("OPINION_SOURCE_RIVAL_HERO_TITLE", -1);
                CThing* thing = ParentThing;
                CWorld* world = GetWorld();
                world->OpinionReactionManager->AttachOpinionSource(thing, name, &source);
            }
        }
    } else {
        Title = 0;
        PHeroTitleSubDef = 0;
        HeroTitleObjectDef = 0;
    }
}

// ---- test stubs for the retail callees ----
static int g_destroyed, g_attached; static const char* g_attachName; static CThing* g_attachThing;
static CDefBase* g_lookup; static CHeroTitleDef* g_titleDef;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroyed; }
template<class T> bool CDefBase::GetSubDef(CDefPointerT<const T>* out) const
{ if (!g_titleDef) return false; ++g_titleDef->m_count; out->Object = g_titleDef; return true; }
CDefPointer CDefinitionManager::GetPDefFromGlobalIndex(long index)
{ CDefPointer p; p.m_p = index == 42 ? g_lookup : 0; if (p.m_p) ++p.m_p->m_count; return p; }
bool COpinionReactionManager::AttachOpinionSource(CThing* t, const CCharString& n, CTCOpinionSource**)
{ ++g_attached; g_attachThing = t; g_attachName = (const char*)n.PStringData; return true; }
static CDefinitionHolder g_defs; static COpinionReactionManager g_orm; static CWorld g_world;
CDefinitionHolder* CTCHero::GetDefinitions() const { return &g_defs; }
CWorld* CTCHero::GetWorld() { return &g_world; }

int main()
{
    int ok = 1;
    g_world.OpinionReactionManager = &g_orm;
    CDefBase objectDef; objectDef.m_count = 100; g_lookup = &objectDef;   // VC7.1 has no NRVO: the return copy skews this count
    CHeroTitleDef titleDef; titleDef.m_count = 1; titleDef.Title = 5; g_titleDef = &titleDef;
    CThing owner; std::memset(&owner, 0, sizeof(owner)); owner.ThingType = 1;
    CTCHero hero; std::memset(&hero, 0, sizeof(hero)); hero.ParentThing = &owner;
    hero.SetTitle(42);
    if (hero.Title != 5 || hero.HeroTitleObjectDef != 42 || hero.PHeroTitleSubDef.Object != &titleDef) ok = 0;
    if (g_attached != 1 || g_attachThing != &owner || g_destroyed != 0) ok = 0;
    if (std::strcmp(g_attachName, "OPINION_SOURCE_RIVAL_HERO_TITLE") != 0) ok = 0;
    owner.ThingType = 2;
    hero.SetTitle(42);
    if (g_attached != 1) ok = 0;
    hero.SetTitle(7);                   // unknown def: everything cleared, the held sub-def released
    if (hero.Title != 0 || hero.HeroTitleObjectDef != 0 || hero.PHeroTitleSubDef.Object != 0) ok = 0;
    if (titleDef.m_count != 2) ok = 0;   // 1 + 2 GetSubDef add-refs - 1 release
    if (!ok) { std::printf("006ca400_TEST FAIL\n"); return 1; }
    std::printf("006ca400_TEST PASS\n");
    return 0;
}
