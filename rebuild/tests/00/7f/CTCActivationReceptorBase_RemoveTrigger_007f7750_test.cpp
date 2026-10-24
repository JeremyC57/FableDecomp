#include <cstdio>
#include <cmath>
// Behaviour test: CTCActivationReceptorBase::RemoveTrigger -- erases every occurrence of the trigger's parent thing from the Triggers vector, keeping the order of the rest.
#define CHECK(c) if (!(c)) { std::printf("007f7750_TEST FAIL\n"); return 1; }
struct CThing { int id; };
struct CTCActivationTrigger { CThing* Parent; CThing* GetParentThing() { return Parent; } };
static CThing** Find(CThing** first, CThing** last, CThing* val) { while (first != last && *first != val) ++first; return first; }
static CThing** RemoveCopy(CThing** first, CThing** last, CThing** result, CThing* val)
{
    for (; first != last; ++first) if (*first != val) *result++ = *first;
    return result;
}
static CThing** Remove(CThing** first, CThing** last, CThing* val)
{
    first = Find(first, last, val);
    CThing** i = first;
    return first == last ? first : RemoveCopy(++i, last, first, val);
}
struct CTCActivationReceptorBase {
    CThing* Items[8]; CThing** Finish;
    void RemoveTrigger(CTCActivationTrigger& trigger)
    {
        CThing** newEnd = Remove(Items, Finish, trigger.GetParentThing());
        Finish = newEnd;
    }
};
int main()
{
    CThing a, b, c; a.id = 1; b.id = 2; c.id = 3;
    CTCActivationReceptorBase r; r.Items[0] = &a; r.Items[1] = &b; r.Items[2] = &c; r.Items[3] = &b; r.Finish = r.Items + 4;
    CTCActivationTrigger tb; tb.Parent = &b;
    r.RemoveTrigger(tb);
    CHECK(r.Finish - r.Items == 2 && r.Items[0] == &a && r.Items[1] == &c)
    CThing d; CTCActivationTrigger td; td.Parent = &d;
    r.RemoveTrigger(td);
    CHECK(r.Finish - r.Items == 2)
    CTCActivationTrigger ta; ta.Parent = &a;
    r.RemoveTrigger(ta);
    CHECK(r.Finish - r.Items == 1 && r.Items[0] == &c)
    std::printf("007f7750_TEST PASS\n");
    return 0;
}