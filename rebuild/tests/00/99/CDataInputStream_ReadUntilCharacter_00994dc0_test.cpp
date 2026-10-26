// Behaviour test for 0x00994DC0 (CDataInputStream text readers), over a two-chunk memory stream.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
class CCharString { public:
    char text[64]; int len;
    CCharString() : len(0) { text[0] = 0; }
    CCharString(const CCharString& o) : len(o.len) { std::memcpy(text, o.text, sizeof(text)); }
    ~CCharString() {}
    CCharString& operator+=(char c) { text[len++] = c; text[len] = 0; return *this; }
};
// a memory stream split into two source chunks so the overflow path is exercised
class CDataInputStream { public:
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
CCharString CDataInputStream::ReadUntilCharacter(char terminator, bool* found)
{
    CCharString result;
    for (;;) {
        if (StreamPos == (unsigned long)StreamSize) {
            *found = false;
            break;
        }
        char c = ReadChar();
        if (c == terminator) {
            SetPosition(GetPosition() - 1);
            *found = true;
            break;
        }
        result += c;
    }
    return result;
}

int main()
{
    int bad = 0;
    CDataInputStream s("key=value;rest", 3);
    bool found = false;
    CCharString k = s.ReadUntilCharacter('=', &found);
    if (std::strcmp(k.text, "key") || !found || s.GetPosition() != 3) bad = 1;   // '=' pushed back
    s.Seek(4);
    CCharString v = s.ReadUntilCharacter(';', &found);
    if (std::strcmp(v.text, "value") || !found || s.GetPosition() != 9) bad = 2;
    s.Seek(10);
    CCharString r = s.ReadUntilCharacter('#', &found);
    if (std::strcmp(r.text, "rest") || found || s.GetPosition() != 14) bad = 3;   // end of stream
    if (bad) { std::printf("00994dc0_TEST FAIL\n"); return 1; }
    std::printf("00994dc0_TEST PASS\n");
    return 0;
}