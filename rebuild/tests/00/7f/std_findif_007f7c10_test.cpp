// Behaviour test for 0x007F7C10 std::__find_if<CThing**, CIsTriggerActivated> (STLport 4-way unrolled find_if)
#include <cstdio>

struct Entry { long kind; void* tc; };
struct Trigger { int pad[6]; bool Activated; };
struct Thing {
    unsigned int mask[8];
    Entry entries[4];
    int count;
    bool HasTC(long kind) const { return (mask[kind >> 5] & (1u << (kind & 31))) != 0; }
    const Entry* LowerBound(long kind) const
    {
        const Entry* first = entries;
        long len = count;
        while (len > 0) {
            long half = len / 2;
            const Entry* middle = first + half;
            if (middle->kind < kind) { first = middle + 1; len = len - half - 1; }
            else len = half;
        }
        return first;
    }
};
static Trigger gUnset;   // stands in for the never-written local an absent component reads
struct IsTriggerActivated {
    bool operator()(Thing* t) const
    {
        const Trigger* trig = &gUnset;
        if (t->HasTC(0x66)) {
            const Entry* it = t->LowerBound(0x66);
            const Entry* last = t->entries + t->count;
            trig = (const Trigger*)((it == last || 0x66 < it->kind) ? last : it)->tc;
        }
        return trig->Activated;
    }
};
template <class It, class P> It find_if_unrolled(It first, It last, P pred)
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

static Trigger gTriggers[16];
static Thing gThings[16];
static void Make(int i, bool hasTrigger, bool activated)
{
    Thing& t = gThings[i];
    for (int w = 0; w < 8; ++w) t.mask[w] = 0;
    t.count = 3;
    t.entries[0].kind = 4;   t.entries[0].tc = 0;
    t.entries[1].kind = 0x66; t.entries[1].tc = &gTriggers[i];
    t.entries[2].kind = 0x90; t.entries[2].tc = 0;
    t.entries[3].kind = 0;   t.entries[3].tc = 0;
    gTriggers[i].Activated = activated;
    if (hasTrigger) t.mask[0x66 >> 5] |= 1u << (0x66 & 31);
}

int main()
{
    Thing* ptrs[16];
    for (int i = 0; i < 16; ++i) { ptrs[i] = &gThings[i]; Make(i, true, false); }
    gUnset.Activated = false;
    int ok = 1;
    for (int n = 0; n <= 11; ++n)                       // none activated: last, for every remainder
        ok &= find_if_unrolled(ptrs, ptrs + n, IsTriggerActivated()) == ptrs + n;
    for (int hit = 0; hit < 11; ++hit) {                // each position, both in the unrolled body and the tail
        Make(hit, true, true);
        ok &= find_if_unrolled(ptrs, ptrs + 11, IsTriggerActivated()) == ptrs + hit;
        Make(hit, true, false);
    }
    Make(5, false, true);                               // component absent: ignored (the unset trigger is clear)
    Make(7, true, true);
    ok &= find_if_unrolled(ptrs, ptrs + 11, IsTriggerActivated()) == ptrs + 7;
    std::printf(ok ? "007f7c10_TEST PASS\n" : "007f7c10_TEST FAIL\n");
    return ok ? 0 : 1;
}