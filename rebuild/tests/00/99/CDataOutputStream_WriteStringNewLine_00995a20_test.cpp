// Test: CDataOutputStream::WriteStringNewLine -- write the string's characters then the stream's new-line string (empty strings write "" with length 0)
#include <cstdio>
#include <cstring>
struct CStringData { const char* PData; long StringLength; };
class CCharString { public:
    CStringData* PStringData;
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->PData; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};
class CDataOutputStream { public:
    char buf[64]; long pos; int writes; CCharString NewLine;
    virtual void Write(const void* data, long length) { std::memcpy(buf + pos, data, length); pos += length; ++writes; }
    void WriteStringNewLine(const CCharString& text)
    {
        const char* characters = text.GetAsCharArray();
        Write(characters, text.GetLength());
        const char* newLine = NewLine.GetAsCharArray();
        Write(newLine, NewLine.GetLength());
    }
    virtual ~CDataOutputStream() { }
};
int main()
{
    CStringData hello = { "hello", 5 }, crlf = { "\r\n", 2 };
    CCharString h = { &hello }, empty = { 0 };
    CDataOutputStream s; s.pos = 0; s.writes = 0; s.NewLine.PStringData = &crlf;
    s.WriteStringNewLine(h); s.WriteStringNewLine(empty);
    if (s.pos != 9 || s.writes != 4 || std::memcmp(s.buf, "hello\r\n\r\n", 9) != 0) { std::printf("00995a20_TEST FAIL\n"); return 1; }
    std::printf("00995a20_TEST PASS\n");
    return 0;
}