#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_table[8];
    int m_count;
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_flags91 = 0; m_count = 0; Sync(); }
    void Sync() { m_interfaces.first = m_table; m_interfaces.last = m_table + m_count; }
    void Add(long type, void* comp)       // keep the table sorted
    {
        int i = m_count;
        while (i > 0 && m_table[i - 1].type > type) { m_table[i] = m_table[i - 1]; --i; }
        m_table[i].type = type; m_table[i].component = comp; ++m_count;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        Sync();
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { CThing* PCreature; };

struct CTCCombatTarget { CThing* enemy; CThing* GetEnemy() { return enemy; } };
static int g_predicted = 0; static bool g_inRange = true;
void GetPredictedTargetPosForThing(CThing*, CThing*) { ++g_predicted; }
bool IsThingInRangeOfPound(CThing*, CThing*) { return g_inRange; }

struct CAIStateGroup_IceTrollPoundGroundAttack {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PTarget;
    unsigned char* m_enemyFlags6c;
    bool IsRunnable()
    {
        CTCCombatTarget* combat;
        if (PBrain->PCreature->GetInterface(&combat, 8)) {
            CThing* enemy = combat->GetEnemy();
            if (enemy && !(enemy->m_flags91 & 1) && (*m_enemyFlags6c & 8)) {
                GetPredictedTargetPosForThing(PBrain->PCreature, enemy);
                if (IsThingInRangeOfPound(PBrain->PCreature, enemy)) {
                    PTarget.SetPItem(enemy);
                    return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    CThing troll, enemy; troll.Init(); enemy.Init();
    CTCCombatTarget combat = { &enemy };
    CAIBrain brain = { &troll };
    unsigned char flags = 8;
    CAIStateGroup_IceTrollPoundGroundAttack g; g.PBrain = &brain; g.PTarget.SetPItem(0); g.m_enemyFlags6c = &flags;
    bool ok = !g.IsRunnable();                  // no combat component
    troll.Add(8, &combat);
    ok = ok && g.IsRunnable() && g.PTarget.GetPItem() == &enemy && g_predicted == 1;
    g.PTarget.SetPItem(0); g_inRange = false;
    ok = ok && !g.IsRunnable() && g.PTarget.GetPItem() == 0 && g_predicted == 2;
    g_inRange = true; enemy.m_flags91 = 1; ok = ok && !g.IsRunnable() && g_predicted == 2;
    enemy.m_flags91 = 0; flags = 0; ok = ok && !g.IsRunnable();
    if (!ok) { std::printf("00961e80_TEST FAIL\n"); return 1; }
    std::printf("00961e80_TEST PASS\n");
    return 0;
}
