// Behaviour test for 0x00695E30: STLport unrolled __find_if over counted event pointers (first finished event)
#include <cstdio>

struct Event { bool Finished; };
struct Ptr { Event* Data; void* Info; };
struct IsFinished { bool operator()(const Ptr& p) const { return p.Data->Finished; } };

static Ptr* FindIf(Ptr* first, Ptr* last, IsFinished pred)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
        if (pred(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (pred(*first)) return first; ++first;
    case 2: if (pred(*first)) return first; ++first;
    case 1: if (pred(*first)) return first;
    case 0:
    default: return last;
    }
}

int main()
{
    Event ev[11];
    Ptr p[11];
    for (int i = 0; i < 11; ++i) { ev[i].Finished = false; p[i].Data = &ev[i]; p[i].Info = 0; }
    if (FindIf(p, p + 11, IsFinished()) != p + 11) { std::printf("00695e30_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 11; ++k) {
        ev[k].Finished = true;
        if (FindIf(p, p + 11, IsFinished()) != p + k) { std::printf("00695e30_TEST FAIL\n"); return 1; }
        if (k > 0 && FindIf(p, p + k, IsFinished()) != p + k) { std::printf("00695e30_TEST FAIL\n"); return 1; }
        ev[k].Finished = false;
    }
    if (FindIf(p, p, IsFinished()) != p) { std::printf("00695e30_TEST FAIL\n"); return 1; }
    std::printf("00695e30_TEST PASS\n");
    return 0;
}