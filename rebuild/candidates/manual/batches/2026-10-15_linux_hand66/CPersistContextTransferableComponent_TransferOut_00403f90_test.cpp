#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
#include <cstring>
// Behaviour test for 00403f90: the bool component appends "TRUE" or "FALSE" to the output string.
struct CPersistContext;
struct CCharString {
    char m_text[16];
    CCharString& operator+=(const char* text) { std::strcat(m_text, text); return *this; }
};
struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    bool* m_pValue;
};
void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    if (*m_pValue)
        *out += "TRUE";
    else
        *out += "FALSE";
}
int main()
{
    bool b = true;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &b;
    CCharString s1; s1.m_text[0] = 0;
    c.TransferOut(0, &s1);
    b = false;
    CCharString s2; s2.m_text[0] = 0;
    c.TransferOut(0, &s2);
    if (std::strcmp(s1.m_text, "TRUE") != 0 || std::strcmp(s2.m_text, "FALSE") != 0) { std::printf("00403f90_TEST FAIL\n"); return 1; }
    std::printf("00403f90_TEST PASS\n");
    return 0;
}
