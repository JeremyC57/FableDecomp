// Behaviour test for 0056cb40: std::make_pair<unsigned long,CAnimationEntry>(const unsigned long&, const CAnimationEntry&)
// -- returns a pair holding the key and a copy of the animation entry (made through its copy constructor).
#include <cstdio>

static int g_copies = 0;
struct CAnimationEntry {
    long  BankIndex;
    void* AnimName;
    void* GroupName;
    void* Components;
    CAnimationEntry(long bank) : BankIndex(bank), AnimName(0), GroupName(0), Components(0) { }
    CAnimationEntry(const CAnimationEntry& other)
        : BankIndex(other.BankIndex), AnimName(other.AnimName), GroupName(other.GroupName), Components(other.Components)
    {
        ++g_copies;
    }
};
template <class T1, class T2> struct SPair {
    T1 first;
    T2 second;
    SPair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T1, class T2> inline SPair<T1, T2> make_pair_model(const T1& x, const T2& y)
{
    return SPair<T1, T2>(x, y);
}

int main()
{
    CAnimationEntry anim(12);
    static int marker;
    anim.AnimName = &marker;
    unsigned long key = 0xDEADBEEFUL;
    SPair<unsigned long, CAnimationEntry> entry = make_pair_model(key, anim);
    if (entry.first != 0xDEADBEEFUL || entry.second.BankIndex != 12 || entry.second.AnimName != &marker || g_copies < 1) {
        std::printf("0056cb40_TEST FAIL\n");
        return 1;
    }
    std::printf("0056cb40_TEST PASS\n");
    return 0;
}