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
#define TEST_ADDR "00ad0750"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static int g_compares = 0;
struct SSortKey { int key; };
typedef SSortKey T;
bool __fastcall Less(const T& a, const T& b) { ++g_compares; return a.key < b.key; }   // by-reference elements
#define VALUE(p) ((p).key)
template <class T, class Compare>
const T* __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b)) {
        if (comp(b, c)) return &b;
        else if (comp(a, c)) return &c;
        else return &a;
    }
    else if (comp(a, c)) return &a;
    else if (comp(b, c)) return &c;
    else return &b;
}
int main()
{
    T x[3];
    int perms[6][3] = { {1,2,3}, {1,3,2}, {2,1,3}, {2,3,1}, {3,1,2}, {3,2,1} };
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) x[j].key = perms[i][j];
        const T* m = __median(x[0], x[1], x[2], Less);
        CHECK(VALUE(*m) == 2);
    }
    CHECK(g_compares >= 12);
    std::printf("00ad0750_TEST PASS\n");
    return 0;
}