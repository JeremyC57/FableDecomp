// behaviour test for 0x009CBD10 CBankFile::GetEnumerationType(unsigned long) const, linked against the built member: the result is
// a copy (through the CCharString copy constructor) of the header enumeration-type string at +0xF0
#include <cstdio>
#include <cstring>
static int g_copies = 0;
int main();
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);
};
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
class CBankFile {
    friend int ::main();
protected:
    virtual CCharString GetEnumerationType(unsigned long entry) const;
public:
    unsigned char _pad_0x04[0xF0 - 4];
    CCharString HeaderEnumerationType;
};
int main()
{
    static unsigned char mem[sizeof(CBankFile)];
    std::memset(mem, 0, sizeof(mem));
    CBankFile* bf = (CBankFile*)mem;
    if ((unsigned char*)&bf->HeaderEnumerationType - mem != 0xF0) { std::printf("009cbd10_TEST FAIL layout\n"); return 1; }
    bf->HeaderEnumerationType.PStringData = (CCharStringData*)"RetailBank";
    CCharString r = bf->CBankFile::GetEnumerationType(7);
    if (r.PStringData != bf->HeaderEnumerationType.PStringData || g_copies < 1) { std::printf("009cbd10_TEST FAIL\n"); return 1; }
    std::printf("009cbd10_TEST PASS\n");
    return 0;
}
