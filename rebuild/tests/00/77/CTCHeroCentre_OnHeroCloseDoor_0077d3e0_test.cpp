#include <cstdio>

struct CSingletonTarget { int called; };

static CSingletonTarget g_target = {0};
CSingletonTarget* g_singleton_13b8790 = &g_target;

extern "C" void __fastcall Singleton_Method_4374b0(CSingletonTarget* self) {
    self->called++;
}

class CTCHeroCentre { public:
    char pad[0x10];
    int CurrentGUIMode;
};

void __fastcall CTCHeroCentre__OnHeroCloseDoor(CTCHeroCentre* self);

int main() {
    CTCHeroCentre h;
    for (int i = 0; i < 0x10; ++i) h.pad[i] = 0;

    // Case 1: f10 == 1 -> reset to 0, call singleton
    h.CurrentGUIMode = 1;
    g_target.called = 0;
    CTCHeroCentre__OnHeroCloseDoor(&h);
    if (h.CurrentGUIMode != 0) { printf("FAIL f10 case1\n"); return 1; }
    if (g_target.called != 1) { printf("FAIL call case1\n"); return 1; }

    // Case 2: f10 == 2 -> no change, no call
    h.CurrentGUIMode = 2;
    g_target.called = 0;
    CTCHeroCentre__OnHeroCloseDoor(&h);
    if (h.CurrentGUIMode != 2) { printf("FAIL f10 case2\n"); return 1; }
    if (g_target.called != 0) { printf("FAIL call case2\n"); return 1; }

    // Case 3: f10 == 0 -> no change, no call
    h.CurrentGUIMode = 0;
    g_target.called = 0;
    CTCHeroCentre__OnHeroCloseDoor(&h);
    if (h.CurrentGUIMode != 0) { printf("FAIL f10 case3\n"); return 1; }
    if (g_target.called != 0) { printf("FAIL call case3\n"); return 1; }

    printf("0077d3e0_TEST PASS\n");
    return 0;
}