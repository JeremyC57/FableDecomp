// Behaviour test for 0065c1fc: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillage::RemoveCrimeIfApologisable(long deedHandle) -- unless the hero apologised less than 450 frames ago, drops the crime when its deed is over and its deed type can be apologised for
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};

class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
    static long GetWorldFrame();                // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0x34];
    long KnowledgeLifeInSeconds;                // +0x3c
    unsigned char _pad_0x40[0x3c];
    bool CanBeHeard : 1;                        // +0x7c bit 0
    bool ReactionRestrictToTarget : 1;          // bit 1
    bool AllowSimultaneousReactionDialogue : 1; // bit 2
    bool CanApologise : 1;                      // bit 3
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
class COpinionDeed { public:
    long          DeedType;                     // +0x00 EOpinionDeedType
    unsigned char _pad_0x04[0x1d];
    bool          IsStillHappening;             // +0x21
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

class CTCVillage { public:
    unsigned char _pad_0x00[0xd8];
    long          LastFrameApologised;          // +0xd8
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void RemoveCrime(long deedHandle);          // 0x0065C146
    void RemoveCrimeIfApologisable(long deedHandle);
};

void CTCVillage::RemoveCrimeIfApologisable(long deedHandle)
{
    long* lastApology = &LastFrameApologised;
    if (*lastApology != 0 && GetWorld()->GetWorldFrame() - *lastApology < 450)
        return;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        COpinionDeed* deed = log->GetDeedFromHandle(deedHandle);
        if (deed) {
            CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(deed->DeedType);
            if (!deed->IsStillHappening && effects.Object->CanApologise)
                RemoveCrime(deedHandle);
        }
    }
}

// ---- stubs and driver ----
bool CBitset::test(unsigned long position) const { return position == 0x51; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
static CPlayer g_player; static CWorld g_world; static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;
CThing* CPlayer::GetHeroThing() { return g_heroPtr; }
CPlayer* CWorld::GetCurrentPlayer() const { return &g_player; }
static long g_frame;
long CWorld::GetWorldFrame() { return g_frame; }
CWorld* CTCVillage::GetWorld() { return &g_world; }
void CDefVirtual::v00() {}
static int g_destroyed;
void CDefVirtual::Destroy() { ++g_destroyed; }
static COpinionDeedEffectsDef g_effects;
static COpinionDeed g_deed; static COpinionDeed* g_deedPtr = &g_deed;
COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long handle) { return handle == 5 ? g_deedPtr : 0; }
static CDefPointer<COpinionDeedEffectsDef> g_holder;   // returned by copy (no NRVO in VC7.1)
CDefPointer<COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::GetDeedEffectsDef(long deedType)
{ g_holder.Object = &g_effects; ++g_effects.m_count; return g_holder; }
static int g_removed; static long g_removedHandle;
void CTCVillage::RemoveCrime(long deedHandle) { ++g_removed; g_removedHandle = deedHandle; }
static CTCHeroOpinionDeedLog g_log;

int main()
{
    g_component.World = &g_world;
    g_entries[0].type = 0x51; g_entries[0].component = (CTCBase*)&g_log;
    g_hero.m_interfaces.first = g_entries; g_hero.m_interfaces.last = g_entries + 1;
    g_effects.m_count = 1; g_effects.CanApologise = true;
    static CTCVillage village;
    village.LastFrameApologised = 0;
    village.RemoveCrimeIfApologisable(5);
    if (g_removed != 1 || g_removedHandle != 5 || g_effects.m_count != 1 || g_destroyed) { std::printf("0065c1fc_TEST FAIL (1)\n"); return 1; }
    village.LastFrameApologised = 1000; g_frame = 1200;                       // apologised 200 frames ago
    village.RemoveCrimeIfApologisable(5);
    if (g_removed != 1) { std::printf("0065c1fc_TEST FAIL (recent)\n"); return 1; }
    g_frame = 1450;
    village.RemoveCrimeIfApologisable(5);
    if (g_removed != 2) { std::printf("0065c1fc_TEST FAIL (450)\n"); return 1; }
    g_deed.IsStillHappening = true;
    village.RemoveCrimeIfApologisable(5);
    if (g_removed != 2) { std::printf("0065c1fc_TEST FAIL (happening)\n"); return 1; }
    g_deed.IsStillHappening = false; g_effects.CanApologise = false;
    village.RemoveCrimeIfApologisable(5);
    if (g_removed != 2) { std::printf("0065c1fc_TEST FAIL (cannot apologise)\n"); return 1; }
    g_effects.CanApologise = true;
    village.RemoveCrimeIfApologisable(6);                                       // no such deed
    if (g_removed != 2 || g_effects.m_count != 1) { std::printf("0065c1fc_TEST FAIL (no deed)\n"); return 1; }
    g_holder.Object = 0;
    std::printf("0065c1fc_TEST PASS\n");
    return 0;
}