#include <cstdio>
class CSimVoice { public: int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
extern "C" void __fastcall CSimVoice_dtor(CSimVoice* voice) { g_dtor_called++; (void)voice; }
extern "C" void __cdecl op_delete(void* p) { g_delete_called++; (void)p; }

void* __fastcall CSimVoice_scalar_deleting_destructor(CSimVoice* voice, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CSimVoice_dtor(voice);
    if (flags & 1) {
        op_delete(voice);
    }
    return voice;
}

int main() {
    CSimVoice obj; obj.f0 = 7;
    void* r1 = CSimVoice_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { std::printf("FAIL: ret mismatch flags=0\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor not called\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL: delete called when flag=0\n"); return 1; }
    void* r2 = CSimVoice_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { std::printf("FAIL: ret mismatch flags=1\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL: dtor not called 2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL: delete not called when flag=1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00455e90_TEST PASS\n");
    return 0;
}