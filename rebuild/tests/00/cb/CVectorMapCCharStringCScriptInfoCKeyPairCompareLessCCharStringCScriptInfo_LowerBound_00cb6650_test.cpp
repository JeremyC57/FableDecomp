#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
struct CBasicString { const char* Text; bool operator<(const CBasicString& o) const { return std::strcmp(Text, o.Text) < 0; } };
class CCharString { public:
    CBasicString* PData;
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData) return false;
        if (PData == 0) return true;
        if (other.PData == 0) return false;
        return *PData < *other.PData;
    }
};
struct Entry { CCharString first; int second[5]; };
struct Map {
    Entry* First; Entry* Last; Entry* End;
    const Entry* LowerBound(const CCharString& key) const
    {
        int count = Last - First;
        const Entry* first = First;
        if (count > 0) {
            CCharString k = key;
            do {
                int half = count / 2;
                const Entry* middle = first + half;
                if (middle->first < k) { first = middle + 1; count -= half + 1; }
                else count = half;
            } while (count > 0);
        }
        return first;
    }
};
int main()
{
    int ok = 1;
    CBasicString s[5] = { { "b" }, { "d" }, { "f" }, { "h" }, { "j" } };
    Entry e[5]; for (int i = 0; i < 5; ++i) e[i].first.PData = &s[i];
    const Map m = { e, e + 5, e + 5 };
    CBasicString qa = { "a" }, qe = { "e" }, qz = { "z" };
    CCharString ka = { &qa }, ke = { &qe }, kz = { &qz }, kf = { &s[2] };
    ok = m.LowerBound(ka) == e && m.LowerBound(ke) == e + 2 && m.LowerBound(kz) == e + 5 && m.LowerBound(kf) == e + 2;
    const Map empty = { e, e, e }; ok = ok && empty.LowerBound(ke) == e;
    if (!ok) { std::printf("00cb6650_TEST FAIL\n"); return 1; }
    std::printf("00cb6650_TEST PASS\n");
    return 0;
}