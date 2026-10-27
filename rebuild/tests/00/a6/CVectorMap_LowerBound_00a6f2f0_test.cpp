// Behaviour test for 0x00A6F2F0 CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >::LowerBound
#include <cstdio>
#include <cstring>
class CCharString {
public:
    const char* PData;
    bool FastCompare(const CCharString& other) const { return std::strcmp(PData, other.PData) < 0; }
};
class C3DDummyObject { public: float Transform[12]; long BoneIndex; };
struct Entry { CCharString first; C3DDummyObject second; };
class TestMap {
public:
    Entry* _M_start;
    Entry* _M_finish;
    const Entry* LowerBound(const CCharString& key) const
    {
        int count = (int)(_M_finish - _M_start);
        const Entry* first = _M_start;
        while (count > 0) {
            int half = count / 2;
            const Entry* middle = first + half;
            if (middle->first.FastCompare(key)) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
};
int main()
{
    Entry e[5];
    const char* names[5] = { "a", "c", "e", "g", "i" };
    for (int i = 0; i < 5; ++i) e[i].first.PData = names[i];
    TestMap m; m._M_start = e; m._M_finish = e + 5;
    CCharString k;
    k.PData = "e"; if (m.LowerBound(k) != e + 2) { std::printf("00a6f2f0_TEST FAIL\n"); return 1; }
    k.PData = "f"; if (m.LowerBound(k) != e + 3) { std::printf("00a6f2f0_TEST FAIL\n"); return 1; }
    k.PData = "0"; if (m.LowerBound(k) != e) { std::printf("00a6f2f0_TEST FAIL\n"); return 1; }
    k.PData = "z"; if (m.LowerBound(k) != e + 5) { std::printf("00a6f2f0_TEST FAIL\n"); return 1; }
    m._M_finish = e; if (m.LowerBound(k) != e) { std::printf("00a6f2f0_TEST FAIL\n"); return 1; }
    std::printf("00a6f2f0_TEST PASS\n");
    return 0;
}