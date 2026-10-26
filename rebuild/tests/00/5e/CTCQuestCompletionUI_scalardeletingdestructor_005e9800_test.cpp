#include <cstdio>

class CTCQuestCompletionUI { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCQuestCompletionUI* g_last_dtor = 0;

void __fastcall CTCQuestCompletionUI_dtor(CTCQuestCompletionUI* completionUI)
{
    g_last_dtor = completionUI;
    completionUI->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCQuestCompletionUI_scalar_deleting_destructor(CTCQuestCompletionUI* completionUI, int edx_unused, unsigned int flags)
{
    CTCQuestCompletionUI_dtor(completionUI);
    if (flags & 1)
        operator delete(completionUI);
    return completionUI;
}

int main()
{
    CTCQuestCompletionUI a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCQuestCompletionUI_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCQuestCompletionUI b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCQuestCompletionUI_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_005e9800_TEST PASS\n");
    return 0;
}