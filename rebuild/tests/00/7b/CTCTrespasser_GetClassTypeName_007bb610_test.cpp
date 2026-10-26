#include <cstdio>

class CCharString { public:
    char* m_data;
    int   m_len;
    int   m_ref;
    CCharString(const char* s, int len) { m_data = (char*)s; m_len = len; m_ref = 0; }
};

static const char* const kActionNameLit = (const char*)0x0123cb18;

class CTCTrespasser { public:
    CCharString GetClassTypeName();
};

CCharString CTCTrespasser::GetClassTypeName()
{
    return CCharString(kActionNameLit, -1);
}

int main()
{
    CTCTrespasser obj;
    CCharString r = obj.GetClassTypeName();
    if (r.m_data == (char*)0x0123cb18 && r.m_len == -1)
        std::printf("007bb610_TEST PASS\n");
    else
        std::printf("FAIL data=%p len=%d\n", (void*)r.m_data, r.m_len);
    return 0;
}