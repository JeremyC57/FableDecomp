// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
// Leaf: GFGetPreMainMemoryUsed
extern int g_value_013babbc;
extern "C" int GetGlobal() { return g_value_013babbc; }