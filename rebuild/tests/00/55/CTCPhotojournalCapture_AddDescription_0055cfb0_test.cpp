// Behaviour test for 0055cfb0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

class CWideString {
public:
    const unsigned short* Text;
    CWideString() : Text(0) { }
    CWideString(const unsigned short* t) : Text(t) { }
    operator const unsigned short*() const { return Text; }
    long GetLength() const { long n = 0; while (Text && Text[n]) ++n; return n; }
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_READ_WRITE = 2, FILE_ACCESS_WRITE = 4 };
static unsigned char g_disk[256];                       // the "file" on disk
static long g_diskLength = 0;
static int g_opens = 0, g_closes = 0;
class CDiskFileWin32 { public:                                 // an in-memory stand-in with the retail call surface
    FileAccessType AccessType;
    long Position;
    long Length;
    bool Opened;
    CWideString Pathname;
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& name, FileAccessType access, unsigned long flags)
    {
        CHECK(access == FILE_ACCESS_READ_WRITE && flags == 2);
        Pathname = name;
        AccessType = access;
        Position = 0;
        Length = g_diskLength;
        Opened = true;
        ++g_opens;
    }
    virtual void Close() { if (Opened) { ++g_closes; g_diskLength = Length; } Opened = false; }
    virtual void Write(const void* data, long size, void* process)
    {
        CHECK(process == 0 && Opened);
        std::memcpy(g_disk + Position, data, size);
        Position += size;
        if (Position > Length) Length = Position;
    }
    virtual void SetPosition(long p) { Position = p; }
    virtual long GetLength() const { return Length; }
};
static void AddDescription(const CWideString& filename, const CWideString& description)
{
    CDiskFileWin32 file;
    file.Open(filename, FILE_ACCESS_READ_WRITE, 2);
    file.SetPosition(file.GetLength());
    const unsigned short* text = description;
    file.Write(text, description.GetLength() * 2, 0);
    long length = description.GetLength();
    file.Write(&length, sizeof(length), 0);
    file.Close();
}
static void RunTest()
{
    static const unsigned short name[] = { 'a', '.', 'b', 'm', 'p', 0 };
    static const unsigned short caption[] = { 'H', 'i', '!', 0 };
    std::memset(g_disk, 0xEE, sizeof(g_disk));
    g_diskLength = 10;                                  // an existing 10-byte screenshot
    AddDescription(CWideString(name), CWideString(caption));
    CHECK(g_opens == 1 && g_closes == 1);               // the explicit Close; the destructor's Close is a no-op
    CHECK(g_diskLength == 10 + 6 + (long)sizeof(long));
    CHECK(g_disk[9] == 0xEE);                           // the image bytes are kept
    unsigned short chars[3];
    std::memcpy(chars, g_disk + 10, 6);
    CHECK(chars[0] == 'H' && chars[1] == 'i' && chars[2] == '!');
    long count;
    std::memcpy(&count, g_disk + 16, sizeof(count));
    CHECK(count == 3);                                  // the trailing character count GetDescription reads back
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055cfb0_TEST FAIL\n");
        return 1;
    }
    std::printf("0055cfb0_TEST PASS\n");
    return 0;
}