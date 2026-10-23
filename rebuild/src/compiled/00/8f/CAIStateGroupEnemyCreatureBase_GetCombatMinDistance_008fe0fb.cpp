// Float global getter: `fld dword ptr [g]; ret`. Free function, no `this`.
extern float g_value_0125aadc;
extern "C" float GetGlobal() { return g_value_0125aadc; }