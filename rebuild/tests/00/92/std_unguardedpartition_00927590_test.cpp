// Behaviour test for 0x00927590 std::__unguarded_partition over CIntelligentPointer<const CThing> with a function-pointer comparator.
#include <cstdio>

class CThing { public: int rank; };
static int g_live = 0;
struct ThingPointer {                       // the 8-byte intelligent pointer: copy = Set(Get())
    const CThing* item;
    ThingPointer() : item(0) { ++g_live; }
    ThingPointer(const ThingPointer& p) : item(p.item) { ++g_live; }
    ThingPointer& operator=(const ThingPointer& p) { item = p.item; return *this; }
    ~ThingPointer() { --g_live; }
};
typedef bool (*Compare)(ThingPointer, ThingPointer);
static bool ByRank(ThingPointer a, ThingPointer b) { return a.item->rank < b.item->rank; }

static ThingPointer* Partition(ThingPointer* first, ThingPointer* last, ThingPointer pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        ThingPointer tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}

int main()
{
    CThing things[10];
    const int ranks[10] = { 7, 2, 9, 4, 5, 1, 8, 3, 6, 0 };
    {
        ThingPointer p[10];
        for (int i = 0; i < 10; ++i) { things[i].rank = ranks[i]; p[i].item = &things[i]; }
        ThingPointer pivot; pivot.item = &things[4];               // rank 5
        ThingPointer* cut = Partition(p, p + 10, pivot, ByRank);
        int c = (int)(cut - p);
        for (int i = 0; i < c; ++i) if (p[i].item->rank > 5) { std::printf("00927590_TEST FAIL\n"); return 1; }
        for (int i = c; i < 10; ++i) if (p[i].item->rank < 5) { std::printf("00927590_TEST FAIL\n"); return 1; }
        if (c < 1 || c > 9) { std::printf("00927590_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("00927590_TEST FAIL\n"); return 1; }
    std::printf("00927590_TEST PASS\n");
    return 0;
}