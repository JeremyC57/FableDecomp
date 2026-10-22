#include <cstdio>

struct CCharString {
    const char* p;
    int n;
    CCharString(const char* s, int len) : p(s), n(len) {}
};

struct CTCTavern {
    char pad;
    CCharString GetClassTypeName();
};

CCharString CTCTavern::GetClassTypeName()
{
    return CCharString((const char*)0x0123caf0, -1);
}

int main()
{
    CTCTavern obj;
    CCharString r = obj.GetClassTypeName();
    if (r.p == (const char*)0x0123caf0 && r.n == -1) {
        std::printf("007bace0_TEST PASS\n");
    } else {
        std::printf("FAIL p=%p n=%d\n", (void*)r.p, r.n);
    }
    return 0;
}