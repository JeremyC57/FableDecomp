// Behaviour test for CThingDeadCreature::WorldUpdateBulletTime (0x00836240): with component 0x83 present the graphic
// appearance is marked changed (reason 2) before the base bullet-time update; without it only the base update runs.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CThing { public:
    unsigned int m_presentBits[9];
    CThing() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; }
    virtual ~CThing() { }
    virtual void WorldUpdateBulletTime() { g_log[g_n++] = 100; }
    void SetGraphicAppearanceChanged(unsigned int flags) { g_log[g_n++] = (int)flags; }
    bool HasTC(long type) const { return (m_presentBits[type >> 5] & (1U << (type & 31))) != 0; }
};
class CThingGameObject : public CThing { };
class CThingDeadCreature : public CThingGameObject { public:
    virtual void WorldUpdateBulletTime()
    {
        if (HasTC(0x83))
            SetGraphicAppearanceChanged(2);
        CThingGameObject::WorldUpdateBulletTime();
    }
};
int main()
{
    CThingDeadCreature body;
    CThing& t = body;
    t.WorldUpdateBulletTime();
    if (g_n != 1 || g_log[0] != 100) { std::printf("00836240_TEST FAIL\n"); return 1; }
    body.m_presentBits[4] = 8;              // bit 0x83 = dword 4, bit 3 (retail `test byte [esi+30h],8`)
    g_n = 0; t.WorldUpdateBulletTime();
    if (g_n != 2 || g_log[0] != 2 || g_log[1] != 100) { std::printf("00836240_TEST FAIL\n"); return 1; }
    std::printf("00836240_TEST PASS\n");
    return 0;
}