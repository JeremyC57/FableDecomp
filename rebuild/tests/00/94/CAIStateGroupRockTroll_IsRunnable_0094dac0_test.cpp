// Behaviour test for 0094dac0: the model below is a copy of src/0094dac0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <cmath>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
using std::sqrt;
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_lowerBounds = 0, g_frames = 0;
struct CTCBase;
struct CWorld;
struct C3DVector { float x, y, z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { ++g_lowerBounds; SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;
    unsigned char m_flags92;
    C3DVector m_pos;
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { ++g_gets; return m_pItem; }
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    void SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual long GetTimeToInitiate() { return 0; }
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct C2DVector {
    float x, y;
    float GetLength() const { return (float)sqrt(x * x + y * y); }
};
struct CWorldF {
    static long GetWorldFrame() { return g_frames; }
};
static CThing* g_enemy = 0;
struct CTCEnemyTracker {
    virtual CThing* GetEnemy() { return g_enemy; }
};
inline CTCEnemyTracker* GetEnemyTracker(CThing* thing)
{
    CTCEnemyTracker* tracker;
    thing->GetInterfaceX(&tracker, 8);
    return tracker;
}
struct CThingPhysical : CThing {
    const C3DVector& GetPhysicsPosition() { return m_pos; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CAIStateGroup_TrollBase : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;
    long EnemyKnockDownFrame;
    long ShotCounter;
    float TROLL_MAX_THROWING_DISTANCE;
    float TROLL_MIN_THROWING_DISTANCE;
    float TROLL_STOP_POUNDING_DISTANCE;
    float TROLL_STOP_THROWING_DISTANCE;
    CWorldF* GetPWorld() { return 0; }
    CAIStateGroup_TrollBase() : PEnemy(0) {}
};
struct CAIStateGroup_RockTroll : CAIStateGroup_TrollBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_RockTroll::IsRunnable()
{
    CThing* enemy = GetEnemyTracker(PBrain->PCreature)->GetEnemy();
    if (enemy && !(enemy->m_flags91 & 1)) {
        CThingPhysical* creature = (CThingPhysical*)PBrain->PCreature;
        const C3DVector& enemyPos = ((CThingPhysical*)enemy)->GetPhysicsPosition();
        const C3DVector& creaturePos = creature->GetPhysicsPosition();
        C2DVector delta;
        delta.x = creaturePos.x - enemyPos.x;
        delta.y = creaturePos.y - enemyPos.y;
        if (delta.GetLength() < TROLL_MAX_THROWING_DISTANCE) {
            PEnemy = CIntelligentPointerCreature(enemy);
            EnemyKnockDownFrame = GetPWorld()->GetWorldFrame();
            return true;
        }
    }
    return false;
}

static bool Run(bool hasEnemy, bool dead, float ex, float ey, float range, bool expect)
{
    static CThingPhysical creature, enemy; static CTCEnemyTracker tracker; static CInterfaceMap::SEntry entries[2];
    std::memset((void*)&creature, 0, sizeof(creature)); std::memset((void*)&enemy, 0, sizeof(enemy));
    entries[0].type = 2; entries[0].component = 0; entries[1].type = 8; entries[1].component = (CTCBase*)&tracker;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2; creature.m_presentBits[0] |= 1UL << 8;
    creature.m_pos.x = 10.0f; creature.m_pos.y = 20.0f; creature.m_pos.z = 5.0f;
    enemy.m_pos.x = ex; enemy.m_pos.y = ey; enemy.m_pos.z = 99.0f; enemy.m_flags91 = dead ? 1 : 0;
    g_enemy = hasEnemy ? &enemy : 0; g_frames = 777;
    CAIBrain brain; std::memset(&brain, 0, sizeof(brain)); brain.PCreature = &creature;
    CAIStateGroup_RockTroll s; s.PBrain = &brain; s.TROLL_MAX_THROWING_DISTANCE = range; s.EnemyKnockDownFrame = -1;
    g_ctors = g_dtors = g_sets = g_gets = g_lowerBounds = 0;
    bool r = s.IsRunnable();
    if (r != expect || g_lowerBounds != 1) return false;
    if (expect) {
        if (s.PEnemy.m_pItem != &enemy || s.EnemyKnockDownFrame != 777) return false;
        if (g_ctors != 1 || g_dtors != 1 || g_sets != 2 || g_gets != 1) return false;
    } else if (s.PEnemy.m_pItem != 0 || s.EnemyKnockDownFrame != -1 || g_ctors != 0) return false;
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run(true, false, 13.0f, 24.0f, 6.0f, true);     // 5 away, range 6
    ok = ok && Run(true, false, 13.0f, 24.0f, 5.0f, false);    // exactly the range: not less
    ok = ok && Run(true, true, 13.0f, 24.0f, 6.0f, false);     // dead enemy
    ok = ok && Run(false, false, 0.0f, 0.0f, 6.0f, false);     // no enemy
    if (!ok) { std::printf("0094dac0_TEST FAIL\n"); return 1; }
    std::printf("0094dac0_TEST PASS\n");
    return 0;
}