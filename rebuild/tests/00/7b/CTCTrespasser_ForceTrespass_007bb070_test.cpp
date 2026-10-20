// Behaviour test for 007bb070: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTrespasser::ForceTrespass() -- when the trespassed thing is alive and trespassable, posts the trespass deed to the owner's opinion deed log, keeping the handle when no trespass deed is open yet
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CTCBase;
struct CBaseObject;
struct CFaction;
struct CThingAICreature;
struct C3DVector { float X, Y, Z; };

enum EOpinionDeedType { OPINION_DEED_TYPE_TRESPASS = 2 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_0x12 = 0x12 };

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CTCHeroOpinionDeedLog {                  // interface 0x51
    long PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CTCTrespasser {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
    long          InsideBuildingNotTrespassingDeedHandle;   // +0x0c
    long          DeedHandles[3];               // +0x10
    long          CurrentDeedHandleIndex;       // +0x1c
    long          OccupantsKnowOfDeedHandleIndex;   // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;    // +0x24
    unsigned long FramePlayerLeft;              // +0x28
    bool          AttachedOpinionSource;        // +0x2c
    CBaseIntelligentPointer TrespassingThing;   // +0x2d CIntelligentPointer<CThing>
    void ForceTrespass();
};

void CTCTrespasser::ForceTrespass()
{
    if (IS_THING_ALIVE(TrespassingThing) && TrespassingThing.GetPItem()->HasTC(0xbd)) {
        CTCHeroOpinionDeedLog* log = 0;
        if (ParentThing->GetInterface(&log, 0x51) && DeedHandles[2] == -1)
            DeedHandles[2] = log->PostDeed(OPINION_DEED_TYPE_TRESPASS, ParentThing->GetPosition(), TrespassingThing.GetPItem(), 0,
                                           OPINION_POST_DEED_TYPE_0x12, true, true, 0, false, 0);
        else
            log->PostDeed(OPINION_DEED_TYPE_TRESPASS, ParentThing->GetPosition(), TrespassingThing.GetPItem(), 0,
                          OPINION_POST_DEED_TYPE_0x12, true, false, 0, false, 0);
    }
}

// ---- stubs and driver ----
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static C3DVector g_pos;
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
static CTCHeroOpinionDeedLog g_log;
static int g_posts; static long g_type, g_post; static const C3DVector* g_where; static const CThing* g_target; static CThingAICreature* g_victim;
static bool g_b1, g_b2, g_b3; static const CFaction* g_faction; static long g_extra = -1;
long CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
    EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra)
{
    if (this == &g_log) ++g_posts;
    g_type = deedType; g_where = &position; g_target = target; g_victim = victim; g_post = postType; g_b1 = b1; g_b2 = b2;
    g_faction = faction; g_b3 = b3; g_extra = extra;
    return 4242;
}

int main()
{
    static CThing owner, building;
    static CTCTrespasser tc;
    tc.ParentThing = &owner;
    tc.TrespassingThing.PData = (CBaseObject*)&building;
    owner.m_presentBits[0x51 >> 5] = 1UL << (0x51 & 31);
    g_entries[0].type = 0x51; g_entries[0].component = (CTCBase*)&g_log;
    owner.m_interfaces.first = g_entries; owner.m_interfaces.last = g_entries + 1;
    tc.DeedHandles[2] = -1;
    // building not trespassable: nothing posted
    tc.ForceTrespass();
    if (g_posts != 0) { std::printf("007bb070_TEST FAIL (not trespassable)\n"); return 1; }
    building.m_presentBits[0xbd >> 5] = 1UL << (0xbd & 31);
    // no open deed: posted with the handle kept
    tc.ForceTrespass();
    if (g_posts != 1 || tc.DeedHandles[2] != 4242 || g_type != 2 || g_where != &g_pos || g_target != &building || g_victim
        || g_post != 0x12 || !g_b1 || !g_b2 || g_faction || g_b3 || g_extra != 0) { std::printf("007bb070_TEST FAIL (first)\n"); return 1; }
    // deed already open: posted again without the flag, handle unchanged
    tc.DeedHandles[2] = 7;
    tc.ForceTrespass();
    if (g_posts != 2 || tc.DeedHandles[2] != 7 || g_b2 || !g_b1) { std::printf("007bb070_TEST FAIL (second)\n"); return 1; }
    // dead building: nothing
    building.m_flags = 1;
    tc.ForceTrespass();
    if (g_posts != 2) { std::printf("007bb070_TEST FAIL (dead)\n"); return 1; }
    std::printf("007bb070_TEST PASS\n");
    return 0;
}