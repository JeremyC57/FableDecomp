// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
// Leaf: _CGroundNavigatorExternalInterface_GetConstantFPS
extern int g_value;
extern "C" int CGroundNavigatorExternalInterface_GetConstantFPS() { return g_value; }
// Stand-in (owner rule 2026-11-05): CGroundNavigatorExternalInterface_GetConstantFPS for CGroundNavigatorExternalInterface::GetConstantFPS; RTTI vtable slot ?GetConstantFPS@CGroundNavigatorExternalInterface@@UBEJXZ + PDB name set (egor/fablewin).
