// Behaviour test for 0x006ED510 CCombatManager::DeregisterAttackingCombatant: the source's model and body, with working
// bodies for the out-of-line callees (map find/erase, intelligent pointer, string building, components) that record
// what the body does. Scenarios: melee and ranged pattern bumps, both attacker-count branches, the non-combatant cleanup
// (live and dead weapon item, unsheathe flag) and the stealth reset; every string temporary must be released.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif

static int g_liveStrings = 0;
class CCharString { public:
    char* m_pData;
    explicit CCharString(const char* text) { m_pData = (char*)malloc(strlen(text) + 1); strcpy(m_pData, text); ++g_liveStrings; }
    CCharString(const CCharString& other) { m_pData = (char*)malloc(strlen(other.m_pData) + 1); strcpy(m_pData, other.m_pData); ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
private:
    CCharString& operator=(const CCharString&);
};
static CCharString Concat(const char* left, const char* right)
{
    char* buffer = (char*)malloc(strlen(left) + strlen(right) + 1);
    strcpy(buffer, left);
    strcat(buffer, right);
    CCharString result(buffer);
    free(buffer);
    return result;
}
CCharString operator+(const char* left, const CCharString& right) { return Concat(left, right.m_pData); }
CCharString operator+(const CCharString& left, const char* right) { return Concat(left.m_pData, right); }
CCharString operator+(const CCharString& left, const CCharString& right) { return Concat(left.m_pData, right.m_pData); }
CCharString GFIntToCharString(long value) { char text[32]; sprintf(text, "%ld", value); return CCharString(text); }
class CDefString { public:
    long m_index;
};
static const char* g_defNames[] = { "NONE", "CREATURE_ARCHER", "CREATURE_BANDIT", "CREATURE_HOBBE", "CREATURE_BALVERINE" };
struct CDefStringTable {
    CCharString GetString(CDefString name) { return CCharString(g_defNames[name.m_index]); }
};
CDefStringTable g_DefStringTable;
static char g_traces[8][160];
static int g_traceCount = 0;
inline void CombatTrace(const CCharString& text) { strcpy(g_traces[g_traceCount++], text.m_pData); }

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    const SEntry* LowerBound(const long* type) const { const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static int g_defDestroyed = 0;
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_defDestroyed; }
};
class CCreatureDef : public CDefVirtual { public:
    long          RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
    unsigned char _pad08[0x148 - 0x08];
    unsigned char Flags148;
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* Get() const { return Object; }
};
class CThing { public:
    unsigned char    _pad0[0x18];
    unsigned __int64 UniqueID;
    unsigned long    m_presentBits[9];
    CInterfaceMap    m_interfaces;
    unsigned char    _pad4c[0x70 - 0x4c];
    CCreatureDef*    m_pDef;
    unsigned char    _pad74[0x91 - 0x74];
    unsigned char    m_flags91;
    unsigned char    _pad92[0x15c - 0x92];
    unsigned char    CombatFlags0 : 2;
    unsigned char    IsAttackingCombatant : 1;
    unsigned char    IsAttackedCombatant : 1;
    unsigned char    CombatFlags4 : 4;
    long             m_defNameIndex;               // test: the def name
    CDefString GetDefName() const { CDefString name; name.m_index = m_defNameIndex; return name; }
    unsigned __int64 GetUID() const { return UniqueID; }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetConstInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
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
};
class CThingCreatureBase : public CThing { public:
    bool m_isCombatant;                            // test: IsCombatant's answer
    bool IsCombatant() { return m_isCombatant; }
};
class CTCCarrying { public:
    CThing* m_primary;
    CThing* GetThingInPrimarySlot() { return m_primary; }
};
class CTCWeapon { public:
    bool m_projectile;
    int  m_resets;
    bool IsProjectileWeapon() { return m_projectile; }
    void ResetAiming() { ++m_resets; }
};
enum EDangerStatus { DANGER_NONE = 0 };
class CTCStealth { public:
    CThing* m_source;
    long    m_status;
    int     m_calls;
    void SetAsInDanger(CThing* source, EDangerStatus status) { m_source = source; m_status = status; ++m_calls; }
};

struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointerThing() : CBaseIntelligentPointer() {}
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

