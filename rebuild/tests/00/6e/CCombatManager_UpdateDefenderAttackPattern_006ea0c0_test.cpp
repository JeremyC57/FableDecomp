// Behaviour test for CCombatManager::UpdateDefenderAttackPattern 0x006EA0C0 through a copy of the model: the first attacker whose
// combat type maps the defender's creature type names the pattern; a non-zero pattern that differs from the defender's own def index
// is fetched and the defender is reset onto it (the def handle add-ref'd for the by-value argument and released afterwards).
#include <cstdio>

enum ECombatCreatureType { CCT_NONE, CCT_HUMAN, CCT_BALVERINE, CCT_TROLL };
struct SPatternEntry { ECombatCreatureType Key; long Pattern; };
class CCombatTypeDef { public:
    const SPatternEntry* AttackPatterns; int NoAttackPatterns; ECombatCreatureType CombatCreatureType;
    const SPatternEntry* find(ECombatCreatureType key) const
    {
        for (int i = 0; i < NoAttackPatterns; ++i)
            if (AttackPatterns[i].Key == key)
                return &AttackPatterns[i];
        return end();
    }
    const SPatternEntry* end() const { return AttackPatterns + NoAttackPatterns; }
};
class CThingCreatureBase { public:
    const CCombatTypeDef* Def;
    const CCombatTypeDef& GetCombatTypeDef() const { return *Def; }
};
class CAttackPatternDef { public: long RefCount; long GlobalIndex; long Deleted; };
struct CDefPointer {
    CAttackPatternDef* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Deleted = 1; }
};
struct CCombatantInfo {
    CThingCreatureBase* Parent; CThingCreatureBase** Attackers; int NoAttackers; long OwnPatternIndex;
    long ResetCount; long ResetIndex; long RefCountSeenInReset;
};
static CAttackPatternDef g_defs[8];
class CCombatManager { public:
    long GetDefCalls;
    bool GetDef(long index, CDefPointer* out)
    {
        ++GetDefCalls;
        if (index <= 0 || index >= 8)
            return false;
        out->Object = &g_defs[index];
        ++g_defs[index].RefCount;
        return true;
    }
    void ResetDefenderAttackPattern(CCombatantInfo& info, CDefPointer pattern)
    {
        ++info.ResetCount;
        info.ResetIndex = pattern.Object ? pattern.Object->GlobalIndex : -1;
        info.RefCountSeenInReset = pattern.Object ? pattern.Object->RefCount : -1;
    }
    void UpdateDefenderAttackPattern(CCombatantInfo& info)
    {
        long pattern = 0;
        long own_index = info.OwnPatternIndex;
        ECombatCreatureType creature_type = info.Parent->GetCombatTypeDef().CombatCreatureType;
        for (int i = 0; i != info.NoAttackers; ++i) {
            CThingCreatureBase* attacker = info.Attackers[i];
            const CCombatTypeDef& def = attacker->GetCombatTypeDef();
            const SPatternEntry* found = def.find(creature_type);
            if (found != def.end() && pattern == 0)
                pattern = found->Pattern;
        }
        if (pattern != 0 && pattern != own_index) {
            CDefPointer p;
            GetDef(pattern, &p);
            ResetDefenderAttackPattern(info, p);
        }
    }
};

int main()
{
    int i;
    for (i = 0; i < 8; ++i) { g_defs[i].RefCount = 0; g_defs[i].GlobalIndex = i; g_defs[i].Deleted = 0; }
    static const SPatternEntry troll_patterns[] = { { CCT_BALVERINE, 5 }, { CCT_TROLL, 6 } };
    static const SPatternEntry human_patterns[] = { { CCT_HUMAN, 3 } };
    static const SPatternEntry zero_patterns[] = { { CCT_HUMAN, 0 } };
    static const SPatternEntry late_patterns[] = { { CCT_HUMAN, 4 } };
    CCombatTypeDef hero_def = { 0, 0, CCT_HUMAN };
    CCombatTypeDef troll_def = { troll_patterns, 2, CCT_TROLL };
    CCombatTypeDef bandit_def = { human_patterns, 1, CCT_HUMAN };
    CCombatTypeDef zero_def = { zero_patterns, 1, CCT_HUMAN };
    CCombatTypeDef late_def = { late_patterns, 1, CCT_HUMAN };
    CThingCreatureBase hero = { &hero_def }, troll = { &troll_def }, bandit = { &bandit_def }, zero = { &zero_def }, late = { &late_def };
    CCombatManager manager; manager.GetDefCalls = 0;
    bool ok = true;

    // 1: no attackers -> nothing happens.
    CCombatantInfo info = { &hero, 0, 0, 2, 0, 0, 0 };
    manager.UpdateDefenderAttackPattern(info);
    ok = ok && info.ResetCount == 0 && manager.GetDefCalls == 0;

    // 2: attackers without a mapping for the defender's type -> nothing happens.
    CThingCreatureBase* only_troll[] = { &troll };
    info.Attackers = only_troll; info.NoAttackers = 1;
    manager.UpdateDefenderAttackPattern(info);
    ok = ok && info.ResetCount == 0 && manager.GetDefCalls == 0;

    // 3: a zero mapping is skipped, the first non-zero one wins (3, not the later 4); the handle is held twice during the reset
    //    and fully released afterwards.
    CThingCreatureBase* mixed[] = { &troll, &zero, &bandit, &late };
    info.Attackers = mixed; info.NoAttackers = 4;
    manager.UpdateDefenderAttackPattern(info);
    ok = ok && info.ResetCount == 1 && info.ResetIndex == 3 && info.RefCountSeenInReset == 2 && manager.GetDefCalls == 1;
    ok = ok && g_defs[3].RefCount == 0 && g_defs[3].Deleted == 1 && g_defs[4].RefCount == 0 && g_defs[4].Deleted == 0;

    // 4: the pattern the defender already runs is left alone.
    info.OwnPatternIndex = 3; info.ResetCount = 0;
    manager.UpdateDefenderAttackPattern(info);
    ok = ok && info.ResetCount == 0 && manager.GetDefCalls == 1;

    // 5: order matters -- with the later attacker first its pattern (4) is the one chosen.
    CThingCreatureBase* reordered[] = { &late, &bandit };
    info.Attackers = reordered; info.NoAttackers = 2;
    manager.UpdateDefenderAttackPattern(info);
    ok = ok && info.ResetCount == 1 && info.ResetIndex == 4 && manager.GetDefCalls == 2 && g_defs[4].Deleted == 1;

    if (!ok) { std::printf("006ea0c0_TEST FAIL\n"); return 1; }
    std::printf("006ea0c0_TEST PASS\n");
    return 0;
}