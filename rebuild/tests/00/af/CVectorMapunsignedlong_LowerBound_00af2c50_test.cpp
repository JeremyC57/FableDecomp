#include <cstdio>
// Behaviour test: the same len/half walk over a copy of the model, checked against a linear scan.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
template <class A, class B> struct pair { A first; B second; };
struct SValue { int words[3]; };        // the 12-byte mapped value
typedef pair<unsigned int, SValue> Entry;
struct Map {
    Entry* First; Entry* Last; Entry* EndOfStorage;
    Entry* LowerBound(const unsigned int& key) const
    {
        Entry* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            Entry* middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
};
int main()
{
    Entry e[8];
    if (sizeof(Entry) != 16) { std::printf("00af2c50_TEST FAIL\n"); return 1; }
    unsigned int keys[8] = { 1, 2, 2, 5, 7, 7, 9, 11 };
    int i = 0;
    for (i = 0; i < 8; ++i) { e[i].first = keys[i]; e[i].second.words[0] = i; }
    int n = 0;
    for (n = 0; n <= 8; ++n) {
        Map m; m.First = e; m.Last = e + n; m.EndOfStorage = e + 8;
        unsigned int probe = 0;
        for (; probe < 13; ++probe) {
            int expect = 0;
            while (expect < n && e[expect].first < probe) ++expect;
            if (m.LowerBound(probe) != e + expect) { std::printf("00af2c50_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00af2c50_TEST PASS\n");
    return 0;
}