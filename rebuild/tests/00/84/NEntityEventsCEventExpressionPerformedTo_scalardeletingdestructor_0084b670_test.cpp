#include <cstdio>

struct CEventExpressionPerformedTo {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEventExpressionPerformedTo* g_last_dtor = 0;

void __fastcall CEventExpressionPerformedTo_dtor(CEventExpressionPerformedTo* event)
{
    g_last_dtor = event;
    event->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEventExpressionPerformedTo_scalar_deleting_destructor(CEventExpressionPerformedTo* event, int edx_unused, unsigned int flags)
{
    CEventExpressionPerformedTo_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}

int main()
{
    CEventExpressionPerformedTo a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEventExpressionPerformedTo_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEventExpressionPerformedTo b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEventExpressionPerformedTo_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}