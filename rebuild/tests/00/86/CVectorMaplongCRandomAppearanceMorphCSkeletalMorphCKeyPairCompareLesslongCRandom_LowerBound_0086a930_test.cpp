// behaviour test for 0086a930: the lower bound over sorted 16-byte entries keyed by a signed dword
#include <cstdio>
struct SValue { unsigned char _payload[12]; };
struct SEntry { long first; SValue second; };
struct SMap { SEntry* First; SEntry* Last; SEntry* EndOfStorage; };
static SEntry* Bound(SMap* m, const long& key)
{
    SEntry* first = m->First;
    int count = (int)(m->Last - m->First);
    while (count > 0)
    {
        int half = count / 2;
        SEntry* middle = first + half;
        if (middle->first < key)
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
int main()
{
    static const long keys[5] = { -7, 2, 2, 9, 40 };
    SEntry entries[5];
    int i = 0;
    for (i = 0; i < 5; ++i) entries[i].first = keys[i];
    SMap m; m.First = entries; m.Last = entries + 5; m.EndOfStorage = entries + 5;
    int ok = 1;
    long k = 2;
    if (Bound(&m, k) != entries + 1) ok = 0;
    k = 100; if (Bound(&m, k) != entries + 5) ok = 0;
    k = -100; if (Bound(&m, k) != entries + 0) ok = 0;
    k = 3; if (Bound(&m, k) != entries + 3) ok = 0;
    SMap e; e.First = entries; e.Last = entries; e.EndOfStorage = entries;
    if (Bound(&e, k) != entries) ok = 0;
    if (!ok) { std::printf("0086a930_TEST FAIL\n"); return 1; }
    std::printf("0086a930_TEST PASS\n");
    return 0;
}