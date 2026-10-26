// Behaviour test for 0x00868690: CHitParameters::GetDamageToThing = 0 - (sum of the damage map's amounts); the thing is
// not consulted.
#include <cstdio>
#include <map>

class CThingGameObject { public: int Dummy; };
class CHitParameters
{ public:
    std::map<int, float> DamageMap;
    float GetDamageToThing(const CThingGameObject&) const
    {
        float damage = 0.0f;
        for (std::map<int, float>::const_iterator it = DamageMap.begin(); it != DamageMap.end(); ++it) damage -= it->second;
        return damage;
    }
};

int main()
{
    CHitParameters hit; CThingGameObject thing; thing.Dummy = 0;
    int fails = 0;
    if (hit.GetDamageToThing(thing) != 0.0f) ++fails;
    hit.DamageMap[0] = 10.0f; hit.DamageMap[3] = 2.5f; hit.DamageMap[1] = 0.5f;
    if (hit.GetDamageToThing(thing) != -13.0f) ++fails;
    if (fails) { std::printf("00868690_TEST FAIL\n"); return 1; }
    std::printf("00868690_TEST PASS\n");
    return 0;
}