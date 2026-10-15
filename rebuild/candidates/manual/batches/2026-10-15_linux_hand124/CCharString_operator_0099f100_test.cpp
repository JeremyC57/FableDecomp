#include <cstdio>
#include <string.h>
#define FAIL { std::printf("0099f100_TEST FAIL\n"); return 1; }
struct CBasicStringChar {
    char buf[64]; unsigned long StringLength;
    CBasicStringChar(const char* text) { strcpy(buf, text); StringLength = (unsigned long)strlen(text); }
    void AppendData(const char* text, unsigned long length) { memcpy(buf + StringLength, text, length); StringLength += length; buf[StringLength] = 0; }
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; CCharStringData(const char* t) : Data(t), NoRefs(1) { } };
static int g_cow = 0;
struct CCharString {
    CCharStringData* PStringData;
    void MakeOwnCopyOfData() { ++g_cow; }
    const CCharString& operator+=(const char* text)
    {
        if (PStringData != 0) {
            MakeOwnCopyOfData();
            CCharStringData* data = PStringData;
            unsigned long length = 0;
            if (text[length] != 0) { do { ++length; } while (text[length] != 0); }
            data->Data.AppendData(text, length);
        } else if (text != 0 && *text != 0) {
            PStringData = new CCharStringData(text);
        }
        return *this;
    }
};
int main()
{
    CCharString s; s.PStringData = 0;
    s += (const char*)0; if (s.PStringData != 0) FAIL;
    s += ""; if (s.PStringData != 0) FAIL;
    s += "foo"; if (!s.PStringData || strcmp(s.PStringData->Data.buf, "foo") != 0 || s.PStringData->NoRefs != 1 || g_cow != 0) FAIL;
    s += "bar"; if (strcmp(s.PStringData->Data.buf, "foobar") != 0 || s.PStringData->Data.StringLength != 6 || g_cow != 1) FAIL;
    s += ""; if (strcmp(s.PStringData->Data.buf, "foobar") != 0 || g_cow != 2) FAIL;
    std::printf("0099f100_TEST PASS\n");
    return 0;
}
