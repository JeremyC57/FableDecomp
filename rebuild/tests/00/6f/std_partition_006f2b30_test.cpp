// Behaviour test for 006f2b30 (std::__partition<CProng*, CIsProngUsable>): usable prongs (no occupier or a dead one,
// and not blocked) move to the front; the returned pointer is the first unusable prong.
#include <cstdio>

class CThingCreatureBase {
public:
    unsigned char Dead : 1;
    bool IsDead() const { return Dead; }
};

class CBaseIntelligentPointer {                // the retail class is polymorphic; only GetPItem matters here
public:
    CThingCreatureBase* PData;
    CThingCreatureBase* GetPItem() const { return PData; }   // 0x00A01B50 stand-in
};

class CProng {
public:
    bool Blocked;
    float Prong[3];
    long ProngBlockageTimer;
    CBaseIntelligentPointer POccupier;
    bool IsBlocked() const { return Blocked; }
    CThingCreatureBase* GetPOccupyingAttacker() const { return POccupier.GetPItem(); }
};

class CIsProngUsable {
public:
    bool operator()(const CProng& prong) const
    {
        const CThingCreatureBase* occupier = prong.GetPOccupyingAttacker();
        return (occupier == 0 || occupier->IsDead()) && !prong.IsBlocked();
    }
};

struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };

template<class T> void iter_swap(T* a, T* b)   // 0x006F15E0 stand-in
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template<class BidirectionalIter, class Predicate>
BidirectionalIter __partition(BidirectionalIter first, BidirectionalIter last, Predicate pred, const bidirectional_iterator_tag&)
{
    while (true) {
        while (true)
            if (first == last)
                return first;
            else if (pred(*first))
                ++first;
            else
                break;
        --last;
        while (true)
            if (first == last)
                return first;
            else if (!pred(*last))
                --last;
            else
                break;
        iter_swap(first, last);
        ++first;
    }
}

int main()
{
    CThingCreatureBase alive, dead;
    alive.Dead = 0;
    dead.Dead = 1;
    CProng prongs[6];
    // usable: 1 (dead occupier), 3 (empty), 5 (empty); unusable: 0 (alive), 2 (blocked), 4 (alive)
    for (int i = 0; i < 6; ++i) {
        prongs[i].Blocked = false;
        prongs[i].ProngBlockageTimer = i;
        prongs[i].POccupier.PData = 0;
    }
    prongs[0].POccupier.PData = &alive;
    prongs[1].POccupier.PData = &dead;
    prongs[2].Blocked = true;
    prongs[4].POccupier.PData = &alive;
    CProng* mid = __partition(prongs, prongs + 6, CIsProngUsable(), bidirectional_iterator_tag());
    int ok = mid == prongs + 3;
    CIsProngUsable usable;
    for (int i = 0; i < 6; ++i)
        if (usable(prongs[i]) != (i < 3))
            ok = 0;
    long sum = 0;
    for (int i = 0; i < 6; ++i)
        sum += 1L << prongs[i].ProngBlockageTimer;
    if (sum != 63)
        ok = 0;
    CProng* none = __partition(prongs, prongs, CIsProngUsable(), bidirectional_iterator_tag());
    if (none != prongs)
        ok = 0;
    if (!ok) {
        std::printf("006f2b30_TEST FAIL\n");
        return 1;
    }
    std::printf("006f2b30_TEST PASS\n");
    return 0;
}