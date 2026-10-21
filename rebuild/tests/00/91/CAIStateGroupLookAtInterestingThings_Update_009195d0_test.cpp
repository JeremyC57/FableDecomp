// Behaviour test for CAIStateGroup_LookAtInterestingThings::Update (0x009195D0): model copy, scripted stubs.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
static int g_released, g_destroyed;
struct CCreatureNavigationDef {
    long RefCount;
    bool AvoidDynamicObstacles;
    void DeleteThis() { ++g_destroyed; }
    void Release() { ++g_released; if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerNavigation {
    CCreatureNavigationDef* m_p;
    CDefPointerNavigation() : m_p(0) { }
    ~CDefPointerNavigation() { if (m_p) m_p->Release(); }
    CCreatureNavigationDef* operator->() const { return m_p; }
};
static CCreatureNavigationDef* g_def;
struct CDefClassBase {
    bool GetSubDef(CDefPointerNavigation* out) const { out->m_p = g_def; if (g_def) ++g_def->RefCount; return g_def != 0; }
};
static int g_removed;
struct CThing {
    CDefClassBase* PhysicsTC;
    unsigned char m_flags91;
    C3DVector pos;
    const C3DVector& GetPosition() { return pos; }
    void RemoveTC(long type) { if (type == 0xb3) ++g_removed; }
};
// the pointer drops its item after a scripted number of reads (the thing dies mid-walk)
static int g_reads, g_dropAfter;
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer(CThing* t) : m_pItem(t) { }
    CThing* GetPItem() const { if (g_dropAfter >= 0 && g_reads++ >= g_dropAfter) return 0; return m_pItem; }
};
inline bool IsThingAlive(const CIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CAIBrain { CThing* PCreature; };

static int g_move[16], g_mi, g_look[16], g_li, g_terminated, g_lastAvoid, g_lastFlags;
static float g_lastProximity, g_lastTurn;
static CThing* g_found;
struct CAIStateGroup_LookAtInterestingThings {
    CAIBrain* PBrain;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThing* FindInterestingThing() { return g_found; }
    void Terminate() { ++g_terminated; }
    long AIAction_MoveToPosProximity(long, C3DVector, float proximity, bool& arrived, long, bool, bool, bool avoid, const CThing*, bool, float)
    {
        g_lastProximity = proximity; g_lastAvoid = avoid; arrived = false; return g_move[g_mi++];
    }
    long AIAction_LookAtPos(long, C3DVector, long flags, float turn) { g_lastFlags = flags; g_lastTurn = turn; return g_look[g_li++]; }
    void Update();
};

void CAIStateGroup_LookAtInterestingThings::Update()
{
    CIntelligentPointer thing(FindInterestingThing());
    if (!IsThingAlive(thing))
        return;
    CDefPointerNavigation navigationDef;
    CDefClassBase* defClass = GetCreature()->PhysicsTC;
    defClass->GetSubDef(&navigationDef);
    bool arrived;
    for (;;) {
        if (thing.GetPItem() == 0) {
            Terminate();
            return;
        }
        CThing* target = thing.GetPItem();
        long result = AIAction_MoveToPosProximity(2, target->GetPosition(), 6.0f, arrived, 3, true, false,
                                                  navigationDef->AvoidDynamicObstacles, 0, true, 0.0f);
        if (result == 4)
            continue;
        if (result == 2) {
            for (;;) {
                if (thing.GetPItem() == 0) {
                    Terminate();
                    return;
                }
                CThing* target = thing.GetPItem();
                if (AIAction_LookAtPos(2, target->GetPosition(), 0x11, 0.0027777f) == 4)
                    continue;
                break;
            }
        } else if (result == 3)
            thing.GetPItem()->RemoveTC(0xb3);
        Terminate();
        return;
    }
}

static void Reset()
{
    g_mi = g_li = g_terminated = g_removed = g_released = g_destroyed = g_reads = 0; g_dropAfter = -1;
    for (int i = 0; i < 16; ++i) { g_move[i] = 4; g_look[i] = 4; }
}

int main()
{
    int ok = 1;
    CDefClassBase defClass;
    CCreatureNavigationDef def = { 1, true };
    g_def = &def;
    CThing me = { &defClass, 0, { 0, 0, 0 } };
    CThing other = { 0, 0, { 1, 2, 3 } };
    CAIBrain brain = { &me };
    CAIStateGroup_LookAtInterestingThings s; s.PBrain = &brain;
    // nothing interesting: no def fetched, no terminate
    Reset(); g_found = 0; s.Update();
    if (g_terminated || g_released || g_mi) ok = 0;
    // dead thing: ignored
    Reset(); g_found = &other; other.m_flags91 = 1; s.Update();
    if (g_terminated || g_mi) ok = 0;
    other.m_flags91 = 0;
    // walk twice (4), arrive (2), look twice (4) then done (2): terminate once, def released
    Reset(); g_found = &other; g_move[2] = 2; g_look[2] = 2; s.Update();
    if (g_mi != 3 || g_li != 3 || g_terminated != 1 || g_released != 1 || g_removed) ok = 0;
    if (g_lastProximity != 6.0f || g_lastAvoid != 1 || g_lastFlags != 0x11 || g_lastTurn != 0.0027777f) ok = 0;
    if (def.RefCount != 1 || g_destroyed) ok = 0;
    // unreachable (3): the interest component is removed
    Reset(); g_move[0] = 3; s.Update();
    if (g_removed != 1 || g_terminated != 1 || g_li) ok = 0;
    // failure (1): terminate only
    Reset(); g_move[0] = 1; s.Update();
    if (g_removed || g_terminated != 1 || g_mi != 1) ok = 0;
    // the thing disappears mid-walk (reads: alive 2, loop test, fetch, loop test -> gone)
    Reset(); g_dropAfter = 4; s.Update();
    if (g_mi != 1 || g_terminated != 1 || g_released != 1) ok = 0;
    // the thing disappears while looking
    Reset(); g_move[0] = 2; g_dropAfter = 6; s.Update();
    if (g_li != 1 || g_terminated != 1 || g_released != 1) ok = 0;
    // last reference: the def is destroyed on release
    Reset(); def.RefCount = 0; g_move[0] = 1; s.Update();
    if (g_destroyed != 1) ok = 0;
    if (!ok) { std::printf("009195d0_TEST FAIL\n"); return 1; }
    std::printf("009195d0_TEST PASS\n");
    return 0;
}