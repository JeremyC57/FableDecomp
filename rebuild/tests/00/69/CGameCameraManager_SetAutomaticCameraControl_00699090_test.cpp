// Behaviour test for 0x00699090 CGameCameraManager::SetAutomaticCameraControl: flag 0x12c clear -> name "CAMERA_LIST_PUSH_ERROR" (retail 0x0125C304)
// and return; set -> name "SET_AUTO_CONTROL" (0x0125C360), PushCurrentManualMode, clear both flags. ScopedGuard = the CCharString(const char*, long)
// stand-in (length -1); Sub100::get / Inner_call_edx get the guard and h134.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
struct ScopedGuard { void* p; ScopedGuard(const char* s, int n); ~ScopedGuard(); };
struct Inner { char x; };
struct Sub100 { Inner* get(ScopedGuard* g); };
class CGameCameraManager { public:
    char pad[0x100];
    Sub100 sub100;
    char pad1[0x2c - sizeof(Sub100)];
    char flag12c;
    char flag12d;
    char pad2[6];
    int  h134;
    void PushCurrentManualMode();
    void SetAutomaticCameraControl();
};
static int g_ctor = 0, g_dtor = 0, g_get = 0, g_inner = 0, g_push = 0, g_edx = 0, g_n = 0; static const char* g_s = 0; static ScopedGuard* g_guard = 0;
static Inner g_innerObj;
ScopedGuard::ScopedGuard(const char* s, int n) { p = (void*)s; g_s = s; g_n = n; ++g_ctor; g_guard = this; }
ScopedGuard::~ScopedGuard() { ++g_dtor; }
Inner* Sub100::get(ScopedGuard* g) { ++g_get; if (g != g_guard) g_get += 100; return &g_innerObj; }
extern "C" void __fastcall Inner_call_edx(Inner* self, int edx) { ++g_inner; g_edx = edx; if (self != &g_innerObj) g_inner += 100; }
void CGameCameraManager::PushCurrentManualMode() { ++g_push; }
int main()
{
    static CGameCameraManager m; std::memset(&m, 0, sizeof(m)); m.h134 = 0x1234;
    m.flag12c = 0; m.SetAutomaticCameraControl();
    if (!(g_ctor == 1 && g_dtor == 1 && g_get == 1 && g_inner == 1 && g_push == 0 && g_edx == 0x1234 && m.flag12c == 0 && g_n == -1
          && g_s && std::strcmp(g_s, "CAMERA_LIST_PUSH_ERROR") == 0)) { std::printf("FAIL branch0\n"); return 1; }
    m.flag12c = 1; m.flag12d = 7; m.SetAutomaticCameraControl();
    if (!(g_ctor == 2 && g_dtor == 2 && g_get == 2 && g_inner == 2 && g_push == 1 && m.flag12c == 0 && m.flag12d == 0 && g_n == -1
          && g_s && std::strcmp(g_s, "SET_AUTO_CONTROL") == 0)) { std::printf("FAIL branch1\n"); return 1; }
    std::printf("CGameCameraManager_00699090_TEST PASS\n");
    return 0;
}
