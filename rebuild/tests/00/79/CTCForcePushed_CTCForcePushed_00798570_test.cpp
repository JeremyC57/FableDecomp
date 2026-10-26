#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

template <class T> struct SRetailAllocProxy {       // STLport _STLP_alloc_proxy
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

class CSpecialAbilitiesForcePushDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x98 - 0x08];
    CRetailVector<float> TimeDelayBetweenCastsSec;  // +0x98
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x00798C10 for the force-push def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() {}
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned char _pad20[0x91 - 0x20];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};
class CThingAnimatable : public CThing {
public:
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class CTCForcePushed : public CTCBase {             // vtable 0x0126C09C
public:
    CIntelligentPointer<CThing> PThingCausingForcePush;     // +0x0C
    CIntelligentPointer<CThing> PParticle;          // +0x14
    float PositionForcePushStarted[3];              // +0x1C C3DVector
    float DistanceBeingPushed;                      // +0x28
    float DirectionBeingPushed[3];                  // +0x2C C3DVector
    float DistanceMovingPerFrame;                   // +0x38
    bool Initialised;                               // +0x3C
    bool StartedMoving;                             // +0x3D
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;  // +0x40
    CRetailVector<CThing*> Emitters_created;        // +0x44
    CTCForcePushed(CThing& parent);
    virtual void v00();
};

CTCForcePushed::CTCForcePushed(CThing& parent)
    : CTCBase(&parent), Initialised(false)
{
    if (ParentThing && !(ParentThing->m_flags91 & 1) && ParentThing->ThingClass == 1)
        ((CThingAnimatable*)ParentThing)->SetSuspendAnimation(true, 0);
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_FORCE_PUSH_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
}

// ---- test stubs for the retail callees ----
static const char* g_lastName; static long g_lastClassIndex; static long g_defIndex; static int g_suspends; static bool g_suspendArg;
static CSpecialAbilitiesForcePushDef g_def;
static CGameDefinitionManager g_manager;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& n, long ci) const
{ g_lastName = (const char*)n.PStringData; g_lastClassIndex = ci; return 9; }
template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{ g_defIndex = index; out->Object = &g_def; return true; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
void CThingAnimatable::SetSuspendAnimation(bool s, long r) { ++g_suspends; g_suspendArg = s && r == 0; }
CTCBase::CTCBase(CThing* parent) : ParentThing(parent) {}
void CTCBase::v00() {}
void CSpecialAbilitiesForcePushDef::v00() {}
void CTCForcePushed::v00() {}

int main()
{
    int ok = 1;
    CThing thing; std::memset(&thing, 0, sizeof(thing)); thing.ThingClass = 1;
    CTCForcePushed a(thing);
    if (a.ParentThing != &thing || a.PDef.Object != &g_def || g_defIndex != 9 || g_lastClassIndex != 1) ok = 0;
    if (a.Initialised || a.PThingCausingForcePush.m_pItem || a.PParticle.m_pItem) ok = 0;
    if (a.Emitters_created._M_start || a.Emitters_created._M_finish || a.Emitters_created._M_end_of_storage._M_data) ok = 0;
    if (g_suspends != 1 || !g_suspendArg || std::strcmp(g_lastName, "SPECIAL_ABILITIES_FORCE_PUSH_DEF") != 0) ok = 0;
    thing.ThingClass = 2;   // not an AI creature: no suspend
    CTCForcePushed b(thing);
    thing.ThingClass = 1; thing.m_flags91 = 1;   // dead: no suspend
    CTCForcePushed c(thing);
    if (g_suspends != 1) ok = 0;
    if (!ok) { std::printf("00798570_TEST FAIL\n"); return 1; }
    std::printf("00798570_TEST PASS\n");
    return 0;
}