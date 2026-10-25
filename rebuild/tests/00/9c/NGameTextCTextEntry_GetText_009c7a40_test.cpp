#include <cstdio>
static int g_copies = 0;
class CWideString { public: void* PStringData; CWideString(void* p) : PStringData(p) {} CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_copies; } };
namespace NGameText {
class CTextEntry { public: unsigned char _pad[0x28]; CWideString Text; CTextEntry() : Text((void*)0x42) {} CWideString GetText() const { return Text; } };
}
int main()
{
    NGameText::CTextEntry e;
    CWideString h = e.GetText();
    if (h.PStringData != (void*)0x42 || g_copies < 1 || &h == &e.Text) { std::printf("009c7a40_TEST FAIL\n"); return 1; }
    std::printf("009c7a40_TEST PASS\n");
    return 0;
}
