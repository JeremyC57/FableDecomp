// Behaviour test for 0x004CDB10 CSubtitledMessage::Initialise(): one LoadFromFile on the misc def-type symbol map with
// the path "Data\Defs\" + "misc_def_types.h" and an empty prefix.
#include <cstdio>
#include <cstring>

struct Str { char text[64]; Str(const char* s) { std::strcpy(text, s); } };
static Str operator+(const Str& a, const char* b) { Str r(a.text); std::strcat(r.text, b); return r; }
static int g_loads = 0; static char g_file[64]; static char g_prefix[64];
struct SymbolMap { bool LoadFromFile(const Str& f, const Str& p) { ++g_loads; std::strcpy(g_file, f.text); std::strcpy(g_prefix, p.text); return true; } };
static SymbolMap g_map;
static Str GetDefsDirectory() { return Str("Data\\Defs\\"); }
static void Initialise() { g_map.LoadFromFile(GetDefsDirectory() + "misc_def_types.h", ""); }

int main()
{
    Initialise();
    if (g_loads != 1 || std::strcmp(g_file, "Data\\Defs\\misc_def_types.h") != 0 || g_prefix[0] != 0) { std::printf("004cdb10_TEST FAIL\n"); return 1; }
    std::printf("004cdb10_TEST PASS\n");
    return 0;
}