// CAIStateGroup_EnemyCreatureBase::GetCombatProximity @ 0x008FE0F4
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

float GetCombatProximity()
{
    return g_CombatMinDistance;
}