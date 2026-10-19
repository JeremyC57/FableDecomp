// Behaviour test for CDataInputStream::ReadString (0x00996BE0): the bytes copied into the temporary memory stream are
// NUL-terminated and returned as a string; the stream buffer is freed on the way out.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0;
static void spy_free(void* p) { ++g_frees; std::free(p); }
struct CCharString {
    char text[64];
    CCharString(const char* t, long) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    ~CCharString() {}
};
struct CDataStream { virtual ~CDataStream() {} };
struct CDataOutputStream : CDataStream {
    CCharString NewLine;
    CDataOutputStream() : NewLine("\r\n", -1) {}
    virtual void Write(const void* data, long length) = 0;
    void WriteUBYTE(unsigned char value) { Write(&value, 1); }
};
struct SByteBuffer {
    unsigned char* First; unsigned char* Last; unsigned char* End; unsigned long Position;
    SByteBuffer() : First(0), Last(0), End(0), Position(0) {}
    unsigned char* GetData() { return (Last - First) ? First : (unsigned char*)this; }
};
struct CMemoryDataOutputStream : CDataOutputStream {
    SByteBuffer Buffer;
    CMemoryDataOutputStream() {}
    ~CMemoryDataOutputStream() { if (Buffer.First) spy_free(Buffer.First); }
    virtual void Write(const void* data, long length)
    {
        long used = (long)(Buffer.Last - Buffer.First);
        unsigned char* p = (unsigned char*)std::malloc(used + length);
        if (used) std::memcpy(p, Buffer.First, used);
        std::memcpy(p + used, data, length);
        if (Buffer.First) std::free(Buffer.First);
        Buffer.First = p; Buffer.Last = Buffer.End = p + used + length;
    }
};
struct CDataInputStream {
    const char* src; unsigned long pos;
    void ReadWithChunking(CDataOutputStream& output, unsigned long length) { output.Write(src + pos, (long)length); pos += length; }
    CCharString ReadString(long length);
};
CCharString CDataInputStream::ReadString(long length)
{
    CMemoryDataOutputStream stream;
    ReadWithChunking(stream, length);
    stream.WriteUBYTE(0);
    CCharString result((const char*)stream.Buffer.GetData(), -1);
    return result;
}
int main()
{
    CDataInputStream in; in.src = "HelloWorld"; in.pos = 0;
    int bad = 0;
    CCharString a = in.ReadString(5);
    if (std::strcmp(a.text, "Hello") || in.pos != 5 || g_frees != 1) bad = 1;
    CCharString b = in.ReadString(5);
    if (std::strcmp(b.text, "World") || g_frees != 2) bad = 2;
    CCharString c = in.ReadString(0);
    if (c.text[0] != 0 || g_frees != 3) bad = 3;   // only the terminator was written
    if (bad) { std::printf("00996be0_TEST FAIL\n"); return 1; }
    std::printf("00996be0_TEST PASS\n");
    return 0;
}
