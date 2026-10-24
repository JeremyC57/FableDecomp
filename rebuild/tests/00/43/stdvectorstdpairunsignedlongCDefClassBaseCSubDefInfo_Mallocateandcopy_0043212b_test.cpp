// Behaviour test for vector<pair<unsigned long, CSubDefInfo>>::_M_allocate_and_copy (0x0043212B): n == 0 gives a null block,
// otherwise a block of n elements whose first (last - first) elements are copies of the source range.
#include <cstdio>
#include <cstdlib>
#include <new>
struct CSubDefInfo { long DefIndex; long OriginalParentDefIndex; };
template <class T1, class T2> struct pair {
    pair() {}
    pair(const pair& o) : first(o.first), second(o.second) {}
    T1 first; T2 second;
};
typedef pair<unsigned long, CSubDefInfo> Elem;
Elem* allocate_and_copy(size_t n, const Elem* first, const Elem* last)
{
    Elem* result = n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
    Elem* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) Elem(*first);
    return result;
}
int main()
{
    int bad = 0;
    Elem src[3];
    for (int i = 0; i < 3; ++i) { src[i].first = 10 + i; src[i].second.DefIndex = 20 + i; src[i].second.OriginalParentDefIndex = 30 + i; }
    if (allocate_and_copy(0, src, src) != 0) bad = 1;
    Elem* block = allocate_and_copy(5, src, src + 3);
    if (!block) bad = 1;
    else {
        for (int i = 0; i < 3; ++i)
            if (block[i].first != (unsigned long)(10 + i) || block[i].second.DefIndex != 20 + i || block[i].second.OriginalParentDefIndex != 30 + i) bad = 1;
        std::free(block);
    }
    if (bad) { std::printf("0043212b_TEST FAIL\n"); return 1; }
    std::printf("0043212b_TEST PASS\n");
    return 0;
}