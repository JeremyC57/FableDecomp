// CAIStateGroup_WorkHammerAnvil::OnReplace() -- give back the anvil's action point (when it was taken and the anvil
// is alive), stop the creature's current action, drop whatever is in both carry slots and make it pushable again.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944DB0, length 201 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PAnvil +0x15, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, user action point 0x2D = bit 13 of +0x24),
// CTCUserActionPointBase::ReleaseSlot(long) @ 0x007FEF40 (manifest "EditCyclePreviousKeyCamera" propagated; landed
// name), CThingCreatureBase::FinishCurrentAction @ 0x00662650, CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0, CAIStateGroupBase::DestroyItem(const CCharString&) @ 0x008FD1B0 (twice),
// SetPushableByHero(bool) @ 0x0097E330. Literals 0x01265440 "CARRY_SLOT_RIGHT_HAND", 0x01265428 "CARRY_SLOT_LEFT_HAND".
// Shape: pack(1) layout from the PDB (TookActionPoint +0x14 then the unaligned PAnvil +0x15, ActionPointIndex
// +0x44); inline IS_THING_ALIVE; the slot names are implicit conversion temporaries (re-addressed with `lea`).
// Local model (the engine header's layout is padding apart from these names).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};


struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCharString {                            // 4 bytes: the shared string data pointer
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CTCUserActionPointBase {                 // interface 0x2D
    void ReleaseSlot(long slot);                // 0x007FEF40
};

struct CThingCreatureBase {
    void FinishCurrentAction();                 // 0x00662650
};

struct CAIStateGroupBaseI : CAIStateGroupBase {
    void DestroyItem(const CCharString& slot);  // 0x008FD1B0
    void SetPushableByHero(bool pushable);      // 0x0097E330
};

#pragma pack(push, 1)
struct CAIStateGroup_WorkHammerAnvil : CAIStateGroupBaseI {
    bool TookActionPoint;                       // +0x14
    CBaseIntelligentPointer PAnvil;             // +0x15 CIntelligentPointer<CThingObject>
    float ActionPointPos_X;                     // +0x1d
    float ActionPointPos_Y;                     // +0x21
    float ActionPointPos_Z;                     // +0x25
    unsigned char ActionPointRHSet[0x1b];       // +0x29 CRightHandedSet
    long ActionPointIndex;                      // +0x44
    long NextFrameHammer;                       // +0x48
    virtual void OnReplace();
};
#pragma pack(pop)

void CAIStateGroup_WorkHammerAnvil::OnReplace()
{
    if (IS_THING_ALIVE(PAnvil) && TookActionPoint) {
        CTCUserActionPointBase* point;
        if (PAnvil.GetPItem()->GetInterface(&point, 0x2D))
            point->ReleaseSlot(ActionPointIndex);
    }
    ((CThingCreatureBase*)PBrain->PCreature)->FinishCurrentAction();
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    DestroyItem("CARRY_SLOT_LEFT_HAND");
    SetPushableByHero(true);
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_releases = 0, g_finishes = 0, g_pushables = 0, g_strings = 0; static long g_lastSlot = -1;
static const void* g_releaseThis = 0; static const void* g_finishThis = 0; static bool g_lastPushable = false;
static char g_destroyed[4][32]; static int g_nd = 0;
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CAIStateGroupBase::~CAIStateGroupBase() { }
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strings; (void)length; }
CCharString::~CCharString() { --g_strings; }
void CTCUserActionPointBase::ReleaseSlot(long slot) { ++g_releases; g_lastSlot = slot; g_releaseThis = this; }
void CThingCreatureBase::FinishCurrentAction() { ++g_finishes; g_finishThis = this; }
void CAIStateGroupBaseI::DestroyItem(const CCharString& slot) { if (g_nd < 4) strcpy(g_destroyed[g_nd++], (const char*)slot.m_pData); }
void CAIStateGroupBaseI::SetPushableByHero(bool pushable) { ++g_pushables; g_lastPushable = pushable; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static CThing g_anvil, g_creature;
static CAIBrain g_brain;
static CTCUserActionPointBase g_point;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_entries[0].type = 0x2d; g_entries[0].component = (CTCBase*)&g_point;
    g_entries[1].type = 0x31; g_entries[1].component = 0;
    g_anvil.m_interfaces.first = g_entries; g_anvil.m_interfaces.last = g_entries + 2;
    g_anvil.m_presentBits[0x2d >> 5] = 1UL << (0x2d & 31);
    g_brain.PCreature = &g_creature;
    CAIStateGroup_WorkHammerAnvil* s = new CAIStateGroup_WorkHammerAnvil;
    s->PBrain = &g_brain;
    s->PAnvil.SetPItem(&g_anvil);
    s->TookActionPoint = true;
    s->ActionPointIndex = 3;
    s->OnReplace();
    CHECK(g_releases == 1 && g_lastSlot == 3 && g_releaseThis == &g_point);
    CHECK(g_finishes == 1 && g_finishThis == &g_creature && g_pushables == 1 && g_lastPushable && g_strings == 0);
    CHECK(g_nd == 2 && strcmp(g_destroyed[0], "CARRY_SLOT_RIGHT_HAND") == 0 && strcmp(g_destroyed[1], "CARRY_SLOT_LEFT_HAND") == 0);
    s->TookActionPoint = false;                     // point not taken
    s->OnReplace();
    CHECK(g_releases == 1 && g_finishes == 2);
    s->TookActionPoint = true; g_anvil.m_flags91 = 1;   // anvil dead
    s->OnReplace();
    CHECK(g_releases == 1 && g_finishes == 3);
    g_anvil.m_flags91 = 0; g_anvil.m_presentBits[0x2d >> 5] = 0;   // no action point component
    s->OnReplace();
    CHECK(g_releases == 1 && g_finishes == 4);
    s->PAnvil.SetPItem(0);                          // no anvil
    s->OnReplace();
    CHECK(g_releases == 1 && g_finishes == 5 && g_pushables == 5);
    delete s;
    if (fails == 0) printf("00944db0_TEST PASS\n");
    return fails;
}