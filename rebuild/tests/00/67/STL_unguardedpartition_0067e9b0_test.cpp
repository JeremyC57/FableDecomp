// Behaviour test for 0x0067E9B0: STLport __unguarded_partition over 28-byte stages, descending health comparator
#include <cstdio>

struct Info { float Health; unsigned char pad[16]; int Emitter; bool Blocks; };
struct GT { bool operator()(const Info& a, const Info& b) const { return a.Health > b.Health; } };

static Info* Partition(Info* first, Info* last, Info pivot, GT comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Info t = *first; *first = *last; *last = t;
        ++first;
    }
}

int main()
{
    float h[9] = { 0.5f, 0.9f, 0.1f, 0.7f, 0.3f, 0.8f, 0.2f, 0.6f, 0.4f };
    Info a[9];
    for (int i = 0; i < 9; ++i) { a[i].Health = h[i]; a[i].Emitter = i; a[i].Blocks = (i & 1) != 0; }
    Info pivot = a[0];
    Info* cut = Partition(a, a + 9, pivot, GT());
    if (cut <= a || cut >= a + 9) { std::printf("0067e9b0_TEST FAIL\n"); return 1; }
    for (Info* p = a; p < cut; ++p) if (p->Health < 0.5f - 1e-6f) { std::printf("0067e9b0_TEST FAIL\n"); return 1; }
    for (Info* p = cut; p < a + 9; ++p) if (p->Health > 0.5f + 1e-6f) { std::printf("0067e9b0_TEST FAIL\n"); return 1; }
    // emitters travel with their health values
    for (int i = 0; i < 9; ++i) { float want = h[a[i].Emitter]; if (a[i].Health != want) { std::printf("0067e9b0_TEST FAIL\n"); return 1; } }
    std::printf("0067e9b0_TEST PASS\n");
    return 0;
}