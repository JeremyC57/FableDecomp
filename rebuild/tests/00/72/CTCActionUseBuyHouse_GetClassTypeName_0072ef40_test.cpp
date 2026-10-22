#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int len);
    const char* m_data;
    int m_len;
};

CCharString::CCharString(const char* s, int len)
{
    m_data = s;
    m_len = (len < 0) ? (int)std::strlen(s) : len;
}

class CTCActionUseBuyHouse {
public:
    CCharString GetClassTypeName();
};

CCharString CTCActionUseBuyHouse::GetClassTypeName()
{
    return CCharString("CActionDoCreatureAction", -1);
}

int main()
{
    CTCActionUseBuyHouse obj;
    CCharString r = obj.GetClassTypeName();
    if (std::strcmp(r.m_data, "CActionDoCreatureAction") == 0 &&
        r.m_len == (int)std::strlen("CActionDoCreatureAction")) {
        std::printf("0072ef40_TEST PASS\n");
    } else {
        std::printf("FAIL\n");
    }
    return 0;
}