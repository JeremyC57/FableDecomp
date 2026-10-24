#include <cstdio>
// Behaviour test: the same len/half walk over a copy of the model, checked against a linear scan.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
template <class A, class B> struct pair { A first; B second; };
struct SValue { int words[2]; };        // the 8-byte mapped value
typedef pair<u64, SValue> Entry;
struct Map {
    Entry* First; Entry* Last; Entry* EndOfStorage;
    Entry* UpperBound(const u64& key) const
    {
        Entry* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            Entry* middle = first + half;
            if (!(key < middle->first)) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
};
int main()
{
    Entry e[8];
    if (sizeof(Entry) != 16) { std::printf("00a7dcc0_TEST FAIL\n"); return 1; }
    u64 keys[8] = { 1, 2, 2, 5, 7, 7, 9, 11 };
    int i = 0;
    for (i = 0; i < 8; ++i) { e[i].first = keys[i]; e[i].second.words[0] = i; }
    int n = 0;
    for (n = 0; n <= 8; ++n) {
        Map m; m.First = e; m.Last = e + n; m.EndOfStorage = e + 8;
        u64 probe = 0;
        for (; probe < 13; ++probe) {
            int expect = 0;
            while (expect < n && !(probe < e[expect].first)) ++expect;
            if (m.UpperBound(probe) != e + expect) { std::printf("00a7dcc0_TEST FAIL\n"); return 1; }
        }
    }
    // high-word ordering: keys that differ only above bit 32
    Entry h[3];
    h[0].first = 5; h[1].first = ((u64)1 << 32) | 1; h[2].first = ((u64)2 << 32);
    Map hm; hm.First = h; hm.Last = h + 3; hm.EndOfStorage = h + 3;
    u64 k = ((u64)1 << 32);
    if (hm.UpperBound(k) != h + 1) { std::printf("00a7dcc0_TEST FAIL\n"); return 1; }
    k = ((u64)1 << 32) | 1;
    if (hm.UpperBound(k) != h + 2) { std::printf("00a7dcc0_TEST FAIL\n"); return 1; }
    std::printf("00a7dcc0_TEST PASS\n");
    return 0;
}