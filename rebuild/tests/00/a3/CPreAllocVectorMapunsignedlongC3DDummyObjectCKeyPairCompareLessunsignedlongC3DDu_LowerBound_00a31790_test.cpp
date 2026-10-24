#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00a31790"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

template <class K, class V> struct pair { K first; V second; };
struct CValue { unsigned char data[52]; };
template <class K, class V> struct CPreAllocVectorMap {
    unsigned char  Compare;
    pair<K, V>*    Elements;
    unsigned long  Size;
    bool           Dirty;
    pair<K, V>* Bound(const K& key) const
    {
        LONG32 len = (LONG32)Size;
        pair<K, V>* first = Elements;
        while (len > 0) {
            LONG32 half = len / 2;
            pair<K, V>* mid = first + half;
            if (mid->first < key) {
                first = mid + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return first;
    }
};
int main()
{
    pair<unsigned int, CValue> e[6];
    unsigned int keys[6] = { 2, 4, 4, 4, 9, 12 };
    for (int i = 0; i < 6; ++i) e[i].first = keys[i];
    CPreAllocVectorMap<unsigned int, CValue> m; m.Elements = e; m.Size = 6;
    unsigned int k;
    k = 4;  CHECK(m.Bound(k) == e + 1);
    k = 1;  CHECK(m.Bound(k) == e + 0);
    k = 9;  CHECK(m.Bound(k) == e + 4);
    k = 13; CHECK(m.Bound(k) == e + 6);
    m.Size = 0; k = 4; CHECK(m.Bound(k) == e);
    std::printf("00a31790_TEST PASS\n");
    return 0;
}