class CCombatSequenceBase { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {}
    virtual bool IsMeleeSequence() { return false; }
    virtual bool IsRangedSequence() { return false; }
};
struct CMeleeSequence : CCombatSequenceBase { bool IsMeleeSequence() { return true; } };
struct CRangedSequence : CCombatSequenceBase { bool IsRangedSequence() { return true; } };
struct SCountBlock;
struct CSequencePointer {
    CCombatSequenceBase* Object;
    SCountBlock*         Count;
};
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};
struct CRetailAllocator {
    CRetailAllocator() { }
};
static int g_destroyedSequences = 0;
CSequencePointer* CopyRange(CSequencePointer* first, CSequencePointer* last, CSequencePointer* result,
                            const random_access_iterator_tag&, int*)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}
void DestroyRange(CSequencePointer* first, CSequencePointer* last, const CRetailAllocator&)
{
    for (; first != last; ++first) {
        first->Object = 0;
        ++g_destroyedSequences;
    }
}
struct CSequenceVector {
    CSequencePointer* _M_start;
    CSequencePointer* _M_finish;
    CSequencePointer* _M_end_of_storage;
    CSequencePointer& operator[](unsigned long n) { return _M_start[n]; }
    CSequencePointer* begin() { return _M_start; }
    CSequencePointer* end() { return _M_finish; }
    CSequencePointer* erase(CSequencePointer* first, CSequencePointer* last)
    {
        CSequencePointer* i = CopyRange(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        DestroyRange(i, _M_finish, CRetailAllocator());
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
static unsigned __int64 g_erasedAttackerKey = 0;
struct CAttackerMap {
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    unsigned long size() const { return _M_node_count; }
    unsigned long erase(const unsigned __int64& key) { g_erasedAttackerKey = key; if (_M_node_count) { --_M_node_count; return 1; } return 0; }
};
class CAttackingCombatantInfo { public:
    CThingCreatureBase* Attacked;
    long  AttackerNumber;
    long  CombatWheelRingIndex;
    bool  PreferredAttackDollySet;
    long  PreferredAttackDollyTimer;
    float PreferredAttackDollyVector[2];
    bool  UsingCombatWheel;
    long  CurrentSequence;
    CSequenceVector CombatSequences;
};
class CDefendingCombatantInfo { public:
    CAttackerMap  Attackers;
    unsigned char CombatWheel[0x0c];
    float         LastVictimWheelUpdatePos[3];
    long          LastWheelUpdateWorldFrame;
    CIntelligentPointerThing PLastAttacker;
    void*         PAttackPattern;
    long          AttackPatternIndex;
    long          FrameForNextInstruction;
};
struct CCombatantInfo {
    CThingCreatureBase*     Parent;
    CAttackingCombatantInfo AttackerInfo;
    CDefendingCombatantInfo DefenderInfo;
};
struct SCombatantNode {
    char             _M_color;
    SCombatantNode*  _M_parent;
    SCombatantNode*  _M_left;
    SCombatantNode*  _M_right;
    unsigned __int64 first;
    CCombatantInfo   second;
};
static int g_combatantErases = 0;
static unsigned __int64 g_erasedCombatantKey = 0;
struct CCombatantMap {
    SCombatantNode* _M_header;                     // test: a flat node array
    unsigned long   _M_node_count;
    unsigned long   _M_key_compare;
    SCombatantNode* find(const unsigned __int64& key)
    {
        for (unsigned long i = 0; i < _M_node_count; ++i)
            if (_M_header[i].first == key)
                return &_M_header[i];
        return 0;
    }
    void erase(const unsigned __int64& key) { ++g_combatantErases; g_erasedCombatantKey = key; }
};

__forceinline void TraceDeregister(CCharString attackerName, const CCharString& attackedName, const CCombatantInfo& attackedInfo)
{
    CombatTrace("Deregistering combatant " + attackerName + " was attacking " + attackedName + " (who has " +
                GFIntToCharString(attackedInfo.DefenderInfo.Attackers.size() - 1) + " things still attacking)");
}
__forceinline void TraceDeregister(const CThing* attacker, CCharString attackedName, const CCombatantInfo& attackedInfo)
{
    TraceDeregister(g_DefStringTable.GetString(attacker->GetDefName()), attackedName, attackedInfo);
}
class CCombatManager {
public:
    virtual void v00() {}
    unsigned char _sequences[0x0c];
    CCombatantMap Combatants;
    // test recorders
    int                 m_patternCalls;
    CCombatantInfo*     m_patternInfo;
    CThingCreatureBase* m_unsheathCreature;
    long                m_unsheathWeapon;
    int                 m_updateCalls;
    CCombatantInfo*     m_updateInfo;
    bool                m_updateForce;
    CThingCreatureBase* m_deregisteredDefender;
    void IncrementDefenderAttackPattern(CCombatantInfo& info) { ++m_patternCalls; m_patternInfo = &info; }
    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long weapon) { m_unsheathCreature = creature; m_unsheathWeapon = weapon; }
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force) { ++m_updateCalls; m_updateInfo = &info; m_updateForce = force; }
    void DeregisterAttackedCombatant(CThingCreatureBase* attacked) { m_deregisteredDefender = attacked; }
    void DeregisterAttackingCombatant(CThingCreatureBase* attacker);
};

void CCombatManager::DeregisterAttackingCombatant(CThingCreatureBase* attacker)
{
    CCombatantInfo& info = Combatants.find(attacker->GetUID())->second;
    CThingCreatureBase* attacked = info.AttackerInfo.Attacked;
    CCombatantInfo& attackedInfo = Combatants.find(attacked->GetUID())->second;
    if (attackedInfo.DefenderInfo.PLastAttacker.GetPItem() == attacker) {
        attackedInfo.DefenderInfo.PLastAttacker = 0;
        if (info.AttackerInfo.CurrentSequence >= 0) {
            CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences[info.AttackerInfo.CurrentSequence].Object;
            if (sequence->IsMeleeSequence() || sequence->IsRangedSequence())
                IncrementDefenderAttackPattern(attackedInfo);
        }
    }
    TraceDeregister(attacker, g_DefStringTable.GetString(info.AttackerInfo.Attacked->GetDefName()), attackedInfo);
    attackedInfo.DefenderInfo.Attackers.erase(attacker->GetUID());
    if (attackedInfo.DefenderInfo.Attackers.size() == 0) {
        DeregisterAttackedCombatant(attacked);
    } else {
        CombatTrace("deregistered guy but defender is still has " + GFIntToCharString(attackedInfo.DefenderInfo.Attackers.size()) +
                    " attackers");
        UpdateAttackingCombatantsList(attackedInfo, true);
    }
    info.AttackerInfo.CombatSequences.clear();
    info.AttackerInfo.Attacked = 0;
    attacker->IsAttackingCombatant = 0;
    if (!attacker->IsCombatant()) {
        Combatants.erase(attacker->GetUID());
        CTCCarrying* carrying;
        if (attacker->GetConstInterface(&carrying, 0x46)) {
            CThing* item = carrying->GetThingInPrimarySlot();
            CTCWeapon* weapon;
            if (item && !(item->m_flags91 & 1) && item->GetInterface(&weapon, 0x48)) {
                if (weapon->IsProjectileWeapon())
                    weapon->ResetAiming();
            }
        }
        CCreatureDef* def = CDefPointer<CCreatureDef>(attacker->m_pDef).Get();
        if (def->Flags148 & 0x20)
            CreatureUnsheathWeapon(attacker, 3);
    }
    if (attacked->HasTC(0x36))
        attacked->GetTC<CTCStealth>(0x36)->SetAsInDanger(attacker, DANGER_NONE);
}

// ---------------------------------------------------------------------------------------------------------------------
static void ClearThing(CThing& thing, unsigned __int64 uid, long defName, CCreatureDef* def)
{
    memset(&thing, 0, sizeof(CThing));
    thing.UniqueID = uid;
    thing.m_defNameIndex = defName;
    thing.m_pDef = def;
}
static void AddInterface(CThing& thing, CInterfaceMap::SEntry* storage, long type, void* component)
{
    storage[0].type = type;
    storage[0].component = (CTCBase*)component;
    storage[1].type = 0x7fffffff;                  // sentinel past the end
    storage[1].component = 0;
    thing.m_interfaces.first = storage;
    thing.m_interfaces.last = storage + 1;
    thing.m_presentBits[type >> 5] |= 1UL << (type & 31);
}
static void SetupAttacker(SCombatantNode& node, CThingCreatureBase& attacker, CThingCreatureBase* attacked,
                          CSequencePointer* sequences, int sequenceCount, long current)
{
    node.first = attacker.UniqueID;
    node.second.Parent = &attacker;
    node.second.AttackerInfo.Attacked = attacked;
    node.second.AttackerInfo.CurrentSequence = current;
    node.second.AttackerInfo.CombatSequences._M_start = sequences;
    node.second.AttackerInfo.CombatSequences._M_finish = sequences + sequenceCount;
    node.second.AttackerInfo.CombatSequences._M_end_of_storage = sequences + sequenceCount;
    node.second.DefenderInfo.Attackers._M_node_count = 0;
    attacker.IsAttackingCombatant = 1;
}
static int Fail(int line)
{
    printf("006ed510_TEST FAIL\n");
    fprintf(stderr, "check failed at line %d\n", line);
    return 1;
}
#define CHECK(c) do { if (!(c)) return Fail(__LINE__); } while (0)

int main()
{
    CCreatureDef defA, defB, defC, defD;
    defA.RefCount = 1; defA.Flags148 = 0;
    defB.RefCount = 1; defB.Flags148 = 0x20;
    defC.RefCount = 1; defC.Flags148 = 0x00;
    defD.RefCount = 1; defD.Flags148 = 0;
    CThingCreatureBase archer, bandit, hobbe, balverine;
    ClearThing(archer, 100, 1, &defA);
    ClearThing(bandit, 200, 2, &defD);
    ClearThing(hobbe, 300, 3, &defB);
    ClearThing(balverine, 400, 4, &defC);
    archer.m_isCombatant = true;
    bandit.m_isCombatant = true;
    hobbe.m_isCombatant = false;
    balverine.m_isCombatant = false;

    CTCStealth stealth;
    stealth.m_source = 0; stealth.m_status = -1; stealth.m_calls = 0;
    CInterfaceMap::SEntry banditInterfaces[2];
    AddInterface(bandit, banditInterfaces, 0x36, &stealth);

    CThing bow, rustyBow;
    ClearThing(bow, 900, 0, 0);
    ClearThing(rustyBow, 901, 0, 0);
    rustyBow.m_flags91 = 1;                        // dead item
    CTCWeapon bowWeapon, rustyWeapon;
    bowWeapon.m_projectile = true; bowWeapon.m_resets = 0;
    rustyWeapon.m_projectile = true; rustyWeapon.m_resets = 0;
    CInterfaceMap::SEntry bowInterfaces[2], rustyInterfaces[2];
    AddInterface(bow, bowInterfaces, 0x48, &bowWeapon);
    AddInterface(rustyBow, rustyInterfaces, 0x48, &rustyWeapon);
    CTCCarrying hobbeCarrying, balverineCarrying;
    hobbeCarrying.m_primary = &bow;
    balverineCarrying.m_primary = &rustyBow;
    CInterfaceMap::SEntry hobbeInterfaces[2], balverineInterfaces[2];
    AddInterface(hobbe, hobbeInterfaces, 0x46, &hobbeCarrying);
    AddInterface(balverine, balverineInterfaces, 0x46, &balverineCarrying);

    CMeleeSequence melee;
    CRangedSequence ranged;
    CSequencePointer archerSequences[2], hobbeSequences[1], balverineSequences[2];
    archerSequences[0].Object = &ranged; archerSequences[0].Count = 0;
    archerSequences[1].Object = &melee;  archerSequences[1].Count = 0;
    hobbeSequences[0].Object = &melee;   hobbeSequences[0].Count = 0;
    balverineSequences[0].Object = &ranged; balverineSequences[0].Count = 0;
    balverineSequences[1].Object = &melee;  balverineSequences[1].Count = 0;

    SCombatantNode nodes[4];
    SetupAttacker(nodes[0], archer, &bandit, archerSequences, 2, 1);
    SetupAttacker(nodes[1], bandit, 0, 0, 0, -1);
    nodes[1].second.AttackerInfo.CombatSequences._M_start = 0;
    nodes[1].second.DefenderInfo.Attackers._M_node_count = 3;
    nodes[1].second.DefenderInfo.PLastAttacker.SetPItem(&archer);
    SetupAttacker(nodes[2], hobbe, &bandit, hobbeSequences, 1, -1);
    SetupAttacker(nodes[3], balverine, &bandit, balverineSequences, 2, 0);

    CCombatManager manager;
    manager.Combatants._M_header = nodes;
    manager.Combatants._M_node_count = 4;
    manager.m_patternCalls = 0; manager.m_patternInfo = 0;
    manager.m_unsheathCreature = 0; manager.m_unsheathWeapon = 0;
    manager.m_updateCalls = 0; manager.m_updateInfo = 0; manager.m_updateForce = false;
    manager.m_deregisteredDefender = 0;
    CCombatantInfo& defender = nodes[1].second;

    // 1. the archer (a combatant, current sequence melee, the defender's last attacker) leaves; 2 attackers stay
    manager.DeregisterAttackingCombatant(&archer);
    CHECK(manager.m_patternCalls == 1 && manager.m_patternInfo == &defender);
    CHECK(defender.DefenderInfo.PLastAttacker.GetPItem() == 0);
    CHECK(g_traceCount == 2);
    CHECK(strcmp(g_traces[0], "Deregistering combatant CREATURE_ARCHER was attacking CREATURE_BANDIT (who has 2 things still attacking)") == 0);
    CHECK(strcmp(g_traces[1], "deregistered guy but defender is still has 2 attackers") == 0);
    CHECK(defender.DefenderInfo.Attackers.size() == 2 && g_erasedAttackerKey == 100);
    CHECK(manager.m_updateCalls == 1 && manager.m_updateInfo == &defender && manager.m_updateForce);
    CHECK(manager.m_deregisteredDefender == 0);
    CHECK(nodes[0].second.AttackerInfo.CombatSequences._M_finish == archerSequences && g_destroyedSequences == 2);
    CHECK(nodes[0].second.AttackerInfo.Attacked == 0 && archer.IsAttackingCombatant == 0);
    CHECK(g_combatantErases == 0 && manager.m_unsheathCreature == 0);
    CHECK(stealth.m_calls == 1 && stealth.m_source == &archer && stealth.m_status == DANGER_NONE);
    CHECK(defA.RefCount == 1);

    // 2. the hobbe (not a combatant, no current sequence, not the last attacker) leaves; its live bow is reset and the
    //    def asks for an unsheathe
    defender.DefenderInfo.PLastAttacker.SetPItem(&archer);
    manager.DeregisterAttackingCombatant(&hobbe);
    CHECK(manager.m_patternCalls == 1);
    CHECK(defender.DefenderInfo.PLastAttacker.GetPItem() == &archer);
    CHECK(g_traceCount == 4);
    CHECK(strcmp(g_traces[2], "Deregistering combatant CREATURE_HOBBE was attacking CREATURE_BANDIT (who has 1 things still attacking)") == 0);
    CHECK(strcmp(g_traces[3], "deregistered guy but defender is still has 1 attackers") == 0);
    CHECK(manager.m_updateCalls == 2 && manager.m_deregisteredDefender == 0);
    CHECK(g_combatantErases == 1 && g_erasedCombatantKey == 300);
    CHECK(bowWeapon.m_resets == 1);
    CHECK(manager.m_unsheathCreature == &hobbe && manager.m_unsheathWeapon == 3);
    CHECK(defB.RefCount == 1 && g_defDestroyed == 0);
    CHECK(hobbe.IsAttackingCombatant == 0 && g_destroyedSequences == 3);
    CHECK(stealth.m_calls == 2 && stealth.m_source == &hobbe);

    // 3. the balverine (not a combatant, current sequence ranged, the last attacker) leaves the defender with nobody;
    //    its bow is dead, the def has no unsheathe flag, and the defender has lost its stealth component
    defender.DefenderInfo.PLastAttacker.SetPItem(&balverine);
    bandit.m_presentBits[0x36 >> 5] = 0;
    manager.m_unsheathCreature = 0;
    manager.DeregisterAttackingCombatant(&balverine);
    CHECK(manager.m_patternCalls == 2 && defender.DefenderInfo.PLastAttacker.GetPItem() == 0);
    CHECK(g_traceCount == 5);
    CHECK(strcmp(g_traces[4], "Deregistering combatant CREATURE_BALVERINE was attacking CREATURE_BANDIT (who has 0 things still attacking)") == 0);
    CHECK(defender.DefenderInfo.Attackers.size() == 0 && manager.m_deregisteredDefender == &bandit);
    CHECK(manager.m_updateCalls == 2);
    CHECK(g_combatantErases == 2 && g_erasedCombatantKey == 400);
    CHECK(rustyWeapon.m_resets == 0 && manager.m_unsheathCreature == 0);
    CHECK(defC.RefCount == 1 && g_defDestroyed == 0);
    CHECK(stealth.m_calls == 2);
    CHECK(g_destroyedSequences == 5);

    CHECK(g_liveStrings == 0);
    printf("006ed510_TEST PASS\n");
    return 0;
}