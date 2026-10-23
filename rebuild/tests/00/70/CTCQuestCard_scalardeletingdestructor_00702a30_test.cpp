#include <cstdio>

struct CTCQuestCard {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCQuestCard* g_last_dtor = 0;

void __fastcall CTCQuestCard_dtor(CTCQuestCard* card)
{
    g_last_dtor = card;
    card->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCQuestCard_scalar_deleting_destructor(CTCQuestCard* card, int edx_unused, unsigned int flags)
{
    CTCQuestCard_dtor(card);
    if (flags & 1)
        operator delete(card);
    return card;
}

int main()
{
    CTCQuestCard a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCQuestCard_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCQuestCard b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCQuestCard_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00702a30_TEST PASS\n");
    return 0;
}