// Test: CDataOutputStream::WritePresizedString -- write the 4-byte length, then the characters
#include <cstdio>
#include <cstring>
struct CStringData { const char* PData; long StringLength; };
class CCharString { public:
    CStringData* PStringData;
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->PData; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};
class CDataOutputStream { public:
    unsigned char buf[64]; long pos;
    virtual void Write(const void* data, long length) { std::memcpy(buf + pos, data, length); pos += length; }
    void WriteULONG(unsigned int value) { Write(&value, sizeof(value)); }
    void WritePresizedString(const CCharString& text)
    {
        const char* characters = text.GetAsCharArray();
        unsigned int length = (unsigned int)text.GetLength();
        WriteULONG(length);
        Write(characters, text.GetLength());
    }
    virtual ~CDataOutputStream() { }
};
int main()
{
    CStringData abc = { "abc", 3 };
    CCharString s3 = { &abc }, empty = { 0 };
    CDataOutputStream s; s.pos = 0;
    s.WritePresizedString(s3); s.WritePresizedString(empty);
    unsigned int n0, n1; std::memcpy(&n0, s.buf, 4); std::memcpy(&n1, s.buf + 7, 4);
    if (s.pos != 11 || n0 != 3 || n1 != 0 || std::memcmp(s.buf + 4, "abc", 3) != 0) { std::printf("00995a80_TEST FAIL\n"); return 1; }
    std::printf("00995a80_TEST PASS\n");
    return 0;
}