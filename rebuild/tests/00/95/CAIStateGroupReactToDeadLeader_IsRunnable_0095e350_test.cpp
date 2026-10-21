// Behaviour test for 0095e350: src/0095e350.cpp copied verbatim; the world's message manager, the creature position
// and the intelligent pointers stubbed so the message lookup (filter on the brain's creature), the 20-unit radius,
// the killer's state test and the enemy assignment are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// Retry 10b (getsw sweep, FIXES B-15): the message event manager is read through an inline getter `GetMessageEventManager()`; the plain member read gives the parked register/scheduling difference.
// CAIStateGroup_ReactToDeadLeader::IsRunnable() -- runnable when a recent leader-death message lies within 20 units
// (2D) of the creature and its killer is a live-flagged creature (all four low state bits set); remember the killer in
// PEnemy and restart FramesRunning.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E350, length 213 bytes (+ int3 padding).
// Callees: the world getter @ 0x008FCFA0 (+0x60 = CMessageEventManager), CMessageEventManager::FindMostRecentMessage<
// leader-death filter> @ 0x0095E6D0, the creature position getter @ 0x004C73D0, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on the message's +0x48 pointer (three times), the converting CIntelligentPointer temporary (vtable
// 0x0123E7D8): ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem, dtor @ 0x00A01C10; 400.0f @ 0x0125D824.
// Shape: the one-word filter is a block-scoped named local built before the world call (its slot is reused by the
// pointer temporary); `C2DVector(msg.X - pos.X, msg.Y - pos.Y).GetMagnitudeSquared() < 400`
// keeps both components on the x87 stack (`fstp st(0)` twice after the compare); `(~flags & 0xF) == 0` gives
// `not ecx ; test cl,0Fh`. Engine header names (PEnemy +0x14, FramesRunning +0x1c); base modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetMagnitudeSquared() const { return X * X + Y * Y; }
};

struct CThingCreaturePos {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CThingState {
    unsigned char _pad[0x6c];
    unsigned long StateFlags;                   // +0x6c
};

struct CMessageEventFilter_IsLeaderDeath {
    CThing* PCreature;
    CMessageEventFilter_IsLeaderDeath(CThing* creature) : PCreature(creature) { }
};
struct CMessageEvent {
    unsigned char _pad[0xc];
    C3DVector Pos;                              // +0x0c
    unsigned char _pad_0x18[0x30];
    CBaseIntelligentPointerCtor PKiller;        // +0x48
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsLeaderDeath& filter);   // 0x0095E6D0
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    CMessageEventManager* GetMessageEventManager() const { return PMessageEventManager; }
};

struct CAIStateGroup_ReactToDeadLeader : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14 CIntelligentPointer<CThingCreatureBase>
    long FramesRunning;                         // +0x1c
    CWorld* GetPWorldA();                       // 0x008FCFA0
    virtual bool IsRunnable();
};

bool CAIStateGroup_ReactToDeadLeader::IsRunnable()
{
    CMessageEvent* message;
    {
        CMessageEventFilter_IsLeaderDeath filter(PBrain->PCreature);
        message = GetPWorldA()->GetMessageEventManager()->FindMostRecentMessage(filter);
    }
    if (message != 0) {
        const C3DVector& pos = ((CThingCreaturePos*)PBrain->PCreature)->GetPosition();
        if (C2DVector(message->Pos.X - pos.X, message->Pos.Y - pos.Y).GetMagnitudeSquared() < 400.0f) {
            CBaseIntelligentPointerCtor& killer = message->PKiller;
            if (killer.GetPItem() != 0 && (~((CThingState*)killer.GetPItem())->StateFlags & 0xf) == 0) {
                CThing* enemy = killer.GetPItem();
                if (enemy != 0) {
                    PEnemy = enemy;
                    FramesRunning = 0;
                    return true;
                }
            }
        }
    }
    return false;
}
// ---- end source ----
CAIStateGroupBase::~CAIStateGroupBase() {}
void CAIStateGroupBase::v01() {} void CAIStateGroupBase::v02() {} void CAIStateGroupBase::v03() {} void CAIStateGroupBase::v04() {}
bool CAIStateGroupBase::IsStillValid() { return false; }
void CAIStateGroupBase::v06() {} void CAIStateGroupBase::v07() {} void CAIStateGroupBase::v08() {}
void CAIStateGroupBase::Cleanup() {} void CAIStateGroupBase::OnReplace() {} bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() {} void CAIStateGroupBase::InitStateGroup() {}
void CBaseIntelligentPointerCtor::v00() {}
CBaseIntelligentPointerCtor::CBaseIntelligentPointerCtor() : m_pItem(0) {}
CBaseIntelligentPointerCtor::~CBaseIntelligentPointerCtor() {}
CThing* CBaseIntelligentPointerCtor::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointerCtor::SetPItem(CThing* item) { m_pItem = item; }
static C3DVector g_creaturePos;
const C3DVector& CThingCreaturePos::GetPosition() { return g_creaturePos; }
static CMessageEvent* g_message = 0; static CThing* g_filterCreature = 0;
CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CMessageEventFilter_IsLeaderDeath& f) { g_filterCreature = f.PCreature; return g_message; }
static CMessageEventManager g_mem; static CWorld g_world;
CWorld* CAIStateGroup_ReactToDeadLeader::GetPWorldA() { return &g_world; }
int main()
{
    g_world.PMessageEventManager = &g_mem;
    static unsigned char creatureRaw[0x100], killerRaw[0x100];
    CThing* creature = (CThing*)creatureRaw; CThing* killer = (CThing*)killerRaw;
    static CAIBrain brain; brain.PCreature = creature;
    static unsigned char raw[sizeof(CAIStateGroup_ReactToDeadLeader)]; memset(raw, 0, sizeof(raw));
    CAIStateGroup_ReactToDeadLeader* g = (CAIStateGroup_ReactToDeadLeader*)raw;
    g->PBrain = &brain; g->FramesRunning = 7;
    CHECK(g->CAIStateGroup_ReactToDeadLeader::IsRunnable() == false && g_filterCreature == creature);   // no message
    static CMessageEvent msg; msg.Pos.X = 10; msg.Pos.Y = 10; msg.Pos.Z = 0;
    g_message = &msg; g_creaturePos.X = 0; g_creaturePos.Y = 0; g_creaturePos.Z = 0;
    msg.PKiller.m_pItem = killer; ((CThingState*)killer)->StateFlags = 0xf;
    CHECK(g->CAIStateGroup_ReactToDeadLeader::IsRunnable() == true && g->PEnemy.m_pItem == killer && g->FramesRunning == 0);
    g->PEnemy.m_pItem = 0; g->FramesRunning = 7;
    msg.Pos.X = 20;                                                   // 500 >= 400: too far
    CHECK(g->CAIStateGroup_ReactToDeadLeader::IsRunnable() == false && g->PEnemy.m_pItem == 0);
    msg.Pos.X = 10; ((CThingState*)killer)->StateFlags = 0x7;        // state test fails
    CHECK(g->CAIStateGroup_ReactToDeadLeader::IsRunnable() == false);
    ((CThingState*)killer)->StateFlags = 0x1f; msg.PKiller.m_pItem = 0;
    CHECK(g->CAIStateGroup_ReactToDeadLeader::IsRunnable() == false && g->FramesRunning == 7);
    if (fails == 0) printf("0095e350_TEST PASS\n"); else printf("0095e350_TEST FAIL\n");
    return fails;
}