// 007b2fd0 behaviour test: CTCGuard::WitnessedCrime (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>

class CTCVillage;
class CTCVillageMember;
class CTCHeroOpinionDeedLog;
static CTCVillageMember* g_member;
static CTCHeroOpinionDeedLog* g_log;

class CThing { public:
    bool hasMember, hasLog;
    __forceinline unsigned long HasTC(long type) const { return type == 0x23 ? hasMember : (type == 0x51 ? hasLog : 0); }
    template<class T> __forceinline T* GetTC(long wanted) { return wanted == 0x23 ? (T*)g_member : (T*)g_log; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
static CThing* g_hero;
class CPlayer { public: CThing* GetHeroCreature() { return g_hero; } };
static CPlayer g_player;
class CWorld { public: CPlayer* PeekMyPlayer() const { return &g_player; } };
class CMainGameComponent { public: CWorld* World; };
static CWorld g_world;
static CMainGameComponent g_game = { &g_world };
static CMainGameComponent* g_pMainGameComponent = &g_game;
static long g_frame;
static long GFGetCurrentFrame() { return g_frame; }

static int g_destroys;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
class COpinionDeedEffectsDef : public CRefCounted { public:
    long Severity;
    void Destroy() { ++g_destroys; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(T* o) : Object(o) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
class COpinionDeed { public:
    long Type;
    CBaseIntelligentPointer Perpetrator;
};
static COpinionDeedEffectsDef g_effects;
static long g_effectsType;
class CTCHeroOpinionDeedLog { public:
    long severityByType[8];
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType)
    {
        g_effectsType = deedType; g_effects.m_count = 1; g_effects.Severity = severityByType[deedType];
        return CDefPointer<COpinionDeedEffectsDef>(&g_effects);
    }
};
static long g_handled;
static int g_sweeps; static long g_sweepFlags;
class CTCVillage { public:
    bool IsDeedHandled(long handle) { return handle == g_handled; }
    void RecalculateSecuritySweep(long flags) { ++g_sweeps; g_sweepFlags = flags; }
};
static CTCVillage* g_village;
class CTCVillageMember { public: CTCVillage* GetPVillage() const { return g_village; } };

static int g_reports;
static CTCVillage* g_rVillage; static long g_rHandle, g_rType; static bool g_rSelf;
class CTCGuard { public:
    CThing* ParentThing;
    long    LastCrimeSeenSeverity;
    long    FrameLastCrimeSeen;
    void ReportCrime(CTCVillage* village, long deedHandle, long deedType, bool self)
    {
        ++g_reports; g_rVillage = village; g_rHandle = deedHandle; g_rType = deedType; g_rSelf = self;
    }
    void WitnessedCrime(const COpinionDeed& deed, long deedHandle);
};

void CTCGuard::WitnessedCrime(const COpinionDeed& deed, long deedHandle)
{
    if (!ParentThing->HasTC(0x23))
        return;
    CTCVillage* village = ParentThing->GetTC<CTCVillageMember>(0x23)->GetPVillage();
    if (!village)
        return;
    if (village->IsDeedHandled(deedHandle))
        return;
    CThing* parent = ParentThing;
    bool self = deed.Perpetrator.GetPItem() == parent;
    CThing* hero = g_pMainGameComponent->World->PeekMyPlayer()->GetHeroCreature();
    bool severe = false;
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(deed.Type);
        LastCrimeSeenSeverity = effects->Severity;
        if (LastCrimeSeenSeverity == 2)
            severe = true;
        else if (LastCrimeSeenSeverity == 3)
            severe = true;
        FrameLastCrimeSeen = GFGetCurrentFrame();
    }
    ReportCrime(village, deedHandle, deed.Type, self);
    if (severe)
        village->RecalculateSecuritySweep(0);
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }

int main()
{
    CTCVillageMember member; g_member = &member;
    CTCHeroOpinionDeedLog log; g_log = &log;
    for (int i = 0; i < 8; ++i) log.severityByType[i] = i % 5;   // type 2 -> 2, 3 -> 3, 4 -> 4, 6 -> 1
    CTCVillage village; g_village = &village;
    CThing guardThing; guardThing.hasMember = true; guardThing.hasLog = false;
    CThing heroThing; heroThing.hasMember = false; heroThing.hasLog = true;
    CThing other; other.hasMember = false; other.hasLog = false;
    CTCGuard guard; guard.ParentThing = &guardThing; guard.LastCrimeSeenSeverity = -1; guard.FrameLastCrimeSeen = -1;
    COpinionDeed deed; deed.Type = 2; deed.Perpetrator.m_pItem = &other;
    g_hero = &heroThing; g_frame = 500; g_handled = 99;

#define RESET() (g_reports = 0, g_sweeps = 0, g_destroys = 0, guard.LastCrimeSeenSeverity = -1, guard.FrameLastCrimeSeen = -1)
    RESET(); guardThing.hasMember = false; guard.WitnessedCrime(deed, 1);
    Check("no village member", g_reports == 0 && guard.LastCrimeSeenSeverity == -1);
    guardThing.hasMember = true;
    RESET(); g_village = 0; guard.WitnessedCrime(deed, 1);
    Check("no village", g_reports == 0);
    g_village = &village;
    RESET(); guard.WitnessedCrime(deed, 99);
    Check("already handled", g_reports == 0 && g_sweeps == 0);
    RESET(); deed.Type = 2; guard.WitnessedCrime(deed, 5);
    Check("severity 2", g_reports == 1 && g_rVillage == &village && g_rHandle == 5 && g_rType == 2 && !g_rSelf
        && guard.LastCrimeSeenSeverity == 2 && guard.FrameLastCrimeSeen == 500 && g_sweeps == 1 && g_sweepFlags == 0
        && g_destroys == 1 && g_effectsType == 2);
    RESET(); deed.Type = 3; guard.WitnessedCrime(deed, 5);
    Check("severity 3", g_reports == 1 && guard.LastCrimeSeenSeverity == 3 && g_sweeps == 1);
    RESET(); deed.Type = 4; guard.WitnessedCrime(deed, 5);
    Check("severity 4", g_reports == 1 && guard.LastCrimeSeenSeverity == 4 && g_sweeps == 0 && g_destroys == 1);
    RESET(); deed.Type = 6; guard.WitnessedCrime(deed, 5);
    Check("severity 1", g_reports == 1 && guard.LastCrimeSeenSeverity == 1 && g_sweeps == 0);
    RESET(); deed.Type = 3; g_hero = 0; guard.WitnessedCrime(deed, 5);
    Check("no hero", g_reports == 1 && g_rType == 3 && guard.LastCrimeSeenSeverity == -1 && guard.FrameLastCrimeSeen == -1
        && g_sweeps == 0 && g_destroys == 0);
    g_hero = &heroThing;
    RESET(); deed.Perpetrator.m_pItem = &guardThing; deed.Type = 2; guard.WitnessedCrime(deed, 5);
    Check("self", g_reports == 1 && g_rSelf && g_sweeps == 1);
    if (g_fail) return 1;
    printf("007b2fd0_TEST PASS\n");
    return 0;
}