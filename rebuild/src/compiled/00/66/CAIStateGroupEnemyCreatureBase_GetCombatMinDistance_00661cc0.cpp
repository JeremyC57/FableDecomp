// Float global getter: `fld dword ptr [g]; ret`. Free function, no `this`.
// Leaf: _GetGlobal
extern float g_value_0125aab8;
extern "C" float GetGlobal() { return g_value_0125aab8; }