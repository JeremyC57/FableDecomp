// Behaviour test for 0x00592495 pair<const CCharString, CCharString>(const pair<const char*, const char*>&) (model copy)
#include <cstdio>
#include <cstring>
struct Str {
    char text[32];
    Str(const char* s, long length = -1) { std::strcpy(text, s); (void)length; }
};
template <class T1, class T2> struct Pair {
    T1 first;
    T2 second;
    Pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <class U1, class U2> Pair(const Pair<U1, U2>& p) : first(p.first), second(p.second) {}
};
int main()
{
    Pair<const char*, const char*> lits("OBJECT_A", "TEXT_B");
    Pair<const Str, Str> entry(lits);
    if (std::strcmp(entry.first.text, "OBJECT_A") != 0 || std::strcmp(entry.second.text, "TEXT_B") != 0) {
        std::printf("00592495_TEST FAIL\n"); return 1;
    }
    std::printf("00592495_TEST PASS\n");
    return 0;
}