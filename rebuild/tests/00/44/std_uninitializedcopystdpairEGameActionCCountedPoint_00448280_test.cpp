// Behaviour test for 0x00448280: std::__uninitialized_copy over pair<EGameAction, CCountedPointer<const CInputTypeBase> > -- each element is
// copy-constructed (key, Data, Info copied; Info->RefCount bumped when Info is set), the end of the copy is returned.
#include <stdio.h>
struct Info { unsigned int RefCount; void* DeleteFunc; void* Data; };
struct Elem { unsigned int key; void* data; Info* info; };
static Elem* copy(Elem* first, Elem* last, Elem* result)
{
    Elem* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) { cur->key = first->key; cur->data = first->data; cur->info = first->info; if (cur->info) ++cur->info->RefCount; }
    return cur;
}
int main()
{
    Info i1 = { 1, 0, 0 }, i2 = { 5, 0, 0 };
    Elem src[3] = { { 7, &i1, &i1 }, { 8, 0, 0 }, { 9, &i2, &i2 } };
    Elem dst[3];
    Elem* e = copy(src, src + 3, dst);
    bool ok = e == dst + 3 && dst[0].key == 7 && dst[1].info == 0 && dst[2].data == &i2 && i1.RefCount == 2 && i2.RefCount == 6
              && sizeof(Elem) == 12;
    ok = ok && copy(src, src, dst) == dst;      // empty range
    if (!ok) { printf("00448280_TEST FAIL\n"); return 1; }
    printf("00448280_TEST PASS\n");
    return 0;
}
