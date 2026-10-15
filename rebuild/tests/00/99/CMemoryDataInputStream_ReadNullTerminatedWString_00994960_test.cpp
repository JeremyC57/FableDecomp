#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct WStr { long bytes; WStr(const unsigned short* t) { bytes = 0; while (t[bytes / 2]) bytes += 2; } long GetDataSize() const { return bytes; } };
struct Stream {
    long position, sourceLength; char* chunkBase; long chunkPosition, chunkLength; char* source;
    long GetPosition() { return position; }
    WStr Read()
    {
        long start = GetPosition();
        WStr temp((const unsigned short*)(source + start));
        long consumed = temp.GetDataSize() + 2;
        if (consumed > 0) {
            i64 wide = (i64)(unsigned long)GetPosition() + (unsigned long)consumed;
            if (wide >= 0 && wide <= 0x7fffffff) {
                long np = (long)wide; long op = position;
                chunkBase += np - op; chunkLength += op - np; position = np;
            }
        }
        return temp;
    }
};
int main()
{
    static unsigned short buf[16] = { 'h', 'i', 0, 'x', 'y', 'z', 0 };
    Stream s; s.position = 0; s.sourceLength = 32; s.chunkBase = (char*)buf; s.chunkPosition = 0; s.chunkLength = 32; s.source = (char*)buf;
    WStr a = s.Read();
    if (a.GetDataSize() != 4 || s.position != 6 || s.chunkBase != (char*)buf + 6 || s.chunkLength != 26) { std::printf("00994960_TEST FAIL\n"); return 1; }
    WStr b = s.Read();
    if (b.GetDataSize() != 6 || s.position != 14 || s.chunkLength != 18) { std::printf("00994960_TEST FAIL\n"); return 1; }
    std::printf("00994960_TEST PASS\n");
    return 0;
}