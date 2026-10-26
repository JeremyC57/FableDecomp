// Behaviour test for 0x00ae98a0: std::__uninitialized_fill_n over pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> > -- n copies of x, each bumping
// the shared count block; returns first + n (n == 0: first).
#include <stdio.h>
struct Info { unsigned int RefCount; void* DeleteFunc; void* Data; };
struct Elem { unsigned int key; void* data; Info* info; };
static Elem* fill(Elem* first, unsigned int n, const Elem& x)
{
    Elem* cur = first;
    for (; n > 0; --n, ++cur)
        if (cur) { cur->key = x.key; cur->data = x.data; cur->info = x.info; if (cur->info) ++cur->info->RefCount; }
    return cur;
}
int main()
{
    Info i = { 1, 0, 0 }; Elem x = { 42, &i, &i }; Elem buf[4]; Elem y = { 7, 0, 0 };
    bool ok = fill(buf, 3, x) == buf + 3 && buf[2].key == 42 && buf[1].info == &i && i.RefCount == 4;
    ok = ok && fill(buf, 0, x) == buf && i.RefCount == 4 && fill(buf + 3, 1, y) == buf + 4 && buf[3].info == 0 && sizeof(Elem) == 12;
    if (!ok) { printf("00ae98a0_TEST FAIL\n"); return 1; }
    printf("00ae98a0_TEST PASS\n");
    return 0;
}
