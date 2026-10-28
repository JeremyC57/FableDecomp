#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, long n);
    char* m_data;
    int m_n;
};

class CTCTavernGameCardPairs {
public:
    CCharString GetClassTypeName();
};

// Local stand-in for the real ctor at 0x99ebf0.
CCharString::CCharString(const char* s, long n)
{
    m_data = (char*)s;
    m_n = n;
}

CCharString CTCTavernGameCardPairs::GetClassTypeName()
{
    return CCharString((const char*)0x123d56c, -1);
}

int main()
{
    CTCTavernGameCardPairs a;
    CCharString r = a.GetClassTypeName();
    if (r.m_data == (char*)0x123d56c && r.m_n == -1)
        printf("OK data=%p n=%d\n", r.m_data, r.m_n);
    else
        printf("FAIL\n");
    return 0;
}