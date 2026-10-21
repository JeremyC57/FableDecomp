// CAIStateGroup_HoverHornetQueen::IsStillRunnable() -- still runnable while the target is alive, the first circle marker (if any) is alive and the group is not stopping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A420, length 97 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x4), CAIStateGroupBase::IsStopping @ 0x008FD010.
//
// Shape: the target guard is the inlined IS_THING_ALIVE helper (GetPItem null test, then GetPItem again for the dead
// bit 0 of the thing's byte at +0x91 -- the landed 0x00930DC0 helper) on PTargetThing (+0x14, address in esi), its
// `return false` tail placed right after it and jumped back to by the later guards; the front of the CircleMarkers
// vector (retail 12-byte STLport vector at +0x50: the PDB's 16-byte vectors put it at +0x54) behind a `size() > 0`
// test (`sub ; sar 3 ; test ; jbe`, element = 8-byte CIntelligentPointer) is a NAMED reference with two separate
// early-return guards -- that keeps `this` in edi and gives the element esi (IS_THING_ALIVE(CircleMarkers[0]) swaps
// esi/edi; re-indexing CircleMarkers[0] per guard re-reads the start). The tail `if (IsStopping()) return false;
// return true;` keeps the false tail first (`neg al ; sbb al,al ; inc al`); `IsStopping() ? false : true` places it
// after the second guard.
// The engine header CAIStateGroup_HoverHornetQueen.h is flattened data-only: local model with the retail offsets.
class CThingCreatureBase;

struct C3DVector { float X, Y, Z; };

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}

// Retail STLport vector<CIntelligentPointer<CThing>>: {start, finish, end_of_storage}.
struct CThingPointerVector {
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    CBaseIntelligentPointer* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    CBaseIntelligentPointer& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
};

struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_EnemyCreatureBase {
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    CThingPointerVector Things;                 // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CThingPointerVector CircleMarkers;          // +0x50
    long EnemyKnockDownFrame;                   // +0x5c
    bool TimingCritical;                        // +0x60
    long RouteDirection;                        // +0x64 eWaspDirection
    bool IsStillRunnable();
};

bool CAIStateGroup_HoverHornetQueen::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    if (CircleMarkers.size() > 0) {
        CBaseIntelligentPointer& marker = CircleMarkers[0];
        if (marker.GetPItem() == 0)
            return false;
        if (marker.GetPItem()->IsDead())
            return false;
    }
    if (IsStopping())
        return false;
    return true;
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool g_stopping = false;
static int g_stopCalls = 0;
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CBaseIntelligentPointer::v00() { }
bool CAIStateGroupBase::IsStopping() { ++g_stopCalls; return g_stopping; }
CAIStateGroupBase::~CAIStateGroupBase() { }
int main()
{
    static CThing target, marker;
    memset(&target, 0, sizeof(target)); memset(&marker, 0, sizeof(marker));
    static unsigned char mem[sizeof(CAIStateGroup_HoverHornetQueen)];
    CAIStateGroup_HoverHornetQueen* g = (CAIStateGroup_HoverHornetQueen*)mem;
    CBaseIntelligentPointer markers[2];
    markers[0].m_pItem = &marker; markers[1].m_pItem = 0;
    g->PTargetThing.m_pItem = 0;
    g->CircleMarkers._M_start = markers; g->CircleMarkers._M_finish = markers + 1;
    CHECK(g->IsStillRunnable() == false && g_stopCalls == 0);       // no target
    g->PTargetThing.m_pItem = &target; target.m_flags91 = 1;
    CHECK(g->IsStillRunnable() == false && g_stopCalls == 0);       // dead target
    target.m_flags91 = 2;
    CHECK(g->IsStillRunnable() == true && g_stopCalls == 1);        // alive target + alive marker
    marker.m_flags91 = 1;
    CHECK(g->IsStillRunnable() == false && g_stopCalls == 1);       // dead marker
    markers[0].m_pItem = 0;
    CHECK(g->IsStillRunnable() == false && g_stopCalls == 1);       // null marker
    g->CircleMarkers._M_finish = markers;                           // no markers
    CHECK(g->IsStillRunnable() == true && g_stopCalls == 2);
    g_stopping = true;
    CHECK(g->IsStillRunnable() == false && g_stopCalls == 3);
    if (fails == 0) printf("0095a420_TEST PASS\n");
    return fails;
}