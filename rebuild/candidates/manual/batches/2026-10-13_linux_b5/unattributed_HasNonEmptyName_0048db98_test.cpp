#include <cstdio>
#include <stddef.h>
struct W { const wchar_t* p; bool operator!=(const wchar_t* o) const { return p[0] != o[0]; } };
struct X { unsigned char pad[0x6c]; W Name; bool Has() { return Name != L""; } };
int main()
{
    X x; x.Name.p = L"abc"; if (!x.Has()) { std::printf("0048db98_TEST FAIL\n"); return 1; }
    x.Name.p = L""; if (x.Has()) { std::printf("0048db98_TEST FAIL\n"); return 1; }

    std::printf("0048db98_TEST PASS\n");
    return 0;
}
