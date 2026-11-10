// Behaviour test for 0x00868860 CHitParameters::IsMeleeStrike() const: true exactly when the damage map holds a
// DAMAGE_MELEE or a DAMAGE_MELEE_UNARMED entry.
#include <stdio.h>
#include <map>
enum EDamageAttribute { DAMAGE_NULL = -1, DAMAGE_MELEE = 0, DAMAGE_MELEE_UNARMED = 1, DAMAGE_LIGHTNING = 2, DAMAGE_FIRE = 3 };
class CHitParameters {
public:
    std::map<EDamageAttribute, float> DamageMap;
    bool IsMeleeStrike() const;
};
bool CHitParameters::IsMeleeStrike() const
{
    typedef std::map<EDamageAttribute, float>::const_iterator const_iterator;
    const_iterator end;
    {
        const_iterator melee = DamageMap.find(DAMAGE_MELEE);
        end = DamageMap.end();
        if (melee != end)
            return true;
    }
    {
        const_iterator unarmed = DamageMap.find(DAMAGE_MELEE_UNARMED);
        return unarmed != end ? true : false;
    }
}
int main()
{
    CHitParameters hit;
    bool ok = !hit.IsMeleeStrike();
    hit.DamageMap[DAMAGE_FIRE] = 3.0f;
    ok = ok && !hit.IsMeleeStrike();
    hit.DamageMap[DAMAGE_MELEE_UNARMED] = 1.0f;
    ok = ok && hit.IsMeleeStrike();
    hit.DamageMap.erase(DAMAGE_MELEE_UNARMED);
    hit.DamageMap[DAMAGE_MELEE] = 2.0f;
    ok = ok && hit.IsMeleeStrike();
    if (!ok) { printf("00868860_TEST FAIL\n"); return 1; }
    printf("00868860_TEST PASS\n");
    return 0;
}
