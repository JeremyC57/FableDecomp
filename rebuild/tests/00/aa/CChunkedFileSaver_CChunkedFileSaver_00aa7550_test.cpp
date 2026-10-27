// Behaviour test for 0x00AA7550 CChunkedFileSaver::CChunkedFileSaver(const char*) -- keeps a NUL-terminated copy of the 4-char file type
#include <cstdio>
#include <cstring>
class CChunkedFileSaver {
public:
    char SupportedFileType[5];
    CChunkedFileSaver(const char* type)
    {
        std::memset(SupportedFileType, 0, sizeof(SupportedFileType));
        std::memcpy(SupportedFileType, type, 4);
    }
    virtual ~CChunkedFileSaver() { }
};
int main()
{
    CChunkedFileSaver s("LEVLXYZ");
    if (std::strcmp(s.SupportedFileType, "LEVL") != 0) { std::printf("00aa7550_TEST FAIL\n"); return 1; }
    CChunkedFileSaver t("ab\0\0");
    if (t.SupportedFileType[1] != 'b' || t.SupportedFileType[2] != 0 || t.SupportedFileType[4] != 0) { std::printf("00aa7550_TEST FAIL\n"); return 1; }
    std::printf("00aa7550_TEST PASS\n");
    return 0;
}