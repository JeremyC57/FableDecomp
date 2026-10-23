// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
extern int g_value_013b8760;
extern "C" int GetGlobal() { return g_value_013b8760; }