// Behaviour test for CCRCSymbolMap::LoadBinary (0x00A38C20), a portable model. The file is read whole (into a buffer one byte
// longer); the first little-endian long is the entry count, the map is resized to it, and each slot takes the next two longs as
// (CRC, symbol). A stream that runs short leaves the unread values as they were (the memory stream's bounded read).
#include <stdio.h>
#include <string.h>
#include <vector>
#include <utility>
typedef int s32; typedef unsigned int u32;   // the retail 32-bit long / unsigned long
struct Stream {
    const unsigned char* data; unsigned long size, pos;
    void Read(void* out, long n) { if (n <= 0 || pos + n > size) return; memcpy(out, data + pos, n); pos += n; }
    s32 ReadSLONG(s32 keep) { s32 v = keep; Read(&v, 4); return v; }
};
static void LoadBinary(const std::vector<unsigned char>& file, std::vector<std::pair<u32, s32> >& map)
{
    std::vector<unsigned char> buffer(file.size() + 1, 0);
    if (!file.empty()) memcpy(&buffer[0], &file[0], file.size());
    Stream os = { &buffer[0], (unsigned long)buffer.size(), 0 };
    s32 no_entries = os.ReadSLONG(0);
    map.resize(no_entries);
    s32 var1 = 0, var2 = 0;
    for (s32 c1 = 0; c1 < no_entries; ++c1) {
        var1 = os.ReadSLONG(var1); var2 = os.ReadSLONG(var2);
        map[c1] = std::pair<u32, s32>((u32)var1, var2);
    }
}
static void Put(std::vector<unsigned char>& f, long v) { for (int i = 0; i < 4; ++i) f.push_back((unsigned char)(v >> (8 * i))); }
int main()
{
    int bad = 0;
    std::vector<unsigned char> f; Put(f, 2); Put(f, 0x1234); Put(f, 7); Put(f, -5); Put(f, 9);
    std::vector<std::pair<u32, s32> > map(5);
    LoadBinary(f, map);
    if (map.size() != 2 || map[0].first != 0x1234 || map[0].second != 7 || map[1].first != (u32)-5 || map[1].second != 9) ++bad;
    std::vector<unsigned char> e; Put(e, 0);
    LoadBinary(e, map);
    if (!map.empty()) ++bad;
    printf(bad ? "00a38c20_TEST FAIL\n" : "00a38c20_TEST PASS\n");
    return bad ? 1 : 0;
}
