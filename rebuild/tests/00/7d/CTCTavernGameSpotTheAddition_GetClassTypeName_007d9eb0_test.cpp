#include <cstdio>

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, long n) { m_data = (char*)s; (void)n; }
};

class CTCTavernGameSpotTheAddition {
public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameSpotTheAddition::GetClassTypeName() {
    return CCharString((const char*)0x123d584, -1);
}

int main() {
    CTCTavernGameSpotTheAddition a;
    CCharString r = a.GetClassTypeName();
    if (r.m_data == (char*)0x123d584) {
        printf("OK ptr=%p\n", (void*)r.m_data);
    } else {
        printf("FAIL\n");
        return 1;
    }
    return 0;
}