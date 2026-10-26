// Behaviour test for 0x008C6430 CCreatureAction_TrollProjectileAttack::PerformAction: runs the reconstructed body over stubbed
// callees; checks WhackGround, the shot def lookup, every CreateShot argument and the ability def release.

#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing { public:
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char m_flags91; float m_health;
    CInterfaceMap::SEntry m_entries[4]; int m_count;
    CThing() : m_flags91(0), m_health(0), m_count(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0;
    }
    void Add(long type, void* component)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_entries[m_count].type = type; m_entries[m_count].component = (CTCBase*)component; ++m_count;
        m_interfaces.first = m_entries; m_interfaces.last = m_entries + m_count;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
static int g_strings = 0;
class CCharString { public: const char* m_pData; CCharString(const char* t, long = -1) : m_pData(t) { ++g_strings; } ~CCharString() { --g_strings; } };
static char g_name[40];
class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& n) { std::strcpy(g_name, n.m_pData); return 555; } };
class CWorld { public: CDefinitionManager* PDefinitionManager; };
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
static int g_destroyed = 0;
struct CDefVirtual { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } };
class CMeleeCombatAbilityDef : public CDefVirtual, public CRefCounted { public: void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); } };
template <class T> class CDefPointer { public: T* m_p; CDefPointer() : m_p(0) { } ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } } };
class CThingCreatureBase : public CThing { public: long acc; long GetShotAccuracyPercentage() { return acc; } };
class C3DVector; class CRightHandedSet;
static CMeleeCombatAbilityDef g_ability;
static long s_idx; static CThing* s_shooter; static CThing* s_origin; static CThing* s_target; static float s_acc, s_a, s_b; static void* s_pos; static void* s_frame;
CThing* CreateShot(long defIndex, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                   CDefPointer<CMeleeCombatAbilityDef>* ability, C3DVector* position, CRightHandedSet* frame)
{
    s_idx = defIndex; s_shooter = shooter; s_origin = origin; s_target = target; s_acc = accuracy; s_a = a; s_b = b; s_pos = position; s_frame = frame;
    ability->m_p = &g_ability; g_ability.m_count = 1; return 0;
}
struct CBaseIntelligentPointer { CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
static int g_whack = 0;
class CCreatureAction_TrollWhackGroundBase {
public:
    virtual void PerformAction() { }
    CBaseIntelligentPointer PCreature; CBaseIntelligentPointer PTarget; CWorld* m_world;
    CWorld* GetPWorld() const { return m_world; }
    void WhackGround() { ++g_whack; }
};
class CCreatureAction_TrollProjectileAttack : public CCreatureAction_TrollWhackGroundBase { public: virtual void PerformAction(); };
void CCreatureAction_TrollProjectileAttack::PerformAction()
{
    WhackGround();
    long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
    long shotIndex;
    {
        CCharString name("SHOT_TROLL_CRACK_IN_GROUND", -1);
        CDefinitionManager* manager = GetPWorld()->PDefinitionManager;
        shotIndex = manager->GetDefGlobalIndexFromName(name);
    }
    {
        CDefPointer<CMeleeCombatAbilityDef> ability;
        CThing* troll = PCreature.GetPItem();
        CreateShot(shotIndex, troll, 0, PTarget.GetPItem(), (float)accuracy, 1.0f, 1.0f, &ability, 0, 0);
    }
}
int main()
{
    CDefinitionManager dm; CWorld world; world.PDefinitionManager = &dm;
    CThingCreatureBase troll; troll.acc = 75; CThingCreatureBase target;
    CCreatureAction_TrollProjectileAttack a; a.PCreature.m_pItem = &troll; a.PTarget.m_pItem = &target; a.m_world = &world;
    a.PerformAction();
    bool ok = g_whack == 1 && std::strcmp(g_name, "SHOT_TROLL_CRACK_IN_GROUND") == 0 && g_strings == 0 && s_idx == 555 &&
              s_shooter == &troll && s_origin == 0 && s_target == &target && s_acc == 75.0f && s_a == 1.0f && s_b == 1.0f &&
              s_pos == 0 && s_frame == 0 && g_ability.m_count == 0 && g_destroyed == 1;
    if (!ok) { std::printf("008c6430_TEST FAIL\n"); return 1; }
    std::printf("008c6430_TEST PASS\n");
    return 0;
}