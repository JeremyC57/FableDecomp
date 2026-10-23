#include <cstdio>

struct CAIStateGroup_EnemyCreatureBase;
float g_CombatMinDistance_0122f59c = 3.5f;

float __fastcall GetCombatMinDistance(CAIStateGroup_EnemyCreatureBase* self);

int main()
{
    g_CombatMinDistance_0122f59c = 7.25f;
    float v = GetCombatMinDistance((CAIStateGroup_EnemyCreatureBase*)0);
    if (v == 7.25f)
        printf("00a76320_TEST PASS\n");
    else
        printf("FAIL %f\n", v);
    return 0;
}