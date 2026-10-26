// behaviour test for 0x00825AE0 std::__destroy_aux over pair<long, CCountedPointer<CContainmentVolume> >: each element's counted pointer
// drops one reference; the last reference runs DeleteFunc(Data) and frees the info block; Data/Info are cleared; an empty range does nothing
#include <cstdio>
static int gDeleted = 0;
struct Info { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void Deleter(void* p) { gDeleted += *(int*)p; }
struct Counted {
    void* Data; Info* PInfo;
    ~Counted()
    {
        if (PInfo) {
            --PInfo->RefCount;
            if (PInfo->RefCount == 0) { PInfo->DeleteFunc(PInfo->Data); delete PInfo; }
        }
        Data = 0; PInfo = 0;
    }
};
struct Pair { int first; Counted second; };
static void destroyRange(Pair* first, Pair* last) { for (; first != last; ++first) first->~Pair(); }
int main()
{
    int fails = 0;
    static int payload = 5;
    Info* shared = new Info; shared->RefCount = 2; shared->DeleteFunc = Deleter; shared->Data = &payload;
    static double raw[3 * sizeof(Pair) / sizeof(double) + 1];
    Pair* p = (Pair*)raw;
    p[0].first = 1; p[0].second.Data = &payload; p[0].second.PInfo = shared;
    p[1].first = 2; p[1].second.Data = 0; p[1].second.PInfo = 0;
    p[2].first = 3; p[2].second.Data = &payload; p[2].second.PInfo = shared;
    destroyRange(p, p);
    if (shared->RefCount != 2 || gDeleted != 0) fails |= 1;
    destroyRange(p, p + 2);
    if (shared->RefCount != 1 || gDeleted != 0 || p[0].second.PInfo != 0 || p[0].second.Data != 0) fails |= 2;
    destroyRange(p + 2, p + 3);
    if (gDeleted != 5 || p[2].second.PInfo != 0) fails |= 4;
    if (fails) { std::printf("00825ae0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00825ae0_TEST PASS\n");
    return 0;
}