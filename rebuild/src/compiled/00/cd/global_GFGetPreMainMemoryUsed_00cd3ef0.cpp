// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
extern int g_value_0143e920;
extern "C" int GetGlobal() { return g_value_0143e920; }