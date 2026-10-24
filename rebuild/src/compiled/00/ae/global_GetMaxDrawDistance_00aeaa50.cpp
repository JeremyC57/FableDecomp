// Float global getter: `fld dword ptr [g]; ret`. Free function, no `this`.
// Leaf: GetMaxDrawDistance
extern float g_value_01399514;
extern "C" float GetGlobal() { return g_value_01399514; }