#include <cstdio>

struct CTCWillResponse {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWillResponse* g_last_dtor = 0;

void __fastcall CTCWillResponse_dtor(CTCWillResponse* response)
{
    g_last_dtor = response;
    response->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWillResponse_scalar_deleting_destructor(CTCWillResponse* response, int edx_unused, unsigned int flags)
{
    CTCWillResponse_dtor(response);
    if (flags & 1)
        operator delete(response);
    return response;
}

int main()
{
    CTCWillResponse a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWillResponse_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWillResponse b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWillResponse_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}