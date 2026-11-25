// Behaviour test for 0x00438570 CPlayerGui::PeekGuiDef: on first use builds the def from the name "PLAYER_GUI_PC" (retail 0x0123173C), length -1
// (InitGuard = the CCharString(const char*, long) stand-in), then returns the cached def without rebuilding.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CPlayerGuiDef { public: int v; };
struct InitGuard { void *slot; InitGuard(int *p, int n); ~InitGuard(); };
struct Builder { Builder *Build(); };
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };
CPlayerGuiDef *g_playerGuiDef = 0;
static CPlayerGuiDef g_def; static Builder g_builder;
static const char* g_s = 0; static int g_n = 0, g_ctor = 0, g_dtor = 0, g_acq = 0, g_build = 0; static InitGuard* g_guard = 0;
InitGuard::InitGuard(int *p, int n) { g_s = (const char*)p; g_n = n; ++g_ctor; g_guard = this; slot = 0; }
InitGuard::~InitGuard() { ++g_dtor; }
Builder *__stdcall AcquireBuilder(InitGuard *out, CPlayerGuiDef **slot) { ++g_acq; if (out != g_guard || slot != &g_playerGuiDef) g_acq += 100; *slot = &g_def; return &g_builder; }
Builder *Builder::Build() { ++g_build; return this; }
int main()
{
    const CPlayerGuiDef& d = CPlayerGui::PeekGuiDef();
    if (&d != &g_def || g_ctor != 1 || g_dtor != 1 || g_acq != 1 || g_build != 1 || g_n != -1 || g_s == 0 || std::strcmp(g_s, "PLAYER_GUI_PC") != 0) {
        std::printf("FAIL first ctor=%d acq=%d s=%s\n", g_ctor, g_acq, g_s ? g_s : "(null)"); return 1;
    }
    const CPlayerGuiDef& d2 = CPlayerGui::PeekGuiDef();
    if (&d2 != &g_def || g_ctor != 1 || g_acq != 1) { std::printf("FAIL cached\n"); return 1; }
    std::printf("_global_00438570_TEST PASS\n");
    return 0;
}
