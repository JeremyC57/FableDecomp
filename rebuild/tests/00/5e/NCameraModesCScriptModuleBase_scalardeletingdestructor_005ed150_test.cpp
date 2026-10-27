#include <new>
#include <cstdio>
// Behaviour model of the module scalar deleting destructor: clear owner/camera manager, destroy pointers in reverse.
static int g_order[2]; static int g_n; static int g_freed;
struct IP { int id; ~IP() { g_order[g_n++] = id; } };
struct Module { IP cam; IP origin; void* owner; void* manager; Module() { cam.id = 1; origin.id = 2; owner = this; manager = this; } };
static Module* scalar_deleting_dtor(Module* m, unsigned int flags)
{
    m->owner = 0; m->manager = 0;
    m->origin.~IP(); m->cam.~IP();
    if (flags & 1) ++g_freed;
    return m;
}
int main()
{
    char raw[sizeof(Module)];
    Module* m = new (raw) Module;
    if (scalar_deleting_dtor(m, 1) != m || m->owner != 0 || m->manager != 0 || g_n != 2 || g_order[0] != 2 || g_order[1] != 1 || g_freed != 1)
        { std::printf("005ed150_TEST FAIL\n"); return 1; }
    std::printf("005ed150_TEST PASS\n");
    return 0;
}