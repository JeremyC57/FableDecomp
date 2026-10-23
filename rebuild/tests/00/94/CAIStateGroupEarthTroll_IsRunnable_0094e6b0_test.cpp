// Behaviour test for 0094e6b0: the model below is a copy of src/0094e6b0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_lowerBounds = 0, g_frames = 0;
struct CTCBase;
struct CThing;
struct CWorld;
struct C3DVector;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { ++g_lowerBounds; SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    C3DVector m_pos;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    const C3DVector& GetPosition_006a4d00() const { return m_pos; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual bool IsStillValid() { return true; }
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CBaseIntelligentPointerCtor {
    CThing* m_pItem;
    CBaseIntelligentPointerCtor() : m_pItem(0) { ++g_ctors; }
    CThing* GetPItem() const { ++g_gets; return m_pItem; }
    void SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
    ~CBaseIntelligentPointerCtor() { ++g_dtors; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
static CThing* g_enemy = 0;
struct CTCEnemySource {
    virtual CThing* GetEnemy() { return g_enemy; }
};
struct CWorldFrame {
    static long GetWorldFrame() { return g_frames; }
};
struct CAIStateGroup_EarthTroll : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;
    long LastRunFrame;
    unsigned char _pad20[4];
    float AttackRange;
    CWorld* GetPWorld() { return 0; }
    virtual bool IsRunnable();
    CAIStateGroup_EarthTroll() : PEnemy(0) {}
};

bool CAIStateGroup_EarthTroll::IsRunnable()
{
    CTCEnemySource* source;
    PBrain->PCreature->GetInterface(&source, 8);
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        CThing* creature = PBrain->PCreature;
        const C3DVector& enemyPos = enemy->GetPosition_006a4d00();
        const C3DVector& pos = creature->GetPosition_006a4d00();
        float dx = pos.X - enemyPos.X;
        float dy = pos.Y - enemyPos.Y;
        if ((float)sqrt(dx * dx + dy * dy) < AttackRange) {
            PEnemy = enemy;
            LastRunFrame = ((CWorldFrame*)GetPWorld())->GetWorldFrame();
            return true;
        }
    }
    return false;
}

static bool Run(bool hasEnemy, bool dead, float ex, float ey, float range, bool expect)
{
    static CThing creature, enemy; static CTCEnemySource source; static CInterfaceMap::SEntry entries[2];
    std::memset(&creature, 0, sizeof(creature)); std::memset(&enemy, 0, sizeof(enemy));
    entries[0].type = 2; entries[0].component = 0; entries[1].type = 8; entries[1].component = (CTCBase*)&source;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2; creature.m_presentBits[0] |= 1UL << 8;
    creature.m_pos.X = 10.0f; creature.m_pos.Y = 20.0f; creature.m_pos.Z = 5.0f;
    enemy.m_pos.X = ex; enemy.m_pos.Y = ey; enemy.m_pos.Z = 99.0f; enemy.m_flags91 = dead ? 1 : 0;
    g_enemy = hasEnemy ? &enemy : 0; g_frames = 1234;
    CAIBrain brain; std::memset(&brain, 0, sizeof(brain)); brain.PCreature = &creature;
    CAIStateGroup_EarthTroll s; s.PBrain = &brain; s.AttackRange = range; s.LastRunFrame = -1;
    g_ctors = g_dtors = g_sets = g_gets = g_lowerBounds = 0;
    bool r = s.IsRunnable();
    if (r != expect || g_lowerBounds != 1) return false;
    if (expect) {
        if (s.PEnemy.m_pItem != &enemy || s.LastRunFrame != 1234) return false;
        if (g_ctors != 1 || g_dtors != 1 || g_sets != 2 || g_gets != 1) return false;   // one temporary from the enemy, copied in
    } else if (s.PEnemy.m_pItem != 0 || s.LastRunFrame != -1 || g_ctors != 0) return false;
    return true;
}
int main()
{
    bool ok = true;
    ok = ok && Run(true, false, 13.0f, 24.0f, 6.0f, true);     // 5 away, range 6
    ok = ok && Run(true, false, 13.0f, 24.0f, 5.0f, false);    // 5 away, range 5: not < range
    ok = ok && Run(true, true, 13.0f, 24.0f, 6.0f, false);     // dead enemy
    ok = ok && Run(false, false, 0.0f, 0.0f, 6.0f, false);     // no enemy
    if (!ok) { std::printf("0094e6b0_TEST FAIL\n"); return 1; }
    std::printf("0094e6b0_TEST PASS\n");
    return 0;
}