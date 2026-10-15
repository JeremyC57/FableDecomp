#include <cstdio>
#include <string.h>
#define FAIL { std::printf("0099ea60_TEST FAIL\n"); return 1; }
struct CBasicStringChar {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; unsigned long m_reserved : 1; unsigned char UseFastExtend : 1;
    void AssignFromData(const char* text, unsigned long length) { PData = new char[length + 1]; memcpy(PData, text, length); PData[length] = 0; StringLength = length; DataLength = length + 1; }
    CBasicStringChar(const char* text) : PData(0), StringLength(0), DataLength(0), UseFastExtend(1) { AssignFromData(text, (unsigned long)strlen(text)); }
    CBasicStringChar(const char* text, unsigned long length) : PData(0), StringLength(0), DataLength(0), UseFastExtend(1) { AssignFromData(text, length); }
};
struct CCharStringData {
    CBasicStringChar Data; long NoRefs;
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
    CCharStringData(const char* text, unsigned long length) : Data(text, length), NoRefs(1) { }
};
struct CCharString {
    CCharStringData* AllocStringData(const char* text, long length)
    {
        if (length == -1) return new CCharStringData(text);
        return new CCharStringData(text, length);
    }
};
int main()
{
    CCharString s;
    CCharStringData* a = s.AllocStringData("hello", -1);
    if (!a || a->NoRefs != 1 || a->Data.StringLength != 5 || strcmp(a->Data.PData, "hello") != 0 || !a->Data.UseFastExtend) FAIL;
    CCharStringData* b = s.AllocStringData("hello", 3);
    if (!b || b->NoRefs != 1 || b->Data.StringLength != 3 || strcmp(b->Data.PData, "hel") != 0) FAIL;
    std::printf("0099ea60_TEST PASS\n");
    return 0;
}