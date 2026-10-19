// Behaviour test for 0x00994E90 (CDataInputStream text readers), over a two-chunk memory stream.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct CCharString {
    char text[64]; int len;
    CCharString() : len(0) { text[0] = 0; }
    CCharString(const CCharString& o) : len(o.len) { std::memcpy(text, o.text, sizeof(text)); }
    ~CCharString() {}
    CCharString& operator+=(char c) { text[len++] = c; text[len] = 0; return *this; }
};
// a memory stream split into two source chunks so the overflow path is exercised
struct CDataInputStream {
    const char* base; long chunk;
    unsigned long StreamPos; long StreamSize; const unsigned char* CurrentSourcePtr; long SourceChunkBytesRemaining;
    CDataInputStream(const char* text, long chunkSize) : base(text), chunk(chunkSize), StreamPos(0), StreamSize((long)std::strlen(text)) { Seek(0); }
    void Seek(unsigned long p) { StreamPos = p; CurrentSourcePtr = (const unsigned char*)base + p; SourceChunkBytesRemaining = (p < (unsigned long)chunk ? chunk : StreamSize) - (long)p; }
    virtual void SetPosition(unsigned long p) { Seek(p); }
    virtual unsigned long GetPosition() const { return StreamPos; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { Seek(StreamPos); std::memcpy(buffer, CurrentSourcePtr, size); Seek(StreamPos + size); }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* dest, long n)
    {
        i64 newPosition = (i64)(StreamPos + n);
        if (!IsValidPosition(newPosition)) return;
        if (SourceChunkBytesRemaining >= n) {
            std::memcpy(dest, CurrentSourcePtr, n);
            CurrentSourcePtr += n;
            SourceChunkBytesRemaining -= n;
            StreamPos += n;
        } else ReadWithSrcChunkOverflow((unsigned char*)dest, n);
    }
    char ReadChar() { char value; Read(&value, 1); return value; }
    CCharString ReadUntilCharacter(char terminator, bool* found);
    CCharString ReadUntilWhiteSpace();
    CCharString ReadWhiteSpace();
};
CCharString CDataInputStream::ReadUntilWhiteSpace()
{
    CCharString result;
    while (StreamPos != (unsigned long)StreamSize) {
        char c = ReadChar();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            SetPosition(GetPosition() - 1);
            break;
        }
        result += c;
    }
    return result;
}

int main()
{
    int bad = 0;
    CDataInputStream s("word\tnext", 2);
    CCharString w = s.ReadUntilWhiteSpace();
    if (std::strcmp(w.text, "word") || s.GetPosition() != 4) bad = 1;
    s.Seek(5);
    CCharString n = s.ReadUntilWhiteSpace();
    if (std::strcmp(n.text, "next") || s.GetPosition() != 9) bad = 2;
    CDataInputStream t2("a b\r\nc", 8);
    CCharString a = t2.ReadUntilWhiteSpace();
    if (std::strcmp(a.text, "a") || t2.GetPosition() != 1) bad = 3;
    if (bad) { std::printf("00994e90_TEST FAIL\n"); return 1; }
    std::printf("00994e90_TEST PASS\n");
    return 0;
}