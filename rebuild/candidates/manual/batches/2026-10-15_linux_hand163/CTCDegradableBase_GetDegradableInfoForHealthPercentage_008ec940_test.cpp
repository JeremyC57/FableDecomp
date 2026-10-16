// behaviour test for 0x008EC940 CTCDegradableBase::GetDegradableInfoForHealthPercentage
#include <cstdio>
struct Info { float HealthPercentage; };
static const Info* Lookup(const Info* first, const Info* last, float pct)
{
    const Info* result = 0;
    const Info* it = first;
    if (it != last)
    {
        if (pct < it->HealthPercentage)
        {
            const Info* previous = it;
            for (++it; it != last; ++it, ++previous)
                if (pct > it->HealthPercentage && pct <= previous->HealthPercentage) break;
            result = previous;
        }
        return result;
    }
    return 0;
}
int main()
{
    Info st[3] = { { 1.0f }, { 0.5f }, { 0.1f } };
    bool ok = Lookup(st, st, 0.3f) == 0
        && Lookup(st, st + 3, 1.0f) == 0
        && Lookup(st, st + 3, 0.7f) == &st[0]
        && Lookup(st, st + 3, 0.5f) == &st[1]
        && Lookup(st, st + 3, 0.3f) == &st[1]
        && Lookup(st, st + 3, 0.05f) == &st[2];
    if (!ok) { std::printf("008ec940_TEST FAIL\n"); return 1; }
    std::printf("008ec940_TEST PASS\n");
    return 0;
}
