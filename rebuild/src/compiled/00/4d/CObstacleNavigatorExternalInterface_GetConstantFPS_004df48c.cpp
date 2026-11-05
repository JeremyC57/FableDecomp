// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
// Leaf: _CObstacleNavigatorExternalInterface_GetConstantFPS
extern int g_value;
extern "C" int CObstacleNavigatorExternalInterface_GetConstantFPS() { return g_value; }
// Stand-in (owner rule 2026-11-05): CObstacleNavigatorExternalInterface_GetConstantFPS for CObstacleNavigatorExternalInterface::GetConstantFPS; RTTI vtable slot ?GetConstantFPS@CObstacleNavigatorExternalInterface@@UBEJXZ + PDB name set (egor/fablewin).
