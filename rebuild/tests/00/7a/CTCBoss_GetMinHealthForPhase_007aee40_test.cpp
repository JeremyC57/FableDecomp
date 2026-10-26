// Behaviour test for 007aee40 CTCBoss::GetMinHealthForPhase: percent * 0.01 * MaxHealth for a valid phase of a physical parent, else 0.
#include <cstdio>

template <class T>
struct CRetailVector
{
    T* _First; T* _Last; T* _End;
    unsigned int size() const { return (unsigned int)(_Last - _First); }
    const T& operator[](unsigned int i) const { return _First[i]; }
};

class CBossDef { public: CRetailVector<long> PhaseHealthPercent; };

class CThing { public: unsigned long ThingTypeFlags; };
class CThingPhysical : public CThing { public: float MaxHealth; float Health; };

class CTCBoss
{ public:
    CThing*         ParentThing;
    const CBossDef* PBossDef;

    float GetMinHealthForPhase(long phase) const
    {
        CThing* thing = ParentThing;
        if (thing->ThingTypeFlags & 4)
        {
            if (PBossDef != 0 && phase >= 0 && (unsigned int)phase < PBossDef->PhaseHealthPercent.size())
                return static_cast<CThingPhysical*>(thing)->MaxHealth * ((float)PBossDef->PhaseHealthPercent[phase] * 0.01f);
        }
        return 0.0f;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    long phases[2] = { 50, 25 };
    CBossDef def; def.PhaseHealthPercent._First = phases; def.PhaseHealthPercent._Last = phases + 2; def.PhaseHealthPercent._End = phases + 2;
    CThingPhysical thing; thing.ThingTypeFlags = 4; thing.MaxHealth = 200.0f; thing.Health = 150.0f;
    CTCBoss boss; boss.ParentThing = &thing; boss.PBossDef = &def;
    if (!Near(boss.GetMinHealthForPhase(0), 100.0f) || !Near(boss.GetMinHealthForPhase(1), 50.0f)) { std::printf("007aee40_TEST FAIL\n"); return 1; }
    if (boss.GetMinHealthForPhase(2) != 0.0f || boss.GetMinHealthForPhase(-1) != 0.0f) { std::printf("007aee40_TEST FAIL\n"); return 1; }
    thing.ThingTypeFlags = 0;
    if (boss.GetMinHealthForPhase(0) != 0.0f) { std::printf("007aee40_TEST FAIL\n"); return 1; }
    thing.ThingTypeFlags = 4; boss.PBossDef = 0;
    if (boss.GetMinHealthForPhase(0) != 0.0f) { std::printf("007aee40_TEST FAIL\n"); return 1; }
    std::printf("007aee40_TEST PASS\n");
    return 0;
}