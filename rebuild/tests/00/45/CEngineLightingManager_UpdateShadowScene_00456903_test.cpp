#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

struct Vec12 { int a, b, c; };

class CEngineLightingManager { public:
    char pad[0x18];
    int mode;        // +0x18
    char pad2[0x24-0x18-4];
    void* obj24;     // +0x24
    void* obj28;     // +0x28
};

struct PBox { Vec12* p; };

static Vec12 g_ret = {11,22,33};
static Vec12* g_h3_arg = 0;
static void* g_h3_self = 0;
static int g_prof = 0;
static const char* g_profText = 0;

void __stdcall Prof_Enter(const char* s) { g_profText = s; g_prof++; }
void __fastcall LM_Handle3(void* self28, PBox p) { g_h3_self = self28; g_h3_arg = p.p; }
Vec12 __fastcall LM_Handle2(void* self24) { return g_ret; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, Vec12* param)
{
    Prof_Enter("");
    switch (self->mode) {
    case 2:
        *param = LM_Handle2(self->obj24);
        break;
    case 3: {
        PBox b; b.p = param;
        LM_Handle3(self->obj28, b);
        break;
    }
    }
}

int main() {
    CEngineLightingManager lm;
    for (unsigned i=0;i<sizeof(lm);++i) ((char*)&lm)[i]=0;
    int dummy24=0, dummy28=0;
    lm.obj24 = &dummy24; lm.obj28 = &dummy28;
    Vec12 out = {0,0,0};

    // mode==2 : copies g_ret into param
    lm.mode = 2;
    CEngineLightingManager_UpdateShadowScene(&lm, 0, &out);
    if (out.a!=11 || out.b!=22 || out.c!=33) { std::printf("FAIL mode2\n"); return 1; }

    // mode==3 : calls handle3 with param ptr and obj28
    lm.mode = 3; g_h3_arg=0; g_h3_self=0;
    Vec12 p3 = {5,6,7};
    CEngineLightingManager_UpdateShadowScene(&lm, 0, &p3);
    if (g_h3_arg != &p3 || g_h3_self != &dummy28) { std::printf("FAIL mode3\n"); return 1; }

    // mode==other : no handler
    lm.mode = 9; g_h3_arg=0;
    CEngineLightingManager_UpdateShadowScene(&lm, 0, &out);
    if (g_h3_arg != 0) { std::printf("FAIL mode-other\n"); return 1; }

    if (g_prof != 3 || !RetailTextEquals(g_profText, "")) { std::printf("FAIL prof\n"); return 1; }
    std::printf("CEngineLightingManager_00456903_TEST PASS\n");
    return 0;
}