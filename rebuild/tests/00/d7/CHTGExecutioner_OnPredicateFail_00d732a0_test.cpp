// Behaviour test for 0x00D732A0 CHTG_Executioner::OnPredicateFail: assigns "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED" (retail 0x012D29D4) to the
// CCharString at obj + 0x9c, then sets the byte flag at obj + 0x4b.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CCharString { public: CCharString const& operator=(char const*a0); };
struct Obj { char pad[0x9c]; };
struct CHTG_Executioner { char pad[0x14]; Obj* obj; };
void __fastcall CHTG_Executioner_OnPredicateFail(CHTG_Executioner* self);
static const void* g_this = 0; static const char* g_s = 0; static int g_calls = 0, g_flagAtCall = -1;
static unsigned char g_obj[0x100];
CCharString const& CCharString::operator=(char const* a0) { g_this = this; g_s = a0; ++g_calls; g_flagAtCall = g_obj[0x4b]; return *this; }
int main()
{
    CHTG_Executioner e; e.obj = (Obj*)g_obj;
    CHTG_Executioner_OnPredicateFail(&e);
    if (g_calls == 1 && g_this == (void*)(g_obj + 0x9c) && g_flagAtCall == 0 && g_obj[0x4b] == 1 && g_s != 0
        && std::strcmp(g_s, "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED") == 0) { std::printf("PARITY_OK\n"); return 0; }
    std::printf("FAIL calls=%d flag=%d s=%s\n", g_calls, g_obj[0x4b], g_s ? g_s : "(null)");
    return 1;
}
