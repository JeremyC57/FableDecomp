#include <cstdio>

struct CEventShownItem {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEventShownItem* g_last_dtor = 0;

void __fastcall CEventShownItem_dtor(CEventShownItem* event)
{
    g_last_dtor = event;
    event->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEventShownItem_scalar_deleting_destructor(CEventShownItem* event, int edx_unused, unsigned int flags)
{
    CEventShownItem_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}

int main()
{
    CEventShownItem a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEventShownItem_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEventShownItem b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEventShownItem_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}