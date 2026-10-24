// Behaviour test for 0x00865E40: IsHitToDaze = health below the dazed threshold AND decapitation component (0xD1)
// present AND killable.
#include <cstdio>

class CThingCreatureBase
{
public:
    unsigned int  m_presentBits[9];
    float         Health;
    unsigned char Damageable : 1;
    unsigned char Killable : 1;
    float         m_dazeThreshold;
    float GetDazedHealthThreshold() const { return m_dazeThreshold; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
            return true;
        return false;
    }
};

bool IsHitToDaze(const CThingCreatureBase& creature)
{
    if (creature.Health < creature.GetDazedHealthThreshold() && creature.HasTC(0xD1) && creature.Killable)
        return true;
    return false;
}

int main()
{
    CThingCreatureBase c;
    for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0;
    c.m_presentBits[6] = 0x20000;                       // type 0xD1 = word 6 bit 17
    c.Health = 5.0f; c.m_dazeThreshold = 8.0f; c.Damageable = 0; c.Killable = 1;
    int fails = 0;
    if (!IsHitToDaze(c)) ++fails;
    c.Health = 9.0f;            if (IsHitToDaze(c)) ++fails;
    c.Health = 5.0f; c.Killable = 0; if (IsHitToDaze(c)) ++fails;
    c.Killable = 1; c.m_presentBits[6] = 0; if (IsHitToDaze(c)) ++fails;
    if (fails) { std::printf("00865e40_TEST FAIL\n"); return 1; }
    std::printf("00865e40_TEST PASS\n");
    return 0;
}