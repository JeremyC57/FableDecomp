// behaviour test for CThingCompare_BestCreatureToAttack::GetCompareValueForThing<CThingAICreature> (0x007F6BF0): 1/(sqrt(2D dist)+1) and the four weights
#include <cstdio>
#include <cmath>
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} float GetLength() const { return (float)std::sqrt(Y * Y + X * X); } };
struct CThing {
    unsigned long ThingTypeFlags; C3DVector m_pos;
    virtual ~CThing() {}
    const C3DVector& GetPosition() const { return m_pos; }
};
template<class T> inline bool IsThingOfType(const CThing* const& thing) { unsigned long mask = T::ThingTypeMask; return (thing->ThingTypeFlags & mask) == mask; }
struct CThingCreatureBase;
struct CIntelligentPointer { CThingCreatureBase* m_p; CThingCreatureBase* GetTypedItem() const { return m_p; } };
struct CThingCreatureBase : CThing {
    bool m_player, m_combatant; CIntelligentPointer PLastAttackedByCreature;
    virtual bool IsPlayer() const { return m_player; }
    bool IsCombatant() { return m_combatant; }
    CThingCreatureBase* GetLastAttackedByCreature() const { return PLastAttackedByCreature.GetTypedItem(); }
};
struct CThingAICreature : CThingCreatureBase { enum { ThingTypeMask = 8 }; };
struct CThingCompare_BestCreatureToAttack {
    CThingCreatureBase* Targeter; CThing* PCurrentBestTargetForAttack;
    float PreferPlayerWeight, PreferNonCombatantsWeight, PreferLastAttackerWeight, PreferCurrentBestEnemyWeight;
    template<class T> float GetCompareValueForThing(T* thing) const
    {
        CThingAICreature* creature = 0;
        if (IsThingOfType<CThingAICreature>(thing)) creature = (CThingAICreature*)thing;
        const C3DVector& targeterPos = Targeter->GetPosition();
        const C3DVector& thingPos = thing->GetPosition();
        C2DVector delta(thingPos.X - targeterPos.X, thingPos.Y - targeterPos.Y);
        float distance = delta.GetLength();
        float value = 1.0f / ((float)std::sqrt(distance) + 1.0f);
        if (thing->IsPlayer()) value *= PreferPlayerWeight;
        if (creature) {
            if (!creature->IsCombatant()) value *= PreferNonCombatantsWeight;
            if (Targeter->GetLastAttackedByCreature() == creature) value *= PreferLastAttackerWeight;
        }
        if (PCurrentBestTargetForAttack && PCurrentBestTargetForAttack == thing) value *= PreferCurrentBestEnemyWeight;
        return value;
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    CThingAICreature me, other;
    me.ThingTypeFlags = 8; me.m_pos.X = 1.0f; me.m_pos.Y = 1.0f; me.m_pos.Z = 50.0f; me.m_player = false; me.m_combatant = true; me.PLastAttackedByCreature.m_p = 0;
    other.ThingTypeFlags = 8; other.m_pos.X = 4.0f; other.m_pos.Y = 5.0f; other.m_pos.Z = -7.0f; other.m_player = false; other.m_combatant = true; other.PLastAttackedByCreature.m_p = 0;
    CThingCompare_BestCreatureToAttack c; c.Targeter = &me; c.PCurrentBestTargetForAttack = 0;
    c.PreferPlayerWeight = 2.0f; c.PreferNonCombatantsWeight = 3.0f; c.PreferLastAttackerWeight = 5.0f; c.PreferCurrentBestEnemyWeight = 7.0f;
    CThingAICreature* t = &other;
    float base = 1.0f / ((float)std::sqrt(5.0f) + 1.0f);           // 2D distance 5 (z ignored)
    int bad = !Near(c.GetCompareValueForThing(t), base);
    other.m_player = true;  bad |= !Near(c.GetCompareValueForThing(t), base * 2.0f);
    other.m_combatant = false; bad |= !Near(c.GetCompareValueForThing(t), base * 6.0f);
    me.PLastAttackedByCreature.m_p = &other; bad |= !Near(c.GetCompareValueForThing(t), base * 30.0f);
    c.PCurrentBestTargetForAttack = &other; bad |= !Near(c.GetCompareValueForThing(t), base * 210.0f);
    other.ThingTypeFlags = 1;   // not an AI creature: combatant and last-attacker weights skipped
    bad |= !Near(c.GetCompareValueForThing(t), base * 14.0f);
    if (bad) { std::printf("007f6bf0_TEST FAIL\n"); return 1; }
    std::printf("007f6bf0_TEST PASS\n");
    return 0;
}