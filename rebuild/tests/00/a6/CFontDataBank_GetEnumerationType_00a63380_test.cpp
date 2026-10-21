// behaviour test for 0x00A63380
#include <stdio.h>
static int g_copies = 0; static unsigned long g_type = 0;
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(long v) : PStringData((CCharStringData*)v) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
    ~CCharString() {}
};
class CBankFile {
public:
    virtual CCharString GetEnumerationType(unsigned long type) const { g_type = type; return CCharString(0x1234); }
};
class CBankFileAsync : public CBankFile { };
class CFontDataBank : public CBankFileAsync {
public:
    virtual CCharString GetEnumerationType(unsigned long type) const;
};
CCharString CFontDataBank::GetEnumerationType(unsigned long type) const
{
    return CBankFile::GetEnumerationType(type);
}

int main()
{
    CFontDataBank b;
    CCharString s = b.GetEnumerationType(7);
    int ok = (long)s.PStringData == 0x1234 && g_type == 7;
    printf(ok ? "00a63380_TEST PASS\n" : "00a63380_TEST FAIL\n");
    return ok ? 0 : 1;
}