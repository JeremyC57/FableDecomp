// Float global getter: `fld dword ptr [g]; ret`. Free function, no `this`.
// Leaf: GetCombatMinDistance
extern float g_value_0129f19c;
extern "C" float GetGlobal() { return g_value_0129f19c; }