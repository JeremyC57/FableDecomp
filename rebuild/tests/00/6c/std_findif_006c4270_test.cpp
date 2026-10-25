// Behaviour test for 0x006C4270 STLport __find_if (random access, 4x unrolled) with CIsThingDead (model copy).
#include <cstdio>

struct CThing { unsigned char StateBits; bool IsDead() const { return (StateBits & 1) != 0; } };
struct Ptr { CThing* item; CThing* GetPItem() const { return item; } };
struct CIsThingDead { bool operator()(const Ptr& t) const { return t.GetPItem() == 0 || t.GetPItem()->IsDead(); } };

static Ptr* find_if(Ptr* first, Ptr* last, CIsThingDead pred)
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
    CThing alive; alive.StateBits = 0x02; CThing dead; dead.StateBits = 0x03;
    Ptr v[11];
    for (int i = 0; i < 11; ++i) v[i].item = &alive;
    CIsThingDead pred;
    int ok = find_if(v, v + 11, pred) == v + 11;
    for (int k = 0; k < 11; ++k) {                                  // a hit at every position (unrolled body and the tail)
        v[k].item = (k & 1) ? &dead : 0;
        if (find_if(v, v + 11, pred) != v + k) ok = 0;
        if (find_if(v + k + 1, v + 11, pred) != v + 11) ok = 0;
        v[k].item = &alive;
    }
    ok = ok && find_if(v, v, pred) == v;
    if (!ok) { std::printf("006c4270_TEST FAIL\n"); return 1; }
    std::printf("006c4270_TEST PASS\n");
    return 0;
}