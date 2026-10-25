// Behaviour test for CWorld::SetCreatureGenerationGroupAsEnabled (0x0049D930): enabling clears the group's bits in the
// disabled mask, disabling sets them, other bits are untouched.
#include <cstdio>

enum ECreatureGroup { CREATURE_GROUP_NONE, CREATURE_GROUP_A = 0x4, CREATURE_GROUP_B = 0x30 };

class CWorld {
public:
    void SetCreatureGenerationGroupAsEnabled(ECreatureGroup group, bool enabled);
    unsigned char _pad_0x00[0xd0];
    unsigned long CreatureGenerationDisabledGroups;
};

void CWorld::SetCreatureGenerationGroupAsEnabled(ECreatureGroup group, bool enabled)
{
    if (enabled)
        CreatureGenerationDisabledGroups &= ~group;
    else
        CreatureGenerationDisabledGroups |= group;
}

int main()
{
    CWorld w;
    w.CreatureGenerationDisabledGroups = 0x101;
    w.SetCreatureGenerationGroupAsEnabled(CREATURE_GROUP_B, false);
    bool ok = w.CreatureGenerationDisabledGroups == 0x131;
    w.SetCreatureGenerationGroupAsEnabled(CREATURE_GROUP_A, false);
    ok = ok && w.CreatureGenerationDisabledGroups == 0x135;
    w.SetCreatureGenerationGroupAsEnabled(CREATURE_GROUP_B, true);
    ok = ok && w.CreatureGenerationDisabledGroups == 0x105;
    w.SetCreatureGenerationGroupAsEnabled(CREATURE_GROUP_B, true);
    ok = ok && w.CreatureGenerationDisabledGroups == 0x105;
    if (!ok) { std::printf("0049d930_TEST FAIL\n"); return 1; }
    std::printf("0049d930_TEST PASS\n");
    return 0;
}