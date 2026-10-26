#include <cstdio>

class CTCInventoryQuests { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCInventoryQuests* g_last_dtor = 0;

void __fastcall CTCInventoryQuests_dtor(CTCInventoryQuests* quests)
{
    g_last_dtor = quests;
    quests->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCInventoryQuests_scalar_deleting_destructor(CTCInventoryQuests* quests, int edx_unused, unsigned int flags)
{
    CTCInventoryQuests_dtor(quests);
    if (flags & 1)
        operator delete(quests);
    return quests;
}

int main()
{
    CTCInventoryQuests a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCInventoryQuests_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCInventoryQuests b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCInventoryQuests_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00625090_TEST PASS\n");
    return 0;
}