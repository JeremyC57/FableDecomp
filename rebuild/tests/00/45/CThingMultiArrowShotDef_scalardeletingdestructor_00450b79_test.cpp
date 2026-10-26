#include <cstdio>

static int g_dtor_called = 0;
static int g_free_called = 0;
const void* CThingMultiArrowShotDef_vftable = (const void*)0x1230ba0;

class CThingMultiArrowShotDef { public:
    const void** vft;
};

extern "C" void __cdecl FableFreeMem(void* p) { (void)p; g_free_called++; }

void __fastcall CThingMultiArrowShotDef_dtor(CThingMultiArrowShotDef* def) { (void)def; g_dtor_called++; }

void* __fastcall CThingMultiArrowShotDef_scalar_deleting_destructor(CThingMultiArrowShotDef* def, int edx, unsigned int flags) {
    (void)edx;
    def->vft = (const void**)&CThingMultiArrowShotDef_vftable;
    CThingMultiArrowShotDef_dtor(def);
    if (flags & 1) {
        FableFreeMem(def);
    }
    return def;
}

int main() {
    CThingMultiArrowShotDef obj;
    obj.vft = 0;
    void* r = CThingMultiArrowShotDef_scalar_deleting_destructor(&obj, 0, 0u);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_free_called != 0) { std::printf("FAIL free0\n"); return 1; }
    if (obj.vft != (const void**)&CThingMultiArrowShotDef_vftable) { std::printf("FAIL vft\n"); return 1; }
    void* r2 = CThingMultiArrowShotDef_scalar_deleting_destructor(&obj, 0, 1u);
    if (r2 != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_free_called != 1) { std::printf("FAIL free1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00450b79_TEST PASS\n");
    return 0;
}