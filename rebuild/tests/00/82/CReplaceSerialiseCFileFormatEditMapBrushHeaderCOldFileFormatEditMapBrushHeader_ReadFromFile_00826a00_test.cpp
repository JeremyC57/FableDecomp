// behaviour test for 00826a00: CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader>::ReadFromFile on an in-memory file (current and old record versions)
#include <cstdio>
#include <cstring>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
struct SFile {
    unsigned char data[256]; long pos;
    void ReadVar(int* v) { std::memcpy(v, data + pos, 4); pos += 4; }
    void ReadVar(unsigned char* v) { *v = data[pos]; pos += 1; }
    long GetPosition() { return pos; }
    void SetPosition(long p) { pos = p; }
    void Read(void* b, long n) { std::memcpy(b, data + pos, n); pos += n; }
};
static const long SIZE = 0x49, VERSION = 2;
static int g_converted = 0;
static long OldGetMaxSize(long version, long size) { return MAX(size, (long)0x49); }
static long GetMaxSize(long version, long size)
{
    long maxSize = (version == VERSION) ? size : OldGetMaxSize(version, size);
    return MAX(maxSize, SIZE);
}
static void ConvertToThisVersion(void* data) { ((unsigned char*)data)[4] = (unsigned char)VERSION; ++g_converted; }
struct SRecord {
    unsigned char bytes[0x49];
    void ReadFromFile(SFile& file)
    {
        int length; file.ReadVar(&length);
        long position = file.GetPosition();
        unsigned char version; file.ReadVar(&version);
        file.SetPosition(position);
        if (version == VERSION) { unsigned int l = (unsigned int)length; std::memcpy(bytes, &l, 4); file.Read(bytes + 4, length - 4); return; }
        unsigned char* buffer = new unsigned char[GetMaxSize(version, 0)];
        file.Read(buffer + 4, length - 4);
        unsigned int l = (unsigned int)length; std::memcpy(buffer, &l, 4);
        ConvertToThisVersion(buffer);
        std::memcpy(bytes, buffer, SIZE);
        delete[] buffer;
    }
};
int main()
{
    int ok = 1;
    long i = 0;
    SFile f; std::memset(&f, 0, sizeof(f));
    int len = (int)0x20;                                 // a record of 0x20 bytes (length counts the 4-byte length)
    std::memcpy(f.data, &len, 4); f.data[4] = (unsigned char)VERSION;
    for (i = 5; i < 0x20; ++i) f.data[i] = (unsigned char)(i * 3);
    static SRecord r; std::memset(&r, 0xEE, sizeof(r));
    r.ReadFromFile(f);
    if (f.pos != 0x20 || r.bytes[4] != VERSION || r.bytes[5] != 15 || g_converted != 0) ok = 0;
    unsigned int bl; std::memcpy(&bl, r.bytes, 4); if (bl != (unsigned int)0x20) ok = 0;
    f.pos = 0; f.data[4] = (unsigned char)(VERSION - 1);  // an older version goes through the scratch block
    r.ReadFromFile(f);
    if (f.pos != 0x20 || r.bytes[4] != VERSION || r.bytes[6] != 18 || g_converted != 1) ok = 0;
    if (GetMaxSize(VERSION, 0) != SIZE || GetMaxSize(VERSION - 1, 0) != MAX(SIZE, (long)0x49)) ok = 0;
    if (!ok) { std::printf("00826a00_TEST FAIL\n"); return 1; }
    std::printf("00826a00_TEST PASS\n");
    return 0;
}