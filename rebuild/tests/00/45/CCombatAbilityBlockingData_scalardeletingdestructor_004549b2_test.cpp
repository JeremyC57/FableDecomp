#include <cstdio>
static int g_dtor_called = 0;
static int g_delete_called = 0;
class CCombatAbilityBlockingData { public:
    int f0;
    int Parent;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
    ~CCombatAbilityBlockingData() { g_dtor_called++; }
};
extern "C" void __cdecl fable_operator_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CCombatAbilityBlockingData_scalar_deleting_destructor(CCombatAbilityBlockingData* blocking, int edx, unsigned int flags) {
    blocking->~CCombatAbilityBlockingData();
    if (flags & 1) {
        fable_operator_delete(blocking);
    }
    return blocking;
}

int main() {
    CCombatAbilityBlockingData obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CCombatAbilityBlockingData_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return not self\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL: dtor count %d\n", g_dtor_called); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL: delete should not be called for flags=0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    r = CCombatAbilityBlockingData_scalar_deleting_destructor(&obj, 0, 1);
    if (g_dtor_called != 1) { std::printf("FAIL: dtor count2 %d\n", g_dtor_called); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL: delete should be called for flags=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004549b2_TEST PASS\n");
    return 0;
}