#include <cstdio>

struct CLHFileCompletionCallback {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CLHFileCompletionCallback* g_last_dtor = 0;

void __fastcall CLHFileCompletionCallback_dtor(CLHFileCompletionCallback* callback)
{
    g_last_dtor = callback;
    callback->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CLHFileCompletionCallback_scalar_deleting_destructor(CLHFileCompletionCallback* callback, int edx_unused, unsigned int flags)
{
    CLHFileCompletionCallback_dtor(callback);
    if (flags & 1)
        operator delete(callback);
    return callback;
}

int main()
{
    CLHFileCompletionCallback a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CLHFileCompletionCallback_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CLHFileCompletionCallback b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CLHFileCompletionCallback_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00add5d0_TEST PASS\n");
    return 0;
}