// Behaviour test for 0x006F8EA0 __find_if(first, last, CIsEventIndexEqualTo, random_access_iterator_tag): the first event whose 30-bit Index equals pred.Index (the flag bits ignored), else last, for every remainder 0..3.
#include <cstdio>
struct Event {
    unsigned int Index : 30;
    unsigned int ExternallyImported : 1;
    unsigned int StopEvent : 1;
    float Time;
};
struct IsIndex {
    unsigned int Index;
    bool operator()(const Event& e) const { return e.Index == Index; }
};
static const Event* find_if(const Event* first, const Event* last, IsIndex pred)
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
    default: return last;
    }
}
static int fail() { std::printf("006f8ea0_TEST FAIL\n"); return 1; }
int main()
{
    Event ev[11];
    for (int i = 0; i < 11; ++i) { ev[i].Index = 100 + i; ev[i].ExternallyImported = i & 1; ev[i].StopEvent = 1; ev[i].Time = 0.0f; }
    for (int n = 0; n <= 11; ++n)
        for (int k = 0; k < 11; ++k) {
            IsIndex p; p.Index = 100 + k;
            const Event* r = find_if(ev, ev + n, p);
            if (r != (k < n ? ev + k : ev + n)) return fail();
        }
    IsIndex none; none.Index = 7;
    if (find_if(ev, ev + 11, none) != ev + 11) return fail();
    std::printf("006f8ea0_TEST PASS\n");
    return 0;
}