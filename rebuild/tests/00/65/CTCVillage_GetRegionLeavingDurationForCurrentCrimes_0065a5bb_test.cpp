// Behaviour test for 0065a5bb: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillage::GetRegionLeavingDurationForCurrentCrimes() -- the longest knowledge life, in frames, of the hero's deeds recorded as crimes against this village (0 when there is no hero)
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
};

class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_framesPerSecond;                  // 0x01375550

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
    long DeedType;                              // +0x00 EOpinionDeedType
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

struct SNode {                                  // STLport _Rb_tree_node<long>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    SNode*        PlayersCrimes;                // +0x64 set<long> header node
    long GetRegionLeavingDurationForCurrentCrimes();
};

long CTCVillage::GetRegionLeavingDurationForCurrentCrimes()
{
    long longest = 0;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        if (hero->HasTC(0x51))
            log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
        SNode* end = PlayersCrimes;
        for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
            COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
            if (deed) {
                long duration = log->GetDeedEffectsDef(deed->DeedType).Object->KnowledgeLifeInSeconds * g_framesPerSecond;
                if (duration > longest)
                    longest = duration;
            }
        }
    }
    return longest;
}

// ---- stubs and driver ----
static bool g_hasLog = true;
bool CBitset::test(unsigned long position) const { return position == 0x51 && g_hasLog; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
static CPlayer g_player; static CWorld g_world; static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;
long g_framesPerSecond = 30;
CThing* CPlayer::GetHeroThing() { return g_heroPtr; }
CPlayer* CWorld::GetCurrentPlayer() const { return &g_player; }
void CDefVirtual::v00() {}
static int g_destroyed;
void CDefVirtual::Destroy() { ++g_destroyed; }
static COpinionDeedEffectsDef g_effects[3];
static COpinionDeed g_deeds[3];
COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long handle) { return handle >= 0 && handle < 3 ? &g_deeds[handle] : 0; }
static CDefPointer<COpinionDeedEffectsDef> g_holder;   // returned by copy: no NRVO in VC7.1, so no local whose dtor would release
CDefPointer<COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::GetDeedEffectsDef(long deedType)
{ g_holder.Object = &g_effects[deedType]; ++g_effects[deedType].m_count; return g_holder; }
SNode* __cdecl Successor(SNode* node) { return node->_M_parent; }   // the test links its nodes through _M_parent
static CTCHeroOpinionDeedLog g_log;

int main()
{
    g_component.World = &g_world;
    g_entries[0].type = 0x51; g_entries[0].component = (CTCBase*)&g_log;
    g_hero.m_interfaces.first = g_entries; g_hero.m_interfaces.last = g_entries + 1;
    for (int i = 0; i < 3; ++i) { g_deeds[i].DeedType = i; g_effects[i].m_count = 1; }
    g_effects[0].KnowledgeLifeInSeconds = 10; g_effects[1].KnowledgeLifeInSeconds = 40; g_effects[2].KnowledgeLifeInSeconds = 25;
    static SNode header, n0, n1, n2;
    header._M_left = &n0; n0._M_parent = &n1; n1._M_parent = &n2; n2._M_parent = &header;
    n0._M_value_field = 0; n1._M_value_field = 7; n2._M_value_field = 2;           // handle 7 has no deed
    static CTCVillage village;
    village.PlayersCrimes = &header;
    long d = village.GetRegionLeavingDurationForCurrentCrimes();
    if (d != 25 * 30 || g_effects[0].m_count != 1 || g_effects[2].m_count != 1 || g_destroyed) { std::printf("0065a5bb_TEST FAIL (%ld)\n", d); return 1; }
    n1._M_value_field = 1;
    d = village.GetRegionLeavingDurationForCurrentCrimes();
    if (d != 40 * 30) { std::printf("0065a5bb_TEST FAIL (2)\n"); return 1; }
    g_heroPtr = 0;
    if (village.GetRegionLeavingDurationForCurrentCrimes() != 0) { std::printf("0065a5bb_TEST FAIL (no hero)\n"); return 1; }
    g_heroPtr = &g_hero; header._M_left = &header;
    if (village.GetRegionLeavingDurationForCurrentCrimes() != 0) { std::printf("0065a5bb_TEST FAIL (no crimes)\n"); return 1; }
    g_holder.Object = 0;
    std::printf("0065a5bb_TEST PASS\n");
    return 0;
}