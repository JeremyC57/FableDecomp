// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
// Leaf: _GetGlobal
extern int g_value_0143e920;
extern "C" int GetGlobal() { return g_value_0143e920; }