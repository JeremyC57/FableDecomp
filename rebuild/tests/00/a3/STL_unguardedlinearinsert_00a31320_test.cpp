// Behaviour test for 0x00A31320 std::__unguarded_linear_insert: the value sinks past every larger key and lands in the
// gap; equal keys stay in front of it (stable for equal keys); the other fields travel with their keys.
#include <cstdio>
#include <cstring>

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject { CMatrix3x4 Transform; int BoneIndex; };
typedef C3DDummyObject TSecond;
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<unsigned int, TSecond> TEntry;
struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};
static void __unguarded_linear_insert(TEntry* last, TEntry val, CKeyPairCompareLess comp)
{
    TEntry* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void Set(TEntry& e, unsigned int key, unsigned int tag)
{
    std::memset(&e, 0, sizeof(e));
    e.first = key;
    *(unsigned int*)&e.second = tag;
}
static unsigned int Tag(const TEntry& e) { return *(const unsigned int*)&e.second; }

int main()
{
    TEntry a[6];
    unsigned int keys[5] = { 1, 3, 3, 7, 9 };
    for (int i = 0; i < 5; ++i) Set(a[i], keys[i], 100 + i);
    TEntry v; Set(v, 3, 555);
    __unguarded_linear_insert(&a[5], v, CKeyPairCompareLess());
    unsigned int wantKey[6] = { 1, 3, 3, 3, 7, 9 };
    unsigned int wantTag[6] = { 100, 101, 102, 555, 103, 104 };
    for (int i = 0; i < 6; ++i)
        if (a[i].first != wantKey[i] || Tag(a[i]) != wantTag[i]) { std::printf("00a31320_TEST FAIL\n"); return 1; }
    // a value larger than the last element stays where it is
    TEntry b[3]; Set(b[0], 2, 1); Set(b[1], 4, 2);
    TEntry w; Set(w, 8, 3);
    __unguarded_linear_insert(&b[2], w, CKeyPairCompareLess());
    if (b[2].first != 8 || Tag(b[2]) != 3 || b[1].first != 4) { std::printf("00a31320_TEST FAIL\n"); return 1; }
    std::printf("00a31320_TEST PASS\n");
    return 0;
}