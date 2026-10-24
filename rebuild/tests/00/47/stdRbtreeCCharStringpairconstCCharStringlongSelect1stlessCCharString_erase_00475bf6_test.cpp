// Behaviour test for 0x00475BF6 _Rb_tree<CCharString, pair<const CCharString, long> >::erase(const key&) (model on a sorted array).
#include <cstdio>
#include <cstring>

struct Tree {
    const char* keys[8];
    int count;
    void equal_range(const char* k, int& first, int& last) const
    {
        first = 0;
        while (first < count && std::strcmp(keys[first], k) < 0) ++first;
        last = first;
        while (last < count && std::strcmp(keys[last], k) == 0) ++last;
    }
    static int distance(const int& first, const int& last) { int n = 0; for (int i = first; i != last; ++i) ++n; return n; }
    void erase(int first, int last)
    {
        int gap = last - first;
        for (int i = first; i + gap < count; ++i) keys[i] = keys[i + gap];
        count -= gap;
    }
    unsigned int erase(const char* k)
    {
        int first, last;
        equal_range(k, first, last);
        unsigned int n = distance(first, last);
        erase(first, last);
        return n;
    }
};

int main()
{
    Tree t;
    const char* init[] = { "apple", "kiwi", "pear", "plum" };
    for (int i = 0; i < 4; ++i) t.keys[i] = init[i];
    t.count = 4;
    unsigned int a = t.erase("kiwi");
    unsigned int b = t.erase("mango");
    int ok = a == 1 && b == 0 && t.count == 3 && std::strcmp(t.keys[1], "pear") == 0;
    if (!ok) { std::printf("00475bf6_TEST FAIL\n"); return 1; }
    std::printf("00475bf6_TEST PASS\n");
    return 0;
}