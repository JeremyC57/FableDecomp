// Behaviour test for 004dfc0b: pair<const long, CEnginePrimitiveHandle> scalar deleting destructor -- destructor always, free only for flag bit 0, returns the object
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0, g_free = 0;
struct Pair { long first; void* vt; void* data; void Destroy() { ++g_dtor; } };
static void* ScalarDeletingDtor(Pair* self, unsigned int flags)
{
    self->Destroy();
    if (flags & 1) { ++g_free; std::free(self); }
    return self;
}
int main()
{
    Pair local; local.first = 1;
    int ok = ScalarDeletingDtor(&local, 0) == &local && g_dtor == 1 && g_free == 0;
    Pair* heap = (Pair*)std::malloc(sizeof(Pair)); heap->first = 2;
    ok = ok && ScalarDeletingDtor(heap, 1) == heap && g_dtor == 2 && g_free == 1;
    ok = ok && ScalarDeletingDtor(&local, 2) == &local && g_dtor == 3 && g_free == 1;
    std::printf(ok ? "004dfc0b_TEST PASS\n" : "004dfc0b_TEST FAIL\n");
    return ok ? 0 : 1;
}
